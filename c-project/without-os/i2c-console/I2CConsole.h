/*
 * I2CConsole.h
 *
 *  Created on: May 29, 2017
 *      Author: dat
 */

#ifndef TEST_I2C_CONSOLE_I2CCONSOLE_H_
#define TEST_I2C_CONSOLE_I2CCONSOLE_H_

#include <inttypes.h>

#define I2CMESSAGE_MAXLEN		128

typedef enum _i2ccommand
{
	SEND = 0,
	SENDNRECV,
	SET_ADDRESS,
	SET_SLOW
} I2CConsoleCommand;

typedef struct _i2cMessage
{
	I2CConsoleCommand command;
	uint8_t isValid;
	unsigned isDelayBetweenBytes;
	int tx_len, rx_len;
	unsigned tx[10];
	uint8_t rx[10];
	char message[I2CMESSAGE_MAXLEN];
	unsigned address;
} I2CConsoleMessage;

/**
 * parse one line of input
 *
 * Sample legal messages:
 *
 * ADDR 28 			- set i2c 7-bit address as 0x28
 * TX 2 00 03 		- send 2 bytes 0x00 and 0x03
 * TX "hello world" - send string "hello world"
 * RX 6 2 ab 03 	- send 2 bytes 0xab and 0x03, receive 6 bytes back to rx
 * LOOP 3 TX "hello world" - loop in 3 seconds for sending TX "hello world"
 * LOOP 5 RX 6 2 ab 03 	- loop in 5 seconds for sending RX 6 2 ab 03
 * SLOW 0			 - set slow sending off
 *
 * @param inputmessage
 * @param result
 * @return	- 0 on success
 * 			- 1 on format error
 * 			- 2 on unsupported command
 * 			- 3 on insufficient input, eg. "TX 3 1 2" or "RX 4 3 a b"
 */
uint8_t I2CConsoleParser(const char * message, I2CConsoleMessage * result);

/**
 * send and/or receive data to i2c bus
 *
 * @param command - message to send, will be modifiable
 * @return 	- 0 on success
 * 			- 1 on failed sending/recv
 * 			- 2 on invalid message
 * 			- 3 on invalid address
 */
uint8_t I2CConsoleSendCommand(I2CConsoleMessage * command);

/**
 * dump I2CConsoleMessage info to serial/stdout
 *
 * note: very expensive, set DEBUG1 flag to enable this api
 *
 * @param command
 */
void I2CConsoleDumpCommand(const I2CConsoleMessage * command);

/**
 * get current address of i2c console
 *
 * @return - 7-bit address
 */
uint8_t I2CConsoleGetCurrentAddress();

/**
 * get current setting of slow sending
 *
 * @return - 1 on 0 off
 */
uint8_t I2CConsoleGetSlowSendingStatus();

///////////////////////////////////////////////////////////
/// Command history support
///////////////////////////////////////////////////////////

/**
 * get previous saved message, doesn't actually clear it
 *
 * @return message if exists, NULL if none
 */
const char * I2CConsoleStackMoveUp();

/**
 * get next saved message, doesn't actually clear it
 *
 * @return message if exists, NULL if none
 */
const char * I2CConsoleStackMoveDown();

/**
 * Push to stack message history, this will roll over obsolete messages
 * Limit to 5 history
 *
 * @param message - string to push in
 */
void I2CConsoleStackPush(const char * message);

/**
 * Reset stack current index to start index
 */
void I2CConsoleStackResetIndex();

/**
 * Call this before calling I2CConsoleStack* apis
 */
void I2CConsoleStackInit();

/**
 * Force init stack
 */
void I2CConsoleStackReInit();

/**
 * get stack count
 *
 * @return stack count
 */
uint8_t I2CConsoleStackGetCount();

#endif /* TEST_I2C_CONSOLE_I2CCONSOLE_H_ */
