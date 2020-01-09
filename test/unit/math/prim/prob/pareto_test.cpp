#include <stan/math/prim.hpp>
#include <stan/math/prim.hpp>
#include <test/unit/math/prim/prob/vector_rng_test_helper.hpp>
#include <test/unit/math/prim/prob/util.hpp>
#include <gtest/gtest.h>
#include <boost/random/mersenne_twister.hpp>
#include <boost/math/distributions.hpp>
#include <limits>
#include <vector>

class ParetoTestRig : public VectorRealRNGTestRig {
 public:
  ParetoTestRig()
      : VectorRealRNGTestRig(10000, 10, {0.5, 1.0, 1.3, 2.0}, {1, 2, 3},
                             {-2.5, -1.7, -0.1, 0.0}, {-3, -2, -1, 0},
                             {0.1, 1.0, 1.7, 2.1}, {1, 2, 3, 4},
                             {-2.7, -1.5, -0.5, 0.0}, {-3, -2, -1, 0}) {}

  template <typename T1, typename T2, typename T3, typename T_rng>
  auto generate_samples(const T1& ymin, const T2& alpha, const T3&,
                        T_rng& rng) const {
    return stan::math::pareto_rng(ymin, alpha, rng);
  }

  std::vector<double> generate_quantiles(double ymin, double alpha,
                                         double) const {
    std::vector<double> quantiles;
    double K = stan::math::round(2 * std::pow(N_, 0.4));
    boost::math::pareto_distribution<> dist(ymin, alpha);

    for (int i = 1; i < K; ++i) {
      double frac = i / K;
      quantiles.push_back(quantile(dist, frac));
    }
    quantiles.push_back(std::numeric_limits<double>::max());

    return quantiles;
  }
};

TEST(ProbDistributionsPareto, errorCheck) {
  check_dist_throws_all_types(ParetoTestRig());
}

TEST(ProbDistributionsPareto, distributionTest) {
  check_quantiles_real_real(ParetoTestRig());
}

TEST(ProbDistributionsPareto, error_check) {
  boost::random::mt19937 rng;
  EXPECT_NO_THROW(stan::math::pareto_rng(2.0, 1.0, rng));

  EXPECT_THROW(stan::math::pareto_rng(2.0, -1.0, rng), std::domain_error);
  EXPECT_THROW(stan::math::pareto_rng(-2.0, 1.0, rng), std::domain_error);
  EXPECT_THROW(
      stan::math::pareto_rng(stan::math::positive_infinity(), 1.0, rng),
      std::domain_error);
  EXPECT_THROW(stan::math::pareto_rng(2, stan::math::positive_infinity(), rng),
               std::domain_error);
}

TEST(ProbDistributionsPareto, chiSquareGoodnessFitTest) {
  boost::random::mt19937 rng;
  int N = 10000;
  int K = stan::math::round(2 * std::pow(N, 0.4));

  std::vector<double> samples;
  for (int i = 0; i < N; ++i) {
    samples.push_back(stan::math::pareto_rng(2.0, 1.0, rng));
  }

  // Generate quantiles from boost's Pareto distribution
  boost::math::pareto_distribution<> dist(2.0, 1.0);
  std::vector<double> quantiles;
  for (int i = 1; i < K; ++i) {
    double frac = static_cast<double>(i) / K;
    quantiles.push_back(quantile(dist, frac));
  }
  quantiles.push_back(std::numeric_limits<double>::max());

  // Assert that they match
  assert_matches_quantiles(samples, quantiles, 1e-6);
}
