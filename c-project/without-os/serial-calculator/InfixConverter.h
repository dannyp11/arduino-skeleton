/*
 * InfixConverter.h
 *
 *  Created on: Jun 2, 2017
 *      Author: dat
 */

#ifndef SERIAL_CALCULATOR_INFIXCONVERTER_H_
#define SERIAL_CALCULATOR_INFIXCONVERTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

/**
 * convert from infix to postfix
 * Support add subtract mult div ()
 * Currently support unsigned input
 *
 * WIP : +
 *
 * @param input		- eg. "1+2", "1 * (2 + 3)"
 * @param output	- eg. "1 2 +", "1 2 3 + *"
 * @return 	- 0 on success
 * 			- 1 on error format
 * 			- 2 on unsupported char
 */
uint8_t InfixConverterConvertString(const char * input, char * output);

#ifdef __cplusplus
}
#endif

#endif /* SERIAL_CALCULATOR_INFIXCONVERTER_H_ */
