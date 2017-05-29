# to program to arduino, type make flash
CC=avr-gcc
CFLAGS = 
TOP = ../../
FLOAT_SUPPORT = YES # support printf, sscanf floats

# required
SOURCES = $(wildcard *.c)
INCLUDES = ./

# unit test files
# remember to put cxx test headers in test/*Test.h
TEST_SOURCES = NMEAParser.c

# extra utils such as soft serial, i2c, serial debug
SOURCES	  += $(wildcard $(TOP)/common/utils/*.c)
INCLUDES  += $(TOP)/common/utils/ 

# specify your preferred .mk file
include $(TOP)/common/without-os.mk
include $(TOP)/common/unittest.mk

