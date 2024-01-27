/**
 *	Title	: Node library for multi file and support ROS
 *
 *	Author	: Danu Andrean
 *	Create	: 2023
 *	Update 	: 2023
 *	
 *	Note	: 
 */

#ifndef RSCUAD_H
#define RSCUAD_H

#include "camera/api/camera.h"
#include "serial/serial.h"
#include "utilities/print.h"
#include "communication/communication.h"

class RSCUAD
{
public:
	std::unique_ptr<Serial> 		serial;
	std::unique_ptr<Communication> 	communication;
	std::unique_ptr<Camera> 		camera;

private:
	static RSCUAD *m_instance;

public:
	RSCUAD()
	: serial(new Serial())
	, communication(new Communication())
	, camera(new Camera())
	{
	}
	
	~RSCUAD()
	{
	}

	static RSCUAD *Get()
	{
		if(m_instance == nullptr)
			m_instance = new RSCUAD();
		return m_instance;
	}
};

RSCUAD *RSCUAD::m_instance;
#endif
