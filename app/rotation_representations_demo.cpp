#include <Eigen/Core>
#include <Eigen/Geometry>

#include <iostream>

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

    const Eigen::Vector3d input(1.0, 0.0, 0.0);
    const Eigen::Vector3d matrix_result = matrix_rotation * input;
    const Eigen::Vector3d angle_axis_result = angle_axis * input;
    const Eigen::Vector3d quaternion_result = quaternion * input;

    std::cout << "matrix rotation:\n" << matrix_rotation << "\n\n";
    std::cout << "angle-axis rotation:\n" << angle_axis_rotation << "\n\n";
    std::cout << "quaternion rotation:\n" << quaternion_rotation << "\n\n";

    std::cout << "quaternion (w, x, y, z): "
              << quaternion.w() << ' '
              << quaternion.x() << ' '
              << quaternion.y() << ' '
              << quaternion.z() << '\n';

    std::cout << "matrix result:     " << matrix_result.transpose() << '\n';
    std::cout << "angle-axis result: " << angle_axis_result.transpose() << '\n';
    std::cout << "quaternion result: " << quaternion_result.transpose() << '\n';

    return 0;
}
