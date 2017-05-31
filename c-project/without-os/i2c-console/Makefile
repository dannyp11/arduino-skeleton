# to program to arduino, type make flash
CC=avr-gcc
CFLAGS = -Wextra
DEBUG = no
FLOAT_SUPPORT = no # support printf, sscanf floats
UTILS_SUPPORT = YES # extra utils such as soft serial, i2c, serial debug

# required
TOP = ../../../
SOURCES = $(wildcard *.c)
INCLUDES = ./

# unit test files
# remember to put cxx test headers in test/*Test.h
TEST_SOURCES = I2CConsole.c
TEST_DEBUG = no

# specify your preferred .mk file
include $(TOP)/common/without-os.mk
include $(TOP)/common/unittest.mk

