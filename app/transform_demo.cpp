#include <Eigen/Core>
#include <Eigen/Geometry>

#include <iostream>

int main() {
    constexpr double kPi = 3.14159265358979323846;

    const Eigen::AngleAxisd rotation(
        kPi / 2.0, Eigen::Vector3d::UnitZ());
    const Eigen::Vector3d translation(1.0, 2.0, 0.0);

    Eigen::Isometry3d transform = Eigen::Isometry3d::Identity();
    transform.linear() = rotation.toRotationMatrix();
    transform.translation() = translation;

    const Eigen::Vector3d point_a(1.0, 0.0, 0.0);
    const Eigen::Vector3d point_b = transform * point_a;
    const Eigen::Vector3d second_point_a(0.0, 1.0, 0.0);
    const Eigen::Vector3d second_point_b = transform * second_point_a;

    std::cout << "point in frame A: " << point_a.transpose() << '\n';
    std::cout << "point in frame B: " << point_b.transpose() << '\n';
    std::cout << "second point in frame A: "
              << second_point_a.transpose() << "\n";
    std::cout << "second point in frame B: "
              << second_point_b.transpose() << "\n";
    return 0;
}

