/*
 * NMEAParser.h
 *
 *  Created on: May 27, 2017
 *      Author: dat
 */

#ifndef TEST_GPS_BREAKOUT_BOARD_NMEAPARSER_H_
#define TEST_GPS_BREAKOUT_BOARD_NMEAPARSER_H_

#include <inttypes.h>

typedef struct _NMEALocation
{
	float lat_deg, lat_min; // degree sign + for N, E, - for S, W
	float lon_deg, lon_min;
} NMEALocation;

typedef struct _NMEAData
{
	NMEALocation location;
	uint32_t timestamp; // military time hhmmss
	uint32_t date; // format ddmmyy
	float speed; // in knot
	uint8_t isValid;
} NMEAData;


/**
 * Parse message and put data in result
 *
 * Log:		supporting GPRMC
 *
 *
 * @param message - NMEA msg to be parsed
 * @param result - pointer to result data
 * @return 	0 on success
 * 			1 on format error
 * 			2 on checksum error
 */
uint8_t NMEAParserParseString(const char * message, NMEAData * result);

#endif /* TEST_GPS_BREAKOUT_BOARD_NMEAPARSER_H_ */
