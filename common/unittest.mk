# TEST_SOURCES: source files to be tested
# TEST_HEADERS: header file for test cases in cxxtest framework, must be test/*Test.h

TEST_SOURCES ?=
TEST_HEADERS ?= $(wildcard test/*Test.h) 
TEST_CC =clang++
TEST_CFLAGS = -g -O0 -DCXXTEST=1
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

test_gen: 
	$(foreach f, $(TEST_HEADERS), cxxtestgen --error-printer -o $(f).cpp $(f) ; )

check: test_gen 
	$(MAKE) TEST_COMPILE
	@printf "Running test binaries \n\n"
	-$(foreach bin, $(TEST_BINARIES), ./$(bin) ; )
	@printf "\nDone running tests\n"
	$(MAKE) testclean > /dev/null

TEST_COMPILE: $(TEST_OBJECTS) $(TEST_BIN_OBJS) $(TEST_BINARIES)

%.cxxtest: %.ocxx
	$(TEST_CC) $(TEST_CFLAGS) $(IFLAGS) $(TEST_OBJECTS) $< -o $@

%.ocxx: %.c
	$(TEST_CC) -c $(TEST_CFLAGS) $(IFLAGS) -o $@ $<

test/%.ocxx: test/%.h.cpp
	$(TEST_CC) -c $(TEST_CFLAGS) $(IFLAGS) -o $@ $<

testclean:
	rm -f $(addsuffix .cpp, $(TEST_HEADERS))
	rm -f $(addsuffix .test, $(TEST_BIN_OBJS))
	rm -f $(TEST_OBJECTS) $(TEST_BIN_OBJS) $(TEST_BINARIES)

