# to program to arduino, type make flash
CFLAGS = -Wextra
DEBUG = no
FLOAT_SUPPORT = no # support printf, sscanf floats
UTILS_SUPPORT = YES # extra utils such as soft serial, i2c, serial debug
PROJECT_NAME = i2c-console
OPTIMIZE = yes
TOP = ../../../

# required
SOURCES = $(wildcard *.c)
INCLUDES = ./

# unit test files
# remember to put cxx test headers in test/*Test.h
UNITTEST_SUPPORT = true
TEST_SOURCES = I2CConsole.c I2CConsoleStack.c
TEST_DEBUG = no
TEST_CFLAGS = -std=gnu++0x

# always include skeleton.mk
include $(TOP)/common/skeleton.mk
