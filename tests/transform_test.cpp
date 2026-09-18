#include <Eigen/Core>
#include <Eigen/Geometry>

#include <array>
#include <cstdlib>
#include <iostream>

namespace {

bool isApprox(const Eigen::Vector3d& actual,
              const Eigen::Vector3d& expected,
              double tolerance = 1e-12) {
    return (actual - expected).norm() < tolerance;
}

struct TransformCase {
    const char* name;
    Eigen::Vector3d input;
    Eigen::Vector3d expected;
};

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

    const std::array<TransformCase, 2> cases{{
        {
            "x-axis point",
            Eigen::Vector3d(1.0, 0.0, 0.0),
            Eigen::Vector3d(1.0, 3.0, 0.0)
        },
        {
            "y-axis point",
            Eigen::Vector3d(0.0, 1.0, 0.0),
            Eigen::Vector3d(0.0, 2.0, 0.0)
        },
    }};

    for (const auto& test_case : cases) {
        const Eigen::Vector3d actual = rotation_translation * test_case.input;

        if (!isApprox(actual, test_case.expected)) {
            std::cerr << test_case.name << " was transformed incorrectly\n";
            return EXIT_FAILURE;
        }
    }

    std::cout << "all transform checks passed\n";
    return EXIT_SUCCESS;
}
