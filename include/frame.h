//
// Created by shaoliming on 19-9-6.
//

#ifndef SLAM_FRAME_H
#define SLAM_FRAME_H

#include <camera.h>
#include <opencv2/core/core.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
namespace slam
{
class Frame
{
private:
    unsigned long long id_; // 帧的id
    double time_stamp_; // 帧的时间戳
    Sophus::SE3 T_c_w_; // camera to world
    cv::Mat color_;
    Camera::Ptr camera_;
public:
    using Ptr=std::shared_ptr<Frame>;
    Frame();
    Frame(long id, double time_stamp = 0, Sophus::SE3 T_c_w = Sophus::SE3(), Camera::Ptr = nullptr, cv::Mat color = cv::Mat());

    //factory method
    static std::unique_ptr<Frame> createFrame();

    // 获得相机光心
    Eigen::Vector3d getCameraCenter() const;

    // Fixme: this function name may be need
    cv::Mat& getColorMat()
    {
        return color_;
    }

    // check if a point is in this frame
    bool isInFrame(const Eigen::Vector3d & pt_world);

    unsigned long long getId()
    {
        return id_;
    }
};
}


#endif //SLAM_FRAME_H
