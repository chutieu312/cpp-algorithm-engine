#include <gtest/gtest.h>
#include "algorithms/LeastSquares.h"
#include <cmath>

TEST(LeastSquaresTest, PerfectLinearFit)
{
    LeastSquares ols;
    // y = 2x + 1  →  exact fit
    auto r = ols.fit({1, 2, 3, 4, 5}, {3, 5, 7, 9, 11});
    EXPECT_NEAR(r.slope,     2.0, 1e-9);
    EXPECT_NEAR(r.intercept, 1.0, 1e-9);
    EXPECT_NEAR(r.r_squared, 1.0, 1e-9);
}

TEST(LeastSquaresTest, NoisyFitWithinTolerance)
{
    LeastSquares ols;
    // Approximate y ≈ 3x + 2 with small noise
    auto r = ols.fit({1, 2, 3, 4, 5}, {5.1, 7.9, 11.0, 14.1, 16.9});
    EXPECT_NEAR(r.slope,     3.0, 0.1);
    EXPECT_NEAR(r.intercept, 2.0, 0.2);
    EXPECT_GT(r.r_squared, 0.99);
}

TEST(LeastSquaresTest, PredictFromFittedModel)
{
    LeastSquares ols;
    ols.fit({0, 1, 2, 3}, {1, 3, 5, 7});   // y = 2x + 1
    EXPECT_NEAR(ols.predict(0.0),  1.0, 1e-6);
    EXPECT_NEAR(ols.predict(5.0), 11.0, 1e-6);
}

TEST(LeastSquaresTest, NegativeSlopeRegression)
{
    LeastSquares ols;
    auto r = ols.fit({1, 2, 3, 4, 5}, {10, 8, 6, 4, 2});  // y = -2x + 12
    EXPECT_NEAR(r.slope,     -2.0, 1e-9);
    EXPECT_NEAR(r.intercept, 12.0, 1e-9);
}

TEST(LeastSquaresTest, SinglePointThrows)
{
    LeastSquares ols;
    EXPECT_THROW(ols.fit({1.0}, {2.0}), std::invalid_argument);
}

TEST(LeastSquaresTest, IdenticalXValuesThrows)
{
    LeastSquares ols;
    EXPECT_THROW(ols.fit({2.0, 2.0, 2.0}, {1.0, 2.0, 3.0}), std::runtime_error);
}

TEST(LeastSquaresTest, PredictBeforeFitThrows)
{
    LeastSquares ols;
    EXPECT_THROW(ols.predict(5.0), std::logic_error);
}

TEST(LeastSquaresTest, UnequalVectorLengthsThrow)
{
    LeastSquares ols;
    EXPECT_THROW(ols.fit({1, 2, 3}, {1, 2}), std::invalid_argument);
}
