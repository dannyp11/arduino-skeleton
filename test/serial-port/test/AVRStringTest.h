/*
 * AVRStringTest.h
 *
 *  Created on: May 31, 2017
 *      Author: dat
 */

#ifndef TEST_SERIAL_PORT_TEST_AVRSTRINGTEST_H_
#define TEST_SERIAL_PORT_TEST_AVRSTRINGTEST_H_

#include <cxxtest/TestSuite.h>
#include <string>

#include "AVRString.h"

using std::string;

class avrTest: public CxxTest::TestSuite
{
private:
public:
	void testStringRefineSuccess()
	{
		char input1[] = "hello\b\b\b\bi world";
		char input2[] = "Danny\b\b\bt";
		char input3[] = "hi\bo";

		AVRStringRefineString(input3);
		TS_ASSERT_SAME_DATA(input3, "ho", strlen(input3));

		AVRStringRefineString(input1);
		TS_ASSERT_SAME_DATA(input1, "hi world", strlen("hi world"));

		AVRStringRefineString(input2);
		TS_ASSERT_SAME_DATA(input2, "Dat", strlen("Dat"));
	}

};

#endif /* TEST_SERIAL_PORT_TEST_AVRSTRINGTEST_H_ */
