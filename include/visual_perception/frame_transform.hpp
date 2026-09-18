#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

namespace visual_perception {

inline Eigen::Isometry3d makeRigidTransform(
    const Eigen::AngleAxisd& rotation,
    const Eigen::Vector3d& translation) {
    Eigen::Isometry3d transform = Eigen::Isometry3d::Identity();
    transform.linear() = rotation.toRotationMatrix();
    transform.translation() = translation;
    return transform;
}

}  // namespace visual_perception
