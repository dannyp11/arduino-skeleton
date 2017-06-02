/*
 * SerialDebug.c
 *
 *  Created on: Mar 14, 2017
 *      Author: dat
 */

#include <avr/io.h>
#include <stdio.h>
#include "SerialDebug.h"

#define MYUBRR F_CPU/16/SERIALDEBUG_DEFAULT_BAUD-1   // Value for UBRR0 register

// custom up/down btn that breaks getline loop
static char g_serialdebug_stopchars[21];
static uint8_t g_serialdebug_stopcharsLen;

void SerialDebugSetStopChars(const char * stopChars)
{
	strcpy(g_serialdebug_stopchars, stopChars);
	g_serialdebug_stopcharsLen = strlen(stopChars);

	SerialDebugPrint("===============================================");
	SerialDebugPrint("| Serial Debug Set Custom Chars '%s'", stopChars);
	SerialDebugPrint("===============================================");
}

/*
 serial_in - Read a byte from the USART0 and return it
 */
char SerialDebugGetChar()
{
	loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
	return (char) UDR0;
}

/*
 sci_out - Output a byte to SCI port
 */
void sci_out(char ch)
{
	loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
	UDR0 = ch;
}

/*
 sci_outs - Print the contents of the character string "s" out the SCI
 port. The stSerialDebugSetStopCharsring must be terminated by a zero byte.
 */
void sci_outs(const char *s)
{
	unsigned char ch;

	while ((ch = *s++) != (char) '\0')
	{
		sci_out(ch);
	}
}

void SerialDebugInit()
{
	SerialDebugInitWithBaudRate(SERIALDEBUG_DEFAULT_BAUD);
}

void SerialDebugInitWithBaudRate(unsigned baudrate)
{
	UBRR0 = F_CPU / 16 / baudrate - 1;          // Set baud rate
	UCSR0B |= (1 << TXEN0);  // Turn on transmitter
	UCSR0B |= (1 << RXEN0); // Turn on receiver
	UCSR0A &= ~(_BV(U2X0));
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */

	SerialDebugPrint("===============================================");
	SerialDebugPrint("| Serial Debug activated, baud rate %u", baudrate);
	SerialDebugPrint("| Compiled on : %s %s", __DATE__, __TIME__);
#ifdef DEBUG
	SerialDebugPrint("| Debug mode ON");
#endif
	SerialDebugPrint("===============================================");

	g_serialdebug_stopcharsLen = 0;
}

/*
 * Printf to serial port
 */
void _SerialDebugPrint1(const char* message)
{
	sci_outs(message);
	sci_outs("\n\r");
}

void _SerialDebugPrintNoEndl(const char* message)
{
	sci_outs(message);
}

uint8_t isDefaultStopCharacter(char c)
{
	uint8_t retVal = 0;
	const char stopChars[] = { '\n', '\0', '\r' };
	uint8_t i;
	for (i = 0; i < 3; ++i)
	{
		retVal |= (stopChars[i] == c);
	}
	return retVal;
}

uint8_t isCustomStopCharacter(char c)
{
	uint8_t retVal = 0;

	// custom stop chars
	uint8_t i;
	if (g_serialdebug_stopcharsLen)
	{
		for (i = 0; i < g_serialdebug_stopcharsLen; ++i)
		{
			retVal |= (c == g_serialdebug_stopchars[i]);
		}
	}

	return retVal;
}

void SerialDebugGetLine(char* buffer, char echo)
{
	uint8_t i = 0;
	while (1)
	{
		buffer[i] = SerialDebugGetChar();
		if (echo)
		{
			if (buffer[i] == '\b')
			{
				sci_outs(" \b");
			}

			if (!isCustomStopCharacter(buffer[i]))
			{
				sci_out(buffer[i]); // echo
			}
		}

		if (isCustomStopCharacter(buffer[i]))
		{
			++i;
			break;
		}

		if (isDefaultStopCharacter(buffer[i]))
		{
			break;
		}

		++i;
	}
	buffer[i] = '\0';
	if (buffer[0] == '\b')
		buffer[0] = 0x02; // \b at start of string

	// refine buffer for backspace support
	AVRStringRefineString(buffer);

	if (echo)
	{
		sci_outs("\n\r");
	}
}
