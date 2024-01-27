/**
 *	Title	: AI Gateway
 *
 *	Author	: Danu Andrean
 *	Create	: 2023
 *	Update 	: 2023
 *	
 *	Note	: gateway for data AI for other languages
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>   // time
#include <sys/poll.h>   // Non Block
#include <vector>
#if USE_ROS
	#include "ros/ros.h"
	#include "std_msgs/String.h"
	#include "rscuad/DataCamera.h"

	#include <ros/ros.h>
	#include <image_transport/image_transport.h>
	#include <opencv2/highgui/highgui.hpp>
	#include <cv_bridge/cv_bridge.h>
#endif
class Camera
{
private:
	struct Point2D
    {
        float X;
        float Y;
    };

    Point2D pos, loc, target,robot;

private:
	int                 m_sockfdData;
   	struct sockaddr_in  m_serverAddrData;

	int					m_portData;

	int                 m_sockfdImage;
   	struct sockaddr_in  m_serverAddrImage;

	int					m_portImage;

	int 				m_length;
	int					m_timeoutMs;
	std::vector<u_char> m_jpegData;
	int 				m_point;

#if USE_ROS
	bool				m_isTarget;
	bool 				m_target;
	bool 				m_robot;
#endif

private:
	void InitSocket();

public:
	Camera();
	~Camera();
	void Init();
	bool Process(bool useTarget = false);
	void ImageProcess();
	bool DataProcess(bool useTarget);
	std::vector<u_char> GetFrame();
	float GetX() const;
	float GetY() const;
	float GetLocX() const;
	float GetLocY() const;
	int GetPoint() const;
	bool GetRobot() const;
	float GetRobotX() const;
	float GetRobotY() const;
#if USE_ROS
	void ImageCallback(const sensor_msgs::ImageConstPtr& msg);
	void DataCallback(rscuad::DataCamera msg);
#endif
};

#endif
