
#ifndef VISUALODOMETRY_H
#define VISUALODOMETRY_H

#include "map.h"
#include <opencv2/features2d/features2d.hpp>
#include <iostream>
#include <string>

namespace slam
{
    class VOState;
    class VisualOdometry
    {
        friend class VOState;

    private:
        VOState     state_;     // current VO status
        Map::Ptr    map_;       // map with all frames and map points
        Frame::Ptr  ref_;       // reference frame

        Frame::Ptr  curr_;      // current frame
        cv::Ptr<cv::ORB> orb_;  // orb detector and computer
        std::vector<cv::Point3f>     pts_3d_ref_;        // 3d points in reference frame
        std::vector<cv::KeyPoint>    keypoints_curr_;    // keypoints in current frame
        cv::Mat                     descriptors_curr_;  // descriptor in current frame
        cv::Mat                     descriptors_ref_;   // descriptor in reference frame

        std::vector<cv::DMatch>      feature_matches_;
        Sophus::SE3 T_c_r_estimated_;  // the estimated pose of current frame
        int num_inliers_;        // number of inlier features in icp

        int num_lost_;           // number of lost times
        // parameters
        int num_of_features_;   // number of features
        double scale_factor_;   // scale in image pyramid
        int level_pyramid_;     // number of pyramid levels
        float match_ratio_;      // ratio for selecting  good matches
        int max_num_lost_;      // max number of continuous lost times

        int min_inliers_;       // minimum inliers
        double key_frame_min_rot;   // minimal rotation of two key-frames

        double key_frame_min_trans; // minimal translation of two key-frames
    public:
        using Ptr = std::shared_ptr<VisualOdometry>;
        VisualOdometry();
        ~VisualOdometry();

        bool addFrame( Frame::Ptr frame );      // add a new frame

        void extractKeyPoints();
        void computeDescriptors();
        void featureMatching();
        void poseEstimationPnP();
        void setRef3DPoints();

        void addKeyFrame();
        bool checkEstimatedPose();
        bool checkKeyFrame();

    };

    class VOState{
    protected:
        enum State {
            INITIALIZING=-1,
            OK=0,
            LOST
        };
        static const int ERROR = -1;
        static const int SUCCESS = 0;
    public:
        // 处理添加帧
        virtual int handleAddFrame(VisualOdometry & vo, Frame::Ptr frame) = 0;
        virtual std::string getState() = 0 ;
        friend  std::ostream &operator << (std::ostream & os, VOState & status){
            os << "the current VO state is :" << status.getState() << std::endl;
        }

    };

    class InitVoState:public VOState{
    protected:
        static const State status = INITIALIZING;
    public:
        int handleAddFrame(VisualOdometry & vo, Frame::Ptr frame);
        std::string getState();
    };

}

#endif // VISUALODOMETRY_H