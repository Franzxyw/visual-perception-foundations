#include "visual_perception/reprojection_error.hpp"

#include <Eigen/Core>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>

namespace {

constexpr double kTolerance = 1e-12;

bool isApprox(double actual, double expected) {
    return std::abs(actual - expected) < kTolerance;
}

bool isApprox(const Eigen::Vector2d& actual,
              const Eigen::Vector2d& expected) {
    return (actual - expected).norm() < kTolerance;
}

}  // namespace

int main() {
    const Eigen::Vector2d predicted_1(360.0, 282.0);
    const Eigen::Vector2d observed_1(363.0, 278.0);

    const Eigen::Vector2d residual =
        visual_perception::reprojectionResidual(predicted_1, observed_1);
    if (!isApprox(residual, Eigen::Vector2d(-3.0, 4.0))) {
        std::cerr << "single-point residual was incorrect\n";
        return EXIT_FAILURE;
    }

    if (!isApprox(
            visual_perception::reprojectionError(predicted_1, observed_1),
            5.0)) {
        std::cerr << "single-point reprojection error was incorrect\n";
        return EXIT_FAILURE;
    }

    const std::vector<Eigen::Vector2d> predicted{
        predicted_1,
        Eigen::Vector2d(520.0, 240.0)
    };
    const std::vector<Eigen::Vector2d> observed{
        observed_1,
        Eigen::Vector2d(518.0, 241.0)
    };

    const std::optional<double> rmse =
        visual_perception::reprojectionRmse(predicted, observed);

    if (!rmse.has_value() || !isApprox(*rmse, std::sqrt(15.0))) {
        std::cerr << "reprojection RMSE was incorrect\n";
        return EXIT_FAILURE;
    }

    const std::vector<Eigen::Vector2d> empty_points;
    if (visual_perception::reprojectionRmse(
            empty_points, empty_points).has_value()) {
        std::cerr << "empty input should not have an RMSE\n";
        return EXIT_FAILURE;
    }

    if (visual_perception::reprojectionRmse(
            predicted, empty_points).has_value()) {
        std::cerr << "mismatched input should not have an RMSE\n";
        return EXIT_FAILURE;
    }

    std::cout << "all reprojection-error checks passed\n";
    return EXIT_SUCCESS;
}
