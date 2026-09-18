#include "visual_perception/frame_transform.hpp"

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
    constexpr double kPi = 3.14159265358979323846;

    const Eigen::Vector3d p_a(1.0, 0.0, 0.0);

    const Eigen::Isometry3d t_ba =
        visual_perception::makeRigidTransform(
            Eigen::AngleAxisd(kPi / 2.0, Eigen::Vector3d::UnitZ()),
            Eigen::Vector3d(1.0, 0.0, 0.0)
        );

    const Eigen::Isometry3d t_cb =
        visual_perception::makeRigidTransform(
            Eigen::AngleAxisd(kPi / 2.0, Eigen::Vector3d::UnitZ()),
            Eigen::Vector3d(0.0, 2.0, 0.0)
        );

    const Eigen::Vector3d p_b = t_ba * p_a;
    const Eigen::Vector3d p_c_sequential = t_cb * p_b;

    const Eigen::Isometry3d t_ca = t_cb * t_ba;
    const Eigen::Vector3d p_c_composed = t_ca * p_a;

    const Eigen::Isometry3d t_ac = t_ca.inverse();
    const Eigen::Vector3d p_a_recovered = t_ac * p_c_composed;

    const Eigen::Isometry3d t_aa = Eigen::Isometry3d::Identity();
    const Eigen::Isometry3d t_cc = Eigen::Isometry3d::Identity();
    const Eigen::Vector3d p_c_via_right_identity = (t_ca * t_aa) * p_a;
    const Eigen::Vector3d p_c_via_left_identity = (t_cc * t_ca) * p_a;

    const Eigen::Vector3d expected_p_c(-1.0, 3.0, 0.0);

    if (!isApprox(p_c_sequential, expected_p_c)) {
        std::cerr << "sequential transformation was incorrect\n";
        return EXIT_FAILURE;
    }

    if (!isApprox(p_c_composed, expected_p_c)) {
        std::cerr << "composed transformation was incorrect\n";
        return EXIT_FAILURE;
    }

    if (!isApprox(p_c_sequential, p_c_composed)) {
        std::cerr << "sequential and composed results disagree\n";
        return EXIT_FAILURE;
    }

    if (!isApprox(p_a_recovered, p_a)) {
        std::cerr << "inverse transform did not recover p_A\n";
        return EXIT_FAILURE;
    }

    if (!isApprox(p_c_via_right_identity, p_c_composed)) {
        std::cerr << "right identity changed the composed transform\n";
        return EXIT_FAILURE;
    }

    if (!isApprox(p_c_via_left_identity, p_c_composed)) {
        std::cerr << "left identity changed the composed transform\n";
        return EXIT_FAILURE;
    }

    std::cout << "all frame composition checks passed\n";
    return EXIT_SUCCESS;
}
