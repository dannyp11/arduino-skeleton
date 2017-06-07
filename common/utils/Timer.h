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
typedef void (*Timer2Callback)(void);

/**
 * init timer 1 to run every ms milliseconds
 *
 * @param ms
 */
void Timer1Init(uint16_t ms);

/**
 * init timer 2 to run every n microseconds
 * note: n should be < 1024
 *
 * @param us
 */
void Timer2init(uint16_t n);

/**
 * register callback 1 for timer 1
 *
 * @param callback
 */
void Timer1SetCallback(Timer1Callback callback);

/**
 * register callback for timer 1
 *
 * @param callback
 */
void Timer2SetCallback(Timer2Callback callback);

#endif /* UTILS_TIMER_H_ */
