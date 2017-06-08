# top project directory #############################################################
MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIRNAME := $(notdir $(patsubst %/,%,$(dir $(MKFILE_PATH))))
TOP :=$(shell dirname $(MKFILE_PATH))/../../

# internal var ######################################################################
INCLUDES ?= ./
IFLAGS       = $(foreach d, $(INCLUDES), -I$d)

# Arduino.mk vars define ############################################################
LOCAL_C_SRCS    = $(filter %.c, $(SOURCES))
LOCAL_CPP_SRCS  = $(filter %.cpp, $(SOURCES))
BOARD_TAG    = uno
CFLAGS  += $(IFLAGS)
TARGET = $(PROJECT_NAME)

include $(TOP)/submodules/Arduino-Makefile/Arduino.mk

# Make skeleton command compatible with Arduino.mk
flash: upload
