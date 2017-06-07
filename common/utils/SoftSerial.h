/*
 * SoftSerial.h
 *
 *  Created on: Jun 6, 2017
 *      Author: dat
 */

#ifndef UTILS_SOFTSERIAL_H_
#define UTILS_SOFTSERIAL_H_

// TX pin config
#define SS_TX_PIN		PB1
#define SS_PORT_TX		PORTB
#define SS_DDR_PORT_TX	DDRB

// RX pin config
#define SS_RX_PIN			PB2
#define SS_PORT_RX			PORTB
#define SS_DDR_PORT_RX		DDRB
#define SS_PINB_RX			PINB

#define SS_BAUD		9600 // baud rate
// 8 bits data, 1 odd parity, 1 stop bit

/**
 * Call this before running any below api
 */
void SoftSerialInit();

/**
 * Send char over soft serial
 *
 * @param c
 */
void SoftSerialPutChar(char c);

/**
 * Get 1 char from soft serial rx port
 *
 * @return character
 */
char SoftSerialGetChar();

#endif /* UTILS_SOFTSERIAL_H_ */
