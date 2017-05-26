/*
 * GY85.h
 *
 *  Created on: May 25, 2017
 *      Author: dat
 */

#ifndef TEST_GY85_GY85_H_
#define TEST_GY85_GY85_H_

/**
 *	Init this first before running any api
 */
void GY85Init(void);

/**
 * Get compass angle, 0-360
 */
float GY85CompassGetAngle(void);

#endif /* TEST_GY85_GY85_H_ */
