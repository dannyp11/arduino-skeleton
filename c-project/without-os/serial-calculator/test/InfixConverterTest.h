/*
 * InfixConverterTest.h
 *
 *  Created on: Jun 2, 2017
 *      Author: dat
 */

#ifndef SERIAL_CALCULATOR_TEST_INFIXCONVERTERTEST_H_
#define SERIAL_CALCULATOR_TEST_INFIXCONVERTERTEST_H_

#include <cxxtest/TestSuite.h>
#include <string>
#include <iostream>
#include "InfixConverter.h"

using namespace std;

extern "C" {
uint8_t addSpaces(char * buffer);
}

class myTS: public CxxTest::TestSuite
{
private:
	char output[256];
public:
	void setUp()
	{
		output[0] = '\0';
	}

	void testConverter1()
	{
		TS_ASSERT_EQUALS(InfixConverterConvertString("123", output), 0);
		TS_ASSERT_SAME_DATA(output, "123", strlen(output));

		TS_ASSERT_EQUALS(InfixConverterConvertString("    65555   ", output), 0);
		TS_ASSERT_SAME_DATA(output, "65555", strlen(output));

		TS_ASSERT_EQUALS(InfixConverterConvertString("1 + 2", output), 0);
		TS_ASSERT_SAME_DATA(output, "1 2 +", strlen(output));
	}

	void testConverter2()
	{
		TS_ASSERT_EQUALS(InfixConverterConvertString("1+2+3", output), 0);
		TS_ASSERT_SAME_DATA(output, "1 2 + 3 +", strlen(output));

		TS_ASSERT_EQUALS(InfixConverterConvertString("1+(2*3)", output), 0);
		TS_ASSERT_SAME_DATA(output, "1 2 3 * +", strlen(output));
	}

	void testConverter_Success_Prior()
	{
		TS_ASSERT_EQUALS(InfixConverterConvertString("1*2+3", output), 0);
		TS_ASSERT_SAME_DATA(output, "1 2 * 3 +", strlen(output));

		TS_ASSERT_EQUALS(InfixConverterConvertString("1*2+3*4", output), 0);
		TS_ASSERT_SAME_DATA(output, "1 2 * 3 4 * +", strlen(output));

		TS_ASSERT_EQUALS(InfixConverterConvertString("1/2+3*4", output), 0);
		TS_ASSERT_SAME_DATA(output, "1 2 / 3 4 * +", strlen(output));

		TS_ASSERT_EQUALS(
				InfixConverterConvertString("1*2+3 *4 - 5 / 6", output), 0);
		TS_ASSERT_SAME_DATA(output, "1 2 * 3 4 * + 5 6 / -", strlen(output));
	}

	void testConverter_FailFormat()
	{
		TS_ASSERT_EQUALS(InfixConverterConvertString("(1", output), 1);
		TS_ASSERT_EQUALS(InfixConverterConvertString(")1(", output), 1);
	}

	void testConverter_Fail_UnsupportedChar()
	{
		TS_ASSERT_EQUALS(InfixConverterConvertString("1.2", output), 2);
		TS_ASSERT_EQUALS(InfixConverterConvertString("1^2", output), 2);
	}

	void testConverter_ExtraSpace()
	{
		char input[] = " A+B";
		char input2[] = "+-*/()";

		TS_ASSERT_EQUALS(addSpaces(input), 0);
		TS_ASSERT_SAME_DATA(input, "A + B", strlen(input));

		TS_ASSERT_EQUALS(addSpaces(input2), 0);
		TS_ASSERT_SAME_DATA(input2, "+ - * / ( )", strlen(input2));
	}

	void testConverter_1parentpair1()
	{
		TS_ASSERT_EQUALS(InfixConverterConvertString("1*(2+3)", output), 0);
		TS_ASSERT_SAME_DATA(output, "1 2 3 + *", strlen(output));

		TS_ASSERT_EQUALS(InfixConverterConvertString("(1+2)", output), 0);
		TS_ASSERT_SAME_DATA(output, "1 2 +", strlen(output));
	}

	void testConverter_Comprehensive()
	{
		TS_ASSERT_EQUALS(InfixConverterConvertString("1* (2+3 * 4) +5", output),
				0);
		TS_ASSERT_SAME_DATA(output, "1 2 3 4 * + * 5 +", strlen(output));

		TS_ASSERT_EQUALS(InfixConverterConvertString("111* (222+333 * 444) +555", output),
				0);
		TS_ASSERT_SAME_DATA(output, "111 222 333 444 * + * 555 +", strlen(output));

		TS_ASSERT_EQUALS(InfixConverterConvertString("1*(2-3/4) +5/6*(7+8)", output),
				0);
		TS_ASSERT_SAME_DATA(output, "1 2 3 4 / - * 5 6 / 7 8 + * +", strlen(output));
	}
};

#endif /* SERIAL_CALCULATOR_TEST_INFIXCONVERTERTEST_H_ */
