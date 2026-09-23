#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

namespace {

struct BoardPose {
    cv::Vec3d rvec;
    cv::Vec3d tvec;
};

struct PoseError {
    double rotation_degrees;
    double translation_metres;
};

PoseError measurePoseError(const BoardPose& true_pose,
                           const cv::Mat& estimated_rvec,
                           const cv::Mat& estimated_tvec) {
    cv::Matx33d true_rotation;
    cv::Matx33d estimated_rotation;
    cv::Rodrigues(true_pose.rvec, true_rotation);
    cv::Rodrigues(estimated_rvec, estimated_rotation);

    const cv::Matx33d relative_rotation =
        estimated_rotation * true_rotation.t();
    // atan2(sin(theta), cos(theta)) retains tiny rotation errors that
    // matrix-to-Rodrigues conversion can round down to zero.
    const cv::Vec3d skew_part(
        relative_rotation(2, 1) - relative_rotation(1, 2),
        relative_rotation(0, 2) - relative_rotation(2, 0),
        relative_rotation(1, 0) - relative_rotation(0, 1)
    );
    const double sin_angle = 0.5 * cv::norm(skew_part);
    const double cos_angle = 0.5 * (
        relative_rotation(0, 0) + relative_rotation(1, 1) +
        relative_rotation(2, 2) - 1.0
    );
    constexpr double kRadiansToDegrees = 180.0 / 3.14159265358979323846;
    const double rotation_error_degrees =
        std::atan2(sin_angle, cos_angle) * kRadiansToDegrees;

    const cv::Vec3d estimated_translation(
        estimated_tvec.at<double>(0),
        estimated_tvec.at<double>(1),
        estimated_tvec.at<double>(2)
    );
    const double translation_error_metres =
        cv::norm(estimated_translation - true_pose.tvec);

    return {rotation_error_degrees, translation_error_metres};
}

}  // namespace

int main() {
    constexpr int kColumns = 7;
    constexpr int kRows = 6;
    constexpr float kSquareSizeMetres = 0.04F;
    constexpr float kPixelNoiseBound = 0.2F;
    const cv::Size image_size(640, 480);
    std::mt19937 noise_engine(20260923);
    std::uniform_real_distribution<float> pixel_noise(
        -kPixelNoiseBound, kPixelNoiseBound
    );

    // The board's own frame has its origin at the first inner corner.
    std::vector<cv::Point3f> board_points;
    board_points.reserve(kColumns * kRows);
    for (int row = 0; row < kRows; ++row) {
        for (int column = 0; column < kColumns; ++column) {
            board_points.emplace_back(
                column * kSquareSizeMetres,
                row * kSquareSizeMetres,
                0.0F
            );
        }
    }

    const cv::Matx33d true_camera_matrix(
        400.0, 0.0, 320.0,
        0.0, 420.0, 240.0,
        0.0, 0.0, 1.0
    );
    const cv::Vec<double, 5> true_dist_coeffs(-0.2, 0.0, 0.0, 0.0, 0.0);

    // Each pose maps the same board-frame points into the camera frame.
    const std::vector<BoardPose> poses{
        {cv::Vec3d(0.10, -0.08, 0.02), cv::Vec3d(-0.12, -0.10, 0.80)},
        {cv::Vec3d(-0.28, 0.15, 0.06), cv::Vec3d(-0.38, -0.25, 0.80)},
        {cv::Vec3d(0.20, 0.30, -0.12), cv::Vec3d(0.15, -0.24, 0.90)},
        {cv::Vec3d(-0.18, -0.25, 0.10), cv::Vec3d(-0.35, 0.08, 0.80)},
        {cv::Vec3d(0.35, -0.05, -0.15), cv::Vec3d(0.12, 0.08, 0.90)},
        {cv::Vec3d(-0.10, 0.18, 0.20), cv::Vec3d(-0.15, -0.10, 0.60)}
    };

    // calibrateCamera expects one 3D/2D correspondence set per view.
    std::vector<std::vector<cv::Point3f>> object_points_by_view;
    std::vector<std::vector<cv::Point2f>> image_points_by_view;
    std::vector<std::vector<cv::Point2f>> pixel_offsets_by_view;
    object_points_by_view.reserve(poses.size());
    image_points_by_view.reserve(poses.size());
    pixel_offsets_by_view.reserve(poses.size());

    std::cout << "board: " << kColumns << " x " << kRows
              << " inner corners, spacing " << kSquareSizeMetres << " m\n";
    std::cout << "pixel noise: uniform +/-" << kPixelNoiseBound
              << " px per coordinate, fixed seed\n";
    std::cout << "first three board points: " << board_points[0] << ' '
              << board_points[1] << ' ' << board_points[2] << '\n';

    for (std::size_t view = 0; view < poses.size(); ++view) {
        cv::Matx33d rotation_matrix;
        cv::Rodrigues(poses[view].rvec, rotation_matrix);
        for (const cv::Point3f& point_b : board_points) {
            const cv::Vec3d point_c = rotation_matrix * cv::Vec3d(
                point_b.x, point_b.y, point_b.z
            ) + poses[view].tvec;
            if (point_c[2] <= 0.0) {
                std::cerr << "view " << view << " has a point behind the camera\n";
                return EXIT_FAILURE;
            }
        }

        std::vector<cv::Point2f> pixels;
        cv::projectPoints(
            board_points,
            poses[view].rvec,
            poses[view].tvec,
            true_camera_matrix,
            true_dist_coeffs,
            pixels
        );
        if (pixels.size() != board_points.size()) {
            std::cerr << "view " << view << " has a point-count mismatch\n";
            return EXIT_FAILURE;
        }
        if (view == 0) {
            // Board origin maps to tvec=(-0.12, -0.10, 0.80). Projecting it
            // by hand with k1=-0.2 gives (260.4575, 187.9003125) px.
            const cv::Point2d expected_first_pixel(260.4575, 187.9003125);
            const double error = cv::norm(
                cv::Point2d(pixels.front()) - expected_first_pixel
            );
            if (error > 1e-3) {
                std::cerr << "first corner disagrees with the hand calculation\n";
                return EXIT_FAILURE;
            }
        }

        // Save each perturbation so the weak-pose case uses exactly the same
        // offset for the corresponding corner in the corresponding view.
        std::vector<cv::Point2f> pixel_offsets;
        pixel_offsets.reserve(pixels.size());
        for (cv::Point2f& pixel : pixels) {
            const float offset_u = pixel_noise(noise_engine);
            const float offset_v = pixel_noise(noise_engine);
            const cv::Point2f offset(offset_u, offset_v);
            pixel_offsets.push_back(offset);
            pixel.x += offset.x;
            pixel.y += offset.y;
        }

        float min_u = pixels[0].x;
        float max_u = pixels[0].x;
        float min_v = pixels[0].y;
        float max_v = pixels[0].y;
        for (const cv::Point2f& pixel : pixels) {
            min_u = std::min(min_u, pixel.x);
            max_u = std::max(max_u, pixel.x);
            min_v = std::min(min_v, pixel.y);
            max_v = std::max(max_v, pixel.y);
            if (pixel.x < 0.0F || pixel.x >= image_size.width ||
                pixel.y < 0.0F || pixel.y >= image_size.height) {
                std::cerr << "view " << view << " has a point outside the image\n";
                return EXIT_FAILURE;
            }
        }

        object_points_by_view.push_back(board_points);
        image_points_by_view.push_back(pixels);
        pixel_offsets_by_view.push_back(pixel_offsets);
        std::cout << "view " << view << ": " << pixels.size()
                  << " correspondences; first observed pixel " << pixels.front()
                  << "; u in [" << min_u << ", " << max_u
                  << "], v in [" << min_v << ", " << max_v << "]\n";
    }

    std::cout << "dataset: " << object_points_by_view.size()
              << " views x " << board_points.size() << " points = "
              << poses.size() * board_points.size()
              << " 3D/2D correspondences\n";

    cv::Mat estimated_camera_matrix;
    cv::Mat estimated_dist_coeffs;
    std::vector<cv::Mat> estimated_rvecs;
    std::vector<cv::Mat> estimated_tvecs;

    const double rms = cv::calibrateCamera(
        object_points_by_view,
        image_points_by_view,
        image_size,
        estimated_camera_matrix,
        estimated_dist_coeffs,
        estimated_rvecs,
        estimated_tvecs
    );
    std::cout << "estimated K:\n" << estimated_camera_matrix << '\n';
    std::cout << "estimated distortion:\n" << estimated_dist_coeffs << '\n';
    std::cout << "calibration RMS: " << rms << " px\n";
    std::cout << "estimated poses: " << estimated_rvecs.size() << '\n';

    if (estimated_rvecs.size() != poses.size() ||
        estimated_tvecs.size() != poses.size()) {
        std::cerr << "calibration returned the wrong number of poses\n";
        return EXIT_FAILURE;
    }

    for (std::size_t view = 0; view < poses.size(); ++view) {
        const PoseError error = measurePoseError(
            poses[view], estimated_rvecs[view], estimated_tvecs[view]
        );
        std::cout << "pose " << view
                  << ": rotation error " << error.rotation_degrees << " deg"
                  << ", translation error " << error.translation_metres
                  << " m\n";
    }

    std::cout << "fx error: "
              << estimated_camera_matrix.at<double>(0, 0)
                     - true_camera_matrix(0, 0)
              << " px\n";
    std::cout << "fy error: "
              << estimated_camera_matrix.at<double>(1, 1)
                     - true_camera_matrix(1, 1)
              << " px\n";
    std::cout << "cx error: "
              << estimated_camera_matrix.at<double>(0, 2)
                     - true_camera_matrix(0, 2)
              << " px\n";
    std::cout << "cy error: "
              << estimated_camera_matrix.at<double>(1, 2)
                     - true_camera_matrix(1, 2)
              << " px\n";
    std::cout << "k1 error: "
              << estimated_dist_coeffs.at<double>(0, 0)
                     - true_dist_coeffs[0]
              << '\n';
    double squared_error_sum = 0.0;
    std::size_t point_count = 0;

    for (std::size_t view = 0; view < object_points_by_view.size(); ++view) {
        std::vector<cv::Point3d> double_board_points;
        double_board_points.reserve(object_points_by_view[view].size());
        for (const cv::Point3f& point_b : object_points_by_view[view]) {
            double_board_points.emplace_back(point_b.x, point_b.y, point_b.z);
        }

        std::vector<cv::Point2d> predicted_pixels;
        cv::projectPoints(
            double_board_points,
            estimated_rvecs[view],
            estimated_tvecs[view],
            estimated_camera_matrix,
            estimated_dist_coeffs,
            predicted_pixels
        );

        for (std::size_t point = 0; point < predicted_pixels.size(); ++point) {
            const cv::Point2d observed_pixel(
                image_points_by_view[view][point]
            );
            const cv::Point2d residual =
                predicted_pixels[point] - observed_pixel;

            squared_error_sum += residual.dot(residual);
            ++point_count;
        }
    }

    const double checked_rms =
        std::sqrt(squared_error_sum / static_cast<double>(point_count));

    std::cout << "recomputed RMS: " << checked_rms << " px\n";
    std::cout << "RMS difference: " << checked_rms - rms << " px\n";

    // Case B changes only the board rotations. Board points, translations,
    // camera truth, image size, and per-corner pixel offsets stay the same.
    constexpr double kWeakRotationScale = 0.01;
    std::vector<BoardPose> weak_poses = poses;
    for (BoardPose& pose : weak_poses) {
        pose.rvec *= kWeakRotationScale;
    }

    std::vector<std::vector<cv::Point2f>> weak_image_points_by_view;
    weak_image_points_by_view.reserve(weak_poses.size());
    std::cout << "case B: board rotations scaled by " << kWeakRotationScale
              << "; translations and pixel offsets unchanged\n";
    for (std::size_t view = 0; view < weak_poses.size(); ++view) {
        std::vector<cv::Point2f> weak_pixels;
        cv::projectPoints(
            board_points,
            weak_poses[view].rvec,
            weak_poses[view].tvec,
            true_camera_matrix,
            true_dist_coeffs,
            weak_pixels
        );
        if (weak_pixels.size() != pixel_offsets_by_view[view].size()) {
            std::cerr << "case B point-count mismatch in view " << view << '\n';
            return EXIT_FAILURE;
        }
        for (std::size_t point = 0; point < weak_pixels.size(); ++point) {
            weak_pixels[point] += pixel_offsets_by_view[view][point];
            const cv::Point2f& pixel = weak_pixels[point];
            if (pixel.x < 0.0F || pixel.x >= image_size.width ||
                pixel.y < 0.0F || pixel.y >= image_size.height) {
                std::cerr << "case B point outside image in view " << view
                          << '\n';
                return EXIT_FAILURE;
            }
        }
        std::cout << "case B view " << view << ": first observed pixel "
                  << weak_pixels.front() << '\n';
        weak_image_points_by_view.push_back(weak_pixels);
    }

    cv::Mat weak_camera_matrix;
    cv::Mat weak_dist_coeffs;
    std::vector<cv::Mat> weak_rvecs;
    std::vector<cv::Mat> weak_tvecs;
    const double weak_rms = cv::calibrateCamera(
        object_points_by_view,
        weak_image_points_by_view,
        image_size,
        weak_camera_matrix,
        weak_dist_coeffs,
        weak_rvecs,
        weak_tvecs
    );
    if (weak_rvecs.size() != weak_poses.size() ||
        weak_tvecs.size() != weak_poses.size()) {
        std::cerr << "case B returned the wrong number of poses\n";
        return EXIT_FAILURE;
    }

    std::cout << "case B estimated K:\n" << weak_camera_matrix << '\n';
    std::cout << "case B estimated distortion:\n" << weak_dist_coeffs
              << '\n';
    std::cout << "case B calibration RMS: " << weak_rms << " px\n";
    for (std::size_t view = 0; view < weak_poses.size(); ++view) {
        const PoseError error = measurePoseError(
            weak_poses[view], weak_rvecs[view], weak_tvecs[view]
        );
        std::cout << "case B pose " << view
                  << ": rotation error " << error.rotation_degrees << " deg"
                  << ", translation error " << error.translation_metres
                  << " m\n";
    }

    const double fx_error = std::abs(
        estimated_camera_matrix.at<double>(0, 0) - true_camera_matrix(0, 0)
    );
    const double weak_fx_error = std::abs(
        weak_camera_matrix.at<double>(0, 0) - true_camera_matrix(0, 0)
    );
    std::cout << "A/B reprojection RMS: " << rms << " / " << weak_rms
              << " px\n";
    std::cout << "A/B absolute fx error: " << fx_error << " / " << weak_fx_error
              << " px\n";

    return EXIT_SUCCESS;
}
