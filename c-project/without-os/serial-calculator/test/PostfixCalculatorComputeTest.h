/*
 * InfixCalculatorComputeTest.h
 *
 *  Created on: Jun 3, 2017
 *      Author: dat
 */

#ifndef SERIAL_CALCULATOR_TEST_INFIXCALCULATORCOMPUTETEST_H_
#define SERIAL_CALCULATOR_TEST_INFIXCALCULATORCOMPUTETEST_H_

#include <cxxtest/TestSuite.h>
#include <string>
#include <iostream>
#include <climits>

#include "PostfixCalculator.h"

using namespace std;

extern uint8_t addSpaces(char * buffer);

class myTS: public CxxTest::TestSuite
{
private:
	int32_t output;
public:
	void setUp()
	{
		output = INT_MAX;
	}

	void test_noop_success()
	{
		TS_ASSERT_EQUALS(PostfixCalculatorCompute("1"), 1);
		TS_ASSERT_EQUALS(PostfixCalculatorCompute("1123"), 1123);
		TS_ASSERT_EQUALS(PostfixCalculatorCompute("0"), 0);
	}

	void test_1op_success()
	{
		TS_ASSERT_EQUALS(PostfixCalculatorCompute("1 2 +"), 3);
		TS_ASSERT_EQUALS(PostfixCalculatorCompute("1 2 *"), 2);
		TS_ASSERT_EQUALS(PostfixCalculatorCompute("1 2 -"), -1);
		TS_ASSERT_EQUALS(PostfixCalculatorCompute("2 1 /"), 2);
		TS_ASSERT_EQUALS(PostfixCalculatorCompute("1 2 /"), 0);
	}

	void test_comprehensive()
	{
		TS_ASSERT_EQUALS(PostfixCalculatorCompute("1 2 * 3 4 * + 5 6 / -"), 14);
	}

	void test_fail()
	{
		TS_ASSERT_EQUALS(PostfixCalculatorCompute("1 + 2"), INT32_MIN);
		TS_ASSERT_EQUALS(PostfixCalculatorCompute("1 + +"), INT32_MIN);
		TS_ASSERT_EQUALS(PostfixCalculatorCompute("1 2"), INT32_MIN);
		TS_ASSERT_EQUALS(PostfixCalculatorCompute(""), INT32_MIN);
	}
};

#endif /* SERIAL_CALCULATOR_TEST_INFIXCALCULATORCOMPUTETEST_H_ */
