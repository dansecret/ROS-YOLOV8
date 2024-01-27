/**
 *	Title	: Network manager 
 *
 *	Author	: Danu Andrean
 *	Create	: 2020
 *	Update 	: 2023
 *	
 *	Note	: Manager all network such as Basestation and Game Controller
 */

#ifndef MANAGER_H
#define MANAGER_H

#include <string>

#include "basestation.h"
#include "referee.h"
#include <memory>

class Manager
{
public:
	struct ConfigRobot
	{
		char*   m_IP;
		char*   m_IPGC; 
		int     m_robotID;
		bool    m_isReferee;
		int     m_team;
		bool 	m_isRealtimeCam;
	};

private:
	struct ConfigRobot  configRobot;

private:
	// class object
	std::shared_ptr<Basestation>    basestation;
	std::shared_ptr<Referee>        referee;

private:
	static void AsyncBaseStation(Basestation* bs);
	static void AsyncReferee(Referee* rf);
	static void AsyncCamera(Basestation *bs);

public:
	Manager();
	~Manager();

	void Init(ConfigRobot *configRb);
	bool GetRobot() const;
	void CreateAsync();

	void UpdateCamera(const std::vector<u_char>& frame) const;

	Basestation::BasestationResponse *GetResponseBS() const;
	GameController *GetGameController() const;
};

#endif