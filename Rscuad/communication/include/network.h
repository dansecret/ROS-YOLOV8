 /**
 *	Title	: Network 
 *
 *	Author	: Danu Andrean
 *	Create	: 2020
 *	Update 	: 2023
 *	
 *	Note	: Parent for all network
 */
 
#ifndef NETWORK_H
#define NETWORK_H

#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>   // time
#include <sys/poll.h>   // Non Block
#include <vector>

class Network
{
private:
	int					m_timeoutMs;

protected:
    int                 m_sockfd;
    struct sockaddr_in  m_serverAddr, m_clientAddr;
                        
public:
    Network();
    virtual ~Network();
	void InitClient(const char *ip, const int port, struct sockaddr_in &addr);
	void InitServer(const int port, struct sockaddr_in &addr);
	void Send(const char *data, struct sockaddr_in &addr);
	void Send(const std::vector<u_char> &frame, struct sockaddr_in &addr);
	void SendMultiPort(const char* data, const int portDest, struct sockaddr_in &addr, struct sockaddr_in &addrDest);
	char* Receive(struct sockaddr_in &addr, const int length);
};
#endif