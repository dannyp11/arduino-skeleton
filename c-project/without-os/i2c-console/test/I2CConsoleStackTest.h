/*
 * I2CConsoleStackTest.h
 *
 * This only works with stack size of 5 limit
 *
 *  Created on: May 31, 2017
 *      Author: dat
 */

#ifndef I2C_CONSOLE_TEST_I2CCONSOLESTACKTEST_H_
#define I2C_CONSOLE_TEST_I2CCONSOLESTACKTEST_H_

#include <cxxtest/TestSuite.h>
#include <string>

#include "I2CConsole.h"

using std::string;

class MyTS: public CxxTest::TestSuite
{
private:
public:
	void setUp()
	{
		I2CConsoleStackReInit();
	}

	void testNullStack()
	{
		TS_ASSERT_EQUALS(I2CConsoleStackMoveDown(), nullptr);
		TS_ASSERT_EQUALS(I2CConsoleStackMoveUp(), nullptr);
		TS_ASSERT_EQUALS(I2CConsoleStackMoveUp(), nullptr);
		TS_ASSERT_EQUALS(I2CConsoleStackMoveDown(), nullptr);
	}

	void test_Push1Stack1_Nonnull()
	{
		const char * input1 = "hello world";

		I2CConsoleStackPush(input1);
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input1, strlen(input1));
	}

	void test_Push1Stack2_Nonnull()
	{
		const char * input1 = "hello world";

		I2CConsoleStackPush(input1);
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input1, strlen(input1));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input1, strlen(input1));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input1, strlen(input1));
	}

	void test_Push2Stack1_Nonnull()
	{
		const char * input1 = "hello world 1";
		const char * input2 = "hello world 2";

		I2CConsoleStackPush(input1);
		I2CConsoleStackPush(input2);

		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input2, strlen(input2));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input1, strlen(input1));
	}

	void test_Push2Stack_Down3()
	{
		const char * input1 = "hello world 1";
		const char * input2 = "hello world 2";

		I2CConsoleStackPush(input1);
		I2CConsoleStackPush(input2);

		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input2, strlen(input2));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input1, strlen(input1));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input1, strlen(input1));
	}

	void test_Push6Stack_Down7()
	{
		const char * input1 = "hello world 1";
		const char * input2 = "hello world 2";
		const char * input3 = "hello world 3";
		const char * input4 = "hello world 4";
		const char * input5 = "hello world 5";
		const char * input6 = "hello world 6";

		I2CConsoleStackPush(input1);
		I2CConsoleStackPush(input2);
		I2CConsoleStackPush(input3);
		I2CConsoleStackPush(input4);
		I2CConsoleStackPush(input5);
		I2CConsoleStackPush(input6);

		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input6, strlen(input6));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input5, strlen(input5));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input4, strlen(input4));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input3, strlen(input3));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input2, strlen(input2));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input2, strlen(input2));

	}

	void test_Push6Stack1_Nonnull()
	{
		const char * input1 = "hello world 1";
		const char * input2 = "hello world 2";
		const char * input3 = "hello world 3";
		const char * input4 = "hello world 4";
		const char * input5 = "hello world 5";
		const char * input6 = "hello world 6";

		I2CConsoleStackPush(input1);
		I2CConsoleStackPush(input2);
		I2CConsoleStackPush(input3);
		I2CConsoleStackPush(input4);
		I2CConsoleStackPush(input5);
		I2CConsoleStackPush(input6);

		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input6, strlen(input6));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input5, strlen(input5));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input4, strlen(input4));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input3, strlen(input3));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input2, strlen(input2));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input2, strlen(input2));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input2, strlen(input2));

		I2CConsoleStackPush(input1);

		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input1, strlen(input1));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input6, strlen(input6));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input5, strlen(input5));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input4, strlen(input4));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input3, strlen(input3));

	}

	void test_Push7Stack2_Nonnull()
	{
		const char * input1 = "hello world 1";
		const char * input2 = "hello world 2";
		const char * input3 = "hello world 3";
		const char * input4 = "hello world 4";
		const char * input5 = "hello world 5";
		const char * input6 = "hello world 6";
		const char * input7 = "hello world 7";

		I2CConsoleStackPush(input1);
		I2CConsoleStackPush(input2);
		I2CConsoleStackPush(input3);
		I2CConsoleStackPush(input4);
		I2CConsoleStackPush(input5);
		I2CConsoleStackPush(input6);

		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input6, strlen(input6));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input5, strlen(input5));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input4, strlen(input4));

		I2CConsoleStackPush(input7);

		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input7, strlen(input7));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input6, strlen(input6));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input5, strlen(input5));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input4, strlen(input4));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input3, strlen(input3));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input3, strlen(input3));
	}

	void test_Push1Stack3_Nonnull()
	{
		const char * input1 = "hello world";

		I2CConsoleStackPush(input1);

		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input1, strlen(input1));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveUp(), input1, strlen(input1));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input1, strlen(input1));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input1, strlen(input1));
	}

	void test_1Stack_Moveup1()
	{
		const char * input1 = "hello world 1";
		I2CConsoleStackPush(input1);

		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveUp(), input1, strlen(input1));
	}

	void test_1Stack_Moveup2()
	{
		const char * input1 = "hello world 1";
		I2CConsoleStackPush(input1);

		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveUp(), input1, strlen(input1));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveUp(), input1, strlen(input1));
	}

	void test_2Stack_Moveup1()
	{
		const char * input1 = "hello world 1";
		const char * input2 = "hello world 2";

		I2CConsoleStackPush(input1);
		I2CConsoleStackPush(input2);

		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveUp(), input2, strlen(input2));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveUp(), input2, strlen(input2));
	}

	void test_6stack_Comprehensive()
	{
		const char * input1 = "hello world 1";
		const char * input2 = "hello world 2";
		const char * input3 = "hello world 3";
		const char * input4 = "hello world 4";
		const char * input5 = "hello world 5";
		const char * input6 = "hello world 6";
		const char * input7 = "hello world 7";

		I2CConsoleStackPush(input1);
		I2CConsoleStackPush(input2);
		I2CConsoleStackPush(input3);
		I2CConsoleStackPush(input4);
		I2CConsoleStackPush(input5);
		I2CConsoleStackPush(input6);
		I2CConsoleStackPush(input7);

		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input7, strlen(input7));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input6, strlen(input6));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input5, strlen(input5));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input4, strlen(input4));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input3, strlen(input3));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveDown(), input3, strlen(input3));

		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveUp(), input3, strlen(input3));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveUp(), input4, strlen(input4));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveUp(), input5, strlen(input5));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveUp(), input6, strlen(input6));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveUp(), input7, strlen(input7));
		TS_ASSERT_SAME_DATA(I2CConsoleStackMoveUp(), input7, strlen(input7));
	}
};

#endif /* I2C_CONSOLE_TEST_I2CCONSOLESTACKTEST_H_ */

