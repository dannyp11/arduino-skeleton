/*
 * GY85.c
 *
 *  Created on: May 25, 2017
 *      Author: dat
 */

#include "GY85.h"
#include "I2C.h"
#include "SerialDebug.h"

#include <math.h>
#include <util/delay.h>

// addresses ---------------------------
#define COMPASS_ADDR	0x1e
#define ACCEL_ADDR		0x53
#define GYRO_ADDR		0x68
// -------------------------------------

/*
 * Calibrated values
 */
#define	CALIBRATED_OFFSET -60.0
#define TRUE_S	180.0
#define TRUE_W	270.0
#define TRUE_E	90.0
#define TRUE_NW	315.0
#define TRUE_NE	45.0
#define TRUE_SW	225.0
#define TRUE_SE	135.0
#define TOLERANCE	22.0

static uint8_t result[8];
static uint8_t cmd[2];

double GY85CompassGetAngle(void)
{
	GY85CompassRawData data;
	GY85CompassGetRawData(&data);

	double mHeader = atan2((double) data.x, (double) data.y) * 180.0 / M_PI
			+ CALIBRATED_OFFSET;
	if (mHeader < 0)
		mHeader += 360.0;

	return mHeader;
}

uint8_t GY85Init(void)
{
	I2CInit();
	uint8_t retVal = 0;

	// Compass init ---------------------------------------
	cmd[0] = 0x02;
	cmd[1] = 0x00;
	retVal += I2CSendData(COMPASS_ADDR, cmd, 2, 0);

	cmd[0] = 0x00;
	cmd[1] = 0x90;
	retVal += I2CSendData(COMPASS_ADDR, cmd, 2, 0);
	// ----------------------------------------------------

	// Accel init -----------------------------------------
	cmd[0] = 0x31;
	cmd[1] = 0x01;	// +-4g
	retVal += I2CSendData(ACCEL_ADDR, cmd, 2, 0);

	cmd[0] = 0x2d;
	cmd[1] = 0x08; // continuous measurement enabled
	retVal += I2CSendData(ACCEL_ADDR, cmd, 2, 0);
	// ----------------------------------------------------

	// Gyro init ------------------------------------------
	cmd[0] = 0x3e;
	cmd[1] = 0x00; // turn on power
	retVal += I2CSendData(GYRO_ADDR, cmd, 2, 0);

	cmd[0] = 0x15;
	cmd[1] = 0x07; // 8ms/sample
	retVal += I2CSendData(GYRO_ADDR, cmd, 2, 0);

	cmd[0] = 0x16;
	cmd[1] = 0x1e; // +-2000/sec, 1kHz internal
	retVal += I2CSendData(GYRO_ADDR, cmd, 2, 0);

	cmd[0] = 0x17;
	cmd[1] = 0x00; // no interrupt
	retVal += I2CSendData(GYRO_ADDR, cmd, 2, 0);
	// ----------------------------------------------------

	return retVal;
}

uint8_t GY85CompassGetRawData(GY85CompassRawData * data)
{
	uint8_t res = 1;

	cmd[0] = 0x03;
	res = I2CSendnRecvData(COMPASS_ADDR, cmd, 1, result, 6, 0);

	data->x = (int16_t) (((uint16_t) (result[0] << 8)) | result[1]);
	data->z = (int16_t) (((uint16_t) (result[2] << 8)) | result[3]);
	data->y = (int16_t) (((uint16_t) (result[4] << 8)) | result[5]);

	return res;
}

/**
 * Get raw data from accelerometer
 * @param data
 * @return
 */
uint8_t GY85AccelGetRawData(GY85AccelRawData * data)
{
	uint8_t retVal = 1;

	cmd[0] = 0x32;
	retVal = I2CSendnRecvData(ACCEL_ADDR, cmd, 1, result, 6, 0);

	if (!retVal)
	{
		data->x = ((int16_t) result[1] << 8) | result[0];
		data->y = ((int16_t) result[3] << 8) | result[2];
		data->z = ((int16_t) result[5] << 8) | result[4];
	}

	return retVal;
}

/**
 * Get raw data from gyroscope
 * @param data
 * @return
 */
uint8_t GY85GyroGetRawData(GY85GyroRawData * data)
{
	uint8_t retVal = 1;

	cmd[0] = 0x1b;
	retVal = I2CSendnRecvData(GYRO_ADDR, cmd, 1, result, 8, 0);

	if (!retVal)
	{
		data->tempearature = ((int16_t) result[0] << 8) | result[1];
		data->x = ((int16_t) result[2] << 8) | result[3];
		data->y = ((int16_t) result[4] << 8) | result[5];
		data->z = ((int16_t) result[6] << 8) | result[7];

		data->temperature_Celcius = 35 + (data->tempearature + 13200) / 280;
	}

	return retVal;
}

