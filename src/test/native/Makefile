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
INCLUDES = -D__STDC_CONSTANT_MACROS -I/opt/libav/include/ -I/usr/include/ -I$(SRC_DIR) -I./

LIBS = -L/usr/lib/i386-linux-gnu -L/opt/libav/lib/ -L/usr/lib/ -L$(LIB_DIR) -lboost_test_exec_monitor \
-lavformat -lavcodec -lavutil -lx264 -lboost_filesystem -llibav

# The C++ test source files.
SRC = example_test.cpp standard_test.cpp libav_test.cpp

TESTS = $(SRC:.cpp=.test)

# The rule for how a ".cpp" file should be compiled into a ".test" file.
%.test : %.cpp
	$(CCC) $(INCLUDES) $< $(LIBS) -o $(LIB_TEST_DIR)$@

# Compile all the test files then run one after the other.
all: $(TESTS)
	$(foreach test, $(TESTS), LD_LIBRARY_PATH="$(LIB_DIR):/opt/libav/lib" $(LIB_TEST_DIR)$(test);) 

clean:
	rm $(LIB_TEST_DIR)*
