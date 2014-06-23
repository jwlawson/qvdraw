#
# 'make depend' uses makedepend to automatically generate dependencies
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define any compile-time flags
# Using cygwin -std=gnu++11 should be used rather than -std=c++11
CXXFLAGS = -Wall -Wextra -Wpedantic -std=gnu++11 -march=native
OPT = -g -O3

# Specify base directory
BASE_DIR = .

# Specify source directory
SRC_DIR = $(BASE_DIR)/src

# Specify test directory
TEST_DIR = $(BASE_DIR)/test

# define the output directory for .o
OBJ_DIR = $(BASE_DIR)/build

# define any directories containing header files other than /usr/include
# -I/home/newhall/include 
INCLUDES = -I$(BASE_DIR)/include -I$(HOME)/include

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like: -L/home/newhall/lib
LFLAGS = -L$(BASE_DIR)/lib -L$(HOME)/lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -lqv -lOGDF -pthread

# define the C source files
SRCS = $(wildcard $(SRC_DIR)/*.cc)
_GML_SRC = $(SRC_DIR)/qv2gml.cc $(SRC_DIR)/graph_factory.cc
_LAY_SRC = $(SRC_DIR)/gmlayout.cc

# define the C object files
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .cc of all words in the macro SRCS
# with the .o suffix
#
_OBJS = $(SRCS:.cc=.o)
_GML_OBJS = $(_GML_SRC:.cc=.o)
_LAY_OBJS = $(_LAY_SRC:.cc=.o)

# Puts objs in obj_dir
OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(_OBJS))
GML_OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(_GML_OBJS))
LAY_OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(_LAY_OBJS))


# define the executable file 
MAIN = qvdraw
GML = qv2gml
LAY = gmlayout


#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: clean

all:   $(GML) $(LAY)

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OPT) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

$(GML): $(GML_OBJS)
	$(CXX) $(CXXFLAGS) $(OPT) $(INCLUDES) -o $(GML) $(GML_OBJS) $(LFLAGS) $(LIBS)

$(LAY): $(LAY_OBJS)
	$(CXX) $(CXXFLAGS) $(OPT) $(INCLUDES) -o $(LAY) $(LAY_OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(OPT) $(INCLUDES) -c $<  -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(OPT) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN) $(OBJ_DIR)/*.o $(GML)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
