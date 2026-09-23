#pragma once

#include <Eigen/Core>

#include <cmath>
#include <optional>
#include <vector>

namespace visual_perception {

inline Eigen::Vector2d reprojectionResidual(
    const Eigen::Vector2d& predicted,
    const Eigen::Vector2d& observed) {
    return predicted - observed;
}

inline double reprojectionError(
    const Eigen::Vector2d& predicted,
    const Eigen::Vector2d& observed) {
    const Eigen::Vector2d residual =
        reprojectionResidual(predicted, observed);
    return residual.norm();
}

inline std::optional<double> reprojectionRmse(
    const std::vector<Eigen::Vector2d>& predicted,
    const std::vector<Eigen::Vector2d>& observed) {
    if (predicted.empty() || predicted.size() != observed.size()) {
        return std::nullopt;
    }

    double squared_error_sum = 0.0;

    for (std::size_t index = 0; index < predicted.size(); ++index) {
        const Eigen::Vector2d residual =
            reprojectionResidual(predicted[index], observed[index]);
        squared_error_sum += residual.squaredNorm();
    }

    return std::sqrt(
        squared_error_sum / static_cast<double>(predicted.size())
    );
}

}  // namespace visual_perception