/*
 * GY85.h
 *
 *  Created on: May 25, 2017
 *      Author: dat
 */

#ifndef TEST_GY85_GY85_H_
#define TEST_GY85_GY85_H_

#include <inttypes.h>

typedef struct _GY85CompassRawData
{
	int16_t x, y, z;
} GY85CompassRawData;

typedef struct _GY85AccelRawData
{
	int16_t x, y, z;
} GY85AccelRawData;

typedef struct _GY85GyroRawData
{
	int16_t x ,y, z;
	int16_t tempearature, temperature_Celcius;
} GY85GyroRawData;

typedef struct _GY85GyroRealData
{
	double yawn, pitch, roll;
} GY85GyroRealData;

/**
 *	Init this first before running any api
 *
 *	return 0 on success
 */
uint8_t GY85Init(void);

/**
 * Get compass angle, 0-360
 */
double GY85CompassGetAngle(void);

/**
 * Get raw data x, y, z to data
 */
uint8_t GY85CompassGetRawData( GY85CompassRawData * data);

/**
 * Get raw data from accelerometer
 * @param data
 * @return
 */
uint8_t GY85AccelGetRawData(GY85AccelRawData * data);

/**
 * Get raw data from gyroscope
 * @param data
 * @return
 */
uint8_t GY85GyroGetRawData(GY85GyroRawData * data);



#endif /* TEST_GY85_GY85_H_ */
