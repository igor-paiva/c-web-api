# Executable
BINFOLDER := bin
# .h
INCFOLDER := inc
# .c
SRCFOLDER := src
# .o
OBJFOLDER := obj
# Views folder
VIEWSFOLDER := views

# Address to run the server
ADDRESS := localhost

# Port to run the server
PORT := 5000

# Compiler
CC := gcc

CFLAGS := -g -W -Wall -Wextra -pedantic -lpthread

SRCFILES := $(wildcard $(SRCFOLDER)/*.c $(SRCFOLDER)/*/*.c)

all: $(SRCFILES:$(SRCFOLDER)/%.c=$(OBJFOLDER)/%.o)
	$(CC) $(OBJFOLDER)/*.o $(OBJFOLDER)/*/*.o  $(CFLAGS) -o $(BINFOLDER)/prog

$(OBJFOLDER)/%.o: $(SRCFOLDER)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I ./$(INCFOLDER)/

$(OBJFOLDER)/%/%.o: $(SRCFOLDER)/%/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I ./$(INCFOLDER)/

run:
	./$(BINFOLDER)/prog $(ADDRESS) $(PORT)

.PHONY: clean

clean:
	rm -rf $(OBJFOLDER)/*.* $(OBJFOLDER)/*/*.*
	rm -rf $(BINFOLDER)/*
