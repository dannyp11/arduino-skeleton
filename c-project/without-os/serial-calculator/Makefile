# to program to arduino, type make flash
CC=avr-gcc
CFLAGS = -Wextra
DEBUG = yes
FLOAT_SUPPORT = no # support printf, sscanf floats
UTILS_SUPPORT = yes # extra utils such as soft serial, i2c, serial debug
HEX_NAME = simple-calculator.hex

# required
TOP = ../../../
SOURCES = $(wildcard *.c)
INCLUDES = ./

# unit test files
# remember to put cxx test headers in test/*Test.h
TEST_SOURCES = InfixConverter.c PostfixCalculator.c $(UTILS_DIR)/AVRString.c
TEST_DEBUG = no
TEST_CFLAGS = -std=gnu++0x

# specify your preferred .mk file
include $(TOP)/common/without-os.mk
include $(TOP)/common/unittest.mk

