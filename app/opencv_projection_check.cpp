#include "visual_perception/pinhole_camera.hpp"
#include "visual_perception/radial_distortion.hpp"
#include "visual_perception/reprojection_error.hpp"

#include <Eigen/Core>

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

#include <cstdlib>
#include <iostream>
#include <vector>

int main() {
    constexpr double kTolerance = 1e-10;

    const visual_perception::PinholeCamera camera{
        400.0, 420.0, 320.0, 240.0
    };
    const visual_perception::RadialDistortion distortion{-0.2};
    const Eigen::Vector3d point_c(1.0, 0.0, 2.0);

    const Eigen::Vector2d ideal_normalized(
        point_c.x() / point_c.z(),
        point_c.y() / point_c.z()
    );
    const Eigen::Vector2d distorted_normalized =
        distortion.distortNormalized(ideal_normalized);
    const Eigen::Vector2d own_pixel =
        camera.normalizedToPixel(distorted_normalized);

    const std::vector<cv::Point3d> object_points{
        cv::Point3d(point_c.x(), point_c.y(), point_c.z())
    };
    const cv::Vec3d rvec(0.0, 0.0, 0.0);
    const cv::Vec3d tvec(0.0, 0.0, 0.0);
    const cv::Matx33d camera_matrix(
        camera.fx, 0.0, camera.cx,
        0.0, camera.fy, camera.cy,
        0.0, 0.0, 1.0
    );
    // OpenCV's first five coefficients are k1, k2, p1, p2, k3.
    const cv::Vec<double, 5> dist_coeffs(distortion.k1, 0.0, 0.0, 0.0, 0.0);
    std::vector<cv::Point2d> opencv_pixels;
    cv::projectPoints(
        object_points, rvec, tvec, camera_matrix, dist_coeffs, opencv_pixels
    );

    if (opencv_pixels.size() != 1) {
        std::cerr << "OpenCV did not return one pixel\n";
        return EXIT_FAILURE;
    }

    const Eigen::Vector2d opencv_pixel(
        opencv_pixels[0].x,
        opencv_pixels[0].y
    );
    const Eigen::Vector2d expected_pixel(510.0, 240.0);
    const double own_error =
        visual_perception::reprojectionError(own_pixel, expected_pixel);
    const double opencv_error =
        visual_perception::reprojectionError(opencv_pixel, expected_pixel);

    std::cout << "p_C: " << point_c.transpose() << '\n';
    std::cout << "ideal normalized: " << ideal_normalized.transpose() << '\n';
    std::cout << "distorted normalized: "
              << distorted_normalized.transpose() << '\n';
    std::cout << "expected pixel: " << expected_pixel.transpose() << '\n';
    std::cout << "own pixel: " << own_pixel.transpose() << '\n';
    std::cout << "OpenCV pixel: " << opencv_pixel.transpose() << '\n';
    std::cout << "own error: " << own_error << " px\n";
    std::cout << "OpenCV error: " << opencv_error << " px\n";

    if (own_error > kTolerance || opencv_error > kTolerance) {
        std::cerr << "projection disagrees with the hand-calculated pixel\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
