/*
 * I2CConsole.c
 *
 *  Created on: May 29, 2017
 *      Author: dat
 */

#include "I2CConsole.h"

#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "SerialDebug.h"
#include "I2C.h"

static uint8_t _i2c_address;
static uint8_t _i2c_slowTx;

static void consoleInit()
{
	static uint8_t isInited = 0;
	if (!isInited)
	{
		I2CInit();
		isInited = 1;
	}
}

uint8_t I2CConsoleSendCommand(I2CConsoleMessage * command)
{
	consoleInit();

	uint8_t retVal = (command->isValid) ? 0 : 2;

	if (!retVal)
	{
		uint8_t data[I2CMESSAGE_MAXLEN]; // for converting to uint8_t array

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

		if (command->command == SET_SLOW)
		{
			_i2c_slowTx = (command->isDelayBetweenBytes) ? 1 : 0;
		}
		else
		{
			command->isDelayBetweenBytes = _i2c_slowTx;
		}

		if (_i2c_address != 0x00)
		{
			if (command->command == SEND)
			{
				if (command->tx_len)
				{
					retVal += I2CSendData(_i2c_address, data, command->tx_len,
							_i2c_slowTx);
				}

				else if (command->message)
				{
					retVal += I2CSendData(_i2c_address, data,
							strlen(command->message), _i2c_slowTx);
				}
			}
			else if (command->command == SENDNRECV)
			{

				if (command->tx_len)
				{
					retVal += I2CSendnRecvData(_i2c_address, data,
							command->tx_len, command->rx, command->rx_len,
							_i2c_slowTx);
				}

				else if (command->message)
				{
					retVal += I2CSendnRecvData(_i2c_address, data,
							strlen(command->message), command->rx,
							command->rx_len, _i2c_slowTx);
				}
			}
		}
		else
		{
			retVal = 3;
		}
	}

	if (!retVal)
		_delay_ms(150);

	return retVal;
}

uint8_t I2CConsoleGetCurrentAddress()
{
	consoleInit();
	return _i2c_address;
}

uint8_t I2CConsoleGetSlowSendingStatus()
{
	consoleInit();
	return _i2c_slowTx;
}
