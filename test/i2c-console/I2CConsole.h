/*
 * I2CConsole.h
 *
 *  Created on: May 29, 2017
 *      Author: dat
 */

#ifndef TEST_I2C_CONSOLE_I2CCONSOLE_H_
#define TEST_I2C_CONSOLE_I2CCONSOLE_H_

#include <inttypes.h>

typedef enum _i2ccommand
{
	SEND = 0,
	SENDNRECV,
	SET_ADDRESS
} I2CConsoleCommand;

typedef struct _i2cMessage
{
	I2CConsoleCommand command;
	uint8_t isValid;
	int tx_len, rx_len;
	unsigned tx[10], rx[10];
	unsigned address;
} I2CConsoleMessage;

/**
 * parse one line of input
 *
 * Sample legal messages:
 *
 * "ADDR 28" - set i2c 7-bit address as 0x28
 * "TX 2 00 03" - send 2 bytes 0x00 and 0x03
 * "RX 6 2 ab 03" - send 2 bytes 0xab and 0x03, receive 6 bytes back to rx
 *
 * @param message
 * @param result
 * @return	- 0 on success
 * 			- 1 on format error
 * 			- 2 on unsupported command
 */
uint8_t I2CConsoleParser(const char * message, I2CConsoleMessage * result);

/**
 * send and/or receive data to i2c bus
 *
 * @param command - message to send
 * @return 	- 0 on success
 * 			- 1 on failed sending/recv
 * 			- 2 on invalid message
 */
uint8_t I2CConsoleSendCommand(I2CConsoleMessage * command);

/**
 * dump I2CConsoleMessage info to serial/stdout
 *
 * @param command
 */
void I2CConsoleDumpCommand(const I2CConsoleMessage * command);

#endif /* TEST_I2C_CONSOLE_I2CCONSOLE_H_ */
