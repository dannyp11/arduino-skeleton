/*
 * AVRString.c
 *
 *  Created on: May 31, 2017
 *      Author: dat
 */

#include "AVRString.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

void AVRStringRefineString(char * buffer)
{
	if (!strlen(buffer))
		return;

	char message[STRING_MAXLEN];

	int i = 0; // buffer index
	int j = 0; // message index

	while (!(buffer[i] == '\0' || buffer[i] == '\n' || buffer[i] == '\r'))
	{
		if (buffer[i] == '\b' && (j - 1 >= 0))
		{
			--j;
		}
		else
		{
			message[j] = buffer[i];
			++j;
		}

		++i;
	}
	message[j] = '\0';
	strcpy(buffer, message);
}

char * AVRStringTrimWhiteSpace(char *str)
{
	char *end;

	// Trim leading space
	while (isspace((unsigned char) *str))
		str++;

	if (*str == 0)  // All spaces?
		return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char) *end))
		end--;

	// Write new null terminator
	*(end + 1) = 0;

	return str;
}

void AVRStrinStripExtraSpace(char* str) {
  int i,x;
  for(i=x=0; str[i]; ++i)
    if(!isspace(str[i]) || (i>0 && !isspace(str[i-1])))
      str[x++] = str[i];
  str[x] = '\0';
}

void AVRStringRemoveChar(char * str, char c)
{
	uint8_t i, new_i;
	new_i = 0;

	for (i = 0; i < strlen(str); ++i)
	{
		if (str[i] != c)
		{
			str[new_i] = str[i];
			++new_i;
		}
	}

	str[new_i] = '\0';
}
