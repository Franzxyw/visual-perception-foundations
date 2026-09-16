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

    std::cout << "transform inverse check passed\n";
    return EXIT_SUCCESS;
}

