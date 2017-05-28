/*
 * NMEAParser.c
 *
 *  Created on: May 27, 2017
 *      Author: dat
 */

#include "NMEAParser.h"
#include <string.h>
#include <stdio.h>

enum TOKEN_RMC
{
	FUNCTION = 0, TIMESTAMP, VALID, LAT, LAT_D, // direction NS
	LON,
	LON_D, // direction EW
	SPEED,
	TRUE,
	DATE,
	MAG_VAR,
	CKSUM
};

/**
 * checksum NMEA message
 * @param message
 * @return 0 on success, 1 on fail
 */
uint8_t checkSum(const char * message)
{
	uint8_t retVal = 1;

	if (message[0] != '$') return retVal;
	const char* token = strchr(message, '*');

	char target[128];
	strncpy(target, message + 1, token - message - 1);

	int expectedCkSum;
	sscanf(token, "*%x", &expectedCkSum);

	int i;
	int actualCksum = 0;
	for ( i = 0; i < strlen(target); ++i)
	{
		actualCksum ^= target[i];
	}

	if (actualCksum == expectedCkSum)
		retVal = 0;

	return retVal;
}

/**
 * WIP - RMC msg
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
		return 1;

	uint8_t cursor = 0;
	int token_n;
	for (token_n = 0; token_n <= CKSUM; ++token_n)
	{
		const char * token = strchr(message + cursor, ',');
		if (token == NULL)
		{
			retVal = 1;
			break;
		}

		strncpy(token_str, message + cursor, token - message - cursor);
		token_str[token - message - cursor] = '\0';
		cursor = token - message + 1;

		// process token_str
		switch (token_n)
		{
		case FUNCTION:
		{
			if (strcmp(token_str, "$GPRMC") != 0)
			{
				retVal = 1;
			}
		}
			break;

		case TIMESTAMP:
		{
			sscanf(token_str, "%d", &result->timestamp);
		}
			break;

		case VALID:
		{
			if (strcmp(token_str, "A") != 0)
			{
				retVal = 1;
			}
		}
			break;

		case LAT:
		{
			double lat;
			if (sscanf(token_str, "%lf", &lat) != 1)
			{
				retVal = 1;
			}
			else
			{
				result->location.lat_deg = (int) (lat / 100.0f + 0.5f);
				result->location.lat_min = (float) (lat - 100.0 * result->location.lat_deg);
			}
		}
			break;

		case LAT_D:
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
				retVal = 1;
			}
		}
			break;

		case LON:
		{
			double lon;
			if (sscanf(token_str, "%lf", &lon) != 1)
			{
				retVal = 1;
			}
			else
			{
				result->location.lon_deg = (int) (lon / 100.0f + 0.5f);
				result->location.lon_min = (float) (lon - 100.0 * result->location.lon_deg);
			}
		}
			break;

		case LON_D:
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
				retVal = 1;
			}
		}
			break;

		case SPEED:
		{
			if (sscanf(token_str, "%f", &result->speed) != 1)
				retVal = 1;
		}
		break;

		case DATE:
		{
			if (sscanf(token_str, "%u", &result->date) != 1)
							retVal = 1;
		}
		break;

		default:
			break;
		}

		if (retVal)
			break;
	}

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
