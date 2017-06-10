/*
 * SoftSerial.c
 *
 *  Created on: Jun 6, 2017
 *      Author: dat
 */
#include "SoftSerial.h"
#include "AVRString.h"

#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>

#define TOGGLE_PIN(port, pin, isOn) \
{	if ((isOn) == 0) port &= ~(1 << (pin)); \
	else port |= (1 << (pin)); \
}

static long _SS_baudrate;

/**
 * Call this before running any below api
 */
void SoftSerialInitWithBaud(long baud)
{
	static uint8_t isInited = 0;
	if (isInited)
	{
		return;
	}
	isInited = 1;

	// config tx, rx port
	TOGGLE_PIN(SS_DDR_PORT_TX, SS_TX_PIN, 1); // output on tx
	TOGGLE_PIN(SS_PORT_TX, SS_TX_PIN, 1); // stop bit
	_SS_baudrate = baud;
}

void SoftSerialInit()
{
	SoftSerialInitWithBaud(SS_DEFAULT_BAUD);
}

inline void mDelayBaud()
{
	switch (_SS_baudrate)
	{
	case 9600:
	{
		_delay_us(1000000ul / 9600);
	}
		break;
	case 19200:
	{
		_delay_us(1000000ul / 19200);
	}
		break;
	case 38400:
	{
		_delay_us(1000000ul / 38400 - 1);
	}
		break;
	case 57600:
	{
		_delay_us(1000000ul / 57600 - 2);
	}
		break;
	case 115200:
	{
		_delay_us(1000000ul / 115200 - 3);
	}
		break;

	default:
		break;
	}
}

/**
 * Send char over soft serial
 *
 * @param c
 */
void SoftSerialPutChar(char c)
{
	// calculate parity bit
	uint8_t parity = 0;

	// start bit
	TOGGLE_PIN(SS_PORT_TX, SS_TX_PIN, 0);
	mDelayBaud();

	// data
	uint8_t bit_mask;
	for (bit_mask = 0x01; bit_mask; bit_mask <<= 1)
	{
		TOGGLE_PIN(SS_PORT_TX, SS_TX_PIN, (c & bit_mask));
		parity ^= (c & bit_mask);
		mDelayBaud();
	}

	// parity bit
	TOGGLE_PIN(SS_PORT_TX, SS_TX_PIN, parity);
	mDelayBaud();

	// stop bit
	TOGGLE_PIN(SS_PORT_TX, SS_TX_PIN, 1);
	_delay_us(1);
}

/**
 * Main internal printf function
 *
 * @param format
 * @param args
 */
static void _mSSPrint(const char* format, va_list args)
{
	char buffer[STRING_MAXLEN];
	vsnprintf(buffer, STRING_MAXLEN, format, args);

	uint8_t i;
	for (i = 0; i < strlen(buffer); ++i)
	{
		SoftSerialPutChar(buffer[i]);
	}

	va_end(args);
}

void SoftSerialPrint(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	_mSSPrint(format, args);
	va_end(args);
}

void SoftSerialPrintLn(const char* format, ...)
{
	/* Declare a va_list type variable */
	va_list myargs;
	/* Initialise the va_list variable with the ... after format */
	va_start(myargs, format);

	_mSSPrint(format, myargs);
	SoftSerialPutChar('\n');
	SoftSerialPutChar('\r');

	/* Clean up the va_list */
	va_end(myargs);
}
