MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIRNAME := $(notdir $(patsubst %/,%,$(dir $(MKFILE_PATH))))
TOP :=$(shell dirname $(MKFILE_PATH))/../../
FUSES      = -U hfuse:w:0xd9:m -U lfuse:w:0xe0:m

# arduino core lib
AVR_DIR     ?= /usr/share/arduino/
IDE_SUPPORT ?= no
CORELIB     ?= 
ifneq ($(filter $(IDE_SUPPORT), $(TRUE)),) # check if arduino ide library is supported, force using c++ if true
CORELIB      = libcorearduino.a
INCLUDES ?= $(AVR_DIR)hardware/arduino/cores/arduino/ $(AVR_DIR)hardware/arduino/variants/standard/
ifeq ($(CORELIB_SOURCES),)
CORELIB_SOURCES += $(wildcard $(AVR_DIR)/hardware/arduino/cores/arduino/*.cpp) 
CORELIB_SOURCES += $(wildcard $(AVR_DIR)/hardware/arduino/cores/arduino/*.c) 
CORELIB_SOURCES += $(wildcard $(AVR_DIR)/hardware/arduino/cores/arduino/avr-libc/*.c)
endif		
endif

# default variable values
SOURCES ?= $(wildcard *.c) $(wildcard *.cpp)
INCLUDES ?= ./

# generate object names
OBJECTS	 = $(patsubst %.cpp, %.o, $(SOURCES))
OBJECTS	 += $(patsubst %.c, %.o, $(SOURCES))
OBJECTS  := $(filter-out %.c, $(OBJECTS))
OBJECTS  := $(filter-out %.cpp, $(OBJECTS))

CORELIB_OBJECTS	 = $(patsubst %.cpp, %.libo, $(CORELIB_SOURCES))
CORELIB_OBJECTS	 += $(patsubst %.c, %.libo, $(CORELIB_SOURCES))
CORELIB_OBJECTS  := $(filter-out %.c, $(CORELIB_OBJECTS))
CORELIB_OBJECTS  := $(filter-out %.cpp, $(CORELIB_OBJECTS))

OBJ_DIR  = tmp/
OBJ_TMP  = $(addprefix $(OBJ_DIR)/, $(notdir ${OBJECTS}))
LIBOBJ_TMP  = $(addprefix $(OBJ_DIR)/, $(notdir ${CORELIB_OBJECTS}))

IFLAGS	 = $(foreach d, $(INCLUDES), -I$d)
CFLAGS  += -Wall -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=106040 #arduino specific
CFLAGS  += -fshort-enums -O2 # more optimizing
CPPFLAGS += $(CFLAGS)

ifeq ($(filter $(FLOAT_SUPPORT), $(TRUE)),) # only turn on this if float support disabled
CFLAGS  += -fdata-sections -ffunction-sections -Wl,--gc-sections # garbage collection
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
COMPILE_CPP = $(CPP) $(CPPFLAGS) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(IFLAGS)
LINK_LIB = $(AR) rcs $(CORELIB)

# symbolic targets:
.default: all

all: clean
	@mkdir -p $(OBJ_DIR)
	$(MAKE) $(HEX_NAME)	
	
.c.o:
	$(COMPILE) -c $< -o $(OBJ_DIR)/$(notdir $@)

.cpp.o:
	$(COMPILE_CPP) -c $< -o $(OBJ_DIR)/$(notdir $@)
	
%.libo: %.c
	$(COMPILE) -c $< -o $(OBJ_DIR)/$(notdir $@)

%.libo: %.cpp
	$(COMPILE_CPP) -c $< -o $(OBJ_DIR)/$(notdir $@)

flash:	all
	$(AVRDUDE) -U flash:w:$(HEX_NAME):i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID $(HEX_NAME)

clean: 
	rm -f $(HEX_NAME) $(BINARY_NAME)
	rm -rf $(OBJ_DIR)

# file targets:
$(BINARY_NAME): $(OBJECTS) $(CORELIB)
	$(COMPILE) -o $(BINARY_NAME) $(OBJ_TMP) $(CORELIB)

$(HEX_NAME): $(BINARY_NAME)
	rm -f $(HEX_NAME)
	avr-objcopy -j .text -j .data -O ihex $(BINARY_NAME) $(HEX_NAME)
	avr-size -C $(BINARY_NAME) --mcu=$(DEVICE)
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

ifeq ($(shell ls $(CORELIB)),)
$(CORELIB): 
	@mkdir -p $(OBJ_DIR)
	$(MAKE) $(CORELIB_OBJECTS)
	$(foreach obj,$(CORELIB_OBJECTS), $(AR) rcs $(CORELIB) $(OBJ_DIR)/$(notdir $(obj)) ;) 
else
$(CORELIB):
	
endif

# Targets for code debugging and analysis:
disasm:	$(BINARY_NAME)
	avr-objdump -d $(BINARY_NAME)
