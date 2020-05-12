CC = gcc
SRCS = src/
PROG = bin/

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(SRCS) $(LIBS) -o $(PROG) -lstdc++
