#include "../include/network.h"

#include <unistd.h>     // sleep
#include <string>
#include <iostream>

Network::Network()
: m_timeoutMs(50)
{
}

Network::~Network()
{
    close(m_sockfd);
}

void Network::InitClient(const char *ip, const int port, struct sockaddr_in &addr)
{
	if ( (m_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
}

void Network::InitServer(const int port, struct sockaddr_in &addr)
{
	// Creating socket file descriptor 
	if ( (m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) 
    { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
		
	memset(&addr, 0, sizeof(addr)); 
		
	// Filling server information 
	addr.sin_family           = AF_INET; // IPv4 
	addr.sin_addr.s_addr      = INADDR_ANY; 
	addr.sin_port             = htons(port); 

	// Bind the socket with the server address 
	if ( bind(m_sockfd, (const struct sockaddr *)&addr, sizeof(addr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}
}

void Network::Send(const char* data, struct sockaddr_in &addr)
{
	sendto(m_sockfd, data, strlen(data), MSG_CONFIRM, 
                    (const struct sockaddr *) &addr, sizeof(addr));
}

void Network::Send(const std::vector<u_char>& frame, struct sockaddr_in &addr)
{
	 sendto(m_sockfd, frame.data(), frame.size(), 0,
                    (const struct sockaddr *) &addr, sizeof(addr));
}

void Network::SendMultiPort(const char* data, const int portDest, struct sockaddr_in &addr, struct sockaddr_in &addrDest)
{

    size_t dataSize = strlen(data)+1;

    char arrData[dataSize];
    memcpy(arrData, data, dataSize);

    struct sockaddr_in destination;
    memset(&destination, 0, sizeof(destination));
    destination.sin_family = AF_INET;
    destination.sin_addr.s_addr = addrDest.sin_addr.s_addr;
    destination.sin_port = htons(portDest);

    // send packet
    sendto(m_sockfd, (char*)arrData, dataSize, 0, (const struct sockaddr *)&destination, sizeof(destination));
}

/*
 *	Receive have allocation memory, it should be destroy!
 */
char* Network::Receive(struct sockaddr_in &addr, const int length)
{
    char buffer[length];
	socklen_t len = sizeof(addr);
 
    // set polling
    struct pollfd ufds[1];
    ufds[0].fd = m_sockfd;
    ufds[0].events = POLLIN;

    int polling = poll(ufds, 1, m_timeoutMs);
	if (polling > 0)
	{
		// receive packet
		int n = recvfrom(m_sockfd, (char *)buffer, length, MSG_WAITALL, 
									( struct sockaddr *) &addr, &len);
		char* newBuff = new char[length];
		memcpy(newBuff, buffer, length);
		return newBuff;
	}
	else
	{
		char* newBuff = new char[length];
		memcpy(newBuff, "00", length);
		return newBuff;
	}
}
