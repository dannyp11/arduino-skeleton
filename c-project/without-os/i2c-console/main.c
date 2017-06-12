/*
 * i2cconsole-test.c
 *
 *  Created on: May 29, 2017
 *      Author: dat
 */
#include "I2CConsole.h"
#include "SerialDebug.h"
#include "PgmStorage.h"
#include "Millis.h"

#include <stdio.h>
#include <util/delay.h>

const char PROGMEM help1[] = "Sample command \t\t - explanation (command case insensitive)";
const char PROGMEM help2[] = "ADDR 28 \t\t - set i2c 7-bit address as 0x28";
const char PROGMEM help3[] = "TX 2 00 03 \t\t - send 2 bytes 0x00 and 0x03";
const char PROGMEM help0[] = "TX \"hello world\" \t - send string \"hello world\"";
const char PROGMEM help4[] = "RX 6 2 ab 03 \t\t - send 2 bytes 0xab and 0x03, receive 6 bytes back to rx";
const char PROGMEM help41[] = "SLOW 0 \t\t\t - set slow sending off";
const char PROGMEM help42[] = "PING 23 \t\t - check if address 0x23 is alive";
const char PROGMEM helploop1[] = "LOOP 2 TX 2 00 03 \t - loop the command 'TX 2 00 03' for 2 seconds";
const char PROGMEM helploop2[] = "LOOP 5 RX 6 1 ab \t - loop the command 'RX 6 1 ab' for 5 seconds";
const char PROGMEM help5[] = "---------------";
const char PROGMEM help6[] = "testlcd \t\t - test the lcd 4x20";
const char PROGMEM help7[] = "testgy85compass \t - test the GY-85 compass";
const char PROGMEM help71[] = "testgy85accel \t\t - test the GY-85 accelerator";
const char PROGMEM help72[] = "HISTORY \t\t - show past entered messages";
const char PROGMEM help73[] = "SCAN \t\t\t - scan all I2C devices on the bus";
const char PROGMEM help8[] = "========================================================================";

void processMessage(const char * buffer);

void showHelp()
{
	char msg[PGM_SIZE];

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

	PgmStorageGet(msg, help42);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, helploop1);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, helploop2);
	SerialDebugPrint(msg);

	// history help
#ifndef DEBUG
	PgmStorageGet(msg, help5);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help6);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help7);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help71);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help72);
	SerialDebugPrint(msg);

	PgmStorageGet(msg, help73);
	SerialDebugPrint(msg);
#endif

	PgmStorageGet(msg, help8);
	SerialDebugPrint(msg);
}

const char PROGMEM testLCD1[] = "addr 28";
const char PROGMEM testLCD2[] = "slow 1";
const char PROGMEM testLCD3[] = "tx 2 fe a3";
const char PROGMEM testLCD4[] = "tx 2 fe 4b";
const char PROGMEM testLCD5[] = "tx 2 fe 51";
const char PROGMEM testLCD6[] = "tx \"hello world\"";

void testLCD()
{
	char msg[PGM_SIZE];

	PgmStorageGet(msg, testLCD1);
	processMessage(msg);

	PgmStorageGet(msg, testLCD2);
	processMessage(msg);

	PgmStorageGet(msg, testLCD3);
	processMessage(msg);

	PgmStorageGet(msg, testLCD4);
	processMessage(msg);

	PgmStorageGet(msg, testLCD5);
	processMessage(msg);

	PgmStorageGet(msg, testLCD6);
	processMessage(msg);
}

const char PROGMEM testcompass1[] = "addr 1e";
const char PROGMEM testcompass2[] = "slow 0";
const char PROGMEM testcompass3[] = "tx 2 2 0";
const char PROGMEM testcompass4[] = "tx 2 0 90";
const char PROGMEM testcompass5[] = "rx 6 1 3";

void testGY85Compass()
{
	char msg[PGM_SIZE];

	PgmStorageGet(msg, testcompass1);
	processMessage(msg);

	PgmStorageGet(msg, testcompass2);
	processMessage(msg);

	PgmStorageGet(msg, testcompass3);
	processMessage(msg);

	PgmStorageGet(msg, testcompass4);
	processMessage(msg);

	PgmStorageGet(msg, testcompass5);
	processMessage(msg);
}

const char PROGMEM testaccel1[] = "addr 53";
const char PROGMEM testaccel2[] = "slow 0";
const char PROGMEM testaccel3[] = "tx 2 31 1"; // data format
const char PROGMEM testaccel4[] = "tx 2 2d 8 "; // power ctl
const char PROGMEM testaccel5[] = "loop 5 rx 6 1 32"; // read 6 bytes starting from address 0x32

void testGY85Accel()
{
	char msg[PGM_SIZE];

	PgmStorageGet(msg, testaccel1);
	processMessage(msg);

	PgmStorageGet(msg, testaccel2);
	processMessage(msg);

	PgmStorageGet(msg, testaccel3);
	processMessage(msg);

	PgmStorageGet(msg, testaccel4);
	processMessage(msg);

	PgmStorageGet(msg, testaccel5);
	processMessage(msg);
}

void showHistory()
{
#ifndef DEBUG
	uint8_t i;
	SerialDebugPrint("------------------------------");
	for (i = 0; i < I2CConsoleStackGetCount(); ++i)
	{
		SerialDebugPrint(I2CConsoleStackMoveDown());
	}
#endif
}

void runScan()
{
	uint8_t device_addr;
	I2CConsoleMessage msg;
	char foundDevice = 0;

	SerialDebugPrint("Scanning for devices ...");
	for (device_addr = 0; device_addr < 128; ++device_addr)
	{
		char buff[20];
		snprintf(buff, 20, "ping %x", device_addr);

		if (!I2CConsoleParser(buff, &msg))
		{
			uint8_t sendResult = I2CConsoleSendCommand(&msg);
			if (!sendResult)
			{
				SerialDebugPrint("Found device 0x%x", device_addr);
				foundDevice = 1;
			}
			else
			{
				LOG("Error code %d for %s", sendResult, buff);
			}
		}
		else
		{
			LOG("Error parsing %s", buff);
		}
	}

	if (!foundDevice)
	{
		SerialDebugPrint("No I2C device found");
	}
}

void parseNRunMessage(const char * message)
{
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
			SerialDebugPrint("OK - '%s'", message);

			if (cmd.command == SENDNRECV)
			{
				SerialDebugPrint("RX result:");
				for (i = 0; i < cmd.rx_len; ++i)
				{
					SerialDebugPrint(" rx[%d] = 0x%x \t= %d \t= '%c'", i, cmd.rx[i],
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

				uint8_t initSec = Millis() / 1000;
				while ((uint8_t) (Millis() / 1000) - initSec
						<= (uint8_t) loop_time)
				{
					parseNRunMessage(
							message + strlen("loop ") + strlen(token) + 1);
				}
			}

			break;
		}

		token = strtok(NULL, " ");
		++n;
	}
}

/*
 * Supports i2cconsole parser format and loop
 */
void processMessage(const char * buffer)
{
	LOG("\n\nProcessing %s", buffer);

	uint8_t i = 0;
	char firstWord[20];
	while (buffer[i] != '\0' && buffer[i] != ' ')
	{
		firstWord[i] = buffer[i];
		++i;
	}
	firstWord[i] = '\0';

	if (strcasecmp(firstWord, "loop") == 0)
	{
		loopCommand(buffer);
	}
	else
	{
		parseNRunMessage(buffer);
	}

	_delay_ms(150);
}

int main()
{
	SerialDebugInitWithBaudRate(57600);

#ifndef DEBUG
	I2CConsoleStackInit();
#endif

	while (1)
	{
		char buffer[I2CMESSAGE_MAXLEN];

		SerialDebugPrint(" ");
		SerialDebugPrint(
				"Enter i2c command or type help (current address 0x%x, slow status %u)",
				I2CConsoleGetCurrentAddress(), I2CConsoleGetSlowSendingStatus());
		SerialDebugGetLine(buffer, 1);
		AVRStringTrimWhiteSpace(buffer);
		AVRStrinStripExtraSpace(buffer);

		if (strcasecmp(buffer, "help") == 0)
		{
			showHelp();
		}
		else if (strcasecmp(buffer, "history") == 0)
		{
			// show command history
			showHistory();
		}
		else if (strcasecmp(buffer, "SCAN") == 0)
		{
			// scan all i2c devices on the bus
			runScan();
		}
		else if (strcasecmp(buffer, "testlcd") == 0)
		{
			// test with lcd
			testLCD();
		}
		else if (strcasecmp(buffer, "testgy85compass") == 0)
		{
			// test with compass
			testGY85Compass();
		}
		else if (strcasecmp(buffer, "testgy85accel") == 0)
		{
			// test with accelerometer
			testGY85Accel();
		}
		else
		{
			// process n send buffer
#ifndef DEBUG
			I2CConsoleStackPush(buffer);
#endif
			processMessage(buffer);
		}

		_delay_ms(1);
	}

	return 0;
}

