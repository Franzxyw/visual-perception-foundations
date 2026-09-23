#pragma once

#include <Eigen/Core>

namespace visual_perception {

struct RadialDistortion {
    double k1;

    Eigen::Vector2d distortNormalized(
        const Eigen::Vector2d& normalized) const {
        const double radius_squared = normalized.squaredNorm();
        const double radial_scale = 1.0 + k1 * radius_squared;
        return radial_scale * normalized;
    }
};

}  // namespace visual_perception
