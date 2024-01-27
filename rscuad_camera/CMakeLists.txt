cmake_minimum_required(VERSION 3.0.2)
project(rscuad_camera)

add_compile_definitions(USE_ROS=1)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  rscuad
  cv_bridge
  image_transport
  sensor_msgs
)
# find_package(OpenCV)

catkin_python_setup()

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES Camera
#  CATKIN_DEPENDS roscpp rospy
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)
# include_directories(${OpenCV_INCLUDE_DIRS})

catkin_install_python(PROGRAMS
  example.py
  yolov8.py
  localization.py
  video.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
# add_executable(rscuad_camera camera.cpp)
# target_link_libraries(rscuad_camera ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})