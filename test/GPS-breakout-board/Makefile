# to program to arduino, type make flash
CC=avr-gcc
CFLAGS =
TOP = ../../

# required
SOURCES = $(wildcard *.c)
INCLUDES = ./

#unit test files
TEST_HEADERS = $(wildcard test/*.h) 
TEST_SOURCES = NMEAParser.c

# specify your preferred .mk file
include $(TOP)/common/without-os.mk
include $(TOP)/common/unittest.mk

