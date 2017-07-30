# main mk file to include in every project, include this at the end of your makefile
# type make help to show variable naming help
TRUE = 1 yes YES true TRUE

# top project directory #############################################################
MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIRNAME := $(notdir $(patsubst %/,%,$(dir $(MKFILE_PATH))))
TOP :=$(shell dirname $(MKFILE_PATH))/../

# Arduino specific ##################################################################
DEVICE     = atmega328p
CLOCK      = 16000000L
TTY	  ?= /dev/ttyACM0
PROGRAMMER = -c arduino -P $(TTY) -b 115200

# Directory path ####################################################################
OS_DIR     = $(TOP)/submodules/ChibiOS/
ARDUINO_MAKEFILE_DIR = $(TOP)/submodules/Arduino-Makefile
CXXTEST_DIR = $(TOP)/submodules/cxxtest
GTEST_DIR = $(TOP)/submodules/googletest
UTILS_DIR  = $(TOP)/common/utils/
MKFILES_DIR = $(TOP)/common/makefiles/

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

OPTIMIZE ?= YES
UNITTEST_SUPPORT ?= no
GTEST_SUPPORT ?= no
UTILS_SUPPORT ?= no
FLOAT_SUPPORT ?= no
DEBUG ?= no
IDE_SUPPORT ?= no

ifneq ($(filter $(DEBUG), $(TRUE)),) # check if -DDEBUG should be added
CFLAGS  += -DDEBUG=1
endif

ifneq ($(filter $(UTILS_SUPPORT), $(TRUE)),) # check if UTILS_SUPPORT is 1|yes|YES
INCLUDES  += $(UTILS_DIR) 
CFLAGS += -fno-move-loop-invariants # fix compiler error push_reload, at reload.c:1360, https://gcc.gnu.org/bugzilla/show_bug.cgi?id=71932
endif

ifneq ($(filter $(FLOAT_SUPPORT), $(TRUE)),) # check if FLOAT_SUPPORT is 1|yes|YES
CFLAGS  += -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm # printf scanf float support
OPTIMIZE = no
endif
###########
ifneq ($(filter $(IDE_SUPPORT), $(TRUE)),) # check if arduino ide library is supported, force using c++ if true
ifneq ($(filter $(UTILS_SUPPORT), $(TRUE)),) # check if UTILS_SUPPORT is 1|yes|YES
SOURCES	  += $(wildcard $(UTILS_DIR)/*.c)
endif
CPP_PROJECT = YES
include $(MKFILES_DIR)/ide.mk
else
###########
ifneq ($(filter $(CPP_PROJECT), $(TRUE)),)
CPPFLAGS += -Wno-write-strings -fno-exceptions
endif

ifneq ($(filter $(WITH_OS), $(TRUE)),) # include with-os.mk
ifneq ($(filter $(UTILS_SUPPORT), $(TRUE)),) # check if UTILS_SUPPORT is 1|yes|YES
SOURCES	  += $(wildcard $(UTILS_DIR)/*.c)
endif
include $(MKFILES_DIR)/with-chibios.mk
else
include $(MKFILES_DIR)/without-os.mk
endif

endif
##########

# Unittest config #####################################################################
TEST_SOURCES ?=
TEST_HEADERS ?= $(wildcard test/*Test.h) 
TEST_CC =g++
TEST_AR =ar
TEST_CFLAGS += -g -O0 -Wall -Wextra -DCXXTEST=1 -DGTEST=1
TEST_DEBUG ?=$(DEBUG)

ifneq ($(filter $(UNITTEST_SUPPORT), $(TRUE)),) # cxxtest
include $(MKFILES_DIR)/unittest.mk
endif

ifneq ($(filter $(GTEST_SUPPORT), $(TRUE)),) # gtest
include $(MKFILES_DIR)/gtest.mk
endif

ifneq ($(filter $(TEST_DEBUG), $(TRUE)),) # check if -DDEBUG should be added
TEST_CFLAGS  += -DDEBUG=1 -g -O0
endif

## help menu #########################################################################
GCC_COLOR=\033[1;34m
SRC_COLOR=\033[1;35m
NO_COLOR=\033[0;0m
define helpMenu
--------------------------------------------------------------------------------------
Configurable Variable names (* means required)
Section         Name            Description                     Current value
Device          DEVICE          uController chip                $(DEVICE)
(non-ide)       CLOCK           uController speed in MHz        $(CLOCK)
                TTY             path to tty device (no ide)     $(TTY)   
                PROGRAMMER      type of uController programmer  $(PROGRAMMER)

Project         CPP_PROJECT     is C++ project?                 $(CPP_PROJECT)  
                WITH_OS         uses ChibiOS?                   $(WITH_OS)
                UTILS_SUPPORT   utils such as i2c, debug, etc.  $(UTILS_SUPPORT)
                UTILS_DIR       path to utils source code       $(UTILS_DIR)
                FLOAT_SUPPORT   support float printf/scanf      $(FLOAT_SUPPORT)
                IDE_SUPPORT     support ide lib (setup, loop)?  $(IDE_SUPPORT)
                DEBUG           add -DDEBUG=1 to cflags         $(DEBUG)       
                OPTIMIZE        enable optimization?            $(OPTIMIZE) 
                PROJECT_NAME    name of project                 $(PROJECT_NAME)

Compiler        CFLAGS          add to cflag of avr gcc         $(GCC_COLOR)$(CFLAGS)$(NO_COLOR)
                CPPFLAGS        add to c++ flags of avr g++     $(GCC_COLOR)$(CPPFLAGS)$(NO_COLOR)
                *INCLUDES       list of directories to include  $(SRC_COLOR)$(INCLUDES)$(NO_COLOR)
                
                *SOURCES        all .c and .cpp files           $(SRC_COLOR)$(SOURCES)$(NO_COLOR)

SW Unittest     UNITTEST_SUPPORT cxx test  support              $(UNITTEST_SUPPORT) 
                GTEST_SUPPORT   Google test support             $(GTEST_SUPPORT)
                TEST_SOURCES    files to be tested              $(SRC_COLOR)$(TEST_SOURCES)$(NO_COLOR)
                                (required if UNITTEST_SUPPORT is enabled)
                TEST_HEADERS    list of .h files for cxxtest    $(SRC_COLOR)$(TEST_HEADERS)$(NO_COLOR)
                                usually inside proj/test/
                TEST_DEBUG      add -DDEBUG flag to unittest?   $(TEST_DEBUG)
                TEST_CC         g++ of unit test                $(TEST_CC)
                TEST_CFLAGS     cflags of unit test             $(GCC_COLOR)$(TEST_CFLAGS)$(NO_COLOR)
--------------------------------------------------------------------------------------
endef
export helpMenu
export GCC_COLOR
export NO_COLOR
export SRC_COLOR
help:
	@$(TOP)/project_manager.sh -t 'Skeleton help menu:'
	@echo -e "$$helpMenu"
        
