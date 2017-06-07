/*
 * Timer.c
 *
 *  Created on: May 30, 2017
 *      Author: dat
 */

#include "Timer.h"
#include <avr/interrupt.h>

static Timer1Callback mT1Callback, mT1Callback2;
static Timer2Callback mT2Callback;

void Timer1Init(uint16_t ms)
{
	static uint8_t isInited = 0;
	if (isInited)
		return;
	isInited = 1;

	mT1Callback = 0;
	mT1Callback2 = 0;

	cli();

	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A); // enable counter compare a
	OCR1A = (int) (62.5 * ms);
	TCCR1B |= (1 << CS12); // 256 prescaler

	sei();
}

void Timer1SetCallback(Timer1Callback callback)
{
	if (!mT1Callback)
	{
		mT1Callback = callback;
	}
	else
	{
		mT1Callback2 = callback;
	}
}

void Timer2SetCallback(Timer2Callback callback)
{
	mT2Callback = callback;
}

ISR(TIMER1_COMPA_vect)
{
	if (mT1Callback)
	{
		mT1Callback();
	}

	if (mT1Callback2)
	{
		mT1Callback2();
	}
}

ISR(TIMER2_COMPA_vect)
{
	if (mT2Callback)
	{
		mT2Callback();
	}
}

void Timer2init(uint16_t n)
{
	static uint8_t isInited = 0;
	if (isInited)
		return;
	isInited = 1;

	mT2Callback = 0;

	cli();

	//set timer2 interrupt at at us2 * 2us
	TCCR2A = 0; // set entire TCCR2A register to 0
	TCCR2B = 0; // same for TCCR2B
	TCNT2 = 0; //initialize counter value to 0
	// set compare match register for n * 2us increments
	OCR2A = (int) ((n / 4));	  // = n/2*(16*10^6) / (64) - 1 (must be <256)
	// turn on CTC mode
	TCCR2A |= (1 << WGM21);
	// Set CS22 bit for 64 prescaler
	TCCR2B |= (1 << CS22);
	// enable timer compare interrupt
	TIMSK2 |= (1 << OCIE2A);

	sei();
}
