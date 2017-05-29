# TEST_SOURCES: source files to be tested
# TEST_HEADERS: header file for test cases in cxxtest framework

TEST_SOURCES ?=
TEST_HEADERS ?=
TEST_CC =g++
TEST_CFLAGS = -g -O0 -DCXXTEST=1
IFLAGS	 = $(foreach d, $(INCLUDES), -I$d)

# test objects definition
TEST_OBJECTS	= $(patsubst %.cpp, %.ocxx, $(TEST_SOURCES))
TEST_OBJECTS	= $(patsubst %.c, %.ocxx, $(TEST_SOURCES))
TEST_OBJECTS  	:= $(filter-out %.c, $(TEST_OBJECTS))
TEST_OBJECTS  	:= $(filter-out %.cpp, $(TEST_OBJECTS))
TEST_OBJECTS  	:= $(filter-out %.h, $(TEST_OBJECTS))

TEST_BIN_OBJS	=  $(patsubst %.h, %.ocxx, $(TEST_HEADERS))
TEST_BIN_OBJS  	:= $(filter-out %.h, $(TEST_BIN_OBJS))

TEST_BINARIES	= $(patsubst %.h, %.ocxx, $(TEST_HEADERS))

test_gen: 
	$(foreach f, $(TEST_HEADERS), cxxtestgen --error-printer -o $(f).cpp $(f) ; )

check: test_gen 
	$(MAKE) TEST_COMPILE
	-$(foreach bin, $(TEST_BINARIES), ./$(bin).test; )
	$(MAKE) testclean > /dev/null

TEST_COMPILE: $(TEST_OBJECTS) $(TEST_BIN_OBJS)
	$(foreach bin, $(TEST_BINARIES), $(TEST_CC) $(TEST_CFLAGS) $(IFLAGS) $(TEST_OBJECTS) $(bin) -o $(bin).test ;)

%.ocxx: %.c
	$(TEST_CC) -c $(TEST_CFLAGS) $(IFLAGS) -o $@ $<

test/%.ocxx: test/%.h.cpp
	$(TEST_CC) -c $(TEST_CFLAGS) $(IFLAGS) -o $@ $<

testclean:
	rm -f $(addsuffix .cpp, $(TEST_HEADERS))
	rm -f $(addsuffix .test, $(TEST_BIN_OBJS))
	rm -f $(TEST_OBJECTS) $(TEST_BIN_OBJS)

