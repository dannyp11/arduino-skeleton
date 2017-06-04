/*
 * AVRStringTest.h
 *
 *  Created on: Jun 2, 2017
 *      Author: dat
 */

#ifndef UTILS_TEST_AVRSTRINGTEST_H_
#define UTILS_TEST_AVRSTRINGTEST_H_

#include <cxxtest/TestSuite.h>
#include <string>

#include "AVRString.h"

class myTS: public CxxTest::TestSuite
{
private:
public:
	void testStrinRmChar()
	{
		char input1[] = "hello world";
		char input1_expected[] = "helloworld";
		char input2[] = "henlnlno wnnnnnnnnnnorld";
		char input2_expected[] = "hello world";

		AVRStringRemoveChar(input1, ' ');
		TS_ASSERT_SAME_DATA(input1, input1_expected, strlen(input1_expected));

		AVRStringRemoveChar(input2, 'n');
		TS_ASSERT_SAME_DATA(input2, input2_expected, strlen(input2_expected));
	}

	void testTrimLeadTrailSpace()
	{
		char input1[] = "        hello  world          ";
		char input1_expected[] = "hello  world";
		char input2[] = "           ";
		char input2_expected[] = "";
		char input3[] = "hi ";
		char input3_expected[] = "hi";

		char * actual_input1 = AVRStringTrimWhiteSpace(input1);
		char * actual_input2 = AVRStringTrimWhiteSpace(input2);
		char * actual_input3 = AVRStringTrimWhiteSpace(input3);

		TS_ASSERT_SAME_DATA(actual_input1, input1_expected, strlen(actual_input1));
		TS_ASSERT_SAME_DATA(actual_input2, input2_expected, strlen(actual_input2));
		TS_ASSERT_SAME_DATA(actual_input3, input3_expected, strlen(actual_input3));
	}

	void testStringRefineSuccess()
	{
		char input1[] = "hello\b\b\b\bi world";
		char input2[] = "Danny\b\b\bt";
		char input3[] = "hi\b\blo";

		AVRStringRefineString(input3);
		TS_ASSERT_SAME_DATA(input3, "lo", strlen(input3));

		AVRStringRefineString(input1);
		TS_ASSERT_SAME_DATA(input1, "hi world", strlen("hi world"));

		AVRStringRefineString(input2);
		TS_ASSERT_SAME_DATA(input2, "Dat", strlen("Dat"));
	}
};

#endif /* UTILS_TEST_AVRSTRINGTEST_H_ */
