/*
 * AVRString.c
 *
 *  Created on: May 31, 2017
 *      Author: dat
 */

#include "AVRString.h"

#include <stdio.h>
#include <string.h>


void AVRStringRefineString(char * buffer)
{
	if (!strlen(buffer)) return;

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
