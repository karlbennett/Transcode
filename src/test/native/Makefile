##################
# Setup the directories #
##################

# The target directory of the maven build, all the compiled code will be placed under this directory.
TARGET = ../../../target/

# The directory where maven will look for files that will be placed in the packaged JAR file.
CLASSES_DIR = $(TARGET)classes/

# The directory that will contain the compiled native files that will be packages into the JAR file.
LIB_DIR = $(CLASSES_DIR)lib/

# The directory that contains the source files.
SRC_DIR = ../../main/native

# The directory where maven will look for files that should be run as tests.
TEST_CLASSES_DIR = $(TARGET)test-classes/

# The directory that will contain the compiled native files that will be linked to by the tests.
LIB_TEST_DIR = $(TEST_CLASSES_DIR)lib-test/


##########################
# Setup native compiling #
##########################

# The C++ compiler that will be used.
CCC = g++

# The required header files for the native code.
INCLUDES = -D__STDC_CONSTANT_MACROS -I/usr/include/ -I$(SRC_DIR) -I./

LIBS = -L/usr/lib/i386-linux-gnu -L/usr/lib/ -L$(LIB_DIR) -lboost_test_exec_monitor \
-lavformat -lavcodec -lavutil -lboost_filesystem -lutil_file -lutil_ffmpeg -lutil_media_ffmpeg

# The C++ test source files.
SRC_TEST_ONE = example_test.cpp
SRC_TEST_TWO = util_media_test.cpp
SRC_TEST_THREE = util_standard_test.cpp
SRC_TEST_FOUR = util_ffmpeg_test.cpp
SRC_TEST_FIVE = util_file_test.cpp

BIN_TEST_ONE = $(LIB_TEST_DIR)$(basename $(SRC_TEST_ONE))
BIN_TEST_TWO = $(LIB_TEST_DIR)$(basename $(SRC_TEST_TWO))
BIN_TEST_THREE = $(LIB_TEST_DIR)$(basename $(SRC_TEST_THREE))
BIN_TEST_FOUR = $(LIB_TEST_DIR)$(basename $(SRC_TEST_FOUR))
BIN_TEST_FIVE = $(LIB_TEST_DIR)$(basename $(SRC_TEST_FIVE))

# The compile command for the first C++ test source file. This places the binary file into the standard lib directory.
COMPILE_NATIVE_ONE_CMD = $(CCC) $(INCLUDES) $(SRC_TEST_ONE) $(LIBS) -o $(BIN_TEST_ONE)
COMPILE_NATIVE_TWO_CMD = $(CCC) $(INCLUDES) $(SRC_TEST_TWO) $(LIBS) -o $(BIN_TEST_TWO)
COMPILE_NATIVE_THREE_CMD = $(CCC) $(INCLUDES) $(SRC_TEST_THREE) $(LIBS) -o $(BIN_TEST_THREE)
COMPILE_NATIVE_FOUR_CMD = $(CCC) $(INCLUDES) $(SRC_TEST_FOUR) $(LIBS) -o $(BIN_TEST_FOUR)
COMPILE_NATIVE_FIVE_CMD = $(CCC) $(INCLUDES) $(SRC_TEST_FIVE) $(LIBS) -o $(BIN_TEST_FIVE)

all:
	$(COMPILE_NATIVE_ONE_CMD)
	$(COMPILE_NATIVE_TWO_CMD)
	$(COMPILE_NATIVE_THREE_CMD)
	$(COMPILE_NATIVE_FOUR_CMD)
	$(COMPILE_NATIVE_FIVE_CMD)
	LD_LIBRARY_PATH=$(LIB_DIR) $(BIN_TEST_ONE)
	LD_LIBRARY_PATH=$(LIB_DIR) $(BIN_TEST_TWO)
	LD_LIBRARY_PATH=$(LIB_DIR) $(BIN_TEST_THREE)
	LD_LIBRARY_PATH=$(LIB_DIR) $(BIN_TEST_FOUR)
	LD_LIBRARY_PATH=$(LIB_DIR) $(BIN_TEST_FIVE)

clean:
	rm $(LIB_TEST_DIR)*
