
#include "../include/referee.h"
#include "../../utilities/print.h"
#include <iostream>

#include <unistd.h>     // sleep

#define HEADER      	"RGrt"
#define VERSION_SEND	2               // Very important version of sending
#define VERSION_RECV	7               // Very important version of receive
#define MAX_LENGTH		1024

Referee::Referee()
: m_isMsgAllocated(false)
, m_gameController(new GameController)
{
}

Referee::~Referee()
{
	if (m_isMsgAllocated) 
	{
		delete[] m_msg;
		delete[] m_ipGC;
	}
}

void Referee::Init(const char* ipgc, const int id, const int team)
{
	Network::InitServer(m_port, m_addr);
	m_msg  = this->ParseSend(id, team);
	m_isMsgAllocated  = true;

	size_t dataSize = strlen(ipgc)+1;
	m_ipGC = new char[dataSize];
	strncpy(m_ipGC,ipgc, dataSize);

	m_number = id;
	m_team = team;

	// temp fix
	m_gameController->STATE = 0;
	m_gameController->KICKOFF = 0;
	m_gameController->SECOND_STATE = 0;
	m_gameController->SECOND_STATE_TEAM = 0;
	m_gameController->SECOND_STATE_CONDITION = 0;
}

void Referee::Send()
{	
	Network::SendMultiPort(m_msg, m_portDest, m_addr, m_addrDest);
}

void Referee::Receive()
{
	char* buff = Network::Receive(m_addrDest, MAX_LENGTH); 

	if(strcmp(m_ipGC,"0.0.0.0") == 0)
	{
		this->ParseReceived(buff);
	}
	else
	{
		if(strcmp(inet_ntoa(m_addrDest.sin_addr), m_ipGC) == 0)
			this->ParseReceived(buff);
	}

	delete[] buff;
}

/*
 *	ParseSend have allocation memory, it should be destroy!
 */
char* Referee::ParseSend(int num, int team)
{
	/*
	 * This method uses a bitmask (0xFF) to isolate the lower 8 bits of the integer, 
	 * effectively converting it to a byte. 
	 * It's safer than casting as it avoids potential overflows.
     *
	 */

	m_responseGC.m_header     = HEADER;
    m_responseGC.m_version    = (unsigned char)VERSION_SEND & 0xFF;
    m_responseGC.m_team       = (unsigned char)team & 0xFF;
    m_responseGC.m_player     = (unsigned char)num & 0xFF;

    std::string robot = m_responseGC.m_header     + 
                        m_responseGC.m_version    + 
                        m_responseGC.m_team       + 
                        m_responseGC.m_player     ;

    char* refereeMsg = new char[robot.length() + 1];
    strcpy(refereeMsg, robot.c_str());

	return refereeMsg;
}

void Referee::ParseReceived(const char* buff)
{
    char* newBuff = new char[MAX_LENGTH];
    memcpy(newBuff, buff, MAX_LENGTH);

 	if(newBuff[GCD_VERSION] == VERSION_RECV)
	{
		m_gameController->STATE = newBuff[GCD_STATE];
		m_gameController->KICKOFF = abs(newBuff[GCD_KICK_OF_TEAM]);
		m_gameController->SECOND_STATE = newBuff[GCD_SEC0NDARY_STATE];
		m_gameController->SECOND_STATE_TEAM = newBuff[GCD_SEC0NDARY_STATE +1];
		m_gameController->SECOND_STATE_CONDITION = newBuff[GCD_SEC0NDARY_STATE +2];

		// check drop
		if(m_gameController->KICKOFF == GC_DROP_BALL || m_gameController->SECOND_STATE == GC_DROP_BALL)
			m_gameController->IS_DROP_BALL = true;
		else
			m_gameController->IS_DROP_BALL = false;

		// pickup my robot
		if(newBuff[GCD_TEAM_NUMBER] == m_team)
		{
			// first team
			int penaltyData = newBuff [ GCD_PENALTY + (GC_PLAYER_DATA*(m_number-1)) ];

			m_gameController->IS_PICKUP = (penaltyData == HL_PICKUP_OR_INCAPABLE) ? true : false;
		}
		else
		{
			//second team
			int secondTeamNumber = GCD_TEAM_NUMBER + GC_TEAM_COUNT + GC_PLAYER_COUT;

			int penaltyData = newBuff[ secondTeamNumber + GC_TEAM_COUNT + (GC_PLAYER_DATA*(m_number-1)) ];
			
			m_gameController->IS_PICKUP = (penaltyData == HL_PICKUP_OR_INCAPABLE) ? true : false;
		}

    }

	delete[] newBuff;
}

GameController *Referee::GetGameController() const
{
	return m_gameController;
}
