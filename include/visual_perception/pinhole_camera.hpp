#pragma once

#include <Eigen/Core>

#include <optional>

namespace visual_perception {

struct PinholeCamera {
    double fx;
    double fy;
    double cx;
    double cy;

    Eigen::Matrix3d intrinsicMatrix() const {
        Eigen::Matrix3d intrinsic_matrix;
        intrinsic_matrix <<
            fx, 0.0, cx,
            0.0, fy, cy,
            0.0, 0.0, 1.0;
        return intrinsic_matrix;
    }

    Eigen::Vector2d normalizedToPixel(
        const Eigen::Vector2d& normalized) const {
        return Eigen::Vector2d(
            fx * normalized.x() + cx,
            fy * normalized.y() + cy
        );
    }

    Eigen::Vector2d pixelToNormalized(const Eigen::Vector2d& pixel) const {
        return Eigen::Vector2d(
            (pixel.x() - cx) / fx,
            (pixel.y() - cy) / fy
        );
    }

    std::optional<Eigen::Vector2d> project(
        const Eigen::Vector3d& point_c) const {
        if (point_c.z() <= 0.0) {
            return std::nullopt;
        }

        const double x_normalized = point_c.x() / point_c.z();
        const double y_normalized = point_c.y() / point_c.z();

        return normalizedToPixel(
            Eigen::Vector2d(x_normalized, y_normalized)
        );
    }

    Eigen::Vector3d backProjectRay(const Eigen::Vector2d& pixel) const {
        const Eigen::Vector2d normalized = pixelToNormalized(pixel);
        return Eigen::Vector3d(
            normalized.x(),
            normalized.y(),
            1.0
        );
    }

    std::optional<Eigen::Vector3d> backProject(
        const Eigen::Vector2d& pixel,
        double z_depth) const {
        if (z_depth <= 0.0) {
            return std::nullopt;
        }

        return z_depth * backProjectRay(pixel);
    }
};

}  // namespace visual_perception
