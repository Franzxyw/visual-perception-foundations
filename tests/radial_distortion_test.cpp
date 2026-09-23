#include "visual_perception/pinhole_camera.hpp"
#include "visual_perception/radial_distortion.hpp"

#include <Eigen/Core>

#include <cstdlib>
#include <iostream>

namespace {

constexpr double kTolerance = 1e-12;

bool isApprox(const Eigen::Vector2d& actual,
              const Eigen::Vector2d& expected) {
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
    const visual_perception::RadialDistortion barrel_distortion{-0.2};

    const Eigen::Vector2d center(0.0, 0.0);
    if (!isApprox(barrel_distortion.distortNormalized(center), center)) {
        std::cerr << "principal point was changed by radial distortion\n";
        return EXIT_FAILURE;
    }

    const Eigen::Vector2d ideal_normalized(0.5, 0.0);
    const Eigen::Vector2d expected_distorted_normalized(0.475, 0.0);
    const Eigen::Vector2d distorted_normalized =
        barrel_distortion.distortNormalized(ideal_normalized);
    if (!isApprox(distorted_normalized, expected_distorted_normalized)) {
        std::cerr << "known radial-distortion result was incorrect\n";
        return EXIT_FAILURE;
    }

    const Eigen::Vector2d ideal_pixel =
        camera.normalizedToPixel(ideal_normalized);
    const Eigen::Vector2d distorted_pixel =
        camera.normalizedToPixel(distorted_normalized);
    if (!isApprox(ideal_pixel, Eigen::Vector2d(520.0, 240.0)) ||
        !isApprox(distorted_pixel, Eigen::Vector2d(510.0, 240.0))) {
        std::cerr << "ideal or distorted pixel was incorrect\n";
        return EXIT_FAILURE;
    }

    const visual_perception::RadialDistortion pincushion_distortion{0.2};
    const Eigen::Vector2d outward_normalized =
        pincushion_distortion.distortNormalized(ideal_normalized);
    if (outward_normalized.norm() <= ideal_normalized.norm()) {
        std::cerr << "positive k1 did not move the point away from center\n";
        return EXIT_FAILURE;
    }

    const Eigen::Vector2d near_center(0.1, 0.0);
    const Eigen::Vector2d near_displacement =
        barrel_distortion.distortNormalized(near_center) - near_center;
    const Eigen::Vector2d far_displacement =
        distorted_normalized - ideal_normalized;
    if (far_displacement.norm() <= near_displacement.norm()) {
        std::cerr << "radial displacement did not grow away from center\n";
        return EXIT_FAILURE;
    }

    std::cout << "all radial-distortion checks passed\n";
    return EXIT_SUCCESS;
}
