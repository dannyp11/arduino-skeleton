/*
 * AVRString.h
 *
 * String manipulation for arduino
 *
 *  Created on: May 31, 2017
 *      Author: dat
 */

#ifndef UTILS_AVRSTRING_H_
#define UTILS_AVRSTRING_H_

#define STRING_MAXLEN	128 // max string length for all strings

/**
 * process string with backspace support
 *
 * eg. buffer = "hello\b\b\b\bi" will return buffer = "hi"
 *
 * @param buffer
 */
void AVRStringRefineString(char * buffer);

#endif /* UTILS_AVRSTRING_H_ */
