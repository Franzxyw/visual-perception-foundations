#include "visual_perception/pinhole_camera.hpp"

#include <Eigen/Core>

#include <cstdlib>
#include <iostream>

int main() {
    const visual_perception::PinholeCamera camera{
        400.0,
        420.0,
        320.0,
        240.0
    };
    const Eigen::Vector3d point_c(0.5, -0.2, 2.0);

    const std::optional<Eigen::Vector2d> pixel = camera.project(point_c);
    if (!pixel.has_value()) {
        std::cerr << "point cannot be projected\n";
        return EXIT_FAILURE;
    }

    const Eigen::Vector3d scaled_point_c = 3.0 * point_c;
    const std::optional<Eigen::Vector2d> scaled_pixel =
        camera.project(scaled_point_c);
    if (!scaled_pixel.has_value()) {
        std::cerr << "scaled point cannot be projected\n";
        return EXIT_FAILURE;
    }

    const Eigen::Vector3d ray_c = camera.backProjectRay(*pixel);
    const std::optional<Eigen::Vector3d> recovered_point_c =
        camera.backProject(*pixel, point_c.z());
    if (!recovered_point_c.has_value()) {
        std::cerr << "pixel cannot be back-projected at the requested depth\n";
        return EXIT_FAILURE;
    }

    std::cout << "p_C: " << point_c.transpose() << '\n';
    std::cout << "pixel: " << pixel->transpose() << '\n';
    std::cout << "scaled p_C: " << scaled_point_c.transpose() << '\n';
    std::cout << "scaled pixel: " << scaled_pixel->transpose() << '\n';
    std::cout << "ray_C: " << ray_c.transpose() << '\n';
    std::cout << "recovered p_C: "
              << recovered_point_c->transpose() << '\n';
    return EXIT_SUCCESS;
}
