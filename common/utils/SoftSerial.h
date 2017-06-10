/*
 * SoftSerial.h
 *
 *  Created on: Jun 6, 2017
 *      Author: dat
 */

#ifndef UTILS_SOFTSERIAL_H_
#define UTILS_SOFTSERIAL_H_

#include <stdarg.h>
#include "MemoryFree.h"

// TX pin config
#define SS_TX_PIN		PB1
#define SS_PORT_TX		PORTB
#define SS_DDR_PORT_TX	DDRB

// RX pin config
//#define SS_RX_PIN			PB2
//#define SS_PORT_RX			PORTB
//#define SS_DDR_PORT_RX		DDRB
//#define SS_PINB_RX			PINB

#define SS_DEFAULT_BAUD		57600

/**
 * Call this before running any below api
 * 8 bits data, 1 odd parity, 1 stop bit
 *
 * Tested baud:
 * 	TX upto 57600, 115200
 * 	RX - N/A
 */
void SoftSerialInitWithBaud(long baud);
void SoftSerialInit();

/**
 * Send char over soft serial
 *
 * @param c
 */
void SoftSerialPutChar(char c);

/**
 *	Printf string with/without endl
 *
 */
void SoftSerialPrint(const char* format, ...);
void SoftSerialPrintLn(const char* format, ...);


// Debug arena
#ifdef DEBUG
	#ifdef CXXTEST
		#ifndef SSLOG
			#define SSLOG(msg, arg...) \
				{ char __msg[STRING_MAXLEN]; \
				sprintf(__msg, msg, ##arg);\
				printf("%s:%d - %s \n", __FILE__, __LINENAME__, __msg); \
				}
		#endif
	#else
		#ifndef __FILENAME__
			#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__) // show file name only
		#endif
		#ifndef SSLOG // main debug code
			#define SSLOG(msg, arg...) \
				SoftSerialPrint("%s:%d - (%d)\t", __FILENAME__, __LINE__, freeMemory()); \
				SoftSerialPrintLn(msg, ##arg)
		#endif
	#endif

#else
	#ifndef SSLOG // main debug code
		#define SSLOG(...) ;
	#endif
#endif

#ifndef SSTRACE
	#define SSTRACE() SSLOG(" ")
#endif

#ifndef SSTRACE_INT
	#define SSTRACE_INT(var) SSLOG("[%s] = %d", #var, var)
#endif

#ifndef SSTRACE_STRING
	#define SSTRACE_STRING(var) SSLOG("[%s] = %s", #var, var)
#endif

#endif /* UTILS_SOFTSERIAL_H_ */
