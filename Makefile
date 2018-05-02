CFLAGS := -g -Wall
CC := gcc
EXEC := main
BIN := $(filter-out $(EXEC).c, $(wildcard *.c))
BINFILES := $(BIN:.c=.o)

V := 4
S := 2
D := 

all: $(EXEC).out

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

$(EXEC).out: $(BINFILES) $(EXEC).c
	$(CC) $(CFLAGS) $^ -o $@

run: all
	./$(EXEC).out -v $(V) -s $(S) $(D)

valgrind: all
	valgrind --leak-check=full ./$(EXEC).out

format: .clang-files
	xargs clang-format -style=file -i <$<

clean:
	rm -f $(EXEC).out *.txt *.o /tmp/rcomp

.PHONY: clean run
