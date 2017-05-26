/*
 * SerialDebug.h
 *
 *  Created on: Mar 14, 2017
 *      Author: dat
 */

#ifndef SERIALDEBUG_H_
#define SERIALDEBUG_H_

#include <stdio.h>

#ifndef FOSC
#define FOSC F_CPU		// Clock frequency
#endif

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
 */
void SerialDebugGetLine(char* buffer);

#endif /* SERIALDEBUG_H_ */
