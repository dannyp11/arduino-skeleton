#include "SerialDebug.h"
#include "I2C.h"
#include <util/delay.h>

int main(void)
{
	SerialDebugInitWithBaudRate(57600);
	I2CInit();

	uint8_t address;

	for (address = 0x00; address < 128; ++address)
	{
		uint8_t checkResult = I2CCheckAlive(address);
		if (!checkResult)
		{
			SerialDebugPrint("0x%x is alive", address);
		}
#ifdef DEBUG
		else
		{
			LOG("code %x on address %x \n\n", checkResult, address);
			_delay_ms(500);
		}
#endif

	}

	while (1)
	{
	}

	return 0; /* never reached */
}