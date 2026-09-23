#include "visual_perception/pinhole_camera.hpp"
#include "visual_perception/radial_distortion.hpp"

#include <Eigen/Core>

#include <iostream>

int main() {
    const visual_perception::PinholeCamera camera{
        400.0,
        420.0,
        320.0,
        240.0
    };
    const visual_perception::RadialDistortion distortion{-0.2};
    const Eigen::Vector2d ideal_normalized(0.5, 0.0);
    const Eigen::Vector2d distorted_normalized =
        distortion.distortNormalized(ideal_normalized);

    const Eigen::Vector2d ideal_pixel =
        camera.normalizedToPixel(ideal_normalized);
    const Eigen::Vector2d distorted_pixel =
        camera.normalizedToPixel(distorted_normalized);

    std::cout << "ideal normalized: "
              << ideal_normalized.transpose() << '\n';
    std::cout << "distorted normalized: "
              << distorted_normalized.transpose() << '\n';
    std::cout << "ideal pixel: " << ideal_pixel.transpose() << '\n';
    std::cout << "distorted pixel: " << distorted_pixel.transpose() << '\n';
    return 0;
}
