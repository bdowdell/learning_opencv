CC = gcc
LD = gcc
PROG_NAME = prog

SRC_DIR = ./src
BUILD_DIR = ./build
BIN_DIR  = ./bin
SRC_LIST = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_LIST = $(BUILD_DIR)/$(notdir $(SRC_LIST:.cpp=.o))

.PHONY: all clean $(PROG_NAME) compile

all: $(PROG_NAME)

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

compile:
	$(CC) $(SRC_LIST) $(LIBS) -o $(OBJ_LIST) -lstdc++

$(PROG_NAME): compile
	$(LD) $(OBJ_LIST) -o $(BIN_DIR)/$@ -lstdc++

clean:
	rm -f $(BIN_DIR)/$(PROG_NAME) $(BUILD_DIR)/*.0


#$(PROG):$(SRCS)
#	$(CC) $(SRCS) $(LIBS) -o $(PROG) -lstdc++
