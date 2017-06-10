/*
 * I2CConsoleTest.h
 *
 *  Created on: May 29, 2017
 *      Author: dat
 */

#ifndef TEST_I2C_CONSOLE_TEST_I2CCONSOLETEST_H_
#define TEST_I2C_CONSOLE_TEST_I2CCONSOLETEST_H_

#include <cxxtest/TestSuite.h>
#include <string>

#include "I2CConsole.h"

using std::string;

class MyTS: public CxxTest::TestSuite
{
private:
	void resetMessage(I2CConsoleMessage * message)
	{
		message->isValid = 0;
		message->tx_len = 0;
		message->rx_len = 0;
	}

public:
	void testConsoleParserSuccess()
	{
		const string input1 = "ADDR 28";
		const string input2 = "TX 2 fe 41";
		const string input3 = "RX 6 2 ab 03";
		const string input4 = "TX 10 1 2 3 4 5 6 7 8 9 10";
		const string input5 = "slow 0";

		I2CConsoleMessage message;
		resetMessage(&message);

		TS_ASSERT_EQUALS(I2CConsoleParser(input1.c_str(), &message), 0);
		TS_ASSERT_EQUALS(message.address, 0x28);
		TS_ASSERT_EQUALS(message.command, SET_ADDRESS);
		TS_ASSERT_EQUALS(message.isValid, 1);
		I2CConsoleDumpCommand(&message);

		resetMessage(&message);
		TS_ASSERT_EQUALS(I2CConsoleParser(input2.c_str(), &message), 0);
		TS_ASSERT_EQUALS(message.command, SEND);
		TS_ASSERT_EQUALS(message.address, 0x28);
		TS_ASSERT_EQUALS(message.isValid, 1);
		TS_ASSERT_EQUALS(message.tx_len, 2);
		TS_ASSERT_EQUALS(message.tx[0], 0xfe);
		TS_ASSERT_EQUALS(message.tx[1], 0x41);
		I2CConsoleDumpCommand(&message);

		resetMessage(&message);
		TS_ASSERT_EQUALS(I2CConsoleParser(input3.c_str(), &message), 0);
		TS_ASSERT_EQUALS(message.command, SENDNRECV);
		TS_ASSERT_EQUALS(message.address, 0x28);
		TS_ASSERT_EQUALS(message.isValid, 1);
		TS_ASSERT_EQUALS(message.tx_len, 2);
		TS_ASSERT_EQUALS(message.rx_len, 6);
		TS_ASSERT_EQUALS(message.tx[0], 0xab);
		TS_ASSERT_EQUALS(message.tx[1], 0x03);
		I2CConsoleDumpCommand(&message);

		resetMessage(&message);
		TS_ASSERT_EQUALS(I2CConsoleParser(input4.c_str(), &message), 0);
		TS_ASSERT_EQUALS(message.command, SEND);
		TS_ASSERT_EQUALS(message.address, 0x28);
		TS_ASSERT_EQUALS(message.isValid, 1);
		TS_ASSERT_EQUALS(message.tx_len, 10);
		I2CConsoleDumpCommand(&message);
//
		resetMessage(&message);
		TS_ASSERT_EQUALS(I2CConsoleParser(input5.c_str(), &message), 0);
		TS_ASSERT_EQUALS(message.command, SET_SLOW);
		TS_ASSERT_EQUALS(message.isValid, 1);
		TS_ASSERT_EQUALS(message.isDelayBetweenBytes, 0);
		I2CConsoleDumpCommand(&message);
	}

	void testConsoleParserMessageSuccess()
	{
		I2CConsoleMessage message;

		TS_ASSERT_EQUALS(I2CConsoleParser("tx \"hello world\"", &message), 0);
		TS_ASSERT_EQUALS(message.isValid, 1);
		TS_ASSERT_EQUALS(strcmp(message.message, "hello world"), 0);

		TS_ASSERT_EQUALS(I2CConsoleParser("tx \"console test here\"", &message),
				0);
		TS_ASSERT_EQUALS(message.isValid, 1);
		TS_ASSERT_EQUALS(strcmp(message.message, "console test here"), 0);

		TS_ASSERT_EQUALS(I2CConsoleParser("rx 6 \"rx test here\"", &message), 0);
		TS_ASSERT_EQUALS(message.isValid, 1);
		TS_ASSERT_EQUALS(strcmp(message.message, "rx test here"), 0);
	}

	void testConsoleParserFail()
	{
		const string input1 = "ADDR ";
		const string input2 = "hello 2 2";
		const string input3 = "tx 3 10 ef";
		const string input4 = "tx \" 10 ef";
		const string input5 = "rx 3 10 ef ab";
		const string input6 = "rx 3 \" ef ab";
		I2CConsoleMessage message;

		TS_ASSERT_EQUALS(I2CConsoleParser(input1.c_str(), &message), 1);
		TS_ASSERT_EQUALS(message.isValid, 0);

		TS_ASSERT_EQUALS(I2CConsoleParser(input2.c_str(), &message), 2);
		TS_ASSERT_EQUALS(message.isValid, 0);

		TS_ASSERT_EQUALS(I2CConsoleParser("", &message), 2);
		TS_ASSERT_EQUALS(message.isValid, 0);

		TS_ASSERT_EQUALS(I2CConsoleParser(input3.c_str(), &message), 3);
		TS_ASSERT_EQUALS(message.isValid, 0);

		TS_ASSERT_EQUALS(I2CConsoleParser(input4.c_str(), &message), 1);
		TS_ASSERT_EQUALS(message.isValid, 0);

		TS_ASSERT_EQUALS(I2CConsoleParser(input5.c_str(), &message), 3);
		TS_ASSERT_EQUALS(message.isValid, 0);

		TS_ASSERT_EQUALS(I2CConsoleParser(input6.c_str(), &message), 1);
		TS_ASSERT_EQUALS(message.isValid, 0);
	}
};

#endif /* TEST_I2C_CONSOLE_TEST_I2CCONSOLETEST_H_ */
