# main mk file to include in every project, include this at the end of your makefile
# type make help to show variable naming help
TRUE = 1 yes YES true TRUE

# top project directory #############################################################
MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIRNAME := $(notdir $(patsubst %/,%,$(dir $(MKFILE_PATH))))
TOP :=$(shell dirname $(MKFILE_PATH))/../

# Arduino specific ##################################################################
DEVICE     = atmega328p
CLOCK      = 16000000
TTY	  ?= /dev/ttyACM0
#PROGRAMMER = -c usbtiny -P usb
PROGRAMMER = -c arduino -P $(TTY) -b 115200

# Directory path ####################################################################
OS_DIR     = $(TOP)/os/ChibiOS/
UTILS_DIR  = $(TOP)/common/utils/
MKFILES_DIR = $(TOP)/common/makefiles/
AVR_DIR     ?= /usr/share/arduino/

# Project types #####################################################################
CPP_PROJECT ?= no
WITH_OS ?= no # currently support chibios

# Compiler config ##################################################################
PROJECT_NAME ?= main
CC=avr-gcc
CPP=avr-g++
AR ?=avr-ar
BINARY_NAME=$(PROJECT_NAME).elf
HEX_NAME=$(PROJECT_NAME).hex
SOURCES ?= $(wildcard *.c) $(wildcard *.cpp)
INCLUDES ?= ./

UNITTEST_SUPPORT ?= no
UTILS_SUPPORT ?= no
FLOAT_SUPPORT ?= no
DEBUG ?= no
IDE_SUPPORT ?= no

ifneq ($(filter $(IDE_SUPPORT), $(TRUE)),) # check if arduino ide library is supported, force using c++ if true
CPP_PROJECT = yes
WITH_OS = no
INCLUDES += $(AVR_DIR)hardware/arduino/cores/arduino/ $(AVR_DIR)hardware/arduino/variants/standard/
SOURCES += $(wildcard $(AVR_DIR)/hardware/arduino/cores/arduino/*.cpp)
SOURCES += $(wildcard $(AVR_DIR)/hardware/arduino/cores/arduino/*.c)
SOURCES += $(wildcard $(AVR_DIR)/hardware/arduino/cores/arduino/avr-libc/*.c)
endif

ifneq ($(filter $(UTILS_SUPPORT), $(TRUE)),) # check if UTILS_SUPPORT is 1|yes|YES
SOURCES	  += $(wildcard $(UTILS_DIR)/*.c)
INCLUDES  += $(UTILS_DIR) 
endif

ifneq ($(filter $(DEBUG), $(TRUE)),) # check if -DDEBUG should be added
CFLAGS  += -DDEBUG=1
endif

ifneq ($(filter $(FLOAT_SUPPORT), $(TRUE)),) # check if FLOAT_SUPPORT is 1|yes|YES
CFLAGS  += -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm # printf scanf float support
endif

ifneq ($(filter $(CPP_PROJECT), $(TRUE)),)
CPPFLAGS := $(CFLAGS)
CPPFLAGS += -Wno-write-strings -fno-exceptions
endif

ifneq ($(filter $(WITH_OS), $(TRUE)),) # include with-os.mk
include $(MKFILES_DIR)/with-chibios.mk
else
include $(MKFILES_DIR)/without-os.mk
endif

# Unittest config #####################################################################
TEST_SOURCES ?=
TEST_HEADERS ?= $(wildcard test/*Test.h) 
TEST_CC =clang++
TEST_CFLAGS += -g -O0 -Wall -Wextra -DCXXTEST=1

ifneq ($(filter $(UNITTEST_SUPPORT), $(TRUE)),) # check if -DDEBUG should be added
include $(MKFILES_DIR)/unittest.mk
endif

ifneq ($(filter $(TEST_DEBUG), $(TRUE)),) # check if -DDEBUG should be added
TEST_CFLAGS  += -DDEBUG=1
endif

## help menu #########################################################################
define helpMenu
--------------------------------------------------------------------------------------
Configurable Variable names (* means required)
Section         Name            Description                     Current value
Device          DEVICE          uController chip                $(DEVICE)
                CLOCK           uController speed in MHz        $(CLOCK)
                TTY             path to tty device              $(TTY)   
                PROGRAMMER      type of uController programmer  $(PROGRAMMER)

Project         CPP_PROJECT     is C++ project?                 $(CPP_PROJECT)  
                WITH_OS         uses ChibiOS?                   $(WITH_OS)
                UTILS_SUPPORT   utils such as i2c, debug, etc.  $(UTILS_SUPPORT)
                FLOAT_SUPPORT   support float printf/scanf      $(FLOAT_SUPPORT)
                IDE_SUPPORT     support ide lib (setup, loop)?  $(IDE_SUPPORT)
                DEBUG           add -DDEBUG=1 to cflags         $(DEBUG)
                AVR_DIR         path to installed library       $(AVR_DIR)
        
Compiler        PROJECT_NAME    name of project                 $(PROJECT_NAME)
                CFLAGS          add to cflag of avr gcc         $(CFLAGS)
                CPPFLAGS	add to c++ flags of avr g++	$(CPPFLAGS)
                *INCLUDES       list of directories to include  $(INCLUDES)
                *SOURCES        all .c and .cpp files           $(SOURCES)

SW Unittest     UNITTEST_SUPPORT                                $(UNITTEST_SUPPORT) 
                TEST_SOURCES    files to be tested              $(TEST_SOURCES)
                                (required if UNITTEST_SUPPORT is enabled)
                TEST_HEADERS    list of .h files for cxxtest    $(TEST_HEADERS)
                                usually inside proj/test/
                TEST_DEBUG      add -DDEBUG flag to unittest?   $(TEST_DEBUG)
                TEST_CC         g++ of unit test                $(TEST_CC)
                TEST_CFLAGS     cflags of unit test             $(TEST_CFLAGS)
--------------------------------------------------------------------------------------
endef
export helpMenu
help:
	@echo  "$$helpMenu"
        
