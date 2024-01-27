/**
 *	Title	: AI Gateway
 *
 *	Author	: Danu Andrean
 *	Create	: 2023
 *	Update 	: 2023
 *	
 *	Note	: gateway for data c++
 */

#ifndef GATEWAY_H
#define GATEWAY_H

#include <string.h>
#include <string>
#include <sstream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>   // time
#include <sys/poll.h>   // Non Block
#include <vector>

class Gateway
{
public:

	struct DataResponse 
		{
			std::string isBall;
			std::string ballX;
			std::string ballY;
			std::string robotX;
			std::string robotY;
			std::string point;
			std::string isTarget;
			std::string targetX;
			std::string targetY;
		};
	
public:
	static int                 	m_sockfdData;
	static int                 	m_sockfdImage;
   	static struct sockaddr_in  	m_addrData;
   	static struct sockaddr_in  	m_addrImage;

	static std::vector<u_char>	m_frame;
	static struct DataResponse *s_dataResponse;

private:
	int							m_portData;
	int							m_portImage;
	int 						m_length;

private:
	std::string FloatToString(const float data) const;
	void CreateAsync();
	static void AsyncData();
	static void AsyncImage();
	void DataInit();

public:
	Gateway();
	~Gateway();
	void Init();
	void Update(bool isBall, int ballX, int ballY, 
				int robotX = 0, int robotY = 0, int point = 0,
				bool isTarget = false, int targetX = 0, int targetY = 0);

	void Update(const std::vector<u_char>& frame);
};

#endif
