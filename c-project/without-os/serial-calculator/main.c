/*
 * main.c
 *
 *  Created on: Jun 2, 2017
 *      Author: dat
 */

#include "PostfixCalculator.h"
#include "InfixConverter.h"

#include "SerialDebug.h"
#include "AVRString.h"
#include "PgmStorage.h"

#include <stdio.h>
#include <util/delay.h>

const char PROGMEM help1[] = "Supported operator + - * / ( )";
const char PROGMEM help2[] = "Example: ";
const char PROGMEM help3[] = "\t 1+1*2-3*(4+5)";
const char PROGMEM help4[] = "\t 2 + 3 / (4 + 5) - 34 * (12 + 34)";

void printPgmString(const char * string)
{
	char msg[PGM_SIZE];
	PgmStorageGet(msg, string);
	SerialDebugPrint(msg);
}

void showHelp()
{
	printPgmString(help1);
	printPgmString(help2);
	printPgmString(help3);
	printPgmString(help4);
}

int main()
{
	SerialDebugInit();

	while (1)
	{
		char input[STRING_MAXLEN];
		char input2[STRING_MAXLEN];
		char postfix_input[STRING_MAXLEN];
		SerialDebugPrint("\n--------------------------------------");
		SerialDebugPrint("Type calculator input or type help:");
		SerialDebugGetLine(input, 1);
		strcpy(input2, input);

		AVRStrinStripExtraSpace(input);
		AVRStringRefineString(input);
		char * buffer = AVRStringTrimWhiteSpace(input);

		/*
		 * Command processing
		 */
		if (strcasecmp(buffer, "help") == 0)
		{
			showHelp();
			continue;
		}

		uint8_t infix2PostfixResult = InfixConverterConvertString(buffer,
				postfix_input);

		// check if parse error
		if (infix2PostfixResult)
		{
			SerialDebugPrint("error code %u parsing '%s'", infix2PostfixResult,
					input2);
		}
		else
		{
			// parse succeeds
			int32_t postfixCalculatedResult = PostfixCalculatorCompute(
					postfix_input);
			if (postfixCalculatedResult == STACK_I32_STOPENTRY)
			{
				SerialDebugPrint("error calculating '%s'", input2);
			}
			else
			{
				SerialDebugPrint("\t%s = %ld", input2, postfixCalculatedResult);
			}
		}

		_delay_ms(1);
	}

	return 0;
}
