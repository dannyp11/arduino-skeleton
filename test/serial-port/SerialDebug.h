/*
 * SerialDebug.h
 *
 *  Created on: Mar 14, 2017
 *      Author: dat
 */

#ifndef SERIALDEBUG_H_
#define SERIALDEBUG_H_

#ifndef FOSC
#define FOSC 16000000		// Clock frequency
#endif

/*
 * Init serial port
 */
void SerialDebugInit(void);

/*
 * Printf to serial port
 */
void SerialDebugPrint(char* message);

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
