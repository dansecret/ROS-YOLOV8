#include "../include/basestation.h"
#include "../../utilities/print.h"

#include <unistd.h>     // sleep

#define MAX_LENGTH		50
#define BS_VERSION 		1

Basestation::Basestation()
: m_responseBS(new BasestationResponse)
, m_robot(false)
, m_gameController(new GameController)
{
}

Basestation::~Basestation()
{
}

void Basestation::Init(const char * ip, const int id)
{   
	m_robotID = id;
    // Selection port for camera robot
    int camPort;
    if(m_robotID == 1)
        camPort = m_portR1;
    else if(m_robotID == 2)
        camPort = m_portR2;
    else if(m_robotID == 3)
        camPort = m_portR3;
    else if(m_robotID == 4)
        camPort = m_portR4;
    else if(m_robotID == 5)
        camPort = m_portR5;

	// temp fix
	m_gameController->STATE = 0;
	m_gameController->KICKOFF = 0;
	m_gameController->SECOND_STATE = 0;
	m_gameController->SECOND_STATE_TEAM = 0;
	m_gameController->SECOND_STATE_CONDITION = 0;
        
	Network::InitClient(ip, m_port, m_addr);
	Network::InitClient(ip, camPort, m_addrCam);

}

void Basestation::Send()
{
	char* data = this->ParseSend();
   	Network::Send(data, m_addr);

	delete [] data;
}

void Basestation::SendCamera()
{
	Network::Send(m_frame, m_addrCam);
}

void Basestation::Receive()
{
	char* buff = Network::Receive(m_addr, MAX_LENGTH); 

	if(buff[0] == BS_VERSION)
	{
        this->ParseReceived(buff);
		delete[] buff;
	}
	else
    {
        RSCUAD_ERROR("[NET] loss connection [basestation] \n");
    }
}

/*
 *	ParseSend have allocation memory, it should be destroy!
 */
char* Basestation::ParseSend()
{
    /**
     *  bit 1 robot, bit 2 value
     *  10  => robot 1 , value 0
     */
	if (m_responseBS->m_robot == "1" )
        m_robotStatus = std::string(utilities.IntToString(m_robotID)) + "1";
	else 
        m_robotStatus = std::string(utilities.IntToString(m_robotID)) + "0";
    
	std::string dataAll;
    {
        std::string checksum =  m_robotStatus			+
								m_responseBS->m_tilt  	+
								m_responseBS->m_pan   	+
								m_responseBS->m_gyro  	+
								m_responseBS->m_ball  	+
								m_responseBS->m_count 	+
								m_responseBS->m_limit	+
								m_responseBS->m_locX	+
								m_responseBS->m_locY 	;
        
        std::string resultChecksum = utilities.SwapStr(checksum);

        //convert to all data
        dataAll =   m_robotStatus			+","+
					m_responseBS->m_tilt    +","+
					m_responseBS->m_pan     +","+
					m_responseBS->m_gyro    +","+
					m_responseBS->m_ball    +","+
					m_responseBS->m_count   +","+
					m_responseBS->m_limit   +","+
					m_responseBS->m_locX    +","+
					m_responseBS->m_locY    +","+
					m_responseBS->m_time    +","+
					resultChecksum          ;
    }
    
    // new memory alocation
    char * robotMsg = new char[dataAll.length()+1];
    strcpy(robotMsg, dataAll.c_str());

	return robotMsg;
}

void Basestation::ParseReceived(const char* buf)
{
    std::string robotFinal = utilities.IntToString(buf[1]);
    
	if (robotFinal == m_robotStatus ||robotFinal == "0" ||  robotFinal == "")
	    m_robot = false;
	else 
		m_robot = true;
	
    // GC assignment
    {

		m_gameController->STATE 					= buf[2];
		m_gameController->KICKOFF 					= buf[3];
		m_gameController->SECOND_STATE 				= buf[4];
		m_gameController->SECOND_STATE_TEAM 		= buf[5];
		m_gameController->SECOND_STATE_CONDITION	= buf[6];
    }
}


bool Basestation::GetRobot() const
{
	return m_robot;
}

Basestation::BasestationResponse* Basestation::GetResponseBS() const
{
	return m_responseBS;
}

void Basestation::UpdateCamera(const std::vector<u_char>& frame)
{
	m_frame.assign(frame.begin(), frame.end());
}

GameController *Basestation::GetGameController() const
{
	return m_gameController;
}