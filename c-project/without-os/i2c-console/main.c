/*
 * i2cconsole-test.c
 *
 *  Created on: May 29, 2017
 *      Author: dat
 */
#include "I2CConsole.h"
#include "SerialDebug.h"
#include "PgmStorage.h"
#include "Timer.h"

#include <stdio.h>
#include <util/delay.h>

const char PROGMEM helpaddr1[] = "Hint address: LCD 0x28, compass 0x19";
const char PROGMEM help1[] =
		"Sample command     - explanation (command case insensitive)";
const char PROGMEM help2[] =
		"ADDR 28            - set i2c 7-bit address as 0x28";
const char PROGMEM help3[] = "TX 2 00 03         - send 2 bytes 0x00 and 0x03";
const char PROGMEM help0[] = "TX \"hello world\" - send string \"hello world\"";
const char PROGMEM help4[] =
		"RX 6 2 ab 03       - send 2 bytes 0xab and 0x03, receive 6 bytes back to rx";
const char PROGMEM help41[] = "SLOW 0			 - set slow sending off";
const char PROGMEM helploop1[] =
		"LOOP 2 TX 2 00 03     - loop the command 'TX 2 00 03' for 2 seconds";
const char PROGMEM helploop2[] =
		"LOOP 5 RX 6 1 ab     - loop the command 'RX 6 1 ab' for 5 seconds";
const char PROGMEM help5[] = "---------------";
const char PROGMEM help6[] = "testlcd            - test the lcd 4x20";
const char PROGMEM help7[] = "testcompass        - test the GY-85 module";
const char PROGMEM help8[] =
		"========================================================================";

static volatile unsigned long mMillis;

void showHelp()
{
	char msg[PGM_SIZE];

	PgmStorageGet(msg, helpaddr1);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help1);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help2);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help3);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help0);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help4);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help41);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, helploop1);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, helploop2);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help5);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help6);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help7);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help8);
	SerialDebugPrint(msg);
}

void processMessage(const char * message)
{
	LOG("\n\nProcessing %s", message);
	I2CConsoleMessage cmd;

	char message2[I2CMESSAGE_MAXLEN];
	strcpy(message2, message);
	uint8_t parseResult = I2CConsoleParser(message2, &cmd);

	if (parseResult == 0)
	{
		uint8_t i;
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

			if (cmd.command == SENDNRECV)
			{
				SerialDebugPrint("RX result:");
				for (i = 0; i < cmd.rx_len; ++i)
				{
					SerialDebugPrint(" rx[%d] = 0x%x = %d = '%c'", i, cmd.rx[i],
							cmd.rx[i], cmd.rx[i]);
				}
			}
		}
		else
		{
			SerialDebugPrint("error code %d sending %s", sendResult, message);
		}

		I2CConsoleDumpCommand(&cmd);
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
	const char input0[] = "slow 1";
	const char input2[] = "tx 2 fe a3";
	const char input3[] = "tx 2 fe 4b";
	const char input4[] = "tx 2 fe 51";
	const char input5[] = "tx \"hello world\"";

	processMessage(input1);
	processMessage(input0);
	processMessage(input2);
	processMessage(input3);
	processMessage(input4);
	processMessage(input5);
}

void testCompass()
{
	const char input1[] = "addr 1e";
	const char input0[] = "slow 0";
	const char input2[] = "tx 2 2 0";
	const char input3[] = "tx 2 0 90";
	const char input4[] = "rx 6 1 3";

	processMessage(input1);
	processMessage(input0);
	processMessage(input2);
	processMessage(input3);
	processMessage(input4);
}

void loopCommand(const char * message)
{
	unsigned loop_time = 0;
	uint8_t n = 0;
	char loopMesssage[20];
	strncpy(loopMesssage, message, 20);

	char * token = strtok(loopMesssage, " ");
	while (token != NULL)
	{
		if (n == 1)
		{
			if (sscanf(token, "%u ", &loop_time) != 1)
			{
				SerialDebugPrint("error parsing loop in '%s'", message);
			}
			else
			{
				// found loop time
				SerialDebugPrint("looping %d seconds for '%s'", loop_time,
						message + strlen("loop ") + strlen(token) + 1);

				TRACE()
				;
				uint8_t initSec = mMillis / 1000;
				while ((uint8_t) (mMillis / 1000) - initSec
						<= (uint8_t) loop_time)
				{
					processMessage(
							message + strlen("loop ") + strlen(token) + 1);
				}
			}

			break;
		}

		token = strtok(NULL, " ");
		++n;
	}
}

static void incMillis()
{
	++mMillis;
}

int main()
{
	SerialDebugInit();
	Timer1Init(1);
	mMillis = 0;
	Timer1SetCallback(incMillis);

	while (1)
	{
		char buffer[I2CMESSAGE_MAXLEN];
		char firstWord[20];

		SerialDebugPrint(" ");
		SerialDebugPrint(
				"Enter i2c command or type help (current address 0x%x, slow status %u)",
				I2CConsoleGetCurrentAddress(), I2CConsoleGetSlowSendingStatus());
		SerialDebugGetLine(buffer, 1);
		uint8_t i = 0;
		while (buffer[i] != '\0' && buffer[i] != ' ')
		{
			firstWord[i] = buffer[i];
			++i;
		}
		firstWord[i] = '\0';

		if (strcasecmp(buffer, "help") == 0)
		{
			showHelp();
		}
		else if (strcasecmp(buffer, "testlcd") == 0)
		{
			// test with lcd
			testLCD();
		}
		else if (strcasecmp(buffer, "testcompass") == 0)
		{
			// test with compass
			testCompass();
		}
		else if (strcasecmp(firstWord, "loop") == 0)
		{
			// loop tx or rx command
			loopCommand(buffer);
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

