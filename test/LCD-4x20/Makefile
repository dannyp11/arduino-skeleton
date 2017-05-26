# to program to arduino, type make flash
CC=avr-gcc
CFLAGS =
TOP = ../../

# required
SOURCES = $(wildcard *.c)
INCLUDES = ./ 

# extra utils such as soft serial, i2c
SOURCES	  += $(wildcard $(TOP)/common/utils/*.c)
INCLUDES  += $(TOP)/common/utils/ 

# specify your preferred .mk file
include ../../common/without-os.mk
