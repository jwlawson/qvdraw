# Using cygwin -std=gnu++11 should be used rather than -std=c++11
CXXFLAGS = -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -std=gnu++11 -march=native
CXXFLAGS += -DUSE_COIN -DCOIN_OSI_CLP -DOGDF_DLL
OPT = -g -O3

# Specify base directory
BASE_DIR = .

# Specify source directory
SRC_DIR = $(BASE_DIR)/src

# define the output directory for .o
OBJ_DIR = $(BASE_DIR)/build

# define any directories containing header files
INCLUDES = -I$(BASE_DIR)/include -I$(HOME)/include

# define library paths in addition to /usr/lib
LFLAGS = -L$(BASE_DIR)/lib -L$(HOME)/lib

# define any libraries to link into executable:
LIBS = -lqv -pthread -lCoinUtils -lOsi -lOsiClp -lClp -lOGDF -lginac

# define the C source files
_GML_SRC = $(SRC_DIR)/qv2gml.cc $(SRC_DIR)/graph_factory.cc
_MOV_SRC = $(SRC_DIR)/qvmove2gml.cc $(SRC_DIR)/graph_factory.cc $(SRC_DIR)/consts.cc
_GRA_SRC = $(SRC_DIR)/qvgraph2gml.cc $(SRC_DIR)/graph_factory.cc
_LAY_SRC = $(SRC_DIR)/gmlayout.cc $(SRC_DIR)/layout.cc
_DRA_SRC = $(SRC_DIR)/qv2tex.cc $(SRC_DIR)/graph_factory.cc $(SRC_DIR)/layout.cc $(SRC_DIR)/consts.cc

_GML_OBJS = $(_GML_SRC:.cc=.o)
_MOV_OBJS = $(_MOV_SRC:.cc=.o)
_GRA_OBJS = $(_GRA_SRC:.cc=.o)
_LAY_OBJS = $(_LAY_SRC:.cc=.o)
_DRA_OBJS = $(_DRA_SRC:.cc=.o)

# Puts objs in obj_dir
GML_OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(_GML_OBJS))
MOV_OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(_MOV_OBJS))
GRA_OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(_GRA_OBJS))
LAY_OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(_LAY_OBJS))
DRA_OBJS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(_DRA_OBJS))

# define the executables
GML = qv2gml
LAY = gmlayout
MOV = qvmove2gml
GRA = qvgraph2gml
DRA = qv2tex

.PHONY: clean

all: $(GML) $(LAY) $(MOV) $(GRA) $(DRA)

$(GML): $(GML_OBJS)
	$(CXX) $(CXXFLAGS) $(OPT) $(INCLUDES) -o $(GML) $(GML_OBJS) $(LFLAGS) $(LIBS)

$(MOV): $(MOV_OBJS)
	$(CXX) $(CXXFLAGS) $(OPT) $(INCLUDES) -o $(MOV) $(MOV_OBJS) $(LFLAGS) $(LIBS)

$(LAY): $(LAY_OBJS)
	$(CXX) $(CXXFLAGS) $(OPT) $(INCLUDES) -o $(LAY) $(LAY_OBJS) $(LFLAGS) $(LIBS)

$(GRA): $(GRA_OBJS)
	$(CXX) $(CXXFLAGS) $(OPT) $(INCLUDES) -o $(GRA) $(GRA_OBJS) $(LFLAGS) $(LIBS)

$(DRA): $(DRA_OBJS)
	$(CXX) $(CXXFLAGS) $(OPT) $(INCLUDES) -o $(DRA) $(DRA_OBJS) $(LFLAGS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(OPT) $(INCLUDES) -c $< -o $@
	
$(LAY_OBJS): | $(OBJ_DIR)

$(GML_OBJS): | $(OBJ_DIR)

$(MOV_OBJS): | $(OBJ_DIR)

$(GRA_OBJS): | $(OBJ_DIR)

$(DRA_OBJS): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(RM) *.o *~ $(MAIN) $(OBJ_DIR)/*.o $(GML) $(LAY) $(MOV) $(GRA) $(DRA)

