 /**
 *	Title	: Gateway communication to robot
 *
 *	Author	: Danu Andrean
 *	Create	: 2020
 *	Update 	: 2023
 *	
 *	Note	:
 */

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "include/manager.h"
#include "../utilities/utilities.h"

class Communication
{
private:
	Manager						manager;
	Utilities					utilities;

	struct Manager::ConfigRobot	configRobot;
    
public:
    static bool s_robot;
	static int	s_team;
	static GameController *s_GC;

    Communication();
    ~Communication();
    void Init(const char *ip, const int robotID, const bool isReferee=false, const int team=7, const bool isRealtimeCam=false);
    void Robot(int robot,int tilt, int pan, 
                int gyro, int ball, int count,
                int limit, float locX = 0, float locY = 0);
    void Camera(const std::vector<u_char>& frame) const;
	void SetIPGC(const char* ip);
};

#endif