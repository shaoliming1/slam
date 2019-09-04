
#include "mappoint.h"

namespace slam
{

MapPoint::MapPoint()
    : id_(-1), pos_(Eigen::Vector3d(0,0,0)), norm_(Eigen::Vector3d(0,0,0)), observed_times_(0), correct_times_(0)
{

}

MapPoint::MapPoint ( long id, Eigen::Vector3d position, Eigen::Vector3d norm )
    : id_(id), pos_(position), norm_(norm), observed_times_(0), correct_times_(0)
{

}

std::unique_ptr<MapPoint> MapPoint::createMapPoint()
{
    static long factory_id = 0;
    return std::unique_ptr<MapPoint>(
               new MapPoint( factory_id++, Eigen::Vector3d(0,0,0), Eigen::Vector3d(0,0,0) )
           );
}

}
