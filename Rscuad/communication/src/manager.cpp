#include "../include/manager.h"
#include "../../utilities/print.h"

#include <unistd.h>

#include <mutex>
#include <thread>

#define TIME_DELAY	100000

std::mutex mtxBS;
std::mutex mtxCam;
std::mutex mtxGC;

Manager::Manager()
: basestation(new Basestation())
, referee(new Referee())
{
}

Manager::~Manager()
{
}

void Manager::Init(ConfigRobot * configRb)
{
    // Copy struct to this memory class
    configRobot = *configRb;

    // Init network
    basestation->Init(configRobot.m_IP, configRobot.m_robotID);
	
	if(configRobot.m_isReferee)
    {
        referee->Init(configRobot.m_IPGC, configRobot.m_robotID, configRobot.m_team);
	}
    
    RSCUAD_SUCCESS("[COM] set IP Target %s \n",configRobot.m_IP );
    RSCUAD_SUCCESS("[COM] set ID Robot %d \n",configRobot.m_robotID);
}

bool Manager::GetRobot() const
{
    return basestation->GetRobot();
}

Basestation::BasestationResponse *Manager::GetResponseBS() const
{
	return basestation->GetResponseBS();
}

GameController *Manager::GetGameController() const
{
	return  (configRobot.m_isReferee) ? referee->GetGameController() : basestation->GetGameController();
}

void Manager::UpdateCamera(const std::vector<u_char>& frame) const
{
	basestation->UpdateCamera(frame);
}

void Manager::CreateAsync()
{
	// Basestation
	{
		Basestation* bs = basestation.get();
		std::thread workerSendBS(AsyncBaseStation, bs);
		workerSendBS.detach();
	}

	// GameController
	if(configRobot.m_isReferee)
	{     
		Referee* rf = referee.get();
		std::thread workerSendRF(AsyncReferee, rf);
		workerSendRF.detach();
	}

	// Realtime Camera
	if(configRobot.m_isRealtimeCam)
	{     
		Basestation* bs = basestation.get();
		std::thread workerSendBS(AsyncCamera, bs);
		workerSendBS.detach();
	}
}

void Manager::AsyncBaseStation(Basestation *bs)
{
	// This is asynchronous
	for(;;)
	{
		std::lock_guard<std::mutex> guard(mtxBS);
		bs->Send();
		bs->Receive();
		usleep(TIME_DELAY);
	}
}

void Manager::AsyncReferee(Referee *rf)
{
	// This is asynchronous
	for(;;)
	{
		std::lock_guard<std::mutex> guard(mtxGC);
		rf->Send();
		rf->Receive();
		usleep(TIME_DELAY);
	}
}

void Manager::AsyncCamera(Basestation *bs)
{
	// This is asynchronous
	for(;;)
	{
		std::lock_guard<std::mutex> guard(mtxCam);
		bs->SendCamera();
		usleep(TIME_DELAY);
	}
}
