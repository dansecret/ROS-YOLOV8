#include "utilities.h"

#include <iostream>
#include <sys/time.h>   // time
#include <sstream>

Utilities::Utilities()
{
}

Utilities::~Utilities()
{
}

std::string Utilities::GetTime()
{
	std::ostringstream times;

	/*get milli*/
	timeval curTime;
	gettimeofday(&curTime, NULL);
	int milli = curTime.tv_usec / 1000;

	/* get seconds*/
	time_t rawtime = time(NULL);

	if (rawtime == -1) 
	{	
		puts("The time() function failed");
	}

	struct tm *ptm = localtime(&rawtime);

	if (ptm == NULL) 
	{
		puts("The localtime() function failed");
	}
    times<<ptm->tm_sec<<"."<<milli;
    
    return times.str();
}

// Function to reverse a string
std::string Utilities::SwapStr(std::string& str)
{
	int pointer1 = 0;
	int pointer2 = str.length()-1;

	// loop until mid stack
	while (pointer1 < pointer2) 
	{
		char temp = str[pointer1] ;
		str[pointer1] = str[pointer2] ;
		str[pointer2] = temp ;
		pointer1 += 1 ;
		pointer2 -= 1 ;
	}
	str.insert (0, 1, '1'); // insert one (1)
	return str;
}


std::string Utilities::IntToString(const int data) const
{
    std::stringstream strs;
    strs << data;
    return strs.str();
}
