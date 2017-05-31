/*
 * Timer.h
 *
 *  Created on: May 30, 2017
 *      Author: dat
 */

#ifndef UTILS_TIMER_H_
#define UTILS_TIMER_H_

#include <inttypes.h>

typedef void (*Timer1Callback)(void);

/**
 * init timer 1 to run every ms milliseconds
 *
 * @param ms
 */
void Timer1Init(uint16_t ms);

/**
 * register callback for timer 1
 *
 * @param callback
 */
void Timer1SetCallback(Timer1Callback callback);

#endif /* UTILS_TIMER_H_ */
