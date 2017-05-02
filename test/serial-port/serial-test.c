/*
 * Demo Serial connection with pc
 */

#include <util/delay.h>

#include "SerialDebug.h"

char str1[] = "Hello World";
char str2[] = "No man has a good enough memory to be a successful liar - A. Lincoln";
char str3[] = "E=mc2";
char str4[] = "Success is not a good teacher, failure makes you humble - S. R. Khan";

int main(void)
{

	SerialDebugInit();

	while (1)
	{               // Loop forever
		SerialDebugPrint(str1);
		SerialDebugPrint(str3);
		SerialDebugPrint(str4);
		SerialDebugPrint(str2);
		_delay_ms(100);
	}

	return 0; /* never reached */
}
