/*
 * gy85-test.c
 *
 *  Created on: May 25, 2017
 *      Author: dat
 */

#include "SerialDebug.h"
#include "GY85.h"
#include <util/delay.h>

int main()
{
	SerialDebugInit();

	if (GY85Init())
	{
		SerialDebugPrint("Error initing GY85 module");
	}


	while (1)
	{
		GY85AccelRawData accelData;
		GY85GyroRawData gyroData;
		GY85CompassRawData compassData;

		uint8_t compassRead = GY85CompassGetRawData(&compassData);
		uint8_t accelRead = GY85AccelGetRawData(&accelData);
		uint8_t gyroRead = GY85GyroGetRawData(&gyroData);

		if (compassRead)
		{
			SerialDebugPrint("Error code %d reading compass data", compassRead);
		}
		else
		{
			SerialDebugPrint("Compass\t x = %d \t y = %d \t z = %d",
					compassData.x, compassData.y, compassData.z);
		}

		if (gyroRead)
		{
			SerialDebugPrint("Error code %d reading gyroscope data", gyroRead);
		}
		else
		{
			SerialDebugPrint("Gyro \t x = %d \t y = %d \t z = %d \t temp = %d",
					gyroData.x, gyroData.y, gyroData.z, gyroData.temperature_Celcius);
		}

		if (accelRead)
		{
			SerialDebugPrint("Error code %d reading accelerometer data", accelRead);
		}
		else
		{
			SerialDebugPrint("Accel \t x = %d \t y = %d \t z = %d",
					accelData.x, accelData.y, accelData.z);
		}

		SerialDebugPrint("---------------------------------------------------");
		_delay_ms(100);
	}

	return 0;
}
