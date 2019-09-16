cmake_minimum_required(VERSION 2.8)
PROJECT("slam")
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall  -O3 -march=native ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall   -O3 -march=native")

# Check C++11 or C++0x support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    add_definitions(-DCOMPILEDWITHC11)
    message(STATUS "Using flag -std=c++11.")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

find_package(OpenCV 3.0 QUIET)
if(NOT OpenCV_FOUND)
    find_package(OpenCV 2.4.3 QUIET)
    if(NOT OpenCV_FOUND)
        message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
    endif()
endif()

set(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost 1.54 COMPONENTS log REQUIRED)
find_package(Threads)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

find_package(Eigen3 3.1.0 REQUIRED)
find_package( Sophus REQUIRED )
message(${Sophus_INCLUDE_DIRS})
include_directories( ${Sophus_INCLUDE_DIRS} )
include_directories(
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${EIGEN3_INCLUDE_DIR}
)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
add_library(${PROJECT_NAME} SHARED
        src/slam.cpp
        src/frame.cpp
        src/camera.cpp
        src/map.cpp
        src/mappoint.cpp
        src/config.cpp
        src/visual_odometry.cpp
        src/log.cpp
        )
add_executable(boost_log test/boost_log_test.cpp)
TARGET_LINK_LIBRARIES(boost_log ${Boost_LIBRARIES} Threads::Threads)

add_executable(basic_log_ex1  test/basic_log_ex.cpp)
TARGET_LINK_LIBRARIES(basic_log_ex1 ${Boost_LIBRARIES})

add_executable(mylogger_test test/mylogger_test.cpp src/log.cpp)
TARGET_LINK_LIBRARIES(mylogger_test ${Boost_LIBRARIES})

add_executable(boost_log_mt_test test/boost_log_mt_test.cpp)
TARGET_LINK_LIBRARIES(boost_log_mt_test ${Boost_LIBRARIES})

add_executable(boost_log_async test/boost_log_async.cpp)
TARGET_LINK_LIBRARIES(boost_log_async ${Boost_LIBRARIES})