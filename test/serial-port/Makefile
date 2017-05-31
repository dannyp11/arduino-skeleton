# to program to arduino, type make flash
CC=avr-gcc
CFLAGS =
TOP = ../../
UTILS_SUPPORT = yes

# required
SOURCES = $(wildcard *.c)
INCLUDES = ./

# unit test files
TEST_SOURCES = $(TOP)/common/utils/AVRString.c

# specify your preferred .mk file
include $(TOP)/common/without-os.mk
include $(TOP)/common/unittest.mk
