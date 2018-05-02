CFLAGS := -g -Wall
CC := gcc
EXEC := main
BIN := $(filter-out $(EXEC).c, $(wildcard *.c))
BINFILES := $(BIN:.c=.o)

V := 4
S := 2
D := 

OBJDIR = build

$(OBJDIR)/%.o: %.c %.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

OBJEXEC = $(addprefix $(OBJDIR)/, $(EXEC).out)

all: $(OBJEXEC)

$(OBJEXEC): $(addprefix $(OBJDIR)/, $(BINFILES)) $(EXEC).c
	$(CC) $(CFLAGS) $^ -o $@

run: all
	./$(OBJDIR)/$(EXEC).out -v $(V) -s $(S) $(D)

run-debug: all
	bash run-debug.sh

valgrind: all
	valgrind --leak-check=full ./$(OBJDIR)/$(EXEC).out

format: .clang-files
	xargs clang-format -style=file -i <$<

clean:
	rm -rf $(OBJDIR)/ *.txt *.o /tmp/rcomp

.PHONY: clean run
