##################
# Setup the directories #
##################

# The target directory of the maven build, all the compiled code will be placed under this directory.
TARGET = ../../../target/

# The directory where maven will look for files that will be placed in the packaged JAR file.
CLASSES_DIR = $(TARGET)classes/

# The directory that will contain the compiled native files that will be packages into the JAR file.
LIB_DIR = $(CLASSES_DIR)lib/


###################
# Setup native compiling #
###################

# The C++ compiler that will be used.
CCC = g++

# The source files to compile.
SRC = utils/MediaUtils.cpp

# The source files with their extensions changed to the object file extension of ".so".
OBJ = $(SRC:.cpp=.so)

# The required head files.
INCLUDES = -I./ -I/usr/include/

# The required libraries.
LIBS = -L/usr/lib/

# The rule for how a ".cpp" file should be compiled into a ".so" file.
%.so : %.cpp
	$(CCC) $(INCLUDES) $(LIBS) -shared $< -o $(LIB_DIR)lib$(notdir $@)

all : $(OBJ)

clean :
	rm $(LIB_DIR)*.so
