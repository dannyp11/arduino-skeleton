TOP		   = ../

# Arduino specific
DEVICE     = atmega328p
CLOCK      = 16000000
#PROGRAMMER = -c usbtiny -P usb
PROGRAMMER = -c arduino -P /dev/ttyACM0 -b 115200
FUSES      = -U hfuse:w:0xd9:m -U lfuse:w:0xe0:m

# OS specific
OS_DIR     = $(TOP)/os/ChibiOS/
