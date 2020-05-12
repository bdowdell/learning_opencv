CC = gcc
LD = gcc
#PROG_NAME = prog

SRC_DIR = ./src
BUILD_DIR = ./build
BIN_DIR  = ./bin
SRC_LIST = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_LIST = $(BUILD_DIR)/$(notdir $(SRC_LIST:.cpp=.o))
BIN = $(patsubst %.cpp,%,$(SRC_LIST))

.PHONY: all clean $(PROG_NAME) compile

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

#all: $(PROG_NAME)
all: $(BIN)
compile:
	$(CC) $(SRC_LIST) $(LIBS) -o $(OBJ_LIST) -lstdc++

$(BIN): compile
	$(LD) $(OBJ_LIST) -o $(BIN_DIR)/$@

clean:
	rm -f $(BIN_DIR)/$(BIN) $(BUILD_DIR)/*.o


#$(PROG):$(SRCS)
#	$(CC) $(SRCS) $(LIBS) -o $(PROG) -lstdc++
