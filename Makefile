CFLAGS := -g -Wall
CC := gcc
BIN := $(filter-out main.c, $(wildcard *.c))
BINFILES := $(BIN:.c=.o)

all: main

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

main: $(BINFILES) main.c
	$(CC) $(CFLAGS) $^ -o $@
	mv main main.out

run: all
	./main.out

valgrind: all
	valgrind --leak-check=full ./main.out

format: .clang-files
	xargs clang-format -style=file -i <$<

clean:
	rm -f main.out *.o

.PHONY: clean run
