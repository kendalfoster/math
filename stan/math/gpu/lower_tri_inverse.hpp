#ifndef STAN_MATH_GPU_LOWER_TRI_INVERSE_HPP
#define STAN_MATH_GPU_LOWER_TRI_INVERSE_HPP

#ifdef STAN_OPENCL
#include <stan/math/gpu/matrix_gpu.hpp>
#include <stan/math/gpu/kernels/lower_tri_inverse_step1.hpp>
#include <stan/math/gpu/kernels/lower_tri_inverse_step2.hpp>
#include <stan/math/gpu/kernels/lower_tri_inverse_step3.hpp>

#include <stan/math/gpu/identity.hpp>
#include <stan/math/gpu/err/check_square.hpp>
#include <string>
#include <vector>

namespace stan {
namespace math {
/**
 * Computes the inverse of the lower triangular matrix
 * that resides in the GPU global memory
 *
 * @param A matrix on the GPU
 *
 * @return the inverse of A
 *
 * @throw <code>std::invalid_argument</code> if the matrix
 *    is not square
 */
inline matrix_gpu lower_triangular_inverse(const matrix_gpu& A) {
  check_square("lower_triangular_inverse (GPU)", "A", A);

  int thread_block_2D_dim = 32;
  int max_1D_thread_block_size = opencl_context.max_thread_block_size();
  // we split the input matrix to 32 blocks
  int thread_block_size_1D
      = (((A.rows() / 32) + thread_block_2D_dim - 1) / thread_block_2D_dim)
        * thread_block_2D_dim;
  if (max_1D_thread_block_size < thread_block_size_1D) {
    thread_block_size_1D = max_1D_thread_block_size;
  }
  int max_2D_thread_block_dim = sqrt(max_1D_thread_block_size);
  if (max_2D_thread_block_dim < thread_block_2D_dim) {
    thread_block_2D_dim = max_2D_thread_block_dim;
  }
  // for small size split in max 2 parts
  if (thread_block_size_1D < 64) {
    thread_block_size_1D = 32;
  }
  if (A.rows() < thread_block_size_1D) {
    thread_block_size_1D = A.rows();
  }

  // pad the input matrix
  int A_rows_padded
      = ((A.rows() + thread_block_size_1D - 1) / thread_block_size_1D)
        * thread_block_size_1D;

  matrix_gpu temp(A_rows_padded, A_rows_padded);
  matrix_gpu inv_padded(A_rows_padded, A_rows_padded);
  matrix_gpu inv_mat(A);
  matrix_gpu zero_mat(A_rows_padded - A.rows(), A_rows_padded);
  zero_mat.zeros<stan::math::TriangularViewGPU::Entire>();
  temp.zeros<stan::math::TriangularViewGPU::Entire>();
  inv_padded.zeros<stan::math::TriangularViewGPU::Entire>();

  int work_per_thread
      = opencl_kernels::lower_tri_inverse_step2.make_functor.get_opts().at(
          "WORK_PER_THREAD");
  // the number of blocks in the first step
  // each block is inverted with using the regular forward substitution
  int parts = inv_padded.rows() / thread_block_size_1D;
  inv_padded.sub_block(inv_mat, 0, 0, 0, 0, inv_mat.rows(), inv_mat.rows());
  try {
    // create a batch of identity matrices to be used in the first step
    opencl_kernels::batch_identity(
        cl::NDRange(parts, thread_block_size_1D, thread_block_size_1D),
        temp.buffer(), thread_block_size_1D, temp.size());
    // spawn parts thread blocks, each responsible for one block
    opencl_kernels::lower_tri_inverse_step1(
        cl::NDRange(parts * thread_block_size_1D),
        cl::NDRange(thread_block_size_1D), inv_padded.buffer(), temp.buffer(),
        inv_padded.rows());
  } catch (cl::Error& e) {
    check_opencl_error("inverse step1", e);
  }
  // set the padded part of the matrix and the upper triangular to zeros
  inv_padded.sub_block(zero_mat, 0, 0, inv_mat.rows(), 0, zero_mat.rows(),
                       zero_mat.cols());
  inv_padded.zeros<stan::math::TriangularViewGPU::Upper>();
  if (parts == 1) {
    inv_mat.sub_block(inv_padded, 0, 0, 0, 0, inv_mat.rows(), inv_mat.rows());
    return inv_mat;
  }
  parts = ceil(parts / 2.0);

  auto result_matrix_dim = thread_block_size_1D;
  auto thread_block_work2d_dim = thread_block_2D_dim / work_per_thread;
  auto ndrange_2d
      = cl::NDRange(thread_block_2D_dim, thread_block_work2d_dim, 1);
  while (parts > 0) {
    int result_matrix_dim_x = result_matrix_dim;
    // when calculating the last submatrix
    // we can reduce the size to the actual size (not the next power of 2)
    if (parts == 1 && (inv_padded.rows() - result_matrix_dim * 2) < 0) {
      result_matrix_dim_x = inv_padded.rows() - result_matrix_dim;
    }
    auto result_work_dim = result_matrix_dim / work_per_thread;
    auto result_ndrange
        = cl::NDRange(result_matrix_dim_x, result_work_dim, parts);
    opencl_kernels::lower_tri_inverse_step2(
        result_ndrange, ndrange_2d, inv_padded.buffer(), temp.buffer(),
        inv_padded.rows(), result_matrix_dim);
    opencl_kernels::lower_tri_inverse_step3(
        result_ndrange, ndrange_2d, inv_padded.buffer(), temp.buffer(),
        inv_padded.rows(), result_matrix_dim);
    // if this is the last submatrix, end
    if (parts == 1) {
      parts = 0;
    } else {
      parts = ceil(parts / 2.0);
    }
    result_matrix_dim *= 2;
    // set the padded part and upper diagonal to zeros
    inv_padded.sub_block(zero_mat, 0, 0, inv_mat.rows(), 0, zero_mat.rows(),
                         zero_mat.cols());
    inv_padded.zeros<stan::math::TriangularViewGPU::Upper>();
  }
  // un-pad and return
  inv_mat.sub_block(inv_padded, 0, 0, 0, 0, A.rows(), A.rows());
  return inv_mat;
}
}  // namespace math
}  // namespace stan

#endif
#endif
