/*
 * Test all timer interrupts in arduino
 *
 */

#include <avr/interrupt.h>
#include "SerialDebug.h"
#include "Timer.h"

static volatile long _milis;

unsigned long millis()
{
	static isInited = 0;
	if (!isInited)
	{
		_milis = 0;
		isInited = 1;
	}

	return _milis;
}

unsigned secs()
{
	return millis() / 1000;
}

void incMillis()
{
	++_milis;
}

int main(void)
{
	SerialDebugInit();
	Timer1Init(1);
	Timer1SetCallback(incMillis);

	int prev_sec = -1;
	while (1)
	{
		int sec = secs();

		if (prev_sec != sec)
		{
			SerialDebugPrint(" %d s passed", sec);
		}

		prev_sec = sec;
	}

	return 0; /* never reached */
}

