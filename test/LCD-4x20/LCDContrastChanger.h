/*
 * LCDContrastChanger.h
 *
 *  Created on: May 26, 2017
 *      Author: dat
 *
 *      Setup:
 *
 *      	Potentialmeter: 1 - gnd
 *      					2 - A1
 *      					3 - 5V
 */

#ifndef TEST_LCD_4X20_LCDCONTRASTCHANGER_H_
#define TEST_LCD_4X20_LCDCONTRASTCHANGER_H_

#include <inttypes.h>

/* Init code */
void LCDContrastChangerInit();

/**
 * Set reference for contrast level variable, interrupt will change this variable
 */
void LCDContrastChangerSetVariable(uint8_t * var);

#endif /* TEST_LCD_4X20_LCDCONTRASTCHANGER_H_ */
