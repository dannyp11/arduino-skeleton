/*
 * Timer.c
 *
 *  Created on: May 30, 2017
 *      Author: dat
 */

#include "Timer.h"
#include <avr/interrupt.h>

static Timer1Callback mT1Callback;

void Timer1Init(uint16_t ms)
{
	static uint8_t isInited = 0;
	if (isInited) return;
	isInited = 1;

	mT1Callback = 0;

	cli();

    TCCR1B |= (1 << WGM12);
    TIMSK1 |= (1 << OCIE1A);
    OCR1A = (int)(62.5*ms);
    TCCR1B |= (1 << CS12);

    sei();
}

void Timer1SetCallback(Timer1Callback callback)
{
	mT1Callback = callback;
}

ISR(TIMER1_COMPA_vect)
{
	if (mT1Callback)
	{
		mT1Callback();
	}
}
