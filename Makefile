CFLAGS := -g -Wall
CC := g++
BIN := $(filter-out main.cpp, $(wildcard *.cpp))
BINFILES := $(BIN:.cpp=.o)

all: main

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $<

main: $(BINFILES) main.cpp
	$(CC) $(CFLAGS) $^ -o $@
	mv main main.out

run: all
	./main.out

valgrind: all
	valgrind --leak-check=full ./main.out

clean:
	rm -f main.out *.o

.PHONY: clean run
