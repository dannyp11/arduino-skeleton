/*
 * Test GPS module with parser
 *
 * Setup:
 * 		GPS TX - arduino RX (0)
 */

#include "NMEAParser.h"
#include "SerialDebug.h"
#include "LCD.h"

#include <util/delay.h>
#include <string.h>

int main(void)
{
	LCDInit();
	SerialDebugInitWithBaudRate(9600);
	float distance = 0.0f;
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
			SerialDebugPrint("Failed initial parsed message: %s", gps_msg);
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
			SerialDebugPrint("Parse succeeds, message %s", gps_msg);
			SerialDebugPrint("lat %d %f", gpsData.location.lat_deg,
					gpsData.location.lat_min);
			SerialDebugPrint("lon %d %f", gpsData.location.lon_deg,
					gpsData.location.lon_min);

			distance += NMEAGetDistance(&prevData.location, &gpsData.location);
			SerialDebugPrint("traveled distance %.2f m", distance);

			float angle = NMEAGetAngle(&initLocation.location, &gpsData.location);
			SerialDebugPrint("angle from initial location %.2f degrees", angle);

			float distanceToInitPlace = NMEAGetDistance(&initLocation.location, &gpsData.location);
			SerialDebugPrint(" %4.0f m to initial place", distanceToInitPlace);

			SerialDebugPrint(" ");

			LCDSetCursor(1,0);
			LCDPrint("traveled %.2f m", distance);
			LCDSetCursor(2,0);
			LCDPrint("angle %.2f degrees", angle);
			LCDSetCursor(3,0);
			LCDPrint("%4.0f m to init place", distanceToInitPlace);

			prevData = gpsData;
		}

		_delay_ms(100);
	}

	return 0; /* never reached */
}
