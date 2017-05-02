/*******************************************************************************
 *
 * Created on May 1, 2017 by Dat (Daniel) Pham
 *
 * Filename: Led.cpp
 *
 * Description: 
 *
 * Limitations: 
 *
 * Environment: ANSI C, X-series
 *
 * Version: @(#) (Internal Only)
 *
 * History: 
 *
 ******************************************************************************/
#include "Led.h"
#include <avr/io.h>

Led::Led()
{
	DDRB |= _BV(DDB5);
}

void Led::turnOn() const
{
	PORTB |= _BV(PORTB5);
}

void Led::turnOff() const
{
	PORTB &= ~_BV(PORTB5);
}

