//
// Created by shaoliming on 19-9-5.
//

#ifndef SLAM_CAMERA_H
#define SLAM_CAMERA_H

#include <memory>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <sophus/se3.h>
namespace  slam
{
class Camera
{
private:
    // 相机内参
    float fx_;
    float fy_;
    float cx_;
    float cy_;
public:
    using Ptr = std::shared_ptr<Camera>;
    Camera();
    Camera(float fx, float fy, float cx, float cy);

    // 坐标变换
    Eigen::Vector3d world2camera(const Eigen::Vector3d& p_w, const Sophus::SE3& T_c_w );
    Eigen::Vector3d camera2world(const Eigen::Vector3d& p_c, const Sophus::SE3& T_c_w);
    Eigen::Vector2d camera2pixel(const Eigen::Vector3d& p_c);
    Eigen::Vector3d pixel2camera(const Eigen::Vector2d& p_p, double depth);
    Eigen::Vector3d pixel2world(const Eigen::Vector2d& p_p, const Sophus::SE3& T_c_w, double depth = 1);
    Eigen::Vector2d world2pixel(const Eigen::Vector3d& p_w, const Sophus::SE3& T_c_w);
};
}
#endif //SLAM_CAMERA_H
