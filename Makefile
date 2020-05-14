CXX := gcc
CXXFLAGS := -Wall -g
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(OBJ_DIR)/$(notdir $(SRCS:.cpp=.o))
BINS = $(BIN_DIR)/$(notdir $(SRCS:.cpp=))

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

all : clean $(BINS) $(OBJS)

$(BINS) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $< -o $@ -lstdc++
	chmod +x $(BINS) 

$(OBJS) : $(SRCS)
	$(CXX) $(CXXFLAGS) $(LIBS) -c $< -o $@ -lstdc++

.PHONY: clean
clean :
	rm -f bin/* obj/*
