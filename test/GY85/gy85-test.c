/*
 * gy85-test.c
 *
 *  Created on: May 25, 2017
 *      Author: dat
 */

#include "I2C.h"
#include "SerialDebug.h"
#include "LCD.h"
#include "GY85.h"

int main()
{
	SerialDebugInit();
	LCDInit();

	GY85Init();
//	I2CInit();

	uint8_t cmd[3];
	uint8_t data[6];

//	I2CSendnRecvData??(0x19, cmd, 1, data, 1);

	while (1)
	{

	}

	return 0;
}
