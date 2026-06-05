/**
 * @file KalmanFilter.h
 * @brief Discrete 1D Kalman Filter (Constant-Position Model)
 *
 * Implements the MAR-specified discrete Kalman filter equations:
 *
 *   Predict:  P_pred = P + Q
 *   Update:   K = P_pred / (P_pred + R)
 *             x = x_pred + K * (z - x_pred)
 *             P = (1 - K) * P_pred
 *
 * State: scalar position estimate x
 * State transition: F = 1 (constant-position model)
 * Measurement matrix: H = 1 (direct observation)
 */
#pragma once

class KalmanFilter {
public:
    /**
     * @param x0  Initial state estimate
     * @param p0  Initial state covariance  (must be > 0)
     * @param q   Process noise covariance  (must be >= 0)
     * @param r   Measurement noise covariance (must be > 0)
     * @throws std::invalid_argument if any constraint is violated
     */
    KalmanFilter(double x0, double p0, double q, double r);

    /// Prediction step: propagate state and grow covariance by Q.
    void predict();

    /// Update step: incorporate measurement z, compute Kalman gain.
    void update(double z);

    double getState()       const noexcept { return x_; }
    double getCovariance()  const noexcept { return p_; }
    double getKalmanGain()  const noexcept { return k_; }

private:
    double x_;  ///< State estimate
    double p_;  ///< State covariance
    double q_;  ///< Process noise covariance
    double r_;  ///< Measurement noise covariance
    double k_;  ///< Last computed Kalman gain
};
