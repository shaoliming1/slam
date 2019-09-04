//
// Created by shaoliming on 19-9-6.
//

#include "frame.h"
namespace slam
{
Frame::Frame() {}
Frame::Frame(long id, double time_stamp, Sophus::SE3 T_c_w, slam::Camera::Ptr camera, cv::Mat color):id_(id), time_stamp_(time_stamp), T_c_w_(T_c_w),color_(color),camera_(camera)
{
}
std::unique_ptr<Frame> Frame::createFrame()
{
    static unsigned  long id = 0;
    return std::unique_ptr<Frame>(new Frame(id++));
}
Eigen::Vector3d Frame::getCameraCenter() const
{
    return T_c_w_.inverse().translation();
}

bool Frame::isInFrame ( const Eigen::Vector3d& pt_world )
{
    Eigen::Vector3d p_cam = camera_->world2camera( pt_world, T_c_w_ );
    if ( p_cam(2,0)<0 )
        return false;
    Eigen::Vector2d pixel = camera_->world2pixel( pt_world, T_c_w_ );
    int x = pixel(0, 0);
    int y = pixel(1, 0);
    return x > 0 && y > 0
           && x < color_.cols
           && y < color_.rows;
}
}