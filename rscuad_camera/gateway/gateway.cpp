#include "gateway.h"

#include <unistd.h>     // sleep
#include <string>
#include <iostream>
#include <mutex>
#include <thread>

#define TIME_DELAY 50000

std::mutex mtxData;
std::mutex mtxImage;


int 				Gateway::m_sockfdData;
int 				Gateway::m_sockfdImage;
struct sockaddr_in 	Gateway::m_addrData;
struct sockaddr_in 	Gateway::m_addrImage;
std::vector<u_char>	Gateway::m_frame;

struct Gateway::DataResponse *Gateway::s_dataResponse;


Gateway::Gateway()
: m_portData(9131)
, m_portImage(9132)
, m_length(10)
{
	s_dataResponse = new DataResponse;
}

Gateway::~Gateway()
{
}

void Gateway::Init()
{
	if ( (m_sockfdData = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	

	memset(&m_addrData, 0, sizeof(m_addrData));
	m_addrData.sin_family = AF_INET;
	m_addrData.sin_port = htons(m_portData);
	m_addrData.sin_addr.s_addr = inet_addr("127.0.0.1");

	if ( (m_sockfdImage = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&m_addrImage, 0, sizeof(m_addrImage));
	m_addrImage.sin_family = AF_INET;
	m_addrImage.sin_port = htons(m_portImage);
	m_addrImage.sin_addr.s_addr = inet_addr("127.0.0.1");

	Gateway::CreateAsync();
}

void Gateway::CreateAsync()
{
	std::thread workerSendData(AsyncData);
	workerSendData.detach();

	std::thread workerSendImage(AsyncImage);
	workerSendImage.detach();

}

void Gateway::AsyncData()
{
	for(;;)
	{
		std::lock_guard<std::mutex> guard(mtxData);

		std::string data = s_dataResponse->isBall +" "+ s_dataResponse->ballX +" "+ s_dataResponse->ballY +" "+
							s_dataResponse->robotX +" "+ s_dataResponse->robotY +" "+ s_dataResponse->point +" "+ 
							s_dataResponse->isTarget +" "+ s_dataResponse->targetX +" "+ s_dataResponse->targetY;

		sendto(m_sockfdData, data.c_str(), data.length(), MSG_CONFIRM, 
						(const struct sockaddr *) &m_addrData, sizeof(m_addrData));

		usleep(TIME_DELAY);

	}
}

void Gateway::AsyncImage()
{
	for(;;)
	{
		std::lock_guard<std::mutex> guard(mtxImage);

		sendto(m_sockfdImage, m_frame.data(), m_frame.size(), MSG_CONFIRM, 
					(const struct sockaddr *) &m_addrImage, sizeof(m_addrImage));

		usleep(TIME_DELAY);
	}
}

void Gateway::Update(const std::vector<u_char>& frame)
{
	m_frame.assign(frame.begin(), frame.end());
}

void Gateway::Update(bool isBall, int ballX, int ballY, 
						int robotX, int robotY, int point,
						bool isTarget, int targetX, int targetY)
{
	s_dataResponse->isBall = FloatToString((int)isBall);
	s_dataResponse->ballX = FloatToString(ballX);
	s_dataResponse->ballY = FloatToString(ballY);
	s_dataResponse->robotX = FloatToString(robotX);
	s_dataResponse->robotY = FloatToString(robotY);
	s_dataResponse->point = FloatToString(point);
	s_dataResponse->isTarget = FloatToString((int)isTarget);
	s_dataResponse->targetX = FloatToString(targetX);
	s_dataResponse->targetY = FloatToString(targetY);
}

std::string Gateway::FloatToString(const float data) const
{
    std::stringstream strs;
    strs << data;
    return strs.str();
}
