#include "visual_perception/frame_transform.hpp"
#include "visual_perception/pinhole_camera.hpp"
#include "visual_perception/reprojection_error.hpp"

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <cstdlib>
#include <iostream>
#include <optional>

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
    const std::optional<Eigen::Vector2d> pixel = camera.project(point_c);
    if (!pixel.has_value()) {
        std::cerr << "world point is not visible in front of the camera\n";
        return EXIT_FAILURE;
    }

    const Eigen::Vector2d observed_pixel(363.0, 278.0);

    const Eigen::Vector2d residual =
        visual_perception::reprojectionResidual(*pixel, observed_pixel);
    const double error =
        visual_perception::reprojectionError(*pixel, observed_pixel);

    const Eigen::Matrix3d intrinsic_matrix = camera.intrinsicMatrix();
    const Eigen::Vector3d homogeneous_pixel = intrinsic_matrix * point_c;
    const Eigen::Vector2d matrix_pixel(
        homogeneous_pixel.x() / homogeneous_pixel.z(),
        homogeneous_pixel.y() / homogeneous_pixel.z()
    );

    std::cout << "p_W: " << point_w.transpose() << '\n';
    std::cout << "p_C: " << point_c.transpose() << '\n';
    std::cout << "observed pixel: " << observed_pixel.transpose() << '\n';
    std::cout << "residual: " << residual.transpose() << '\n';
    std::cout << "reprojection error: " << error << " px\n";
    std::cout << "K:\n" << intrinsic_matrix << '\n';
    std::cout << "project() pixel: " << pixel->transpose() << '\n';
    std::cout << "K * p_C pixel: " << matrix_pixel.transpose() << '\n';
    return EXIT_SUCCESS;
}
