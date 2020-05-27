CXX := gcc
CXXFLAGS := -Wall -g
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst %.cpp,%.o, $(notdir $(SRCS)))
BINS := $(patsubst %.o,%, $(OBJS))

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

all : $(addprefix $(BIN_DIR)/,$(BINS)) $(addprefix $(OBJ_DIR)/,$(OBJS))

bin/% : obj/%.o
	$(CXX) $(CXXFLAGS) $(LIBS) $? -o $@ -lstdc++
	chmod +x $@ 

obj/%.o : src/%.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) -c $? -o $@ -lstdc++

.PHONY: clean
clean :
	rm -f bin/* obj/*
