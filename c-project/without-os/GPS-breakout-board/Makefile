# to program to arduino, type make flash
CC=avr-gcc
CFLAGS = 
FLOAT_SUPPORT = YES # support printf, sscanf floats
UTILS_SUPPORT = YES # extra utils such as soft serial, i2c, serial debug

# required
TOP = ../../../
SOURCES = $(wildcard *.c) $(TOP)/test/LCD-4x20/LCD.c
INCLUDES = ./  $(TOP)/test/LCD-4x20/

# unit test files
# remember to put cxx test headers in test/*Test.h
TEST_SOURCES = NMEAParser.c

# specify your preferred .mk file
include $(TOP)/common/without-os.mk
include $(TOP)/common/unittest.mk

