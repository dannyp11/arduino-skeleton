#include "NMEAParser.h"
#include "SerialDebug.h"

int main(void)
{
	SerialDebugInit();

	while (1)
	{
		char gps_msg[128];
		NMEAData gpsData;
		SerialDebugGetLine(gps_msg, 0);
		SerialDebugPrint("message %s", gps_msg);
		NMEAParserParseString(gps_msg, &gpsData);

		if (gpsData.isValid)
		{
			SerialDebugPrint("lat %d %f", gpsData.location.lat_deg, gpsData.location.lat_min);
		}
	}

	return 0; /* never reached */
}
