
#ifndef MAPPOINT_H
#define MAPPOINT_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <memory>
#include <opencv2/core/mat.hpp>

namespace slam
{

class Frame;
class MapPoint
{
public:
    using Ptr = std::shared_ptr<MapPoint>;
    unsigned long      id_; // ID
    Eigen::Vector3d    pos_;       // Position in world
    Eigen::Vector3d    norm_;      // Normal of viewing direction
    cv::Mat         descriptor_; // Descriptor for matching
    int         observed_times_;    // being observed by feature matching algo.
    int         correct_times_;     // being an inliner in pose estimation

    MapPoint();
    MapPoint( long id, Eigen::Vector3d position, Eigen::Vector3d norm );

    // factory function
    static std::unique_ptr<MapPoint> createMapPoint();
};
}

#endif // MAPPOINT_H
