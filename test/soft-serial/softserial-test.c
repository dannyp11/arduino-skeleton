/*
 * Demo Serial connection with pc
 *
 *  TX
 *
 */

#include <util/delay.h>
#include "SoftSerial.h"

const char str1[] = "Hello World";
const char str2[] = "No man has a good enough memory to be a successful liar - A. Lincoln";
const char str3[] = "E=mc2";
const char str4[] = "Success is not a good teacher, failure makes you humble - S. R. Khan";

int main()
{
	SoftSerialInit();

	while (1)
	{
		SoftSerialPrintLn(str1);
		SoftSerialPrintLn(str2);
		SoftSerialPrintLn(str3);
		SoftSerialPrintLn(str4);

		SSLOG("message 1 is %s", str1);
		SSTRACE();
		SSTRACE_INT(1);
		SSTRACE_STRING("hi");

		_delay_ms(1);
	}

	return 0;
}
