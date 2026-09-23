#include "visual_perception/pinhole_camera.hpp"

#include <Eigen/Core>

#include <array>
#include <cstdlib>
#include <iostream>
#include <optional>

namespace {

constexpr double kTolerance = 1e-12;

bool isApprox(const Eigen::Vector2d& actual,
              const Eigen::Vector2d& expected) {
    return (actual - expected).norm() < kTolerance;
}

struct ProjectionCase {
    const char* name;
    Eigen::Vector3d point_c;
    Eigen::Vector2d expected_pixel;
};

}  // namespace

int main() {
    const visual_perception::PinholeCamera camera{
        400.0,
        420.0,
        320.0,
        240.0
    };

    const std::array<ProjectionCase, 3> cases{{
        {
            "principal-axis point",
            Eigen::Vector3d(0.0, 0.0, 2.0),
            Eigen::Vector2d(320.0, 240.0)
        },
        {
            "known projection",
            Eigen::Vector3d(0.5, -0.2, 2.0),
            Eigen::Vector2d(420.0, 198.0)
        },
        {
            "negative-x positive-y point",
            Eigen::Vector3d(-0.4, 0.3, 2.0),
            Eigen::Vector2d(240.0, 303.0)
        },
    }};

    for (const auto& test_case : cases) {
        const std::optional<Eigen::Vector2d> pixel =
            camera.project(test_case.point_c);

        if (!pixel.has_value()) {
            std::cerr << test_case.name << " unexpectedly had no projection\n";
            return EXIT_FAILURE;
        }

        if (!isApprox(*pixel, test_case.expected_pixel)) {
            std::cerr << test_case.name << " projected to the wrong pixel\n";
            return EXIT_FAILURE;
        }
    }

    const std::optional<Eigen::Vector2d> original_pixel =
        camera.project(Eigen::Vector3d(0.5, -0.2, 2.0));
    const std::optional<Eigen::Vector2d> scaled_pixel =
        camera.project(Eigen::Vector3d(1.5, -0.6, 6.0));

    if (!original_pixel.has_value() || !scaled_pixel.has_value() ||
        !isApprox(*original_pixel, *scaled_pixel)) {
        std::cerr << "scaling along a camera ray changed the pixel\n";
        return EXIT_FAILURE;
    }

    if (camera.project(Eigen::Vector3d(1.0, 2.0, 0.0)).has_value()) {
        std::cerr << "zero-depth point unexpectedly had a projection\n";
        return EXIT_FAILURE;
    }

    if (camera.project(Eigen::Vector3d(1.0, 2.0, -1.0)).has_value()) {
        std::cerr << "behind-camera point unexpectedly had a projection\n";
        return EXIT_FAILURE;
    }

    std::cout << "all pinhole projection checks passed\n";
    return EXIT_SUCCESS;
}
