/*
 * InfixCalculatorComputeTest.h
 *
 *  Created on: Jun 3, 2017
 *      Author: dat
 */

#include "gtest/gtest.h"
#include <limits.h>

#include <string>
#include <iostream>
#include <inttypes.h>

#include "PostfixCalculator.h"

using namespace std;

extern "C" {
uint8_t addSpaces(char * buffer);
}

namespace
{
class MyTest : public ::testing::Test
{
protected:
   MyTest() : output(INT_MAX)
   {

   }

   virtual ~MyTest()
   {

   }

   virtual void SetUp()
   {
      output = INT_MAX;
   }

   virtual void TearDown()
   {

   }

   int32_t output;
};
}

TEST_F(MyTest, testConverter2)
{
}

TEST_F(MyTest, test_noop_success)
{
   EXPECT_EQ(PostfixCalculatorCompute("1"), 1);
   EXPECT_EQ(PostfixCalculatorCompute("1123"), 1123);
   EXPECT_EQ(PostfixCalculatorCompute("0"), 0);
}

TEST_F(MyTest, test_1op_success)
{
   EXPECT_EQ(PostfixCalculatorCompute("1 2 +"), 3);
   EXPECT_EQ(PostfixCalculatorCompute("1 2 *"), 2);
   EXPECT_EQ(PostfixCalculatorCompute("1 2 -"), -1);
   EXPECT_EQ(PostfixCalculatorCompute("2 1 /"), 2);
   EXPECT_EQ(PostfixCalculatorCompute("1 2 /"), 0);
}

TEST_F(MyTest, test_comprehensive)
{
   EXPECT_EQ(PostfixCalculatorCompute("1 2 * 3 4 * + 5 6 / -"), 14);
}

TEST_F(MyTest, test_fail)
{
   EXPECT_EQ(PostfixCalculatorCompute("1 + 2"), INT32_MIN);
   EXPECT_EQ(PostfixCalculatorCompute("1 + +"), INT32_MIN);
   EXPECT_EQ(PostfixCalculatorCompute("1 2"), INT32_MIN);
   EXPECT_EQ(PostfixCalculatorCompute(""), INT32_MIN);
}
