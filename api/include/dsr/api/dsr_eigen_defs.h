//
// Created by pbustos on 1/10/20.
//

#ifndef DSR_EIGEN_DEFS_H
#define DSR_EIGEN_DEFS_H

#include <Eigen/Dense>

// typedefs of Eigen types
namespace Mat
{
    using Vector2d = Eigen::Matrix<double, 2, 1>;
    using Vector3d = Eigen::Matrix<double, 3, 1>;
    using Vector6d = Eigen::Matrix<double, 6, 1>;
    using RTMat = Eigen::Transform<double, 3, Eigen::Affine>;
    using Rot3D = Eigen::Matrix3d;
    using Mat22d = Eigen::Matrix<float, 4, 4>;
    using Mat33d = Eigen::Matrix<float, 4, 4>;
    using Mat44d = Eigen::Matrix<float, 4, 4>;
};

#endif //DSR_EIGEN_DEFS_H
