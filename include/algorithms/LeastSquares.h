/**
 * @file LeastSquares.h
 * @brief Ordinary Least Squares (OLS) linear regression
 *
 * Fits y = slope * x + intercept using the closed-form normal equations:
 *
 *   slope     = (n * Σ(xi*yi) - Σxi * Σyi) / (n * Σ(xi²) - (Σxi)²)
 *   intercept = (Σyi - slope * Σxi) / n
 *   R²        = 1 - SS_res / SS_tot
 */
#pragma once

#include <vector>
#include <stdexcept>

/// Result returned by LeastSquares::fit()
struct LsqResult {
    double slope;       ///< Regression slope
    double intercept;   ///< Regression intercept
    double r_squared;   ///< Coefficient of determination (0..1)
};

class LeastSquares {
public:
    /**
     * Fit a linear model to the given (x, y) data points.
     * @throws std::invalid_argument  if fewer than 2 points, or x/y sizes differ
     * @throws std::runtime_error     if all x values are identical (degenerate)
     */
    LsqResult fit(const std::vector<double>& x, const std::vector<double>& y);

    /**
     * Predict y for a given x using the fitted model.
     * @throws std::logic_error if fit() has not been called first
     */
    double predict(double x) const;

    bool isFitted() const noexcept { return fitted_; }

private:
    double slope_    {0.0};
    double intercept_{0.0};
    bool   fitted_   {false};
};
