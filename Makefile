
# CLI commands
CC      = g++
LEX     = lex
YACC    = yacc

# Platform-specific libraries
LFL     = -lfl

# Debug logging level (0 - None, 4 - Max)
DEBUG   = 4
CFLAGS += -DDEBUG=$(DEBUG)

# OS detection, since -lfl on OS X is -ll
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LFL = -ll
	CFLAGS += -DOS_X
endif

# Aliases
all: no-clean clean-tmp

no-clean: shell examples mocha

# Shell
lex.yy.o: shell.l
	$(LEX) shell.l
	$(CC) -x c++ $(CFLAGS) -c lex.yy.c

y.tab.o: shell.y
	$(YACC) -d shell.y
	$(CC) -x c++ $(CFLAGS) -c y.tab.c

command.o: command.cc
	$(CC) $(CFLAGS) -c command.cc

shell: y.tab.o lex.yy.o command.o main.cc
	$(CC) $(CFLAGS) -o shell main.cc lex.yy.o y.tab.o command.o $(LFL)

# Test
mocha: shell
	-mocha --reporter nyan

# Example executables
examples:  examples/ctrl-c.cc examples/regular.cc
	$(CC) $(CFLAGS) -o cat_grep examples/cat_grep.cc
	$(CC) $(CFLAGS) -o ctrl-c examples/ctrl-c.cc
	$(CC) $(CFLAGS) -o regular examples/regular.cc

# Cleanup
clean: clean-tmp
	rm -f shell ctrl-c regular cat_grep

clean-tmp:
	rm -f lex.yy.* y.tab.* y.tab.* *.o
	rm -rf *.dSYM

.PHONY: examples
