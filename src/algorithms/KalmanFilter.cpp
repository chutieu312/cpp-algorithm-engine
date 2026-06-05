#include "algorithms/KalmanFilter.h"
#include <stdexcept>

KalmanFilter::KalmanFilter(double x0, double p0, double q, double r)
    : x_(x0), p_(p0), q_(q), r_(r), k_(0.0)
{
    if (p0 <= 0.0) throw std::invalid_argument("p0 must be > 0");
    if (q  <  0.0) throw std::invalid_argument("q must be >= 0");
    if (r  <= 0.0) throw std::invalid_argument("r must be > 0");
}

void KalmanFilter::predict()
{
    // State transition F = 1 (constant-position model):
    //   x_pred = F * x  →  x_pred = x  (no change)
    //   P_pred = F * P * F^T + Q  →  P_pred = P + Q
    p_ += q_;
}

void KalmanFilter::update(double z)
{
    // Kalman gain:  K = P_pred * H^T / (H * P_pred * H^T + R)
    // With H = 1:   K = P / (P + R)
    k_ = p_ / (p_ + r_);

    // State update:       x = x_pred + K * (z - H * x_pred)
    x_ = x_ + k_ * (z - x_);

    // Covariance update:  P = (1 - K * H) * P_pred
    p_ = (1.0 - k_) * p_;
}
