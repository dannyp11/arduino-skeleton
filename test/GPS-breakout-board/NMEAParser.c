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
				printf("hi\n");
				retVal = 1;
			}
		}
			break;

		case TIMESTAMP:
		{

		}
			break;

		default:
			break;
		}

		if (retVal) break;
	}

	result->isValid = 1;
	return retVal;
}
