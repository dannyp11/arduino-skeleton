#ifndef TEST_LSM303DLHC_I2C_H_
#define TEST_LSM303DLHC_I2C_H_

#include <util/delay.h>

//! Initialize I2C (TWI) interface
void i2cInit(void);

/**
 *
 */
void writeRegister(uint8_t address, uint8_t data);

/**
 *
 */
void writeRegisters(uint8_t address, uint8_t regsiter, uint8_t i,
		uint8_t *data);

/**
 *
 */
uint8_t readRegister(uint8_t address, uint8_t regsiter);

/**
 *
 */
void readRegisters(uint8_t address, uint8_t regsiter, uint8_t i, uint8_t * data);

#endif /* _TWI_LIB_H_ */

