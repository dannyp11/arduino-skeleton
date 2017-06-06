# to program to arduino, type make flash
CFLAGS = -Wextra
DEBUG = yes
FLOAT_SUPPORT = no # support printf, sscanf floats
UTILS_SUPPORT = yes # extra utils such as soft serial, i2c, serial debug
PROJECT_NAME = simple-calculator
TOP = ../../../

# required
#SOURCES = $(wildcard *.c)
#INCLUDES = ./

# unit test files
# remember to put cxx test headers in test/*Test.h
UNITTEST_SUPPORT = yes
TEST_SOURCES = InfixConverter.c PostfixCalculator.c $(UTILS_DIR)/AVRString.c
TEST_DEBUG = no
TEST_CFLAGS = -std=gnu++0x

# always include skeleton.mk
include $(TOP)/common/skeleton.mk

