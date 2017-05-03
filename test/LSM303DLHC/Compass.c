#include <avr/io.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include <util/twi.h>

#include "Compass.h"
#include "SerialDebug.h"
#include "i2c.h"

// Find divisors for the UART0 and I2C baud rates
#ifndef FOSC
#define FOSC F_CPU            // Clock frequency = Oscillator freq.
#endif
#define BDIV (FOSC / 100000 - 16) / 2 + 1    // Puts I2C rate just below 100kHz

void Compassi2c_init(uint8_t bdiv);
uint8_t Compassi2c_io(uint8_t device_addr, uint8_t *ap, uint16_t an,
		uint8_t *wp, uint16_t wn, uint8_t *rp, uint16_t rn);

#define COMPASS_ADDR  0x3d

/*
 * Calibrated values
 */
#define	CALIBRATED_OFFSET -60.0
#define TRUE_S	180.0
#define TRUE_W	270.0
#define TRUE_E	90.0
#define TRUE_NW	315.0
#define TRUE_NE	45.0
#define TRUE_SW	225.0
#define TRUE_SE	135.0
#define TOLERANCE	22.0

/*
 * Private Variables
 */
static int16_t x, y, z;
static float mHeader;
static uint8_t result[6];
static uint8_t cmd[2];

uint8_t CompassGetDirectionText(char * buffer, CompassDirection direction)
{
	uint8_t retVal = 0;

	switch (direction)
	{
	case NORTH:
	{
		snprintf(buffer, 6, "North");
		retVal = 5;
	}
		break;
	case EAST:
	{
		snprintf(buffer, 5, "East");
		retVal = 4;
	}
		break;
	case SOUTH:
	{
		snprintf(buffer, 6, "South");
		retVal = 5;
	}
		break;
	case WEST:
	{
		snprintf(buffer, 5, "West");
		retVal = 4;
	}
		break;
	case NORTHEAST:
	{
		snprintf(buffer, 10, "NorthEast");
		retVal = 9;
	}
		break;
	case NORTHWEST:
	{
		snprintf(buffer, 10, "NorthWest");
		retVal = 9;
	}
		break;
	case SOUTHEAST:
	{
		snprintf(buffer, 10, "SouthEast");
		retVal = 9;
	}
		break;
	case SOUTHWEST:
	{
		snprintf(buffer, 10, "SouthWest");
		retVal = 9;
	}
		break;
	default:
		break;
	}

	return retVal;
}

float CompassGetAngle()
{
	cmd[0] = 0x03;
	Compassi2c_io(COMPASS_ADDR, cmd, 1, 0, 0, result, 6);

	x = (int16_t) (((int16_t) result[0] << 8) | result[1]);
	z = (int16_t) (((int16_t) result[2] << 8) | result[3]);
	y = (int16_t) (((int16_t) result[4] << 8) | result[5]);

	mHeader = atan2(x, y) * 180.0 / M_PI + CALIBRATED_OFFSET;
	if (mHeader < 0)
		mHeader += 360.0;

	return mHeader;
}

CompassDirection CompassGetDirection()
{
	CompassDirection retVal = NORTH;

	CompassGetAngle();

	if (abs(mHeader - TRUE_S) < TOLERANCE)
		retVal = SOUTH;
	else if (abs(mHeader - TRUE_W) < TOLERANCE)
		retVal = WEST;
	else if (abs(mHeader - TRUE_E) < TOLERANCE)
		retVal = EAST;
	else if (abs(mHeader - TRUE_SW) < TOLERANCE)
		retVal = SOUTHWEST;
	else if (abs(mHeader - TRUE_SE) < TOLERANCE)
		retVal = SOUTHEAST;
	else if (abs(mHeader - TRUE_NE) < TOLERANCE)
		retVal = NORTHEAST;
	else if (abs(mHeader - TRUE_NW) < TOLERANCE)
		retVal = NORTHWEST;

	return retVal;
}

void CompassInit()
{
	/*
	 * Compass stuff
	 */
//	Compassi2c_init(BDIV);
//
//	cmd[0] = 0x02;
//	cmd[1] = 0x00;
//
//	Compassi2c_io(COMPASS_ADDR, cmd, 2, 0, 0, 0, 0);
//
////	cmd[0] = 0x00;
////	cmd[1] = 0x90;
////	Compassi2c_io(COMPASS_ADDR, cmd, 2, 0, 0, 0, 0);
//
////	uint8_t res[3];
////	res[0] = 10;
//	result[0] = 10;
//	cmd[0] = 0x02;
//	Compassi2c_io(COMPASS_ADDR, cmd, 1, 0, 0, result, 1);
//	SerialDebugPrint("result %d", result[0]);
//
//	while (1)
//	{
//		cmd[0] = 0x03;
//		Compassi2c_io(COMPASS_ADDR, cmd, 1, 0, 0, result, 6);
//		SerialDebugPrint("result %d %d ", result[0], result[1]);
//
//		_delay_ms(500);
//	}

	cmd[0] = 1;
	i2cInit();
	writeRegisters(0x19, 0x02, 1, cmd);

	while(1)
	{
		SerialDebugPrint("reg %d", readRegister(0x19, 0x02));
		_delay_ms(500);
	}
}

/*
 i2c_io - write and read bytes to a slave I2C device

 This funtions write "an" bytes from array "ap" and then "wn" bytes from array
 "wp" to I2C device at address "device_addr".  It then reads "rn" bytes from
 the same device to array "rp".

 Return values (might not be a complete list):
 0    - Success
 0x20 - NAK received after sending device address for writing
 0x30 - NAK received after sending data
 0x38 - Arbitration lost with address or data
 0x48 - NAK received after sending device address for reading

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 This "i2c_io" I2C routine is an attempt to provide an I/O function for both
 reading and writing, rather than have separate functions.

 I2C writes consist of sending a stream of bytes to the slave device.  In some
 cases the first few bytes may be the internal address in the device, and then
 the data to be stored follows.  For example, EEPROMs like the 24LC256 require a
 two-byte address to precede the data.  The DS1307 RTC requires a one-byte
 address.

 I2C reads often consist of first writing one or two bytes of internal address
 data to the device and then reading back a stream of bytes starting from that
 address.  Some devices appear to claim that that reads can be done without
 first doing the address writes, but so far I haven't been able to get any to
 work that way.

 This function does writing and reading by using pointers to three arrays "ap",
 "wp", and "rp".  The function performs the following actions in this order:
 If "an" is greater than zero, then "an" bytes are written from array "ap"
 If "wn" is greater then zero, then "wn" bytes are written from array "wp"
 If "rn" is greater then zero, then "rn" byte are read into array "rp"
 Any of the "an", "wn", or "rn" can be zero.

 The reason for separate "ap" and "wp" arrays is that the address data can be
 taken from one array (ap), and then the write data from another (wp) without
 requiring that the contents be merged into one array before calling the
 function.  This means the following three calls all do exactly the same thing.

 i2c_io(0xA0, buf, 100, NULL, 0, NULL, 0);
 i2c_io(0xA0, NULL, 0, buf, 100, NULL, 0);
 12c_io(0xA0, buf, 2, buf+2, 98, NULL, 0);

 In all cases 100 bytes from array "buf" will be written to the I2C device at
 bus address 0xA0.

 A typical write with a 2-byte address is done with

 i2c_io(0xA0, abuf, 2, wbuf, 50, NULL, 0);

 A typical read with a 1-byte address is done with

 i2c_io(0xD0, abuf, 1, NULL, 0, rbuf, 20);
 */

uint8_t Compassi2c_io(uint8_t device_addr, uint8_t *ap, uint16_t an,
		uint8_t *wp, uint16_t wn, uint8_t *rp, uint16_t rn)
{
	uint8_t status, send_stop;
	uint8_t wrote, start_stat;

	status = 0;
	wrote = 0;
	send_stop = 0;

	if (an > 0 || wn > 0)
	{
		wrote = 1;
		send_stop = 1;

		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA); // Send start condition
		while (!(TWCR & (1 << TWINT)))
			;     // Wait for TWINT to be set
		status = TWSR & 0xf8;
		if (status != 0x08)                 // Check that START was sent OK
			return (status);

		TWDR = device_addr & 0xfe;          // Load device address and R/W = 0;
		TWCR = (1 << TWINT) | (1 << TWEN);  // Start transmission
		while (!(TWCR & (1 << TWINT)))
			;     // Wait for TWINT to be set
		status = TWSR & 0xf8;
		if (status != 0x18)
		{               // Check that SLA+W was sent OK
			if (status == 0x20)             // Check for NAK
				goto nakstop;
			// Send STOP condition
			return (status);                 // Otherwise just return the status
		}

		// Write "an" data bytes to the slave device
		while (an-- > 0)
		{
			TWDR = *ap++;                   // Put next data byte in TWDR
			TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
			while (!(TWCR & (1 << TWINT)))
				; // Wait for TWINT to be set
			status = TWSR & 0xf8;
			if (status != 0x28)
			{           // Check that data was sent OK
				if (status == 0x30)         // Check for NAK
					goto nakstop;
				// Send STOP condition
				return (status);             // Otherwise just return the status
			}

			_delay_ms(1);
		}

		// Write "wn" data bytes to the slave device
		while (wn-- > 0)
		{
			TWDR = *wp++;                   // Put next data byte in TWDR
			TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
			while (!(TWCR & (1 << TWINT)))
				; // Wait for TWINT to be set
			status = TWSR & 0xf8;
			if (status != 0x28)
			{           // Check that data was sent OK
				if (status == 0x30)         // Check for NAK
					goto nakstop;
				// Send STOP condition
				return (status);             // Otherwise just return the status
			}

			_delay_ms(1);
		}

		status = 0;                         // Set status value to successful
	}

	if (rn > 0)
	{
		send_stop = 1;

		// Set the status value to check for depending on whether this is a
		// START or repeated START
		start_stat = (wrote) ? 0x10 : 0x08;

		// Put TWI into Master Receive mode by sending a START, which could
		// be a repeated START condition if we just finished writing.
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
		// Send start (or repeated start) condition
		while (!(TWCR & (1 << TWINT)))
			;     // Wait for TWINT to be set
		status = TWSR & 0xf8;
		if (status != start_stat)  // Check that START or repeated START sent OK
			return (status);

		TWDR = device_addr | 0x01;         // Load device address and R/W = 1;
		TWCR = (1 << TWINT) | (1 << TWEN);  // Send address+r
		while (!(TWCR & (1 << TWINT)))
			;     // Wait for TWINT to be set
		status = TWSR & 0xf8;
		if (status != 0x40)
		{               // Check that SLA+R was sent OK
			if (status == 0x48)             // Check for NAK
				goto nakstop;
			return (status);
		}

		// Read all but the last of n bytes from the slave device in this loop
		rn--;
		while (rn-- > 0)
		{
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // Read byte and send ACK
			while (!(TWCR & (1 << TWINT)))
				; // Wait for TWINT to be set
			status = TWSR & 0xf8;
			if (status != 0x50)             // Check that data received OK
				return (status);
			*rp++ = TWDR;                   // Read the data

			_delay_ms(1);
		}

		// Read the last byte
		TWCR = (1 << TWINT) | (1 << TWEN);  // Read last byte with NOT ACK sent
		while (!(TWCR & (1 << TWINT)))
			;     // Wait for TWINT to be set
		status = TWSR & 0xf8;
		if (status != 0x58)                 // Check that data received OK
			return (status);
		*rp++ = TWDR;                       // Read the data

		status = 0;                         // Set status value to successful
	}

	nakstop:                              // Come here to send STOP after a NAK
	if (send_stop)
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Send STOP condition

	return (status);
}

/*
 i2c_init - Initialize the I2C port
 */
void Compassi2c_init(uint8_t bdiv)
{
	TWSR = 0;                           // Set prescalar for 1
	TWBR = bdiv;                        // Set bit rate register
}
