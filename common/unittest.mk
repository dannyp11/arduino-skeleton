# TEST_SOURCES: source files to be tested
# TEST_HEADERS: header file for test cases in cxxtest framework, must be test/*Test.h

TEST_SOURCES ?=
TEST_HEADERS ?= $(wildcard test/*Test.h) 
TEST_CC =clang++
TEST_CFLAGS += -g -O0 -Wall -Wextra -DCXXTEST=1
IFLAGS	 = $(foreach d, $(INCLUDES), -I$d)

TRUE = 1 yes YES true TRUE
ifneq ($(filter $(TEST_DEBUG), $(TRUE)),) # check if -DDEBUG should be added
TEST_CFLAGS  += -DDEBUG=1
endif

# test objects definition
TEST_OBJECTS	= $(patsubst %.cpp, %.ocxx, $(TEST_SOURCES))
TEST_OBJECTS	+= $(patsubst %.c, %.ocxx, $(TEST_SOURCES))
TEST_OBJECTS  	:= $(filter-out %.c, $(TEST_OBJECTS))
TEST_OBJECTS  	:= $(filter-out %.cpp, $(TEST_OBJECTS))
TEST_OBJECTS  	:= $(filter-out %.h, $(TEST_OBJECTS))

TEST_BIN_OBJS	=  $(patsubst %.h, %.ocxx, $(TEST_HEADERS))
TEST_BIN_OBJS  	:= $(filter-out %.h, $(TEST_BIN_OBJS))

TEST_BINARIES	= $(patsubst %.h, %.cxxtest, $(TEST_HEADERS))

# main rules to run
##########################################################################################
TEST_GEN: 
	$(foreach f, $(TEST_HEADERS), cxxtestgen --error-printer -o $(f).cpp $(f) ; )
	$(MAKE) test_compile

ifneq ($(filter $(BRIEF), $(TRUE)),) # check if compile message should be output, BRIEF means no
check: 
	@$(MAKE) TEST_GEN > /dev/null 
	@printf "Running test binaries \n\n"
	-$(foreach bin, $(TEST_BINARIES), ./$(bin) ; )
	@printf "\nDone running tests\n"
	@$(MAKE) cleantest > /dev/null
else
check: TEST_GEN 
	@printf "Running test binaries \n\n"
	-$(foreach bin, $(TEST_BINARIES), ./$(bin) ; )
	@printf "\nDone running tests\n"
	$(MAKE) cleantest > /dev/null
endif
##########################################################################################

test_compile: $(TEST_OBJECTS) $(TEST_BIN_OBJS) $(TEST_BINARIES)

%.cxxtest: %.ocxx
	$(TEST_CC) $(TEST_CFLAGS) $(IFLAGS) $(TEST_OBJECTS) $< -o $@

%.ocxx: %.c
	$(TEST_CC) -c $(TEST_CFLAGS) $(IFLAGS) -o $@ $<

test/%.ocxx: test/%.h.cpp
	$(TEST_CC) -c $(TEST_CFLAGS) $(IFLAGS) -o $@ $<

cleantest:
	rm -f $(addsuffix .cpp, $(TEST_HEADERS))
	rm -f $(addsuffix .test, $(TEST_BIN_OBJS))
	rm -f $(TEST_OBJECTS) $(TEST_BIN_OBJS) $(TEST_BINARIES)

