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
	int lat_deg, lon_deg; // degree sign + for N, E, - for S, W
	float lat_min, lon_min;
} NMEALocation;

typedef struct _NMEAData
{
	NMEALocation location;
	int timestamp; // military time hhmmss
	unsigned date; // format ddmmyy
	float speed; // in knot
	uint8_t isValid;
} NMEAData;


/**
 * Parse message and put data in result
 *
 * Log:		supported GPRMC, GPGGA
 *
 * WIP:
 *
 *
 * @param message - NMEA msg to be parsed, example "$GPRMC,192826.000,A,4448.9261,N,09337.4366,W,0.02,123.77,270517,,,D*72"
 * @param result - pointer to result data
 * @return 	0 on success
 * 			1 on format error
 * 			2 on checksum error
 * 			3 unsupported message
 */
uint8_t NMEAParserParseString(const char * message, NMEAData * result);

/**
 * Get distance in kilometers between  a and b
 *
 * @param a
 * @param b
 * @return 	distance on success
 * 			-1 on fail
 */
float NMEAGetDistance(const NMEALocation * a, const NMEALocation * b);

/**
 * Get angle in degree compared to North CCW
 *
 * @param a
 * @param b
 * @return angle
 * 			-1 on fail
 */
float NMEAGetAngle(const NMEALocation * a, const NMEALocation * b);

#endif /* TEST_GPS_BREAKOUT_BOARD_NMEAPARSER_H_ */
