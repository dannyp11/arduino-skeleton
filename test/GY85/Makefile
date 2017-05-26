# to program to arduino, type make flash
CC=avr-gcc
CFLAGS = -Wl,-u,vfprintf -lprintf_flt -lm
TOP = ../../

# required
SOURCES = $(wildcard *.c) ../LCD-4x20/LCD.c
INCLUDES = ./ ../LCD-4x20/

# extra utils such as soft serial, i2c, serial debug
SOURCES	  += $(wildcard $(TOP)/common/utils/*.c)
INCLUDES  += $(TOP)/common/utils/ 

# specify your preferred .mk file
include ../../common/without-os.mk
