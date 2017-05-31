/*
 * I2CConsole.c
 *
 *  Created on: May 29, 2017
 *      Author: dat
 */

#include "I2CConsole.h"
#include <stdio.h>
#include <string.h>

#include "SerialDebug.h"

static uint8_t _i2c_address;

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
	if (sscanf(message + 5, "%x", &result->address) != 1)
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

	errcode = processTXPart((char*) message + strlen("TX "), result, errcode);

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
	char message2[128];
	strcpy(message2, message);
	token = strtok((char*) message, " ");
	while (token != NULL)
	{
		// break into token strings
		if (n != 0) // ignore 1st token
		{
			// rx len
			if (n == 1)
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
							(char*) message2 + strlen("RX ") + strlen(token)
									+ 1, result, errcode);

					break;
				}
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
 * log - done command:
 *
 * wip - tx string
 *
 * @param message
 * @param result
 * @return
 */
uint8_t I2CConsoleParser(const char * message, I2CConsoleMessage * result)
{
	uint8_t retVal = 0;
	result->isValid = 0;
	result->rx_len = 0;
	result->tx_len = 0;
	result->message[0] = '\0';

	char command[10];
	if (sscanf(message, "%s ", command) != 1)
	{
		TRACE()
		retVal = 1;
	}

	if (strcasecmp(command, "ADDR") == 0)
	{
		// set address
		retVal = processAddr(message, result, retVal);
	}
	else if (strcasecmp(command, "TX") == 0)
	{
		retVal = processTX(message, result, retVal);
	}
	else if (strcasecmp(command, "RX") == 0)
	{
		retVal = processRX(message, result, retVal);
	}
	else
	{
		TRACE()
		retVal = 2;
	}

	if (!retVal)
	{
		result->isValid = 1;
	}
	return retVal;
}

void I2CConsoleDumpCommand(const I2CConsoleMessage * command)
{
	LOG("========================================================");
	LOG("address %x", command->address);
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
}

#ifndef CXXTEST
#include "I2C.h"
#include <util/delay.h>

uint8_t I2CConsoleSendCommand(I2CConsoleMessage * command)
{
	static uint8_t isInited = 0;
	if (!isInited)
	{
		I2CInit();
		_i2c_address = 0x00;
		isInited = 1;
	}

	uint8_t retVal = (command->isValid) ? 0 : 2;

	if (!retVal)
	{
		uint8_t data[256]; // for converting to uint8_t array

		if (command->command == SET_ADDRESS)
		{
			_i2c_address = (uint8_t) command->address;
		}
		else
		{
			command->address = _i2c_address;
			int i;

			if (command->tx_len)
			{
				for (i = 0; i < command->tx_len; ++i)
				{
					data[i] = command->tx[i];
				}
			}
			else if (command->message)
			{
				for (i = 0; i < (int) strlen(command->message); ++i)
				{
					data[i] = command->message[i];
				}
			}
		}

		if (_i2c_address != 0x00)
		{
			if (command->command == SEND)
			{
				if (command->tx_len)
				{
					retVal += I2CSendData(_i2c_address, data, command->tx_len,
							1);
				}

				else if (command->message)
				{
					retVal += I2CSendData(_i2c_address, data,
							strlen(command->message), 1);
				}
			}
			else if (command->command == SENDNRECV)
			{

				if (command->tx_len)
				{
					retVal += I2CSendnRecvData(_i2c_address, data,
							command->tx_len, command->rx, command->rx_len, 1);
				}

				else if (command->message)
				{
					retVal += I2CSendnRecvData(_i2c_address, data,
							strlen(command->message), command->rx,
							command->rx_len, 1);
				}
			}
		}
		else
		{
			retVal = 3;
		}
	}

	if (!retVal) _delay_ms(150);

	return retVal;
}

uint8_t I2CConsoleGetCurrentAddress()
{
	static uint8_t isInited = 0;
	if (!isInited)
	{
		I2CInit();
		_i2c_address = 0x00;
		isInited = 1;
	}

	return _i2c_address;
}

#endif
