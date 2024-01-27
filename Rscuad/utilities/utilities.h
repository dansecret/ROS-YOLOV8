/**
 *	Title	: Utilities
 *
 *	Author	: Danu Andrean
 *	Create	: 2022
 *	Update 	: 
 *	
 *	Note	:
 */


#ifndef UTILITIES_H
#define UTILITIES_H
#include <string.h>
#include <string>

class Utilities
{
private:

public:
    Utilities();
    ~Utilities();
    std::string GetTime();
	std::string SwapStr(std::string& str);
	std::string IntToString(const int data) const;

};
#endif