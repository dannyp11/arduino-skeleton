# to program to arduino, type make flash
CFLAGS = 
FLOAT_SUPPORT = YES # support printf, sscanf floats
UTILS_SUPPORT = YES # extra utils such as soft serial, i2c, serial debug
PROJECT_NAME = GPS-breakout-board
TOP = ../../../

# required
SOURCES = $(wildcard *.c) $(TOP)/test/LCD-4x20/LCD.c
INCLUDES = ./  $(TOP)/test/LCD-4x20/

# unit test files
# remember to put cxx test headers in test/*Test.h
UNITTEST_SUPPORT = true
TEST_SOURCES = NMEAParser.c

# always include skeleton.mk
include $(TOP)/common/skeleton.mk

