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

#ifndef CXXTEST
#define LOG(msg, arg...) \
	{ char __msg[128]; \
	snprintf(__msg, 128, msg, ##arg);\
	SerialDebugPrint("%s:%d - %s", __FILE__, __LINE__, __msg); \
	}
#else
#define LOG(msg, arg...) \
	{ char __msg[128]; \
	sprintf(__msg, msg, ##arg);\
	printf("%s:%d - %s \n", __FILE__, __LINE__, __msg); \
	}
#endif

#define TRACE() LOG(" ");
#define TRACE_INT(var) LOG("[%s] = %d", #var, var);
#define TRACE_STRING(var) LOG("[%s] = %s", #var, var);

/*
 * Init serial port
 */
void SerialDebugInit(void);

/*
 * Printf to serial port
 */
#define SerialDebugPrint(arg...) \
{ 	char _msg[128];\
	snprintf(_msg, 128, ##arg); \
	_SerialDebugPrint1(_msg); \
}
void _SerialDebugPrint1(const char* message);

/*
 * Fetches byte from receive buffer
 */
char SerialDebugGetChar(void);

/**
 * Get line from serial input
 * This will wait until \n is entered, so not async compatible
 *
 * @param buffer - line will be stored in here, make sure buffer is allocated
 * 					before calling this function
 * @param echo - 1 will echo character to console
 */
void SerialDebugGetLine(char* buffer, char echo);

#endif /* SERIALDEBUG_H_ */
