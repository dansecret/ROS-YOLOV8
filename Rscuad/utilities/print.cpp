
#include <string> //vprintf
#include <stdarg.h>
#include <cstring>

#include "print.h"

void RSCUAD_PRINT(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	vprintf(format, argList);
	va_end(argList);
}

void RSCUAD_ERROR(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);

	printf(_BOLDRED);
	vprintf(format, argList);
	printf(_RESET);

	va_end(argList);
}

void RSCUAD_INFO(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);

	printf(_BOLDBLUE);
	vprintf(format, argList);
	printf(_RESET);

	va_end(argList);
}

void RSCUAD_WARNING(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);

	printf(_BOLDYELLOW);
	vprintf(format, argList);
	printf(_RESET);

	va_end(argList);
}

void RSCUAD_SUCCESS(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);

	printf(_BOLDGREEN);
	vprintf(format, argList);
	printf(_RESET);

	va_end(argList);
}