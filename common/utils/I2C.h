/*
 * I2C.h
 *
 *  Created on: May 25, 2017
 *      Author: dat
 */

#ifndef UTILS_I2C_H_
#define UTILS_I2C_H_

#include <inttypes.h>

/**
 *
 */
void I2CInit(void);

/**
 *
 */
uint8_t I2CSendData(uint8_t address, const uint8_t * data, uint8_t datalen, uint8_t isSlowTX);

/**
 *
 */
uint8_t I2CSendnRecvData(uint8_t address, const uint8_t * txdata, uint8_t txdatalen,
		uint8_t * rxdata, uint8_t rxdatalen, uint8_t isSlowTX);

/**
 * Check if address responds on I2C
 *
 * @param address - 7bit address
 * @return 0 on alive
 * 			1 on NAK recv
 * 			2 on START not ok
 * 			3 on address not sent ok
 * 			4 on timeout
 * 			5 on unknown error
 */
uint8_t I2CCheckAlive(uint8_t address);

#endif /* UTILS_I2C_H_ */
