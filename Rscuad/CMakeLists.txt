cmake_minimum_required(VERSION 3.0.2)
project(rscuad)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)
add_compile_definitions(USE_ROS=1)

find_package(catkin REQUIRED COMPONENTS
  std_msgs
  roscpp
  rospy
  message_generation
  std_msgs
  cv_bridge
  image_transport
  sensor_msgs
)
find_package(OpenCV)

add_message_files(
  FILES
  DataCamera.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)


catkin_package(
 INCLUDE_DIRS ./
 LIBRARIES rscuad
#  CATKIN_DEPENDS ros_msgs roscpp rospy
#  DEPENDS system_lib
)

include_directories(
include
  ${catkin_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}/camera
  ${catkin_INCLUDE_DIRS}/serial
  ${catkin_INCLUDE_DIRS}/utilities
  ${catkin_INCLUDE_DIRS}/communication
)
include_directories(${OpenCV_INCLUDE_DIRS})

## Declare a C++ library
add_library(${PROJECT_NAME}
  camera/api/camera.cpp
  serial/serial.cpp
  utilities/print.cpp
  utilities/utilities.cpp
  communication/communication.cpp
  communication/src/network.cpp
  communication/src/basestation.cpp
  communication/src/referee.cpp
  communication/src/manager.cpp
)

add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)
