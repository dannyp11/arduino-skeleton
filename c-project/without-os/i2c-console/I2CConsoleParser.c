/*
 * I2CConsoleParser.c - handling parsing command
 *
 *  Created on: Jun 10, 2017
 *      Author: dat
 */

#include "I2CConsole.h"
#include <stdio.h>
#include <string.h>

#include "SerialDebug.h"

/**
 * Callback signature for each command
 *
 * @param message - contains parameters for the command, should not contain command name, i.e. "28" "\"hello\""
 * @param result - message result to be updated
 * @param errcode - has to be 0 to be continue parsing
 * @return	- updated errcode
 */
typedef uint8_t (*I2CConsolerParserCallback) (const char * message, I2CConsoleMessage * result, uint8_t errcode);

typedef struct _commandSignature
{
	char cmdName[10];
	I2CConsolerParserCallback callback;
} CommandSignature;

#define _I2C_MAX_PARSER_CALLBACKS	10
static CommandSignature g_ParserCallbackDB[_I2C_MAX_PARSER_CALLBACKS]; // main DB for all parser callbacks

/**
 * internal - process and parse array of hex numbers to result
 *
 * @param message - format "2 ef 34" : 2 numbers: ef and 34
 * @param result
 * @param errcode - must be 0 to continue
 *
 * @return 0 on success
 * 			1 on format error
 * 			3 on insufficient input, eg. "3 a b"
 */
uint8_t processTXArray(const char * message, I2CConsoleMessage * result,
		uint8_t errcode)
{
	if (errcode)
		return errcode;

	uint8_t n = 0;
	char * token;
	token = strtok((char*) message, " ");
	while (token != NULL)
	{
		// break into token strings
		// tx len
		if (n == 0)
		{
			if (sscanf(token, "%u", &result->tx_len) != 1)
			{
				TRACE()
				errcode = 1;
			}
		}
		else if (n <= result->tx_len)
		{
			if (sscanf(token, "%x", &result->tx[n - 1]) != 1)
			{
				TRACE()
				errcode = 1;
			}
		}

		if (errcode)
			break;
		++n;
		token = strtok(NULL, " ");
	}

	if (result->tx_len >= n)
	{
		errcode = 3;
		result->tx_len = 0;
	}

	return errcode;
}

/**
 * internal - process string and put it to result
 * @param message - eg. "\"hello world\""
 * @param result
 * @param errcode
 * @return - 0 on success
 * 			1 on fail format
 */
uint8_t processTXString(const char * message, I2CConsoleMessage * result,
		uint8_t errcode)
{
	if (errcode)
		return errcode;
	errcode = (message[0] == '"') ? 0 : 1;

	// check for another '"'
	char * lastQuote = strchr((char*) message + 1, '"');
	if (lastQuote)
	{
		strncpy(result->message, message + 1, lastQuote - message - 1);
		result->message[lastQuote - message - 1] = '\0';
	}
	else
	{
		errcode = 1;
	}

	return errcode;
}

/**
 * internal - process "2 a b" or "\" hello world\"" TX messages
 * @param message
 * @param result
 * @param errcode
 * @return
 */
uint8_t processTXPart(const char * message, I2CConsoleMessage * result,
		uint8_t errcode)
{
	if (errcode)
		return errcode;

	// process string
	if (message[0] == '"')
	{
		errcode = processTXString(message, result, errcode);
	}

	// else process array
	else
	{
		errcode = processTXArray(message, result, errcode);
	}

	return errcode;
}

uint8_t processAddr(const char * message, I2CConsoleMessage * result,
		uint8_t errcode)
{
	if (errcode)
		return errcode;

	// set address
	result->command = SET_ADDRESS;
	if (sscanf(message, "%x", &result->address) != 1)
	{
		TRACE()
		errcode = 1;
	}

	return errcode;
}

uint8_t processSlow(const char * message, I2CConsoleMessage * result,
		uint8_t errcode)
{
	if (errcode)
		return errcode;

	// set address
	result->command = SET_SLOW;
	if (sscanf(message, "%u", &result->isDelayBetweenBytes) != 1)
	{
		TRACE()
		errcode = 1;
	}

	return errcode;
}

uint8_t processTX(const char * message, I2CConsoleMessage * result,
		uint8_t errcode)
{
	if (errcode)
		return errcode;

	// send
	result->command = SEND;

	errcode = processTXPart(message, result, errcode);

	return errcode;
}

uint8_t processRX(const char * message, I2CConsoleMessage * result,
		uint8_t errcode)
{
	if (errcode)
		return errcode;

	// sendnrecv
	result->command = SENDNRECV;

	uint8_t n = 0;
	char * token;
	char message2[I2CMESSAGE_MAXLEN];
	strcpy(message2, message);
	token = strtok(message2, " ");
	while (token != NULL)
	{
		// break into token strings
		// rx len
		if (n == 0)
		{
			if (sscanf(token, "%u", &result->rx_len) != 1)
			{
				TRACE()
				errcode = 1;
			}
			else
			{
				// parse tx part
				errcode = processTXPart(
						(char*) message + strlen(token) + 1,
						result, errcode);

				break;
			}
		}

		if (errcode)
			break;
		++n;
		token = strtok(NULL, " ");
	}

	return errcode;
}

/**
 * internal - final touch of result
 *
 * @param result - message to process
 * @return	- 0 on success
 */
uint8_t finalizeMessage(I2CConsoleMessage * result)
{
	uint8_t retVal = 0;
	(void) result;
	return retVal;
}

/**
 * Internal - doesn't check for error if DB is full, be careful
 *
 * @param signature - max 10 chars signature
 * @param callback
 */
void registerParserCallback(const char * signature, I2CConsolerParserCallback callback)
{
	uint8_t i;
	for (i = 0; i < _I2C_MAX_PARSER_CALLBACKS; ++i)
	{
		if (g_ParserCallbackDB[i].callback == 0)
		{
			// only empty db entry is eligible
			strcpy(g_ParserCallbackDB[i].cmdName, signature);
			g_ParserCallbackDB[i].callback = callback;
			return;
		}
	}
}

/**
 * Init and register all parser callbacks
 */
void parserInit()
{
	// set all cb entries in db to 0 (invalid cb)
	uint8_t i;
	for (i = 0; i < _I2C_MAX_PARSER_CALLBACKS; ++i)
	{
		g_ParserCallbackDB[i].callback = 0;
		g_ParserCallbackDB[i].cmdName[0] = '\0';
	}

	// put your favorite parser here
	registerParserCallback("addr", processAddr);
	registerParserCallback("slow", processSlow);
	registerParserCallback("tx", processTX);
	registerParserCallback("rx", processRX);
}

/**
 * log - done command: tx rx addr slow
 *
 * wip -
 *
 * @param message
 * @param result
 * @return
 */
uint8_t I2CConsoleParser(const char * message, I2CConsoleMessage * result)
{
	static char isInited = 0;
	if (!isInited)
	{
		// register module callbacks
		parserInit();
		isInited = 1;
	}

	uint8_t retVal = 0;
	result->isValid = 0;
	result->rx_len = 0;
	result->tx_len = 0;
	result->isDelayBetweenBytes = 1;
	result->message[0] = '\0';

	char command[10];

	if (sscanf(message, "%s ", command) != 1)
	{
		TRACE()
		retVal = 1;
	}

	// scan db and run respective parser callback
	uint8_t i;
	uint8_t isValidCommand = 0;
	for (i = 0; i< _I2C_MAX_PARSER_CALLBACKS; ++i)
	{
		if (g_ParserCallbackDB[i].callback && strcasecmp(command, g_ParserCallbackDB[i].cmdName) == 0)
		{
			retVal = g_ParserCallbackDB[i].callback(message + strlen(command) + 1, result, retVal);
			isValidCommand = 1;
			break;
		}
	}

	// if no parser cb is found
	if (!isValidCommand)
	{
		TRACE();
		retVal = 2;
	}

	// reserved for future
	if (!retVal)
	{
		retVal = finalizeMessage(result);
	}

	if (!retVal)
	{
		result->isValid = 1;
	}
	return retVal;
}

void I2CConsoleDumpCommand(const I2CConsoleMessage * command)
{
#ifdef DEBUG1
	LOG("========================================================");
	LOG("address %x", command->address);
	TRACE_INT(command->isDelayBetweenBytes);
	TRACE_INT(command->isValid);
	TRACE_INT(command->rx_len);
	TRACE_INT(command->tx_len);

	if (command->tx_len)
	{
		int i;
		for (i = 0; i < command->tx_len; ++i)
		{
			LOG("tx[%d] = %x", i, command->tx[i]);
		}
	}

	if (command->message)
	{
		TRACE_STRING(command->message);
	}

	if (command->rx_len)
	{
		int i;
		for (i = 0; i < command->rx_len; ++i)
		{
			LOG("rx[%d] = %x", i, command->rx[i]);
		}
	}
	LOG("========================================================");
#else
	(void) command;
#endif
}
