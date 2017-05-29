/*
 * Test GPS module with parser
 *
 * Setup:
 * 		GPS TX - arduino RX (0)
 */

#include "NMEAParser.h"
#include "SerialDebug.h"

#include <util/delay.h>
#include <string.h>

int main(void)
{
	SerialDebugInit();
	float distance = 0.0f;
	NMEAData initLocation;
	initLocation.isValid = 0;

	// get init location
	while (!initLocation.isValid)
	{
		char gps_msg[128];
		SerialDebugGetLine(gps_msg, 0);
		if (strlen(gps_msg) < 10)
			continue;

		NMEAParserParseString(gps_msg, &initLocation);
	}

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

			distance += NMEAGetDistance(&initLocation.location, &gpsData.location);
			SerialDebugPrint("traveled distance %f", distance);

			SerialDebugPrint(" ");
		}

		_delay_ms(100);
	}

	return 0; /* never reached */
}
