#ifndef STAN_MATH_FWD_FUN_MDIVIDE_RIGHT_HPP
#define STAN_MATH_FWD_FUN_MDIVIDE_RIGHT_HPP

#include <stan/math/fwd/core.hpp>
#include <stan/math/fwd/fun/multiply.hpp>
#include <stan/math/fwd/fun/to_fvar.hpp>
#include <stan/math/prim/err.hpp>
#include <stan/math/prim/fun/Eigen.hpp>
#include <stan/math/prim/fun/mdivide_right.hpp>
#include <stan/math/prim/fun/multiply.hpp>
#include <stan/math/prim/fun/subtract.hpp>
#include <vector>

namespace stan {
namespace math {
/*
template <typename EigMat1, typename EigMat2,
          require_all_eigen_vt<is_fvar, EigMat1, EigMat2>* = nullptr>
inline auto mdivide_right(const EigMat1& A, const EigMat2& b) {
  using T1 = typename return_type_t<EigMat1>::Scalar;
  using T2 = typename return_type_t<EigMat2>::Scalar;
  using ret_scalar = return_type_t<EigMat1, EigMat2>;
  constexpr int R1 = EigMat1::RowsAtCompileTime;
  constexpr int C1 = EigMat1::ColsAtCompileTime;
  constexpr int R2 = EigMat2::RowsAtCompileTime;
  constexpr int C2 = EigMat2::ColsAtCompileTime;

  check_square("mdivide_right", "b", b);
  check_multiplicable("mdivide_right", "A", A, "b", b);
  if (b.size() == 0) {
    return Eigen::Matrix<ret_scalar, R1, C2>(A.rows(), 0);
  }


  auto&& A_ref = to_ref(A);
  Eigen::Matrix<T1, R1, C1> val_A = A_ref.val();
  Eigen::Matrix<T1, R1, C1> deriv_A = A_ref.d();

  auto&& b_ref = to_ref(b);
  Eigen::Matrix<T2, R2, C2> val_b = b_ref.val();
  Eigen::Matrix<T2, R2, C2> deriv_b = b_ref.d();

  Eigen::Matrix<return_type_t<T1, T2>, R1, C2> A_mult_inv_b = mdivide_right(val_A, val_b).template cast<return_type_t<T1, T2>>().eval();
  Eigen::Matrix<ret_scalar, R1, C2> res = A_mult_inv_b.template cast<ret_scalar>().eval();
  res.d() = subtract(mdivide_right(deriv_A, val_b),
  multiply(A_mult_inv_b, mdivide_right(deriv_b, val_b)));
  return res;
}

template <typename EigMat1, typename EigMat2,
          require_eigen_vt<is_fvar, EigMat1>* = nullptr,
          require_eigen_vt<std::is_arithmetic, EigMat2>* = nullptr>
inline Eigen::Matrix<value_type_t<EigMat1>, EigMat1::RowsAtCompileTime,
                     EigMat2::ColsAtCompileTime>
mdivide_right(const EigMat1& A, const EigMat2& b) {
  using T = typename value_type_t<EigMat1>::Scalar;
  constexpr int R1 = EigMat1::RowsAtCompileTime;
  constexpr int C1 = EigMat1::ColsAtCompileTime;
  constexpr int C2 = EigMat2::ColsAtCompileTime;

  check_square("mdivide_right", "b", b);
  check_multiplicable("mdivide_right", "A", A, "b", b);
  if (b.size() == 0) {
    return {A.rows(), 0};
  }


  auto&& A_ref = to_ref(A);
  Eigen::Matrix<T, R1, C1> val_A = A_ref.val();
  Eigen::Matrix<T, R1, C1> deriv_A = A_ref.d();
  Eigen::Matrix<value_type_t<EigMat1>, R1, C2> res = mdivide_right(val_A, b).template cast<value_type_t<EigMat1>>();
  res.d() = mdivide_right(deriv_A, b);
  return res;
}

template <typename EigMat1, typename EigMat2,
          require_eigen_vt<std::is_arithmetic, EigMat1>* = nullptr,
          require_eigen_vt<is_fvar, EigMat2>* = nullptr>
inline Eigen::Matrix<value_type_t<EigMat2>, EigMat1::RowsAtCompileTime,
                     EigMat2::ColsAtCompileTime>
mdivide_right(const EigMat1& A, const EigMat2& b) {
  using T = typename value_type_t<EigMat2>::Scalar;
  constexpr int R1 = EigMat1::RowsAtCompileTime;
  constexpr int C1 = EigMat1::ColsAtCompileTime;
  constexpr int R2 = EigMat2::RowsAtCompileTime;
  constexpr int C2 = EigMat2::ColsAtCompileTime;

  check_square("mdivide_right", "b", b);
  check_multiplicable("mdivide_right", "A", A, "b", b);
  if (b.size() == 0) {
    return {A.rows(), 0};
  }

  auto&& b_ref = to_ref(b);
  Eigen::Matrix<T, R2, C2> val_b = b_ref.val();
  Eigen::Matrix<T, R2, C2> deriv_b = b_ref.d();

  Eigen::Matrix<T, R1, C2> A_mult_inv_b = mdivide_right(A, val_b);
  Eigen::Matrix<value_type_t<EigMat2>, R1, C2> res = A_mult_inv_b.template cast<value_type_t<EigMat2>>();
  res.d() = -multiply(A_mult_inv_b, mdivide_right(deriv_b, val_b));
  return res;
}
*/
}  // namespace math
}  // namespace stan
#endif
