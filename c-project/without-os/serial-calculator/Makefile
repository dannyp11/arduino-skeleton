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
# or gtest test .cpp files in test/*gtest.cpp
UNITTEST_SUPPORT = no
GTEST_SUPPORT = yes
TEST_SOURCES = InfixConverter.c PostfixCalculator.c $(UTILS_DIR)/AVRString.c
TEST_DEBUG = no
TEST_CFLAGS = -fno-stack-protector

# always include skeleton.mk
include $(TOP)/common/skeleton.mk

