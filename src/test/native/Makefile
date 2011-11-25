##################
# Setup the directories #
##################

# The target directory of the maven build, all the compiled code will be placed under this directory.
TARGET = ../../../target/

# The directory that will contain the compiled java test classes.
TEST_CLASSES_DIR = $(TARGET)test-classes/

# The directory that will contain the compiled native files.
LIB_TEST_DIR = $(TEST_CLASSES_DIR)lib-test/


#####################
# Setup native compiling #
#####################

# The C++ compiler that will be used.
CCC = g++

# The required header files for the native code.
INCLUDES = -I/usr/include/

LIBS = -lboost_test_exec_monitor

# The first C++ test source file.
SRC_TEST_ONE = GstreamerMediaFileTest.cpp

BIN_TEST_ONE = $(LIB_TEST_DIR)$(basename $(SRC_TEST_ONE))

# The compile command for the first C++ test source file. This places the source file into the standard lib directory.
COMPILE_NATIVE_TWO_CMD = $(CCC) $(INCLUDES) $(SRC_TEST_ONE) $(LIBS) -o $(BIN_TEST_ONE)

all:
	$(COMPILE_NATIVE_TWO_CMD)
	$(BIN_TEST_ONE)

clean:
