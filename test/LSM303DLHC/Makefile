# to program to arduino, type make flash
CC=avr-gcc
CFLAGS =  -DNOCHIBI=1

# required
SOURCES = $(wildcard *.c) ../serial-port/SerialDebug.c
INCLUDES = ./ ../serial-port/

# specify your preferred .mk file
include ../../common/without-os.mk
