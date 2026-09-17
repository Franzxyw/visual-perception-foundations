#include <Eigen/Core>
#include <Eigen/Geometry>

#include <cstdlib>
#include <iostream>

namespace {

bool isApprox(const Eigen::Vector3d& actual,
              const Eigen::Vector3d& expected,
              double tolerance = 1e-12) {
    return (actual - expected).norm() < tolerance;
}

}  // namespace

int main() {
    Eigen::Isometry3d transform = Eigen::Isometry3d::Identity();
    transform.translation() = Eigen::Vector3d(1.0, 2.0, 3.0);

    const Eigen::Vector3d original(4.0, 5.0, 6.0);
    const Eigen::Vector3d transformed = transform * original;
    const Eigen::Vector3d recovered = transform.inverse() * transformed;

    if (!isApprox(recovered, original)) {
        std::cerr << "inverse transform did not recover the original point\n";
        return EXIT_FAILURE;
    }

    constexpr double kPi = 3.14159265358979323846;

    Eigen::Isometry3d rotation_translation = Eigen::Isometry3d::Identity();
    rotation_translation.linear() =
        Eigen::AngleAxisd(
            kPi / 2.0,
            Eigen::Vector3d::UnitZ()
        ).toRotationMatrix();
    rotation_translation.translation() = Eigen::Vector3d(1.0, 2.0, 0.0);

    const Eigen::Vector3d first_input(1.0, 0.0, 0.0);
    const Eigen::Vector3d first_expected(1.0, 3.0, 0.0);
    const Eigen::Vector3d first_actual = rotation_translation * first_input;

    if (!isApprox(first_actual, first_expected)) {
        std::cerr << "first transformed point was incorrect\n";
        return EXIT_FAILURE;
    }

    const Eigen::Vector3d second_input(0.0, 1.0, 0.0);
    const Eigen::Vector3d second_expected(0.0, 2.0, 0.0);
    const Eigen::Vector3d second_actual = rotation_translation * second_input;

    if (!isApprox(second_actual, second_expected)) {
        std::cerr << "second transformed point was incorrect\n";
        return EXIT_FAILURE;
    }

    std::cout << "transform inverse check passed\n";
    return EXIT_SUCCESS;
}

