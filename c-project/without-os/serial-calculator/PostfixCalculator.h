/*
 * InfixCalculator.h
 *
 *  Created on: Jun 3, 2017
 *      Author: dat
 */

#ifndef SERIAL_CALCULATOR_INFIXCALCULATOR_H_
#define SERIAL_CALCULATOR_INFIXCALCULATOR_H_

#include <inttypes.h>

#define STACK_I32_STOPENTRY	INT32_MIN
#define STACK_TYPE	long long

/**
 * calculate postfix from input
 * supports + - / * ( ), positive numbers
 *
 * @param input eg. "1 2 +"
 *
 * return - result
 * 			INT32_MIN on error
 *
 */
STACK_TYPE PostfixCalculatorCompute( const char * input);

#endif /* SERIAL_CALCULATOR_INFIXCALCULATOR_H_ */
