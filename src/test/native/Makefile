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
INCLUDES = -pthread -I/usr/include/ -I$(SRC_DIR) -I./ -I/usr/include/glib-2.0\
	-I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/include/gstreamer-0.10\
	-I/usr/include/libxml2

LIBS = -L/usr/lib/i386-linux-gnu -L$(LIB_DIR) -lboost_test_exec_monitor -lgstreamer-0.10\
	-lgstpbutils-0.10 -lgstapp-0.10 -lgobject-2.0 -lgmodule-2.0 -lxml2 -lgthread-2.0 -lrt\
	-lglib-2.0 -lGstreamerMediaFile -lGstreamerTranscoder

# The C++ test source files.
SRC_TEST_ONE = GstreamerMediaFileTest.cpp
SRC_TEST_TWO = GstreamerTranscoderTest.cpp

BIN_TEST_ONE = $(LIB_TEST_DIR)$(basename $(SRC_TEST_ONE))
BIN_TEST_TWO = $(LIB_TEST_DIR)$(basename $(SRC_TEST_TWO))

# The compile command for the first C++ test source file. This places the binary file into the standard lib directory.
COMPILE_NATIVE_ONE_CMD = $(CCC) $(INCLUDES) $(SRC_TEST_ONE) $(LIBS) -o $(BIN_TEST_ONE)
COMPILE_NATIVE_TWO_CMD = $(CCC) $(INCLUDES) $(SRC_TEST_TWO) $(LIBS) -o $(BIN_TEST_TWO)

all:
	$(COMPILE_NATIVE_ONE_CMD)
	$(COMPILE_NATIVE_TWO_CMD)
	LD_LIBRARY_PATH=$(LIB_DIR) $(BIN_TEST_ONE)
	LD_LIBRARY_PATH=$(LIB_DIR) $(BIN_TEST_TWO)

clean:
	rm $(LIB_TEST_DIR)*
