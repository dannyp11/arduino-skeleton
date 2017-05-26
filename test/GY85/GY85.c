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

static uint8_t result[6];
static uint8_t cmd[2];

float GY85CompassGetAngle(void)
{
	cmd[0] = 0x03;
	I2CSendnRecvData(COMPASS_ADDR, cmd, 1, result, 6, 0);

	int16_t x = (int16_t) (((int16_t) result[0] << 8) | result[1]);
	int16_t z = (int16_t) (((int16_t) result[2] << 8) | result[3]);
	int16_t y = (int16_t) (((int16_t) result[4] << 8) | result[5]);

	float mHeader = atan2(x, y) * 180.0 / M_PI + CALIBRATED_OFFSET;
	if (mHeader < 0)
		mHeader += 360.0;

	return mHeader;
}

void GY85Init(void)
{
	I2CInit();

	// Compass init ---------------------------------------
	cmd[0] = 0x02;
	cmd[1] = 0x00;
	I2CSendData(COMPASS_ADDR, cmd, 2, 0);

	cmd[0] = 0x00;
	cmd[1] = 0x90;
	I2CSendData(COMPASS_ADDR, cmd, 2, 0);
	// ----------------------------------------------------

	SerialDebugInit();

	while (1)
	{
		SerialDebugPrint("data %f", GY85CompassGetAngle());

		_delay_ms(5);
	}

//	SerialDebugPrint("data %f", 1.2);

}
