/*
 * Millis.c
 *
 *  Created on: Jun 8, 2017
 *      Author: dat
 */

#include "Millis.h"
#include "Timer.h"

static Timer2Callback _mCallback_DB[MILLIS_MAX_CALLBACKS];
static uint8_t _nCallback;

static volatile unsigned long long _mmillis;
static void incMillis()
{
	++_mmillis;

	if (_nCallback)
	{
		uint8_t i;
		for (i = 0; i < _nCallback; ++i)
		{
			_mCallback_DB[i]();
		}
	}
}


void MillisInit()
{
	static char isInit = 0;
	if (isInit)
	{
		return;
	}
	isInit = 1;

	_mmillis = 0;
	_nCallback = 0;

	Timer2Init(1000);
	Timer2SetCallback(incMillis);
}

unsigned long long Millis()
{
	MillisInit();
	return _mmillis;
}

uint8_t MillisRegisterCallback(Timer2Callback callback)
{
	uint8_t retVal = 0;
	MillisInit();

	if ( _nCallback == MILLIS_MAX_CALLBACKS)
	{
		return 1;
	}
	else
	{
		_mCallback_DB[_nCallback++] = callback;
	}

	return retVal;
}
