CC   = gcc.exe
CPP   = g++.exe

LDFLAGS =  -L. 
DEFS =
CFLAGS_COMMON = -Os -Wall
CFLAGS = $(CFLAGS_COMMON) -I ..
CPPFLAGS = -std=gnu++17 $(CFLAGS_COMMON)
BIN  = app.exe
RM = rm -f 
SRCC = main.cc
OBJS    = $(SRCC:.cc=.o)
LDFLAGS += -Wl,-Map=main.map,--cref,--no-warn-mismatch 

.PHONY: all clean
all: $(BIN)

%.o: %.c
	$(CC) -c $(CFLAGS) $(DEFS) -I . $(INCDIR) $< -o $@

%.o: %.cc
	$(CPP) -c $(CPPFLAGS) $(DEFS) -I . $(INCDIR) $< -o $@ 

clean: clean-custom
	${RM} $(LINKOBJ) $(BIN) *~ .*.swp

$(BIN): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(BIN)
	strip.exe $(BIN)
