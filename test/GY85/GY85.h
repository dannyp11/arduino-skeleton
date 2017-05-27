/*
 * GY85.h
 *
 *  Created on: May 25, 2017
 *      Author: dat
 */

#ifndef TEST_GY85_GY85_H_
#define TEST_GY85_GY85_H_

#include <inttypes.h>

typedef struct _GY85CompassData
{
	int16_t x, y, z;
} GY85CompassData;

/**
 *	Init this first before running any api
 */
void GY85Init(void);

/**
 * Get compass angle, 0-360
 */
float GY85CompassGetAngle(void);

/**
 * Get raw data x, y, z to data
 */
uint8_t GY85CompassGetRawData( GY85CompassData * data);

#endif /* TEST_GY85_GY85_H_ */
