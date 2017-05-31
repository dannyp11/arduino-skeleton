/*
 * Demo Serial connection with pc
 */

#include <util/delay.h>
#include "SerialDebug.h"

char str1[] = "Hello World";
char str2[] =
		"No man has a good enough memory to be a successful liar - A. Lincoln";
char str3[] = "E=mc2";
char str4[] =
		"Success is not a good teacher, failure makes you humble - S. R. Khan";

int main(void)
{

	SerialDebugInit();

	SerialDebugPrint(str1);
	SerialDebugPrint(str3);
	SerialDebugPrint(str4);
	SerialDebugPrint(str2);

	char buffer[64];

	while (1)
	{
		SerialDebugPrint("Please enter a string :");
		SerialDebugGetLine(buffer, 1);
		SerialDebugPrint("Entered string: %s", buffer);
		SerialDebugPrint("Exact content of entered string:");
		int i = 0;
		while (1)
		{
			if (buffer[i] == '\0' || buffer[i] == '\n' || buffer[i] == '\r')
			{
				break;
			}

			SerialDebugPrint(" 0x%x = %d = '%c'", buffer[i], buffer[i], buffer[i]);
			++i;
		}

		_delay_ms(1);
	}

	return 0; /* never reached */
}
