/*
 * SoftSerial.c
 *
 *  Created on: Jun 6, 2017
 *      Author: dat
 */
#include "SoftSerial.h"
#include "SerialDebug.h"
#include "Timer.h"

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define US_PER_BIT (1000000ul / SS_BAUD)

#define TOGGLE_PIN(port, pin, isOn) \
{	if (isOn == 0) port &= ~(1 << pin); \
	else port |= (1 << pin); \
}

typedef enum e_GetChStates
{
	START = 0,
	DATA0,
	DATA1,
	DATA2,
	DATA3,
	DATA4,
	DATA5,
	DATA6,
	DATA7,
	PARITY,
	STOP
} GetChStates;

typedef struct _SSGetC
{
	uint8_t isRequested;
	uint8_t isDone;
	uint8_t isValid;
	char data;
	uint8_t parity_data;
	GetChStates state;
} SSGetC;

static volatile SSGetC mGetCData;
static void fetchChar()
{
	if (!mGetCData.isRequested)
	{
		return;
	}

	switch (mGetCData.state)
	{
	case START:
	{
		mGetCData.isValid = 0;
		mGetCData.isDone = 0;
		if (SS_PINB_RX & (1 << SS_RX_PIN))
			return;
		TRACE_INT(SS_PINB_RX & (1 << SS_RX_PIN));
	}
		break;

	case DATA0:
	{
		if ((SS_PINB_RX & (1 << SS_RX_PIN)))
		{
			mGetCData.data |= (1 << 0);
			mGetCData.parity_data ^= 1;
		}
	}
		break;

	case DATA1:
	{
		if ((SS_PINB_RX & (1 << SS_RX_PIN)))
		{
			mGetCData.data |= (1 << 1);
			mGetCData.parity_data ^= 1;
		}
	}
		break;

	case DATA2:
	{
		if ((SS_PINB_RX & (1 << SS_RX_PIN)))
		{
			mGetCData.data |= (1 << 2);
			mGetCData.parity_data ^= 1;
		}
	}
		break;

	case DATA3:
	{
		if ((SS_PINB_RX & (1 << SS_RX_PIN)))
		{
			mGetCData.data |= (1 << 3);
			mGetCData.parity_data ^= 1;
		}
	}
		break;

	case DATA4:
	{
		if ((SS_PINB_RX & (1 << SS_RX_PIN)))
		{
			mGetCData.data |= (1 << 4);
			mGetCData.parity_data ^= 1;
		}
	}
		break;

	case DATA5:
	{
		if ((SS_PINB_RX & (1 << SS_RX_PIN)))
		{
			mGetCData.data |= (1 << 5);
			mGetCData.parity_data ^= 1;
		}
	}
		break;

	case DATA6:
	{
		if ((SS_PINB_RX & (1 << SS_RX_PIN)))
		{
			mGetCData.data |= (1 << 6);
			mGetCData.parity_data ^= 1;
		}
	}
		break;

	case DATA7:
	{
		if ((SS_PINB_RX & (1 << SS_RX_PIN)))
		{
			mGetCData.data |= (1 << 7);
			mGetCData.parity_data ^= 1;
		}
	}
		break;

	case PARITY:
	{
		if ((SS_PINB_RX & (1 << SS_RX_PIN) != 0) != mGetCData.parity_data)
		{
			mGetCData.isValid = 0;
		}
		else
		{
			mGetCData.isValid = 1;
		}
	}
		break;

	case STOP:
	{
		if (SS_PINB_RX & (1 << SS_RX_PIN))
		{
			mGetCData.isDone = 1;
			mGetCData.isRequested = 0;
			mGetCData.state = START;
		}
	}
	}

	if (mGetCData.state < STOP)
	{
		++mGetCData.state;
	}
}

/**
 * Call this before running any below api
 */
void SoftSerialInit()
{
	static uint8_t isInited = 0;
	if (isInited)
	{
		return;
	}
	isInited = 1;

	// config tx, rx port
	TOGGLE_PIN(SS_DDR_PORT_TX, SS_TX_PIN, 1); // output on tx
	TOGGLE_PIN(SS_PORT_TX, SS_TX_PIN, 1); // stop bit
	TOGGLE_PIN(SS_DDR_PORT_RX, SS_RX_PIN, 0); // input on rx

	// config interrupt
	Timer2init(1000000ul / SS_BAUD - 2);
	Timer2SetCallback(fetchChar);

	// config getcdata
	mGetCData.data = 0x00;
	mGetCData.isDone = 0;
	mGetCData.isRequested = 0;
	mGetCData.parity_data = 2;
	mGetCData.state = START;
	mGetCData.isValid = 0;
}

/**
 * Send char over soft serial
 *
 * @param c
 */
void SoftSerialPutChar(char c)
{
	// calculate parity bit
	uint8_t parity = 0;

	// start bit
	TOGGLE_PIN(SS_PORT_TX, SS_TX_PIN, 0);
	_delay_us(US_PER_BIT);

	// data
	uint8_t bit_mask;
	for (bit_mask = 0x01; bit_mask; bit_mask <<= 1)
	{
		TOGGLE_PIN(SS_PORT_TX, SS_TX_PIN, (c & bit_mask));
		parity ^= (c & bit_mask);
		_delay_us(US_PER_BIT);
	}

	// parity bit
	TOGGLE_PIN(SS_PORT_TX, SS_TX_PIN, parity);
	_delay_us(US_PER_BIT);

	// stop bit
	TOGGLE_PIN(SS_PORT_TX, SS_TX_PIN, 1);
	_delay_us(1);
}

char SoftSerialGetChar()
{
	char retVal = 0x00;

	while (1)
	{
		// enable request bit
		while (mGetCData.isRequested)
			;
		mGetCData.isRequested = 1;

		// wait for timer to finish
		while (!mGetCData.isDone)
			;

		retVal = mGetCData.data;
		mGetCData.isDone = 0;
		mGetCData.data = 0x00;

		if (mGetCData.isValid) break; // parity bit check
	}

	return retVal;
}
