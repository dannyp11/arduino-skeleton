/*
 * SerialDebug.c
 *
 *  Created on: Mar 14, 2017
 *      Author: dat
 */

#include <avr/io.h>
#include "SerialDebug.h"

#define BAUD 9600              // Baud rate used by the LCD
#define MYUBRR FOSC/16/BAUD-1   // Value for UBRR0 register

/*
 serial_in - Read a byte from the USART0 and return it
 */
char serial_in()
{
	while (!( UCSR0A & (1 << RXC0)))
		;
	return UDR0;
}

/*
 sci_out - Output a byte to SCI port
 */
void sci_out(char ch)
{
	while ((UCSR0A & (1 << UDRE0)) == 0)
		;
	UDR0 = ch;
}

/*
 sci_outs - Print the contents of the character string "s" out the SCI
 port. The string must be terminated by a zero byte.
 */
void sci_outs(char *s)
{
	unsigned char ch;

	while ((ch = *s++) != (char) '\0')
	{
		sci_out(ch);
	}
}

void SerialDebugInit()
{
	UBRR0 = MYUBRR;          // Set baud rate
	UCSR0B |= (1 << TXEN0);  // Turn on transmitter
	UCSR0B |= (1 << RXEN0); // Turn on receiver
	UCSR0C = (3 << UCSZ00);  // Set for asynchronous operation, no parity,
							 // one stop bit, 8 data bits
}

/*
 * Printf to serial port
 */
void SerialDebugPrint(char* message)
{
	sci_outs(message);
	sci_outs("\n\r");
}
