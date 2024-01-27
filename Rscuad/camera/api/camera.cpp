#include "camera.h"

#include <unistd.h>     // sleep
#include <string>
#include <iostream>

Camera::Camera()
: m_portData(9131)
, m_portImage(9132)
, m_timeoutMs(50)
, m_length(100)
, m_jpegData(24536)	// Assuming maximum image size
, m_point(0)

#if USE_ROS
, m_target(0)
, m_isTarget(false)
, m_robot(false)

#endif
{
	robot.X=0;
	robot.Y=0;
	pos.X=0;
	pos.Y=0;
	loc.X=0;
	loc.Y=0;
}

Camera::~Camera()
{
}

#if USE_ROS
void Camera::ImageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  cv_bridge::CvImagePtr cvPtr;
   
  try
  { 
    cvPtr = cv_bridge::toCvCopy(msg, "8UC3");
     
    cv::Mat frame = cvPtr->image;

	cv::imencode(".jpg", frame, m_jpegData, std::vector<int>({1, 70}));
     
    // Display the current frame
    // cv::imshow("view", frame); 
    // cv::waitKey(30);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to '8UC3'.", msg->encoding.c_str());
  }
}

void Camera::DataCallback(rscuad::DataCamera msg)
{
	m_target =  msg.ball;
	pos.X =  msg.ball_x;
	pos.Y =  msg.ball_y;
	loc.X = msg.loc_x;
	loc.Y = msg.loc_y;
	m_point = msg.point;
	m_robot = msg.target;
	robot.X = msg.target_x;
	robot.Y = msg.target_y;
}
#endif

void Camera::Init()
{
	if ( (m_sockfdData = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) 
    { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	if ( (m_sockfdImage = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) 
    { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&m_serverAddrImage, 0, sizeof(m_serverAddrImage)); 	
	memset(&m_serverAddrData, 0, sizeof(m_serverAddrData)); 
		
	// Filling server information Data
	m_serverAddrData.sin_family           = AF_INET; // IPv4 
	m_serverAddrData.sin_addr.s_addr      = INADDR_ANY; 
	m_serverAddrData.sin_port             = htons(m_portData); 

	// Filling server information Image
	m_serverAddrImage.sin_family           = AF_INET; // IPv4 
	m_serverAddrImage.sin_addr.s_addr      = INADDR_ANY; 
	m_serverAddrImage.sin_port             = htons(m_portImage); 
	
	// Bind the socket with the server address 
	if ( bind(m_sockfdData, (const struct sockaddr *)&m_serverAddrData, sizeof(m_serverAddrData)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}
	// Bind the socket with the server address 
	if ( bind(m_sockfdImage, (const struct sockaddr *)&m_serverAddrImage, sizeof(m_serverAddrImage)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}
}

void Camera::ImageProcess()
{
	socklen_t len = sizeof(m_serverAddrImage);
 
    // set polling
    struct pollfd ufds[1];
    ufds[0].fd = m_sockfdImage;
    ufds[0].events = POLLIN;

    int polling = poll(ufds, 1, m_timeoutMs);

    if (polling > 0)
    {
        int n = recvfrom(m_sockfdImage, m_jpegData.data(), m_jpegData.size(), MSG_WAITALL, 
                                    ( struct sockaddr *) &m_serverAddrImage, &len);
	}
}

bool Camera::DataProcess(bool useTarget)
{
	char isBall[10], ballX[10], ballY[10], locX[10], locY[10], point[10], isTarget[10], targetX[10], targetY[10];
	
	char buffer[m_length];
	socklen_t len = sizeof(m_serverAddrData);

	static bool isOnTarget = false;
 
    // set polling
    struct pollfd ufds[1];
    ufds[0].fd = m_sockfdData;
    ufds[0].events = POLLIN;

    int polling = poll(ufds, 1, m_timeoutMs);
    if (polling > 0)
    {
		// receive packet
        int n = recvfrom(m_sockfdData, (char *)buffer, m_length, MSG_WAITALL, 
                                    ( struct sockaddr *) &m_serverAddrData, &len);
		// write code for parsing
		sscanf(buffer,"%s %s %s %s %s %s %s %s %s", isBall, ballX, ballY, locX, locY, point, isTarget, targetX, targetY);
		
		if (useTarget)
		{
			isOnTarget = atoi(isTarget);
			pos.X = atof(targetX);
			pos.Y = atof(targetY);
		}
		else
		{
			isOnTarget = atoi(isBall);
			pos.X = atof(ballX);
			pos.Y = atof(ballY);
		}
		loc.X = atof(locX);
		loc.Y = atof(locY);
		m_point = atoi(point);
	}
	return isOnTarget;
}

bool Camera::Process(bool useTarget)
{
#if USE_ROS
	m_isTarget = useTarget;
	return m_target;
#else
	ImageProcess();
	return DataProcess(useTarget);
#endif
}
// target position
float Camera::GetX() const
{
	return pos.X;
}
// target position
float Camera::GetY() const
{
	return pos.Y;
}

// robot location
float Camera::GetLocX() const
{
	return loc.X;
}
// robot location
float Camera::GetLocY() const
{
	return loc.Y;
}

std::vector<u_char> Camera::GetFrame()
{
	return m_jpegData;
}

int Camera::GetPoint() const
{
	return m_point;
}
bool Camera::GetRobot() const
{
	return m_robot;
}
// robot location
float Camera::GetRobotX() const
{
	return robot.X;
}
// robot location
float Camera::GetRobotY() const
{
	return robot.Y;
}