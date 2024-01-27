/**
 *	Title	: Basestation UDP protocol 
 *
 *	Author	: Danu Andrean
 *	Create	: 2020
 *	Update 	: 2023
 *	
 *	Note	: Child for network
 */
 
#ifndef BASESTATION_H
#define BASESTATION_H

#include "network.h"
#include "gamecontroller.h"
#include "../../utilities/utilities.h"

class Basestation : public Network
{
public:
 	struct BasestationResponse 
    {
		std::string m_robot;
		std::string m_tilt;
		std::string m_pan;
		std::string m_gyro;
		std::string m_ball;
		std::string m_count;
		std::string m_limit;
		std::string m_time;
		std::string m_locX;
		std::string m_locY;
	};
	
private:
    const int   m_port          = 8124;
	const int   m_portR1		= 8131;
	const int   m_portR2		= 8132;
	const int   m_portR3		= 8133;
	const int   m_portR4		= 8134;
	const int   m_portR5		= 8135;

private:
    std::string           		m_robotStatus;
	struct sockaddr_in  		m_addr,
								m_addrCam;
	struct BasestationResponse* m_responseBS;
	Utilities 					utilities;

	bool						m_robot;
	int 						m_robotID;
	std::vector<u_char>			m_frame;
	struct GameController 		*m_gameController;

public:
	Basestation();
	~Basestation();
	void Init(const char * ip, const int id);
	void Send();
	void SendCamera();
	void Receive() ;
	bool GetRobot() const;
	Basestation::BasestationResponse* GetResponseBS() const;
	void UpdateCamera(const std::vector<u_char>& frame);
	GameController *GetGameController() const;

private:
	char* ParseSend();
    void ParseReceived(const char* buf);

};
#endif