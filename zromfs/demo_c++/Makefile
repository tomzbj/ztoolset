CC 	= gcc.exe
CPP = g++.exe

LDFLAGS =  -L. -lmingw32 -lstdc++
LDFLAGS += -Wl,-Map=main.map,--cref,--no-warn-mismatch

CFLAGS = -O3 -Wall -fno-exceptions
CFLAGS += -I .
CPPFLAGS = -std=gnu++17

BIN  = app.exe
RM = rm -f 
SRCC = main.cc ../zromfs.cc cli.cc crc32.cc

INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
OBJS    = $(ASRC:.s=.o) $(SRCC:.cc=.o) $(SRC:.c=.o)
SIZE := size.exe

all: $(BIN)
	@$(SIZE) -B $(BIN) 
%.o: %.c
	$(CC) -c $(CFLAGS) $(DEFS) -I . $(INCDIR) $< -o $@ 
%.o: %.cc
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(DEFS) -I . $(INCDIR) $< -o $@ 
clean: clean-custom
	${RM} $(LINKOBJ) $(BIN) *~ .*.swp 
$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LDFLAGS)
	strip.exe $(BIN)
	upx.exe $(BIN) > /dev/nul 
.PHONY: all clean 
