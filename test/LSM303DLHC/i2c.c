#include "i2c.h"
#include <avr/io.h>

/*********************
 ****I2C Functions****
 *********************/

void i2cInit(void)
{
	// set SCL freq = F_CPU/(16+2*TWBR*4^TWPS))=16*10^6/(16+2*12)=400kHz
	TWSR &= ~(_BV(TWPS0) | _BV(TWPS1)); // clear TWPS in TWSR register, setting prescaler(TWPS) = 0
	TWBR = 12;  // set bit rate register (12 -> 400kHz; 32 -> 200kHz)
	TWCR |= _BV(TWEN);  // Enable TWI
}

void i2cSendStart(void)
{
	// send start condition
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
}

void i2cSendStop(void)
{
	// transmit stop condition
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
}

void i2cWaitForComplete(void)
{
	// wait for i2c interface to complete operation, use i for timeout
	uint8_t i = 0;
	while ((!(TWCR & (1 << TWINT))) && (i < 90))
		i++;
}

void i2cSendByte(uint8_t data)
{
	_delay_us(10);
	TWDR = data;  // save data to the TWDR
	TWCR = (1 << TWINT) | (1 << TWEN);  // begin send
}

void i2cReceiveByte(uint8_t ackFlag)
{
	if (ackFlag)
	{
		// ackFlag = 1: ACK the recevied data (would like to receive more data)
		TWCR = (TWCR & TWCR_CMD_MASK) | _BV(TWINT) | _BV(TWEA);
	}
	else
	{
		// ackFlag = 0: NACK the recevied data (want to stop receiving data)
		TWCR = (TWCR & TWCR_CMD_MASK) | _BV(TWINT);
	}
}

unsigned char i2cGetReceivedByte(void)
{
	// retieve received data byte from i2c TWDR
	return (TWDR);
}

unsigned char i2cGetStatus(void)
{
	// retieve current i2c status from i2c TWSR
	return (TWSR);
}

/*********************
 ****I2C High Level Functions****
 *********************/

void writeRegister(uint8_t address, uint8_t data)
{
	i2cSendStart();
	i2cWaitForComplete();
	i2cSendByte((address << 1));
	i2cWaitForComplete();
	i2cSendByte(data);
	i2cWaitForComplete();
	i2cSendStop();
}

void writeRegisters(uint8_t address, uint8_t regsiter, uint8_t i,
		uint8_t *data)
{
	i2cSendStart();
	i2cWaitForComplete();
	i2cSendByte((address << 1));
	i2cWaitForComplete();
	i2cSendByte(regsiter);
	i2cWaitForComplete();

	uint8_t j;
	for (j = 0; j < i; j++)
	{
		i2cSendByte(data[j]);
		i2cWaitForComplete();
	}
	i2cSendStop();
}

uint8_t readRegister(uint8_t address, uint8_t regsiter)
{
	i2cSendStart();
	i2cWaitForComplete();
	i2cSendByte((address << 1));
	i2cWaitForComplete();
	i2cSendByte(regsiter);
	i2cWaitForComplete();

	i2cSendStart();
	i2cWaitForComplete();
	i2cSendByte((address << 1) | 0x01);
	i2cWaitForComplete();
	i2cReceiveByte(0);
	i2cWaitForComplete();
	uint8_t data = i2cGetReceivedByte();
	i2cSendStop();

	return data;
}
