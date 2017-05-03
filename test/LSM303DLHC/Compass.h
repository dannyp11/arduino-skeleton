/*
 * Compass.h
 *
 *  Created on: Feb 28, 2017
 *      Author: dat
 */

#ifndef COMPASS_H_
#define COMPASS_H_

typedef enum compassDirection
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
	NORTHEAST,
	NORTHWEST,
	SOUTHEAST,
	SOUTHWEST
} CompassDirection;

/*
 * Call this function to initialize the Compass
 */
void CompassInit();

/*
 * main getter here
 */
CompassDirection CompassGetDirection();

/*
 * Helper for converting CompassDirection to text
 * print the text to buffer
 * return strlen of the printed buffer
 */
uint8_t CompassGetDirectionText(char * buffer, CompassDirection direction);

/*
 * secondary getter
 * Note: this is raw angle, not comparing to true North
 * for calibrated direction, use CompassGetDirection
 */
float CompassGetAngle();

#endif /* COMPASS_H_ */
