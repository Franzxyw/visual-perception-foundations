#include "visual_perception/frame_transform.hpp"
#include "visual_perception/pinhole_camera.hpp"

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <cstdlib>
#include <iostream>
#include <optional>

namespace {

constexpr double kTolerance = 1e-12;

bool isApprox(const Eigen::Vector2d& actual,
              const Eigen::Vector2d& expected) {
    return (actual - expected).norm() < kTolerance;
}

bool isApprox(const Eigen::Vector3d& actual,
              const Eigen::Vector3d& expected) {
    return (actual - expected).norm() < kTolerance;
}

}  // namespace

int main() {
    constexpr double kPi = 3.14159265358979323846;

    const visual_perception::PinholeCamera camera{
        400.0,
        420.0,
        320.0,
        240.0
    };
    const Eigen::Vector3d point_w(1.0, 0.0, 4.0);
    const Eigen::Isometry3d t_cw =
        visual_perception::makeRigidTransform(
            Eigen::AngleAxisd(kPi / 2.0, Eigen::Vector3d::UnitZ()),
            Eigen::Vector3d(0.5, -0.5, 1.0)
        );

    const Eigen::Vector3d point_c = t_cw * point_w;
    const Eigen::Vector3d expected_point_c(0.5, 0.5, 5.0);
    if (!isApprox(point_c, expected_point_c)) {
        std::cerr << "T_CW produced the wrong camera-frame point\n";
        return EXIT_FAILURE;
    }

    const std::optional<Eigen::Vector2d> projected_pixel =
        camera.project(point_c);
    const Eigen::Vector2d expected_pixel(360.0, 282.0);
    if (!projected_pixel.has_value() ||
        !isApprox(*projected_pixel, expected_pixel)) {
        std::cerr << "camera-frame point projected to the wrong pixel\n";
        return EXIT_FAILURE;
    }

    const Eigen::Vector3d homogeneous_pixel =
        camera.intrinsicMatrix() * point_c;
    const Eigen::Vector2d matrix_pixel(
        homogeneous_pixel.x() / homogeneous_pixel.z(),
        homogeneous_pixel.y() / homogeneous_pixel.z()
    );
    if (!isApprox(matrix_pixel, *projected_pixel)) {
        std::cerr << "K matrix and scalar projection disagreed\n";
        return EXIT_FAILURE;
    }

    const Eigen::Vector3d recovered_point_w = t_cw.inverse() * point_c;
    if (!isApprox(recovered_point_w, point_w)) {
        std::cerr << "inverse extrinsic did not recover the world point\n";
        return EXIT_FAILURE;
    }

    std::cout << "all intrinsic/extrinsic projection checks passed\n";
    return EXIT_SUCCESS;
}
