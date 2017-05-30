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
 * log - done command:
 *
 * wip - addr command
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

	char command[10];
	if (sscanf(message, "%s ", command) != 1)
	{
		TRACE()
		retVal = 1;
	}

	if (strcasecmp(command, "ADDR") == 0)
	{
		// set address
		result->command = SET_ADDRESS;
		if (sscanf(message + 5, "%x", &result->address) != 1)
		{
			TRACE()
			retVal = 1;
		}
	}
	else if (strcasecmp(command, "TX") == 0)
	{
		// send
		result->command = SEND;

		uint8_t n = 0;
		char * token;
		token = strtok((char*) message, " ");
		while (token != NULL)
		{
			// break into token strings
			if (n != 0) // ignore 1st token
			{
				// tx len
				if (n == 1)
				{
					if (sscanf(token, "%u", &result->tx_len) != 1)
					{
						TRACE()
						retVal = 1;
					}
				}
				else
				{
					if (sscanf(token, "%x", &result->tx[n - 2]) != 1)
					{
						TRACE()
						retVal = 1;
					}
				}
			}

			if (retVal)
				break;
			++n;
			token = strtok(NULL, " ");
		}
	}
	else if (strcasecmp(command, "RX") == 0)
	{
		// sendnrecv
		result->command = SENDNRECV;

		uint8_t n = 0;
		char * token;
		token = strtok((char*) message, " ");
		while (token != NULL)
		{
			// break into token strings
			if (n != 0) // ignore 1st token
			{
				// tx len
				if (n == 1)
				{
					if (sscanf(token, "%u", &result->rx_len) != 1)
					{
						TRACE()
						retVal = 1;
					}
				}
				else if (n == 2)
				{
					if (sscanf(token, "%u", &result->tx_len) != 1)
					{
						TRACE()
						retVal = 1;
					}
				}
				else
				{
					if (sscanf(token, "%x", &result->tx[n - 3]) != 1)
					{
						TRACE()
						retVal = 1;
					}
				}
			}

			if (retVal)
				break;
			++n;
			token = strtok(NULL, " ");
		}
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
	TRACE_INT(command->address);
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

uint8_t I2CConsoleSendCommand(I2CConsoleMessage * command)
{
	static uint8_t isInited = 0;
	if (!isInited)
	{
		I2CInit();
		_i2c_address = 0x00;
		isInited = 1;
	}

	uint8_t retVal = (command->isValid) ? 0 : 1;

	if (!retVal)
	{
		if (command->command == SET_ADDRESS)
		{
			_i2c_address = (uint8_t) command->address;
		}
		else
		{
			command->address = (unsigned) _i2c_address;
		}

		if (command->command == SEND)
		{
			retVal += I2CSendData(_i2c_address, (uint8_t*) command->tx,
					(uint8_t) command->tx_len, 1);
		}
		else if (command->command == SEND)
		{
			retVal += I2CSendnRecvData(_i2c_address, (uint8_t*) command->tx,
					(uint8_t) command->tx_len, (uint8_t*) command->rx,
					(uint8_t) command->rx_len, 1);
		}
	}

	return retVal;
}

#endif
