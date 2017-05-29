/*
 * NMEAParser.c
 *
 *  Created on: May 27, 2017
 *      Author: dat
 */

#include "NMEAParser.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "SerialDebug.h"

enum TOKEN_RMC
{
	RMC_FUNCTION = 0, RMC_TIMESTAMP, RMC_VALID, RMC_LAT, RMC_LAT_D, // direction NS
	RMC_LON,
	RMC_LON_D, // direction EW
	RMC_SPEED,
	RMC_TRUE,
	RMC_DATE,
	RMC_MAG_VAR,
	RMC_CKSUM
};

enum TOKEN_GGA
{
	GGA_FUNCTION = 0,
	GGA_TIMESTAMP,
	GGA_LAT,
	GGA_LAT_D,
	GGA_LON,
	GGA_LON_D,
	GGA_VALID,
	GGA_NSATELITE,
	GGA_CKSUM
};

/**
 * checksum NMEA message
 * @param message
 * @return 0 on success, 1 on fail
 */
uint8_t checkSum(const char * message)
{
	uint8_t retVal = 1;

	if (message[0] != '$')
		return retVal;
	const char* token = strchr(message, '*');
	if (token == NULL)
	{
		return retVal;
	}

	char target[128];
	strncpy(target, message + 1, token - message - 1);
	target[token - message - 1] = '\0';

	int expectedCkSum;
	sscanf(token, "*%x", &expectedCkSum);

	int i;
	int actualCksum = 0;
	for (i = 0; i < strlen(target); ++i)
	{
		actualCksum ^= target[i];
	}

	if (actualCksum == expectedCkSum)
		retVal = 0;

	return retVal;
}

/**
 * internal function - parse GPGGA message
 *
 * @param message - body of message, exclude $GPGGA,
 * @param result
 * @param errcode
 * @return
 */
uint8_t parseGGA(const char * message, NMEAData * result, uint8_t errcode)
{
	result->isValid = 0;

	// break into tokens
	uint8_t cursor = 0;
	char token_str[20];
	int token_n;
	for (token_n = GGA_TIMESTAMP; token_n <= GGA_CKSUM; ++token_n)
	{
		const char * token = strchr(message + cursor, ',');
		if (token == NULL)
		{
			errcode = 1;
			break;
		}

		strncpy(token_str, message + cursor, token - message - cursor);
		token_str[token - message - cursor] = '\0';
		cursor = token - message + 1;

		// process token_str
		switch (token_n)
		{
		case GGA_TIMESTAMP:
		{
			sscanf(token_str, "%d", &result->timestamp);
		}
			break;

		case GGA_VALID:
		{
			if (strcmp(token_str, "1") != 0 && strcmp(token_str, "2") != 0)
			{
				errcode = 1;
			}
		}
			break;

		case GGA_LAT:
		{
			double lat;
			if (sscanf(token_str, "%lf", &lat) != 1)
			{
				errcode = 1;
			}
			else
			{
				result->location.lat_deg = (int) (lat / 100.0f + 0.5f);
				result->location.lat_min = (float) (lat
						- 100.0 * result->location.lat_deg);
			}
		}
			break;

		case GGA_LAT_D:
		{
			if (token_str[0] == 'N' || token_str[0] == 'S')
			{
				if (token_str[0] == 'S')
				{
					result->location.lat_deg *= -1;
				}
			}
			else
			{
				errcode = 1;
			}
		}
			break;

		case GGA_LON:
		{
			double lon;
			if (sscanf(token_str, "%lf", &lon) != 1)
			{
				errcode = 1;
			}
			else
			{
				result->location.lon_deg = (int) (lon / 100.0f + 0.5f);
				result->location.lon_min = (float) (lon
						- 100.0 * result->location.lon_deg);
			}
		}
			break;

		case GGA_LON_D:
		{
			if (token_str[0] == 'E' || token_str[0] == 'W')
			{
				if (token_str[0] == 'W')
				{
					result->location.lon_deg *= -1;
				}
			}
			else
			{
				errcode = 1;
			}
		}
			break;

		default:
			break;
		}

	}
	return errcode;
}

/**
 * internal function - parse GPRMC message
 *
 * @param message - body of message, exclude $GPRMC,
 * @param result
 * @param errcode
 * @return
 */
uint8_t parseRMC(const char * message, NMEAData * result, uint8_t errcode)
{
	result->isValid = 0;

	// break into tokens
	uint8_t cursor = 0;
	char token_str[20];
	int token_n;
	for (token_n = RMC_TIMESTAMP; token_n <= RMC_CKSUM; ++token_n)
	{
		const char * token = strchr(message + cursor, ',');
		if (token == NULL)
		{
			errcode = 1;
			break;
		}

		strncpy(token_str, message + cursor, token - message - cursor);
		token_str[token - message - cursor] = '\0';
		cursor = token - message + 1;

		// process token_str
		switch (token_n)
		{
		case RMC_TIMESTAMP:
		{
			sscanf(token_str, "%d", &result->timestamp);
		}
			break;

		case RMC_VALID:
		{
			if (strcmp(token_str, "A") != 0)
			{
				errcode = 1;
			}
		}
			break;

		case RMC_LAT:
		{
			double lat;
			if (sscanf(token_str, "%lf", &lat) != 1)
			{
				errcode = 1;
			}
			else
			{
				result->location.lat_deg = (int) (lat / 100.0f + 0.5f);
				result->location.lat_min = (float) (lat
						- 100.0 * result->location.lat_deg);
			}
		}
			break;

		case RMC_LAT_D:
		{
			if (token_str[0] == 'N' || token_str[0] == 'S')
			{
				if (token_str[0] == 'S')
				{
					result->location.lat_deg *= -1;
				}
			}
			else
			{
				errcode = 1;
			}
		}
			break;

		case RMC_LON:
		{
			double lon;
			if (sscanf(token_str, "%lf", &lon) != 1)
			{
				errcode = 1;
			}
			else
			{
				result->location.lon_deg = (int) (lon / 100.0f + 0.5f);
				result->location.lon_min = (float) (lon
						- 100.0 * result->location.lon_deg);
			}
		}
			break;

		case RMC_LON_D:
		{
			if (token_str[0] == 'E' || token_str[0] == 'W')
			{
				if (token_str[0] == 'W')
				{
					result->location.lon_deg *= -1;
				}
			}
			else
			{
				errcode = 1;
			}
		}
			break;

		case RMC_SPEED:
		{
			if (sscanf(token_str, "%f", &result->speed) != 1)
				errcode = 1;
		}
			break;

		case RMC_DATE:
		{
			if (sscanf(token_str, "%u", &result->date) != 1)
				errcode = 1;
		}
			break;

		default:
			break;
		}

		if (errcode)
			break;
	}

	return errcode;
}

/**
 * Supported - RMC, GGA
 *
 * WIP -
 *
 * @param message
 * @param result
 * @return
 */
uint8_t NMEAParserParseString(const char * message, NMEAData * result)
{
	result->isValid = 0;
	uint8_t retVal = 0;
	char token_str[20];

	if (message[0] != '$')
	{
		return 1;
	}

	sscanf(message, "$%5s", token_str);

	if (strcmp(token_str, "GPRMC") == 0)
	{
		retVal = parseRMC(message + 7, result, retVal);
	}
	else if (strcmp(token_str, "GPGGA") == 0)
	{
		retVal = parseGGA(message + 7, result, retVal);
	}
	else
	{
		retVal = 3;
	}

	// cksum
	if (checkSum(message) != 0)
	{
		retVal = 2;
	}

	if (retVal == 0)
	{
		result->isValid = 1;
	}

	return retVal;
}

inline float toRadian(int degree, float min)
{
	return (((float) degree + min / 60.0) * M_PI) / 180.0;
}

/**
 *
 * @param a
 * @param b
 * @return
 */
float NMEAGetDistance(const NMEALocation * a, const NMEALocation * b)
{
	float retVal = -1;

	float lata = toRadian(a->lat_deg, a->lat_min);
	float lona = toRadian(a->lon_deg, a->lon_min);
	float latb = toRadian(b->lat_deg, b->lat_min);
	float lonb = toRadian(b->lon_deg, b->lon_min);

	float R = 6378.0f;
	float dlat = fabs(lata - latb);
	float dlon = fabs(lona - lonb);

	float fa = sin(dlat/2) * sin(dlat/2) + cos(lata) * cos(latb) * sin (dlon/2) * sin(dlon/2);
	float fc = 2 * atan2(sqrt(fa), sqrt(1-fa));
	retVal = fc * R;

	return retVal;
}
