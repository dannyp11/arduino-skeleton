/*
 * InfixConverterTest.h
 *
 *  Created on: Jun 2, 2017
 *      Author: dat
 */

#include <limits.h>
#include "gtest/gtest.h"

#include <string>
#include <iostream>
#include "InfixConverter.h"

using namespace std;

extern "C" {
uint8_t addSpaces(char * buffer);
}

namespace
{
class MyTest : public ::testing::Test
{
protected:
   MyTest()
   {

   }

   virtual ~MyTest()
   {

   }

   virtual void SetUp()
   {
      output[0] = '\0';
   }

   virtual void TearDown()
   {

   }

   char output[256];
};
}

TEST_F(MyTest, testConverter2)
{
   EXPECT_EQ(InfixConverterConvertString("1+2+3", output), 0);
   EXPECT_STREQ(output, "1 2 + 3 +");

   EXPECT_EQ(InfixConverterConvertString("1+(2*3)", output), 0);
   EXPECT_STREQ(output, "1 2 3 * +");
}

TEST_F(MyTest, testConverter1)
{
   EXPECT_EQ(InfixConverterConvertString("123", output), 0);
   EXPECT_STREQ(output, "123");

   EXPECT_EQ(InfixConverterConvertString("    65555   ", output), 0);
   EXPECT_STREQ(output, "65555");

   EXPECT_EQ(InfixConverterConvertString("1 + 2", output), 0);
   EXPECT_STREQ(output, "1 2 +");
}

TEST_F(MyTest, testConverter_Success_Prior)
{
   EXPECT_EQ(InfixConverterConvertString("1*2+3", output), 0);
   EXPECT_STREQ(output, "1 2 * 3 +");

   EXPECT_EQ(InfixConverterConvertString("1*2+3*4", output), 0);
   EXPECT_STREQ(output, "1 2 * 3 4 * +");

   EXPECT_EQ(InfixConverterConvertString("1/2+3*4", output), 0);
   EXPECT_STREQ(output, "1 2 / 3 4 * +");

   EXPECT_EQ(InfixConverterConvertString("1*2+3 *4 - 5 / 6", output), 0);
   EXPECT_STREQ(output, "1 2 * 3 4 * + 5 6 / -");
}

TEST_F(MyTest, testConverter_FailFormat)
{
   EXPECT_EQ(InfixConverterConvertString("(1", output), 1);
   EXPECT_EQ(InfixConverterConvertString(")1(", output), 1);
}

TEST_F(MyTest, testConverter_Fail_UnsupportedChar)
{
   EXPECT_EQ(InfixConverterConvertString("1.2", output), 2);
   EXPECT_EQ(InfixConverterConvertString("1^2", output), 2);
}

TEST_F(MyTest, testConverter_ExtraSpace)
{
   char input[] = " A+B";
   char input2[] = "+-*/()";

   EXPECT_EQ(addSpaces(input), 0);
   EXPECT_STREQ(input, "A + B");

   EXPECT_EQ(addSpaces(input2), 0);
   EXPECT_STREQ(input2, "+ - * / ( )");
}

TEST_F(MyTest, testConverter_1parentpair1)
{
   EXPECT_EQ(InfixConverterConvertString("1*(2+3)", output), 0);
   EXPECT_STREQ(output, "1 2 3 + *");

   EXPECT_EQ(InfixConverterConvertString("(1+2)", output), 0);
   EXPECT_STREQ(output, "1 2 +");
}

TEST_F(MyTest, testConverter_Comprehensive)
{
   EXPECT_EQ(InfixConverterConvertString("1* (2+3 * 4) +5", output), 0);
   EXPECT_STREQ(output, "1 2 3 4 * + * 5 +");

   EXPECT_EQ(InfixConverterConvertString("111* (222+333 * 444) +555", output), 0);
   EXPECT_STREQ(output, "111 222 333 444 * + * 555 +");

   EXPECT_EQ(InfixConverterConvertString("1*(2-3/4) +5/6*(7+8)", output), 0);
   EXPECT_STREQ(output, "1 2 3 4 / - * 5 6 / 7 8 + * +");
}












