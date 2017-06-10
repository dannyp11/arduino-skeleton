/*
 * Millis.c
 *
 *  Created on: Jun 8, 2017
 *      Author: dat
 */

#include "Millis.h"
#include "Timer.h"
#include "SerialDebug.h"

static Timer2Callback _mCallback_DB[MILLIS_MAX_CALLBACKS];
static uint8_t _nCallback;
static int _millisInit;

static volatile unsigned long _mmillis;
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

char MillisInit(char timerNumber)
{
	static char isInit = 0;
	if (isInit)
	{
		return _millisInit;
	}
	isInit = 1;

	_mmillis = 0;
	_nCallback = 0;
	_millisInit = 0;

	if (timerNumber == 1)
	{
		Timer1Init(1);
		Timer1SetCallback(incMillis);
	}
	else if (timerNumber == 2)
	{
		Timer2Init(1000);
		Timer2SetCallback(incMillis);
	}
	else
	{
		_millisInit = 1;
	}

	return _millisInit;
}

unsigned long Millis()
{
	if (MillisInit(2))
	{
		return 1;
	}

	return _mmillis;
}

uint8_t MillisRegisterCallback(Timer2Callback callback)
{
	uint8_t retVal = 0;
	MillisInit(2);

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
