/**
 *	Title	: Serial from arduino tx
 *
 *	Author	: Danu Andrean
 *	Create	: 2020
 *	Update 	: 2023
 *	
 *	Note	:
 */

#ifndef SERIAL_H
#define SERIAL_h

#include <termios.h>
#include <stdio.h>

class Serial
{
private:
	char*		m_source;
	const char*	m_source1;
	const char*	m_source2;
	
	int			m_baud;
	FILE*		m_serPort;
	int			m_serPortFD;
	int 		m_dataGyro;
	int			m_dataButton;
	int 		m_mode;
	int			m_charSearch;

private:
	void SetInterfaceAtrribute(int fd, int speed);
	void CSetup();
	void CProcess();
	void OldSetup();
	void OldProcess();

public:
	Serial();
	~Serial();

	void Init(int mode = 0, const char* source="/dev/ttyUSB0", int baud=B9600);
	void Process();
	int Gyro() const;
	int Button() const;
};

#endif

