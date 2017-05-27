/*
 * lcd-test.c
 *
 *  Created on: May 24, 2017
 *      Author: dat
 */

/*
 * LCD pinout:
 *
 * J2:
 * 		3 - SCL
 * 		4 - SDA
 * 		5 - gnd
 * 		6 - 5v
 */
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "LCD.h"
#include "LCDContrastChanger.h"

#define BUFF_LEN LCD_LINE_LEN + 1

int main()
{
	char buffer[BUFF_LEN];
	uint8_t contrastLevel;

	LCDContrastChangerInit();
	LCDContrastChangerSetVariable(&contrastLevel);
	LCDInit();
	LCDSendCommand(CURSOROFF);
	LCDSendCommand(SHOWFIRMWARE);

	LCDSetCursor(2, 0);
	LCDPrint("LCD Demo by Dat Pham");
	LCDSetCursor(3, 0);
	snprintf(buffer, BUFF_LEN, "Version: %s", __DATE__);
	LCDPrint(buffer);

	char i;
	for (i = 0; i < 5; ++i)
	{
		LCDSetCursor(4, 0);
		snprintf(buffer, BUFF_LEN, "Demo in %d seconds...", 5 - i);
		LCDPrint(buffer);
		_delay_ms(1000);
	}

	/*
	 * brightness
	 */
	LCDSendCommand(CLEARSCREEN);
	LCDPrint("Changing brightness");
	for (i = 1; i <= 8; ++i)
	{
		LCDSetCursor(3, 0);
		snprintf(buffer, BUFF_LEN, "Level: %d", (int) i);
		LCDPrint(buffer);
		LCDSetBrightness(i);
		_delay_ms(500);
	}
	LCDReset();

	/*
	 * contrast
	 */
	LCDPrint("Changing contrast");
	for (i = 1; i <= 50; ++i)
	{
		LCDSetCursor(3, 0);
		snprintf(buffer, BUFF_LEN, "Level: %d", (int) i);
		LCDPrint(buffer);
		LCDSetContrast(i);
		_delay_ms(50);
	}
	LCDReset();

	/*
	 * Turn on and off
	 */
	char* blinking_msg = "..Blinking Srceen...";
	LCDPrint(blinking_msg);
	LCDSetCursor(2, 0);
	LCDPrint(blinking_msg);
	LCDSetCursor(3, 0);
	LCDPrint(blinking_msg);
	LCDSetCursor(4, 0);
	LCDPrint(blinking_msg);
	for (i = 1; i <= 5; ++i)
	{
		LCDSendCommand(LCDOFF);
		_delay_ms(500);
		LCDSendCommand(LCDON);
		_delay_ms(500);
	}
	LCDReset();

	/*
	 * Move cursor
	 */
	LCDPrint("Moving Cursor");
	_delay_ms(2000);
	LCDSendCommand(CLEARSCREEN);
	LCDSendCommand(CURSORON);
	char j;
	for (i = 1; i<=4; ++i)
	{
		for (j = 0; j < 20; ++j)
		{
			LCDSetCursor(i, j);
			_delay_ms(100);
		}
	}
	LCDReset();

	LCDSetCursor(2, 5);
	LCDPrint("Done demo");

	// contrast is manually changed here since i2c is not async
	while (1)
	{
		LCDSetContrast(contrastLevel);
	}

	return 0;
}
