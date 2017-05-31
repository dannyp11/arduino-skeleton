# This is i2c console app for arduino, this tool makes i2c debug easier

# Setup
-- connect SCL SDA pins to your I2C device
-- connect serial port with arduino to activate the console

# Usage
* Type help to open help menu
* 
* Sample legal messages:
*
* ADDR 28                       - set i2c 7-bit address as 0x28
* TX 2 00 03                    - send 2 bytes 0x00 and 0x03
* TX "hello world"              - send string "hello world"
* RX 6 2 ab 03                  - send 2 bytes 0xab and 0x03, receive 6 bytes back to rx
* LOOP 3 TX "hello world"       - loop in 3 seconds for sending TX "hello world"
* LOOP 5 RX 6 2 ab 03           - loop in 5 seconds for sending RX 6 2 ab 03
* SLOW 0                        - set slow sending off
