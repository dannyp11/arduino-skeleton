/*
 * I2CConsoleStack.c
 *
 *  Created on: May 31, 2017
 *      Author: dat
 */

#include "I2CConsole.h"
#include <stdio.h>
#include <string.h>

#define HISTORY_COUNT	5

static char g_i2c_history[HISTORY_COUNT][50];
static uint8_t g_i2c_current_index;
static uint8_t g_i2c_start_index; // latest index
static uint8_t g_i2c_stack_count;

/**
 * internal - get distance from 2 indexes
 *
 * @param from
 * @param to
 * @return - eg. from 1 to 3 is 2, from 4 to 0 is 1
 */
uint8_t getIndexDistance(uint8_t from, uint8_t to)
{
	if (from <= to)
	{
		return to - from;
	}

	return (to + HISTORY_COUNT - from);
}

/*
 * Index rollover, 4->0 since we only support 5 histories
 */
uint8_t getNextIndex(uint8_t index)
{
	if (index == HISTORY_COUNT -1)
	{
		return 0;
	}

	return index + 1;
}

/*
 * Index rewind, 0->4 since we only support 5 histories
 */
uint8_t getPreviousIndex(uint8_t index)
{
	if (index == 0)
	{
		return (HISTORY_COUNT - 1);
	}

	return index - 1;
}


/**
 * get previous saved message, doesn't actually clear it
 *
 * @return message if exists, NULL if none
 */
const char * I2CConsoleStackMoveUp()
{
	// null if nothing from stack
	if (g_i2c_stack_count == 0)
	{
		return NULL;
	}

	const char * retVal = g_i2c_history[g_i2c_current_index];

	// limit current index
	if (getIndexDistance(g_i2c_current_index, g_i2c_start_index) > 0)
	{
		g_i2c_current_index = getNextIndex(g_i2c_current_index);
	}


	return retVal;
}

/**
 * get next saved message, doesn't actually clear it
 *
 * @return message if exists, NULL if none
 */
const char * I2CConsoleStackMoveDown()
{
	// null if nothing from stack
	if (g_i2c_stack_count == 0)
	{
		return NULL;
	}

	const char * retVal = g_i2c_history[g_i2c_current_index];

	// limit current index
	if (getIndexDistance(g_i2c_current_index, g_i2c_start_index) < g_i2c_stack_count - 1)
	{
		g_i2c_current_index = getPreviousIndex(g_i2c_current_index);
	}

	return retVal;
}

/**
 * Push to stack message history, this will roll over obsolete messages
 * Limit to 5 history
 *
 * @param message - string to push in
 */
void I2CConsoleStackPush(const char * message)
{
	I2CConsoleStackInit();

	// push message to history

	// update start index
	g_i2c_start_index = (g_i2c_stack_count == 0)? 0 : getNextIndex(g_i2c_start_index);

	// update history[start index]
	strcpy(g_i2c_history[g_i2c_start_index], message);

	// set current index to start index
	g_i2c_current_index = g_i2c_start_index;

	// limit stack count to history count
	if (g_i2c_stack_count < HISTORY_COUNT)
	{
		++g_i2c_stack_count;
	}
}

void I2CConsoleStackResetIndex()
{
	g_i2c_current_index = g_i2c_start_index;
}

void I2CConsoleStackInit()
{
	static uint8_t _stack_inited = 0;
	if (!_stack_inited)
	{
		I2CConsoleStackReInit();
		_stack_inited = 1;
	}
}

void I2CConsoleStackReInit()
{
	g_i2c_stack_count = 0;
	g_i2c_current_index = 0;
	g_i2c_start_index = 0;
}

uint8_t I2CConsoleStackGetCount()
{
	return g_i2c_stack_count;
}
