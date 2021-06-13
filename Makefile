# Executable
BINFOLDER := bin/
# .h
INCFOLDER := inc/
# .c
SRCFOLDER := src/
# .o
OBJFOLDER := obj/

# Address to run the server
ADDRESS := localhost

# Port to run the server
PORT := 5000

# Compiler
CC := gcc

CFLAGS := -pedantic -lpthread

SRCFILES := $(wildcard src/*.c)

all: $(SRCFILES:src/%.c=obj/%.o)
	$(CC) $(OBJFOLDER)*.o $(CFLAGS) -o $(BINFOLDER)prog

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I ./$(INCFOLDER)

run: $(BINFOLDER)
	./$(BINFOLDER)prog $(ADDRESS) $(PORT)

.PHONY: clean

clean:
	rm -rf obj/*
	rm -rf bin/*
