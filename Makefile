
PROGRAM = mydebug
CC = g++
SRC = debug.cpp wbranch.cpp
OBJ = debug.o wbranch.o
CFLAGS = -m32 -g -Wall -Wno-write-strings -Wno-sign-compare # -O2

.PHONY: all
all: $(OBJ)
	$(CC) -g $(CFLAGS) -o $(PROGRAM) $(OBJ)

.cpp.o:
	$(CC) -c $(CFLAGS) $<

.PHONY: clean
clean:
	-rm -f $(PROGRAM) $(OBJ) *.h.gch

.PHONY: run
run: all
	./$(PROGRAM)

.PHONY: debug
debug: all
	cgdb ./$(PROGRAM)

