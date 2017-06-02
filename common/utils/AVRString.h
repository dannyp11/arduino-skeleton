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

#define STRING_MAXLEN	80 // max string length for all strings

/**
 * process string with backspace support
 *
 * eg. buffer = "hello\b\b\b\bi" will return buffer = "hi"
 *
 * @param buffer
 */
void AVRStringRefineString(char * buffer);


/**
 * Trail leading and trailing space in str
 *
 *  Note: This function returns a pointer to a substring of the original string.
 If the given string was allocated dynamically, the caller must not overwrite
 that pointer with the returned value, since the original pointer must be
 deallocated using the same allocator with which it was allocated.  The return
 value must NOT be deallocated using free() etc.
 *TLDR: str will be modified in length so be careful about dealloc it
 *
 * @param str
 * @return
 */
char * AVRStringTrimWhiteSpace(char *str);

/**
 * Remove duplicate space in str
 *
 * @param str
 */
void AVRStrinStripExtraSpace(char* str);

#endif /* UTILS_AVRSTRING_H_ */
