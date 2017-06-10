/*
 * Millis.h
 *
 *  Created on: Jun 8, 2017
 *      Author: dat
 */

#ifndef MILLIS_H_
#define MILLIS_H_

#include "Timer.h"

#define MILLIS_MAX_CALLBACKS	3

/**
 * Init millisecond, this will use timer
 * If other apis called before this API, it will default to timer 2
 *
 * @param timerNumber - timer 1 or 2
 */
char MillisInit(char timerNumber);

/**
 * main getter of millis, will always return 1 if error on init
 */
unsigned long Millis();

/**
 * Register callback function for every millisecond
 * Supports up to MILLIS_MAX_CALLBACKS functions
 *
 * @param callback
 * @return 0 on success , 1 on overload
 */
uint8_t MillisRegisterCallback(Timer2Callback callback);

#endif /* MILLIS_H_ */
