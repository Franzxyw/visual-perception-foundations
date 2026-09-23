#include "visual_perception/pinhole_camera.hpp"

#include <Eigen/Core>

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
    const visual_perception::PinholeCamera camera{
        400.0,
        420.0,
        320.0,
        240.0
    };
    const Eigen::Vector2d pixel(420.0, 198.0);

    const Eigen::Vector3d ray_c = camera.backProjectRay(pixel);
    const Eigen::Vector3d expected_ray_c(0.25, -0.1, 1.0);
    if (!isApprox(ray_c, expected_ray_c)) {
        std::cerr << "back-projected camera ray was incorrect\n";
        return EXIT_FAILURE;
    }

    const std::optional<Eigen::Vector3d> point_c =
        camera.backProject(pixel, 4.0);
    const Eigen::Vector3d expected_point_c(1.0, -0.4, 4.0);
    if (!point_c.has_value() || !isApprox(*point_c, expected_point_c)) {
        std::cerr << "back-projected 3D point was incorrect\n";
        return EXIT_FAILURE;
    }

    const std::optional<Eigen::Vector2d> round_trip_pixel =
        camera.project(*point_c);
    if (!round_trip_pixel.has_value() ||
        !isApprox(*round_trip_pixel, pixel)) {
        std::cerr << "projection/back-projection round trip failed\n";
        return EXIT_FAILURE;
    }

    if (camera.backProject(pixel, 0.0).has_value()) {
        std::cerr << "zero z-depth unexpectedly produced a 3D point\n";
        return EXIT_FAILURE;
    }

    if (camera.backProject(pixel, -1.0).has_value()) {
        std::cerr << "negative z-depth unexpectedly produced a 3D point\n";
        return EXIT_FAILURE;
    }

    std::cout << "all pinhole back-projection checks passed\n";
    return EXIT_SUCCESS;
}
