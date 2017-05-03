/*
 * Demo Compass I2C connection
 */

#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "Compass.h"
#include "SerialDebug.h"

int main(void)
{
	SerialDebugInit();
	SerialDebugPrint("Compass test");

	CompassInit();
	char msgSerial[80];
	char directionText[10];

	while (1)
	{
		// Loop forever
//		float header = CompassGetAngle();
//		CompassDirection direction = CompassGetDirection();
//
//		CompassGetDirectionText(directionText, direction);
//		snprintf(msgSerial, 80, "compass angle %3.3f direction %s", header, directionText);
//		SerialDebugPrint(msgSerial);

		_delay_ms(200);
	}

	return 0;
}

