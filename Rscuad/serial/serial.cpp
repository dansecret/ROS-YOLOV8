#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>	/* atoi */
#include <string.h>
#include <unistd.h>
#include <iostream>

#include "serial.h"
#include "../utilities/print.h"

#define VERSION 1

Serial::Serial()
: m_source1("/dev/ttyUSB1")
, m_source2("/dev/ttyACM1")
, m_charSearch(' ')
, m_dataButton(0)
, m_dataGyro(0)
{
}

Serial::~Serial()
{
	close(m_serPortFD);
}

void Serial::Init(int mode, const char * source, int baud)
{
	m_source = (char*)source;
	m_mode = mode;
	m_baud = baud;

	if(m_mode == 0)
		OldSetup();
	else if(m_mode == 1)
		CSetup();	// more faster but it is [vulnerability]

}

void Serial::SetInterfaceAtrribute(int fd, int speed)
{
	struct termios tty;

	/*
	* Get the current options for the port...
	*/
	if (tcgetattr(fd, &tty) < 0) 
	{
		RSCUAD_PRINT("[USB] Error from tcgetattr: %s\n", strerror(errno));
		return;
	}

	/*
	* Set the baud rates...
	*/
	cfsetospeed(&tty, (speed_t)speed);
	cfsetispeed(&tty, (speed_t)speed);

	tty.c_cflag &= ~PARENB; 		// Clear parity bit, disabling parity (most common)
	tty.c_cflag &= ~CSTOPB; 		// Clear stop field, only one stop bit used in communication (most common)
	tty.c_cflag &= ~CSIZE; 			// Clear all bits that set the data size 
	tty.c_cflag |= CS8; 			// 8 bits per byte (most common)
	tty.c_cflag &= ~CRTSCTS; 		// Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; 		// Turn on READ & ignore ctrl lines (CLOCAL = 1)

	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; 			// Disable echo
	tty.c_lflag &= ~ECHOE; 			// Disable erasure
	tty.c_lflag &= ~ECHONL; 		// Disable new-line echo
	tty.c_lflag &= ~ISIG; 			// Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

	tty.c_oflag &= ~OPOST; 			// Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; 			// Prevent conversion of newline to carriage return/line feed
	// tty.c_oflag &= ~OXTABS; 		// Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
	// tty.c_oflag &= ~ONOEOT; 		// Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

	tty.c_cc[VTIME] = 10;    		// Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 5;	
	tcflush(fd, TCIFLUSH);			// Flushes/Discards received data, but not read.
	/*
	* Set the new options for the port...
	*/
	if (tcsetattr(fd, TCSANOW, &tty) != 0) 
	{
		RSCUAD_PRINT("[USB] Error from tcsetattr: %s\n", strerror(errno));
		return ;
	}

}

void Serial::CSetup()
{
	m_serPortFD = open(m_source, O_RDWR | O_NOCTTY | O_NDELAY);

	if (m_serPortFD < 0) 
	{
		// Override source
		if((strcmp(m_source, "/dev/ttyUSB0") == 0))
		{
			m_serPortFD = open(m_source1, O_RDWR | O_NOCTTY | O_NDELAY);

		}
		else
			m_serPortFD = open(m_source2, O_RDWR | O_NOCTTY | O_NDELAY);

		if (m_serPortFD < 0) 
			RSCUAD_ERROR("[USB] ERROR \n");
	}

	else
		RSCUAD_SUCCESS("[USB] OK POSIX \n");

	/*baudrate B9600, 8 bits, no parity, 1 stop bit */
	SetInterfaceAtrribute(m_serPortFD, m_baud);
}

void Serial::OldSetup()
{
	m_serPort = fopen(m_source, "rb");

	if (m_serPort == NULL)
	{
		// Override source
		if((strcmp(m_source, "/dev/ttyUSB0") == 0))
			m_serPort = fopen(m_source1, "rb");
		else
			m_serPort = fopen(m_source2, "rb");

		if (m_serPort == NULL)
			RSCUAD_ERROR("[USB] ERROR \n");	
	}
	
	else
		RSCUAD_SUCCESS("[USB] OK \n");   

}

void Serial::CProcess()
{
	char gyro[50], button[50], version[10];
	char buff[100];
	int rdlen = read(m_serPortFD, buff, sizeof(buff) - 1);
	
	if (rdlen > 0) 
	{	
		buff[rdlen] = '\0';
		
		// scan space to split data 
		char *result = strchr(buff, m_charSearch );
		
		// check space available or not
		if(result != NULL)
		{
			sscanf(buff,"%s %s %s", gyro, button, version );	//split to gyro, button, version
			
			if (atoi(version) == VERSION)
			{
				m_dataGyro = atoi(gyro);
				m_dataButton = atoi(button);
			}
		}
		else
		{
			// check null value in buff
			if (atoi(version) != VERSION)
				RSCUAD_ERROR("[USB] button serial Space not found! %s\n", gyro);
		}
	}
}

void Serial::OldProcess()
{
	m_serPort =  fopen(m_source, "r");

	char gyro[50], button[50], version[10];
	char buff[1024];

	memset(buff, 0, 1024);
	size_t size = fread(buff, sizeof(char), 1024, m_serPort);

	if(size > 0)
	{
		// scan space to split data 
		char *result = strchr(buff, m_charSearch );
		
		// check space available or not
		if(result != NULL)
		{
			sscanf(buff,"%s %s %s", gyro, button, version );	//split to gyro, button, version

			if (atoi(version) == VERSION)
			{
				m_dataGyro = atoi(gyro);
				m_dataButton = atoi(button);
			}
		}
		else
		{
			// check null value in buff
			if (atoi(version) != VERSION)
				RSCUAD_ERROR("[USB] button serial Space not found! %s\n", gyro);	
		}
	}

	// close 
	fclose(m_serPort);
}

void Serial::Process()
{
	if(m_mode == 0)
		OldProcess();
	else if(m_mode == 1)
		CProcess();		// more faster
}

int Serial::Gyro() const
{
	return m_dataGyro;
}

int Serial::Button() const
{
	return m_dataButton;
}
