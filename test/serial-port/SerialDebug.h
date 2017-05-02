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
void SerialDebugInit();

/*
 * Printf to serial port
 */
void SerialDebugPrint(char* message);

/*
 * Fetches byte from receive buffer
 */
char serial_in();

#endif /* SERIALDEBUG_H_ */
