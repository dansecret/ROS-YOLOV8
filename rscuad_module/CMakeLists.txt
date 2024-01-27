cmake_minimum_required(VERSION 3.0.2)
project(rscuad_module)

add_compile_definitions(USE_ROS=1)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  rscuad
  cv_bridge
  image_transport
  sensor_msgs
)
find_package(OpenCV)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES rscuad_module
#  CATKIN_DEPENDS roscpp rospy
#  DEPENDS system_lib
)


include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/rscuad_module.cpp)
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)

add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})