#include <Eigen/Core>
#include <Eigen/Geometry>

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace {

constexpr double kTolerance = 1e-12;

bool isApprox(const Eigen::Vector3d& actual,
              const Eigen::Vector3d& expected) {
    return (actual - expected).norm() < kTolerance;
}

bool isApprox(const Eigen::Matrix3d& actual,
              const Eigen::Matrix3d& expected) {
    return (actual - expected).norm() < kTolerance;
}

bool isApprox(double actual, double expected) {
    return std::abs(actual - expected) < kTolerance;
}

}  // namespace

int main() {
    constexpr double kPi = 3.14159265358979323846;

    Eigen::Matrix3d matrix_rotation;
    matrix_rotation <<
         0.0, -1.0, 0.0,
         1.0,  0.0, 0.0,
         0.0,  0.0, 1.0;

    const Eigen::AngleAxisd angle_axis(
        kPi / 2.0,
        Eigen::Vector3d::UnitZ()
    );
    const Eigen::Matrix3d angle_axis_rotation = angle_axis.toRotationMatrix();

    const Eigen::Quaterniond quaternion(angle_axis);
    const Eigen::Matrix3d quaternion_rotation = quaternion.toRotationMatrix();

    if (!isApprox(angle_axis_rotation, matrix_rotation)) {
        std::cerr << "angle-axis and matrix rotations differed\n";
        return EXIT_FAILURE;
    }

    if (!isApprox(quaternion_rotation, matrix_rotation)) {
        std::cerr << "quaternion and matrix rotations differed\n";
        return EXIT_FAILURE;
    }

    const Eigen::Vector3d input(1.0, 0.0, 0.0);
    const Eigen::Vector3d expected(0.0, 1.0, 0.0);

    if (!isApprox(matrix_rotation * input, expected) ||
        !isApprox(angle_axis * input, expected) ||
        !isApprox(quaternion * input, expected)) {
        std::cerr << "rotation representations transformed the vector differently\n";
        return EXIT_FAILURE;
    }

    const Eigen::Quaterniond negative_quaternion(
        -quaternion.w(),
        -quaternion.x(),
        -quaternion.y(),
        -quaternion.z()
    );

    if (!isApprox(negative_quaternion.toRotationMatrix(), quaternion_rotation)) {
        std::cerr << "q and -q represented different rotations\n";
        return EXIT_FAILURE;
    }

    const Eigen::Quaterniond scaled_quaternion(
        2.0 * quaternion.w(),
        2.0 * quaternion.x(),
        2.0 * quaternion.y(),
        2.0 * quaternion.z()
    );
    const Eigen::Quaterniond normalized_quaternion =
        scaled_quaternion.normalized();

    if (!isApprox(normalized_quaternion.norm(), 1.0)) {
        std::cerr << "normalized quaternion did not have unit norm\n";
        return EXIT_FAILURE;
    }

    if (!isApprox(normalized_quaternion.toRotationMatrix(), quaternion_rotation)) {
        std::cerr << "normalization changed the represented rotation\n";
        return EXIT_FAILURE;
    }

    const double expected_half_angle_component = std::sqrt(0.5);
    if (!isApprox(quaternion.w(), expected_half_angle_component) ||
        !isApprox(quaternion.x(), 0.0) ||
        !isApprox(quaternion.y(), 0.0) ||
        !isApprox(quaternion.z(), expected_half_angle_component)) {
        std::cerr << "quaternion coefficients did not match the half-angle formula\n";
        return EXIT_FAILURE;
    }

    std::cout << "all rotation representation checks passed\n";
    return EXIT_SUCCESS;
}
