#include "communication.h"

// static declaration
bool 	Communication::s_robot;
int		Communication::s_team;
GameController *Communication::s_GC;

Communication::Communication()
{
	configRobot.m_IPGC = (char*)"0.0.0.0";
}

Communication::~Communication()
{
}

void Communication::Init(const char * ip, const int robotID, const bool isReferee, const int team, const bool isRealtimeCam)
{
	{
		configRobot.m_IP        	= (char*)ip;
		configRobot.m_robotID   	= robotID;
		configRobot.m_isReferee 	= isReferee;
		configRobot.m_team      	= team;
		configRobot.m_isRealtimeCam	= isRealtimeCam;
	}

	manager.Init(&configRobot);
	Communication::s_robot	= false;
	Communication::s_team	= configRobot.m_team;
	Communication::s_GC		= manager.GetGameController(); // create zero memory
	manager.CreateAsync();
}

void Communication::Robot(int robot,int tilt, int pan, 
                        int gyro, int ball, int count,
                        int limit, float locX, float locY)
{
    {
        manager.GetResponseBS()->m_robot       = utilities.IntToString(robot);
        manager.GetResponseBS()->m_tilt        = utilities.IntToString(tilt);
        manager.GetResponseBS()->m_pan         = utilities.IntToString(pan);
        manager.GetResponseBS()->m_gyro        = utilities.IntToString(gyro);
        manager.GetResponseBS()->m_ball        = utilities.IntToString(ball);
        manager.GetResponseBS()->m_count       = utilities.IntToString(count);
        manager.GetResponseBS()->m_limit       = utilities.IntToString(limit);
		manager.GetResponseBS()->m_locX       = utilities.IntToString(locX);
		manager.GetResponseBS()->m_locY       = utilities.IntToString(locY);
        manager.GetResponseBS()->m_time        = std::string(utilities.GetTime());
    }


    Communication::s_robot      = manager.GetRobot();
	Communication::s_GC			= manager.GetGameController();
}

void Communication::Camera(const std::vector<u_char>& frame)const
{
    manager.UpdateCamera(frame);
}

void Communication::SetIPGC(const char* ip)
{
	configRobot.m_IPGC = (char*)ip;
}