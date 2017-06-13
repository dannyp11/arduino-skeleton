# TEST_SOURCES: source files to be tested
# TEST_HEADERS: header file for test cases in gtest framework, must be test/*gtest.h

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIRNAME := $(notdir $(patsubst %/,%,$(dir $(MKFILE_PATH))))
TOP :=$(shell dirname $(MKFILE_PATH))/../../

# gtest library definition
GTEST_DIR = $(TOP)/submodules/googletest/googletest
GTEST_SRC = $(wildcard $(GTEST_DIR)/src/*.cc)
GTEST_LIB = gtest.a
TEST_CFLAGS += -isystem $(GTEST_DIR)/include
INCLUDES += $(GTEST_DIR)/include/gtest/ $(GTEST_DIR)/include/gtest/internal/  $(GTEST_DIR)/
LDFLAGS += -pthread -lpthread

GTEST_OBJS = $(patsubst %.cc, %.ogtest, $(GTEST_SRC))
GTEST_OBJS := $(filter-out %.cc, $(GTEST_OBJS))

# test objects definition
TEST_HEADERS    := $(wildcard test/*gtest.cpp) 
TEST_OBJECTS	= $(patsubst %.cpp, %.ogtest, $(TEST_SOURCES))
TEST_OBJECTS	+= $(patsubst %.c, %.ogtest, $(TEST_SOURCES))
TEST_OBJECTS  	:= $(filter-out %.c, $(TEST_OBJECTS))
TEST_OBJECTS  	:= $(filter-out %.cpp, $(TEST_OBJECTS))
TEST_OBJECTS  	:= $(filter-out %.h, $(TEST_OBJECTS))

TEST_BIN_OBJS	=  $(patsubst %.cpp, %.ogtest, $(TEST_HEADERS))
TEST_BIN_OBJS  	:= $(filter-out %.cpp, $(TEST_BIN_OBJS))

TEST_BINARIES	= $(patsubst %.cpp, %.gtest, $(TEST_HEADERS))

IFLAGS	 = $(foreach d, $(INCLUDES), -I$d)

# main rules to run
##########################################################################################
TEST_GEN: $(GTEST_LIB)
	$(MAKE) test_compile
	echo $(TEST_OBJECTS)
	echo $(TEST_BINARIES)

ifneq ($(filter $(BRIEF), $(TRUE)),) # check if compile message should be output, BRIEF means no
check: 
	@$(MAKE) TEST_GEN -j4 > /dev/null 
	@printf "Running test binaries \n\n"
	-$(foreach bin, $(TEST_BINARIES), ./$(bin) ; )
	@printf "\nDone running tests\n"
	@$(MAKE) cleantest > /dev/null
else
check: 
	$(MAKE) TEST_GEN -j4
	@printf "Running test binaries \n\n"
	-$(foreach bin, $(TEST_BINARIES), ./$(bin) ; )
	@printf "\nDone running tests\n"
	$(MAKE) cleantest > /dev/null
endif
##########################################################################################

test_compile: $(TEST_OBJECTS) $(TEST_BIN_OBJS) $(TEST_BINARIES)

%.gtest: %.ogtest
	$(TEST_CC) $(TEST_CFLAGS) $(LDFLAGS) $(IFLAGS) $(TEST_OBJECTS) $< $(GTEST_LIB) -o $@

%.ogtest: %.c
	$(TEST_CC) -c $(TEST_CFLAGS) $(IFLAGS) -o $@ $<

%.ogtest: %.cpp
	$(TEST_CC) -c $(TEST_CFLAGS) $(IFLAGS) -o $@ $<

%.ogtest: %.cc
	$(TEST_CC) -c $(TEST_CFLAGS) $(IFLAGS) -o $@ $<

$(GTEST_LIB): $(GTEST_OBJS)
	$(TEST_AR) r $(GTEST_LIB) $(GTEST_OBJS)
	
cleantest:
	rm -f $(TEST_OBJECTS) $(TEST_BIN_OBJS) $(TEST_BINARIES)
	rm -f $(GTEST_LIB) $(GTEST_OBJS)

