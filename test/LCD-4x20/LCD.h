/*
 * LCD.h
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdio.h>

#define LCD_LINE_LEN 20 // max length of each line

typedef enum _LCDCommand
{
	LCDON, // on off - doesn't affect backlight brightness
	LCDOFF,
	CLEARSCREEN, // this will also move cursor to start
	CURSORHOME, // move cursor to start
	CURSORON,
	CURSOROFF,
	SHOWFIRMWARE,
	NUMBER_OF_COMMANDS
} LCDCommand;

/*
 * Call this function to initialize the LCD
 * This also calls CLEARSCREEN command and set cursor off at home
 */
void LCDInit(void);

/*
 * This will clearscreen, turnoff cursor,
 * move cursor to home, set default brightness & contrast
 */
void LCDReset(void);

/*
 * LCDPrint - write char array to the LCD over the I2C bus
 * input: msg
 * return value: 0 on success
 */
#define LCDPrint(arg...) \
{ 	char _msg[21];\
	snprintf(_msg, 21, ##arg); \
	_LCDPrint(_msg); \
}
uint8_t _LCDPrint(const char* msg);

/*
 * Pretty straightforward: send command in the LCDCommand enum above
 */
uint8_t LCDSendCommand(LCDCommand command);

/*
 * Set brightness of the LCD, level can be 1 to 8, default is 8
 */
uint8_t LCDSetBrightness(char level);

/*
 * Set contrast of the LCD, level can be 1 to 50, default is 40
 */
uint8_t LCDSetContrast(char level);

/*
 * Set position and line number of the cursor
 * line: 1-4
 * position: 0-19
 */
uint8_t LCDSetCursor(char line, char position);

#endif /* LCD_H_ */
