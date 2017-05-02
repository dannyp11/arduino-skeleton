/*
 Blinking led on arduino
 */

#include "Led.h"
#include <util/delay.h>

#define BLINK_DELAY_MS 1000

int main(void)
{
	Led led;

	while (1)
	{
		/* set pin 5 high to turn led on */
		led.turnOn();
		_delay_ms(BLINK_DELAY_MS);

		/* set pin 5 low to turn led off */
		led.turnOff();
		_delay_ms(BLINK_DELAY_MS);
	}
}
