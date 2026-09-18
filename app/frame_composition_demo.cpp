#include "visual_perception/frame_transform.hpp"

#include <iostream>

int main() {
    const Eigen::Vector3d p_a(1.0, 0.0, 0.0);

    constexpr double kPi = 3.14159265358979323846;

    const Eigen::Isometry3d t_ba =
        visual_perception::makeRigidTransform(
            Eigen::AngleAxisd(kPi / 2.0, Eigen::Vector3d::UnitZ()),
            Eigen::Vector3d(1.0, 0.0, 0.0)
        );

    const Eigen::Vector3d p_b = t_ba * p_a;

    const Eigen::Isometry3d t_cb =
        visual_perception::makeRigidTransform(
            Eigen::AngleAxisd(kPi / 2.0, Eigen::Vector3d::UnitZ()),
            Eigen::Vector3d(0.0, 2.0, 0.0)
        );

    const Eigen::Vector3d p_c_sequential = t_cb * p_b;

    const Eigen::Isometry3d t_ca = t_cb * t_ba;
    const Eigen::Vector3d p_c_composed = t_ca * p_a;

    std::cout << "p_A: " << p_a.transpose() << '\n';
    std::cout << "p_B: " << p_b.transpose() << '\n';
    std::cout << "p_C sequential: " << p_c_sequential.transpose() << '\n';
    std::cout << "p_C composed:   " << p_c_composed.transpose() << '\n';
    return 0;
}
