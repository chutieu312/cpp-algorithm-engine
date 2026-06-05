#include <gtest/gtest.h>
#include "algorithms/KalmanFilter.h"
#include <cmath>

TEST(KalmanFilterTest, PredictIncreasesCovariance)
{
    KalmanFilter kf(0.0, 1.0, 0.5, 1.0);
    double p_before = kf.getCovariance();
    kf.predict();
    EXPECT_GT(kf.getCovariance(), p_before);
}

TEST(KalmanFilterTest, UpdateReducesCovariance)
{
    KalmanFilter kf(0.0, 1.0, 0.5, 1.0);
    kf.predict();
    double p_after_predict = kf.getCovariance();
    kf.update(1.0);
    EXPECT_LT(kf.getCovariance(), p_after_predict);
}

TEST(KalmanFilterTest, KalmanGainBoundedZeroToOne)
{
    KalmanFilter kf(0.0, 1.0, 0.1, 1.0);
    kf.predict();
    kf.update(5.0);
    EXPECT_GE(kf.getKalmanGain(), 0.0);
    EXPECT_LE(kf.getKalmanGain(), 1.0);
}

TEST(KalmanFilterTest, ZeroInnovationNoStateChange)
{
    // When z == predicted state, the update should leave x unchanged.
    KalmanFilter kf(5.0, 1.0, 0.1, 1.0);
    kf.predict();
    double x_pred = kf.getState();
    kf.update(x_pred);
    EXPECT_NEAR(kf.getState(), x_pred, 1e-9);
}

TEST(KalmanFilterTest, NumericalStabilityLargeProcessNoise)
{
    // Very large Q must not produce NaN or Inf.
    KalmanFilter kf(0.0, 1.0, 1e6, 1.0);
    kf.predict();
    kf.update(100.0);
    EXPECT_FALSE(std::isnan(kf.getState()));
    EXPECT_FALSE(std::isinf(kf.getState()));
}

TEST(KalmanFilterTest, StateConvergesToTrueValue)
{
    // After 100 updates at z=10, the estimate converges to ~10.
    KalmanFilter kf(0.0, 1.0, 0.1, 0.5);
    for (int i = 0; i < 100; ++i) {
        kf.predict();
        kf.update(10.0);
    }
    EXPECT_NEAR(kf.getState(), 10.0, 0.05);
}

TEST(KalmanFilterTest, InvalidArgumentsThrow)
{
    EXPECT_THROW(KalmanFilter(0.0, 0.0,  0.1, 1.0), std::invalid_argument); // p0 <= 0
    EXPECT_THROW(KalmanFilter(0.0, 1.0, -1.0, 1.0), std::invalid_argument); // q < 0
    EXPECT_THROW(KalmanFilter(0.0, 1.0,  0.1, 0.0), std::invalid_argument); // r <= 0
}
