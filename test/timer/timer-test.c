/*
 * Test all timer interrupts in arduino
 *
 */

#include <avr/interrupt.h>
#include "SerialDebug.h"
#include "Timer.h"

#define MICROS_INC	100

static volatile long _milis, _milis2;
static volatile long _micros;

unsigned long millis()
{
	static uint8_t isInited = 0;
	if (!isInited)
	{
		_milis = 0;
		isInited = 1;
	}

	return _milis;
}

unsigned long millis2()
{
	static uint8_t isInited = 0;
	if (!isInited)
	{
		_milis2 = 0;
		isInited = 1;
	}

	return _milis2;
}

unsigned long micros()
{
	static uint8_t isInited = 0;
	if (!isInited)
	{
		_micros = 0;
		isInited = 1;
	}

	return _micros;
}

unsigned secs()
{
	return millis() / 1000;
}

void incMillis()
{
	++_milis;
}

void incMillis2()
{
	_milis2 += MICROS_INC;
}

void incMicros()
{
	_micros += 104;
}

int main(void)
{
	SerialDebugInit();

	Timer1Init(1);
	Timer1SetCallback(incMillis);

	Timer2init(MICROS_INC);
	Timer2SetCallback(incMicros);

	int prev_sec = -1;
	while (1)
	{
		int sec = secs();

		if (prev_sec != sec)
		{
			TRACE_INT(TCCR2B)
			TRACE_INT(TCNT2)
			TRACE_INT(OCR2A)
			SerialDebugPrint(" %d s passed", sec);
			SerialDebugPrint("%lu ms passed\n", micros()/1000);
		}

		prev_sec = sec;
	}

	return 0; /* never reached */
}

