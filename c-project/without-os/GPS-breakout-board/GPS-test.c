/*
 * Test GPS module with parser
 *
 * Setup:
 * 		GPS TX - arduino RX (0)
 */

#include "NMEAParser.h"
#include "SerialDebug.h"
#include "SoftSerial.h"
#include "LCD.h"

#include <util/delay.h>
#include <string.h>

int main(void)
{
	LCDInit();
	SerialDebugInitWithBaudRate(9600);
	SoftSerialInit();
	double distance = 0.0f;
	NMEAData initLocation, prevData;
	initLocation.isValid = 0;

	// get init location
	while (!initLocation.isValid)
	{
		char gps_msg[128];
		SerialDebugGetLine(gps_msg, 0);
		if (strlen(gps_msg) < 10)
			continue;

		NMEAParserParseString(gps_msg, &initLocation);

		if (!initLocation.isValid)
		{
			SoftSerialPrintLn("Failed initial parsed message: %s", gps_msg);
		}
	}

	prevData = initLocation;

	while (1)
	{
		char gps_msg[128];
		NMEAData gpsData;
		SerialDebugGetLine(gps_msg, 0);
		if (strlen(gps_msg) < 10)
			continue;

		uint8_t parseResult = NMEAParserParseString(gps_msg, &gpsData);

		if (gpsData.isValid && parseResult == 0)
		{
			SoftSerialPrintLn("Parse succeeds, message %s", gps_msg);
			SoftSerialPrintLn("lat %d %lf", gpsData.location.lat_deg,
					gpsData.location.lat_min);
			SoftSerialPrintLn("lon %d %lf", gpsData.location.lon_deg,
					gpsData.location.lon_min);

			distance += NMEAGetDistance(&prevData.location, &gpsData.location);
			SoftSerialPrintLn("traveled distance %.2lf m", distance);

			double angle = NMEAGetAngle(&initLocation.location, &gpsData.location);
			SoftSerialPrintLn("angle from initial location %.2lf degrees", angle);

			double distanceToInitPlace = NMEAGetDistance(&initLocation.location, &gpsData.location);
			SoftSerialPrintLn(" %4.0lf m to initial place", distanceToInitPlace);

			SoftSerialPrintLn(" ");

			LCDSetCursor(1,0);
			LCDPrint("traveled %.2lf m", distance);
			LCDSetCursor(2,0);
			LCDPrint("angle %.2lf degrees", angle);
			LCDSetCursor(3,0);
			LCDPrint("%4.0f m to init place", distanceToInitPlace);

			prevData = gpsData;
		}

		_delay_ms(100);
	}

	return 0; /* never reached */
}
