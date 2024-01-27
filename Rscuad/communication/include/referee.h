/**
 *	Title	: Basestation UDP protocol 
 *
 *	Author	: Danu Andrean
 *	Create	: 2020
 *	Update 	: 2023
 *	
 *	Note	: Child for network, Only used if set true in com!
 */
 
#ifndef REFEREE_H
#define REFEREE_H

#include "network.h"
#include "gamecontroller.h"
#include <string>

class Referee : public Network
{
public:
	struct RefereeResponse 
    {
		std::string m_header;
		std::string m_version;
		std::string m_team;
		std::string m_player;
	};

private:
	char* 		m_msg;
	char*		m_ipGC;
	bool 		m_isMsgAllocated;
	int			m_number;
	int 		m_team;
    
    const int   m_port          = 3838;
	const int   m_portDest		= 3838;		// new version using 3939
    const int   m_maxLine       = 2048;

    struct RefereeResponse		m_responseGC;
	struct sockaddr_in  		m_addr,
								m_addrDest;
	struct GameController 		*m_gameController;

public:
	Referee();
	~Referee();
	void Init(const char* ipgc, const int id, const int team);
	void Send();
	void Receive();
	void ParseReceived(const int buff);
	GameController *GetGameController() const;

private:
    char* ParseSend(int num, int team);
	void ParseReceived(const char* buf);

};
#endif
