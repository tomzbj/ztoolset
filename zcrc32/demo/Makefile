CC = g++
SRC = main.cc
DEFS = 
INC = 
LIB = 
BIN = $(SRC:.cc=.exe)
CFLAGS = -Wall -O3 -std=gnu++17

OBJ = $(SRC:.cc=.o)

all: 
	$(CC) $(CFLAGS) $(INC) -c $(SRC)
	$(CC) $(OBJ) $(CFLAGS) $(LIB) -o $(BIN)

clean:
	-rm *.o *.exe *~
