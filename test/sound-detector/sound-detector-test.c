/*******************************************************************************
 *
 * Created on May 2, 2017 by Dat (Daniel) Pham
 *
 * Filename: sound-detector-test.c
 *
 * Description: detects sound and output to serial port
 *
 * Limitations: - depends on serial-port
 *
 * Environment:
 * circuit setup: 	OUT - pin10
 * 					GND - GND
 * 					VCC - 5v
 *
 *
 * Version:
 *
 * History: 
 *
 ******************************************************************************/

#include <util/delay.h>
#include <avr/io.h>
#include "SerialDebug.h"

int main()
{
	// init
	PORTB |= (1 << PB2);
	SerialDebugInit();
	char mode = 0;

	while(1)
	{
		if (PINB & (1 << PB2))
		{
			if (!mode)
				SerialDebugPrint("Detected");
			mode = 1;
		}
		else
		{
			if (mode)
				SerialDebugPrint("Quiet");
			mode = 0;
		}

		_delay_ms(1);
	}

	return 0;
}
