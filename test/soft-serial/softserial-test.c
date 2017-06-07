/*
 * Demo Serial connection with pc
 */

#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "SerialDebug.h"
#include "SoftSerial.h"

int main()
{
	SerialDebugInit();
	SoftSerialInit();

	while (1)
	{
		SoftSerialPutChar('c');
		SoftSerialPutChar('c');
		SoftSerialPutChar('a');
		_delay_ms(500);
		SoftSerialPutChar('\n');
		SoftSerialPutChar('\r');
		_delay_us(30);

		SerialDebugPrint("reading 1 char...");
		char c = SoftSerialGetChar();
		SerialDebugPrint("'%c' = %d", c, c);
	}

	return 0;
}
