/*
 * SerialDebug.h
 *
 *  Created on: Mar 14, 2017
 *      Author: dat
 */

#ifndef SERIALDEBUG_H_
#define SERIALDEBUG_H_

#include <stdio.h>
#include <string.h>

#include "MemoryFree.h"
#include "AVRString.h"

#define SERIALDEBUG_DEFAULT_BAUD	57600 // default baud rate when calling SerialDebugInit()
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__) // show file name only


#ifndef LOG
#ifdef DEBUG
#ifndef CXXTEST

#define LOG(msg, arg...) \
	{  char __msg[STRING_MAXLEN];\
	snprintf(__msg, STRING_MAXLEN, msg, ##arg);\
	SerialDebugPrint("%s:%d - (%d)\t%s", __FILENAME__, __LINE__, freeMemory(), __msg); \
	}
#else
#define LOG(msg, arg...) \
	{ char __msg[STRING_MAXLEN]; \
	sprintf(__msg, msg, ##arg);\
	printf("%s:%d - %s \n", __FILE__, __LINENAME__, __msg); \
	}
#endif
#else
#define LOG(...) ;
#endif

#define TRACE() LOG(" ");
#define TRACE_INT(var) LOG("[%s] = %d", #var, var);
#define TRACE_STRING(var) LOG("[%s] = %s", #var, var);

#endif

/*
 * Init serial port, use default baud rate
 */
void SerialDebugInit(void);

/**
 * Set baud rate for serial port and reinit it
 */
void SerialDebugInitWithBaudRate(unsigned baudrate);

#ifndef SerialDebugPrint
/*
 * Printf to serial port
 */
#define SerialDebugPrint(arg...) \
{ 	char _msg[STRING_MAXLEN];\
	snprintf(_msg, STRING_MAXLEN, ##arg); \
	_SerialDebugPrint1(_msg); \
}
#define SerialDebugPrintNoEndl(arg...) \
{ 	char _msg[STRING_MAXLEN];\
	snprintf(_msg, STRING_MAXLEN, ##arg); \
	_SerialDebugPrintNoEndl(_msg); \
}
#endif
void _SerialDebugPrint1(const char* message);
void _SerialDebugPrintNoEndl(const char* message);


/*
 * Fetches byte from receive buffer
 */
char SerialDebugGetChar(void);

/**
 * Get line from serial input
 * This will wait until \n is entered, so not async compatible
 *
 * Also, it will contain custom stopchars
 *
 * @param buffer - line will be stored in here, make sure buffer is allocated
 * 					before calling this function
 * @param echo - 1 will echo character to console
 */
void SerialDebugGetLine(char* buffer, char echo);

/**
 * Set custom stop chars beside normal ones like \r \n
 * @param stopChars - has to be normal viewable chars such as "[]{}", limit to 20 chars
 */
void SerialDebugSetStopChars(const char * stopChars);

#endif /* SERIALDEBUG_H_ */
