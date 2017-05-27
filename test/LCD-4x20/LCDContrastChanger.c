/*
 * LCDContrastChanger.c
 *
 *  Created on: May 26, 2017
 *      Author: dat
 */

#include "LCDContrastChanger.h"
#include <avr/interrupt.h>
#include "LCD.h"
#include "SerialDebug.h"

static volatile uint8_t _old_level;
static volatile uint8_t * mLevel;

void LCDContrastChangerInit()
{
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz
	ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
	ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading
	ADMUX |= (1 << MUX0);
	// No MUX values needed to be changed to use ADC0
	ADCSRA |= (1 << ADATE);  // Set ADC to Free-Running Mode
	ADCSRA |= (1 << ADEN);  // Enable ADC
	ADCSRA |= (1 << ADIE);  // Enable ADC Interrupt
	cli();
	// Enable Global Interrupts
	ADCSRA |= (1 << ADSC);  // Start A2D Conversions
	sei();

	SerialDebugInit();
	_old_level = 60;
}

void LCDContrastChangerSetVariable(uint8_t * var)
{
	mLevel = var;
}

ISR(ADC_vect)
{
	*mLevel = ((uint16_t)ADCW) / (65535/48) + 1;
}
