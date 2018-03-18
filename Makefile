CFLAGS := -g -Wall
CC := g++
BIN := $(filter-out main.cpp, $(wildcard *.cpp))
BINFILES := $(BIN:.cpp=.o)

all: main

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $<

main: $(BINFILES) main.cpp
	$(CC) $(CFLAGS) $^ -o $@

run: all
	./main

valgrind: all
	valgrind --leak-check=full ./main

clean:
	rm -f main *.o

.PHONY: clean run
