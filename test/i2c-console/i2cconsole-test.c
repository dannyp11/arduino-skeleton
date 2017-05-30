/*
 * i2cconsole-test.c
 *
 *  Created on: May 29, 2017
 *      Author: dat
 */
#include "I2CConsole.h"
#include "SerialDebug.h"
#include "PgmStorage.h"

#include <stdio.h>
#include <util/delay.h>

const char PROGMEM help1[] = "Sample command - explaination";
const char PROGMEM help2[] = "ADDR 28        - set i2c 7-bit address as 0x28";
const char PROGMEM help3[] = "TX 2 00 03     - send 2 bytes 0x00 and 0x03";
const char PROGMEM help4[] =
		"RX 6 2 ab 03   - send 2 bytes 0xab and 0x03, receive 6 bytes back to rx";
const char PROGMEM help5[] =
		"========================================================================";

void showHelp()
{
	char msg[128];
	PgmStorageGet(msg, help1);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help2);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help3);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help4);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help5);
	SerialDebugPrint(msg);
}

void processMessage(const char * message)
{
	LOG("Processing %s", message);
	I2CConsoleMessage cmd;
	uint8_t parseResult = I2CConsoleParser(message, &cmd);
	if (parseResult == 0)
	{
		uint8_t i;
		TRACE_INT(cmd.tx_len);

		if (cmd.tx_len)
		{
			for (i = 0; i < cmd.tx_len; ++i)
			{
				LOG("tx[%d] = %x", i, cmd.tx[i]);
			}
		}

		uint8_t sendResult = I2CConsoleSendCommand(&cmd);
		if (sendResult == 0)
		{
			SerialDebugPrint("sent %s", message);
		}
		else
		{
			SerialDebugPrint("error code %d sending %s", sendResult, message);
		}
	}
	else
	{
		SerialDebugPrint("error code %d parsing %s", parseResult, message);
	}

	_delay_ms(150);
}

void testLCD()
{
	const char input1[] = "addr 28";
	const char input2[] = "tx 2 fe 41";
	const char input3[] = "tx 2 fe 4b";

	processMessage(input1);
	processMessage(input2);
	processMessage(input3);
}

int main()
{
	SerialDebugInit();

	while (1)
	{
		char buffer[50];

		SerialDebugPrint(" ");
		SerialDebugPrint("Enter i2c command or type help");
		SerialDebugGetLine(buffer, 1);

		if (strcasecmp(buffer, "help") == 0)
		{
			showHelp();
		}
		else if (strcasecmp(buffer, "test") == 0)
		{
			// test with lcd
			testLCD();
		}
		else
		{
			// process n send buffer
			processMessage(buffer);
		}

		_delay_ms(1);
	}

	return 0;
}

