##################
# Setup the directories #
##################

# The target directory of the maven build, all the compiled code will be placed under this directory.
TARGET = ../../../target/

# The directory that where maven will look for files to be placed in the packaged JAR file.
CLASSES_DIR = $(TARGET)classes/

# The directory that will contain the compiled C++ files.
LIB_DIR = $(CLASSES_DIR)lib/


###################
# Setup native compiling #
###################

# The C++ compiler that will be used.
CCC = g++

# The source files to compile.
SRC = media/io/gstreamer/GstreamerMediaFile.cpp media/io/gstreamer/GstreamerTranscoder.cpp

# The source files with their extensions changed to the object file extension of ".so".
OBJ = $(SRC:.cpp=.so)

# The required head files.
INCLUDES = -pthread -I./ -I/usr/include/glib-2.0\
	-I/usr/lib/i386-linux-gnu/glib-2.0/include\
	-I/usr/include/gstreamer-0.10 -I/usr/include/libxml2

# The required libraries.
LIBS = -L/usr/lib/i386-linux-gnu -lgstreamer-0.10\
	-lgstpbutils-0.10 -lgstapp-0.10 -lgobject-2.0\
	-lgmodule-2.0 -lxml2 -lgthread-2.0 -lrt -lglib-2.0

# The rule for how a ".cpp" file should be compiled into a ".so" file.
%.so : %.cpp
	$(CCC) $(INCLUDES) $(LIBS) -shared $< -o $(LIB_DIR)lib$(notdir $@)

all : $(OBJ)

clean :
	rm $(LIB_DIR)*.so
