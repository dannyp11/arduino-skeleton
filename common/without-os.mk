TOP		   ?= ${CURDIR}/..

# Arduino specific
DEVICE     = atmega328p
CLOCK      = 16000000
TTY	  ?= /dev/ttyACM0
#PROGRAMMER = -c usbtiny -P usb
PROGRAMMER = -c arduino -P $(TTY) -b 115200
FUSES      = -U hfuse:w:0xd9:m -U lfuse:w:0xe0:m

# OS specific
OS_DIR     = $(TOP)/os/ChibiOS/
###########################################################################
CC ?=avr-gcc
BINARY_NAME ?=main.elf
TRUE = 1 yes YES true TRUE

ifneq ($(filter $(UTILS_SUPPORT), $(TRUE)),) # check if UTILS_SUPPORT is 1|yes|YES
SOURCES	  += $(wildcard $(TOP)/common/utils/*.c)
INCLUDES  += $(TOP)/common/utils/ 
endif

OBJECTS	 = $(patsubst %.cpp, %.o, $(SOURCES))
OBJECTS	 += $(patsubst %.c, %.o, $(SOURCES))
OBJECTS  := $(filter-out %.c, $(OBJECTS))
OBJECTS  := $(filter-out %.cpp, $(OBJECTS))

IFLAGS	 = $(foreach d, $(INCLUDES), -I$d)
CFLAGS  += -Wall -Os 

ifneq ($(filter $(DEBUG), $(TRUE)),) # check if -DDEBUG should be added
CFLAGS  += -DDEBUG=1
endif

ifneq ($(filter $(FLOAT_SUPPORT), $(TRUE)),) # check if FLOAT_SUPPORT is 1|yes|YES
CFLAGS  += -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm # printf scanf float support
endif

# Fuse Low Byte = 0xe0   Fuse High Byte = 0xd9   Fuse Extended Byte = 0xff
# Bit 7: CKDIV8  = 0     Bit 7: RSTDISBL  = 1    Bit 7:
#     6: CKOUT   = 1         6: DWEN      = 1        6:
#     5: SUT1    = 1         5: SPIEN     = 0        5:
#     4: SUT0    = 0         4: WDTON     = 1        4:
#     3: CKSEL3  = 0         3: EESAVE    = 1        3:
#     2: CKSEL2  = 0         2: BOOTSIZ1  = 0        2: BODLEVEL2 = 1
#     1: CKSEL1  = 0         1: BOOTSIZ0  = 0        1: BODLEVEL1 = 1
#     0: CKSEL0  = 0         0: BOOTRST   = 1        0: BODLEVEL0 = 1
# External clock source, start-up time = 14 clks + 65ms
# Don't output clock on PORTB0, don't divide clock by 8,
# Boot reset vector disabled, boot flash size 2048 bytes,
# Preserve EEPROM disabled, watch-dog timer off
# Serial program downloading enabled, debug wire disabled,
# Reset enabled, brown-out detection disabled

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = $(CC) $(CFLAGS) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(IFLAGS)

# symbolic targets:
.default: all

all:	clean
	$(MAKE) main.hex
	
	
.c.o:
	$(COMPILE) -c $< -o $@

.cpp.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

.cpp.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean: 
	rm -f main.hex $(BINARY_NAME) $(OBJECTS)

# file targets:
$(BINARY_NAME): $(OBJECTS)
	$(COMPILE) -o $(BINARY_NAME) $(OBJECTS)

main.hex: $(BINARY_NAME)
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex $(BINARY_NAME) main.hex
	avr-size $(BINARY_NAME)
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	$(BINARY_NAME)
	avr-objdump -d $(BINARY_NAME)

cpp:
	$(COMPILE) -E main.c
