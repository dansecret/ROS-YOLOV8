/**
 *	Title	: Print console RSCUAD
 *
 *	Author	: Danu Andrean
 *	Create	: 2022
 *	Update 	: 2023
 *	
 *	Note	:
 */

#ifndef PRINT_H
#define PRINT_h

#define _RESET   "\033[0m"
#define _BLACK   "\033[30m"      /* Black */
#define _RED     "\033[31m"      /* Red */
#define _GREEN   "\033[32m"      /* Green */
#define _YELLOW  "\033[33m"      /* Yellow */
#define _BLUE   "\033[34m"      /* Blue */
#define _MAGENTA "\033[35m"      /* Magenta */
#define _CYAN    "\033[36m"      /* Cyan */
#define _WHITE   "\033[37m"      /* White */
#define _BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define _BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define _BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define _BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define _BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define _BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define _BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define _BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

void RSCUAD_PRINT(const char* format, ...);
void RSCUAD_ERROR(const char* format, ...);
void RSCUAD_INFO(const char* format, ...);
void RSCUAD_WARNING(const char* format, ...);
void RSCUAD_SUCCESS(const char* format, ...);

#endif