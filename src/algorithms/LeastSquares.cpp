#include "algorithms/LeastSquares.h"
#include <cmath>
#include <numeric>

LsqResult LeastSquares::fit(const std::vector<double>& x, const std::vector<double>& y)
{
    if (x.size() < 2)
        throw std::invalid_argument("At least 2 data points are required");
    if (x.size() != y.size())
        throw std::invalid_argument("x and y vectors must have the same length");

    const auto n = static_cast<double>(x.size());

    double sum_x  = std::accumulate(x.begin(), x.end(), 0.0);
    double sum_y  = std::accumulate(y.begin(), y.end(), 0.0);
    double sum_xy = 0.0;
    double sum_xx = 0.0;

    for (std::size_t i = 0; i < x.size(); ++i) {
        sum_xy += x[i] * y[i];
        sum_xx += x[i] * x[i];
    }

    // Normal equations (closed form)
    const double denom = n * sum_xx - sum_x * sum_x;
    if (std::abs(denom) < 1e-15)
        throw std::runtime_error("Degenerate input: all x values are identical");

    slope_     = (n * sum_xy - sum_x * sum_y) / denom;
    intercept_ = (sum_y - slope_ * sum_x) / n;
    fitted_    = true;

    // Coefficient of determination R²
    const double mean_y = sum_y / n;
    double ss_tot = 0.0;
    double ss_res = 0.0;
    for (std::size_t i = 0; i < x.size(); ++i) {
        const double diff_tot = y[i] - mean_y;
        const double diff_res = y[i] - (slope_ * x[i] + intercept_);
        ss_tot += diff_tot * diff_tot;
        ss_res += diff_res * diff_res;
    }
    const double r_sq = (ss_tot < 1e-15) ? 1.0 : (1.0 - ss_res / ss_tot);

    return LsqResult{slope_, intercept_, r_sq};
}

double LeastSquares::predict(double x) const
{
    if (!fitted_)
        throw std::logic_error("Model has not been fitted; call fit() first");
    return slope_ * x + intercept_;
}
