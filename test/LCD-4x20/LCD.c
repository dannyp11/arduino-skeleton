/*
 * LCD.c
 *
 *  Created on: Feb 27, 2017
 *      Author: danny
 */

#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "I2C.h"
#include "LCD.h"

/* Address of the LCD on the I2C bus */
#define LCD_ADDR  0x28

/*
 * Portability
 */
#ifndef CHIBI
#define mSleepms(ms) _delay_ms(ms);
#define mSleepus(us) _delay_us(us);
#else
#define mSleepms(ms) chThdSleepMilliseconds(ms);
#define mSleepus(us) chThdSleepMicroseconds(us);
#endif

/*
 * LCD APIs here
 */
// Start LCD APIs -------------------------------------------------------------
uint8_t LCDSendCommand(LCDCommand command)
{
	uint8_t status;
	uint8_t cmd[2];
	cmd[0] = 0xfe;

	switch (command)
	{
	case LCDON:
	{
		cmd[1] = 0x41;
		status = I2CSendData(LCD_ADDR, cmd, 2, 1);
		mSleepus(150);
	}
		break;
	case LCDOFF:
	{
		cmd[1] = 0x42;
		status = I2CSendData(LCD_ADDR, cmd, 2, 1);
		mSleepus(150);
	}
		break;
	case CLEARSCREEN:
	{
		cmd[1] = 0x51;
		status = I2CSendData(LCD_ADDR, cmd, 2, 1);
		mSleepms(150);
	}
		break;
	case CURSORHOME:
	{
		cmd[1] = 0x46;
		status = I2CSendData(LCD_ADDR, cmd, 2, 1);
		mSleepms(2);
	}
		break;
	case CURSORON:
	{
		cmd[1] = 0x4b;
		status = I2CSendData(LCD_ADDR, cmd, 2, 1);
		mSleepus(150);
	}
		break;
	case CURSOROFF:
	{
		cmd[1] = 0x4c;
		status = I2CSendData(LCD_ADDR, cmd, 2, 1);
		mSleepus(150);
	}
		break;
	case SHOWFIRMWARE:
	{
		cmd[1] = 0x70;
		status = I2CSendData(LCD_ADDR, cmd, 2, 1);
		mSleepms(5);
	}
		break;
	default:
		return 1;
		break;
	}

	return status;
}

uint8_t LCDSetBrightness(char level)
{
	uint8_t status;

	uint8_t cmd[3];
	cmd[0] = 0xfe;
	cmd[1] = 0x53;
	cmd[2] = level;
	status = I2CSendData(LCD_ADDR, cmd, 3, 1);
	mSleepus(100);

	return status;
}

uint8_t LCDSetContrast(char level)
{
	uint8_t status;

	uint8_t cmd[3];
	cmd[0] = 0xfe;
	cmd[1] = 0x52;
	cmd[2] = level;
	status = I2CSendData(LCD_ADDR, cmd, 3, 1);
	mSleepms(50);

	return status;
}

uint8_t LCDSetCursor(char line, char position)
{
	uint8_t status;
	char col = 0x00;
	if (line == 2)
	{
		col = 0x40;
	}
	else if (line == 3)
	{
		col = 0x14;
	}
	else if (line == 4)
	{
		col = 0x54;
	}

	uint8_t cmd[3];
	cmd[0] = 0xfe;
	cmd[1] = 0x45;
	cmd[2] = col + position;
	status = I2CSendData(LCD_ADDR, cmd, 3, 1);
	mSleepus(150);

	return status;
}

void LCDInit()
{
	I2CInit();             // Initialize the I2C port
	mSleepms(200);

	LCDReset();
	LCDPrint("LCD inited");
	_delay_ms(1000);
	LCDReset();
}

uint8_t LCDPrint(const char *msg)
{
	uint8_t status;

	uint8_t i;
	char len = strlen(msg);
	for (i = 0; i < (uint8_t) len; ++i)
	{
		status = I2CSendData(LCD_ADDR, (uint8_t *) &msg[i], 1, 1);
		mSleepus(150);
		if (status)
		{
			return status;
		}
	}
	return status;
}

void LCDReset()
{
	LCDSendCommand(CLEARSCREEN);
	LCDSendCommand(CURSORHOME);
	LCDSendCommand(CURSOROFF);
	LCDSetContrast(40);
	LCDSetBrightness(8);
}
// End LCD APIs -------------------------------------------------------------
