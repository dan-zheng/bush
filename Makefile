
# CLI commands
CC      = g++
LEX     = lex
YACC    = yacc

# Platform-specific libraries
LFL     = -lfl -L./lib -ltty

# Debug logging level (0 - None, 4 - Max)
DEBUG   = 4
CCFLAGS = $(CFLAGS) -DDEBUG=$(DEBUG)

# OS detection, since -lfl on OS X is -ll
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LFL = -ll
	CCFLAGS += -DOS_X
endif

# Aliases
all: no-clean clean-tmp

no-clean: shell examples

# Shell
lex.yy.o: shell.l
	$(LEX) shell.l
	$(CC) -x c++ $(CCFLAGS) -c lex.yy.c

y.tab.o: shell.y
	$(YACC) -d shell.y
	$(CC) -x c++ $(CCFLAGS) -c y.tab.c

cfiles: command.cc plumber.cc
	$(CC) $(CCFLAGS) -c command.cc
	$(CC) $(CCFLAGS) -c plumber.cc

builtin.o: builtin.cc
	$(CC) $(CCFLAGS) -c builtin.cc


shell: cfiles y.tab.o lex.yy.o main.cc builtin.o
	$(CC) $(CCFLAGS) -o shell main.cc lex.yy.o y.tab.o plumber.o command.o builtin.o $(LFL)

# Test
mocha: shell
	-mocha --reporter nyan

# Example executables
examples:  examples/cat_grep.cc plumber.o examples/ctrl-c.cc examples/regular.cc
	$(CC) $(CCFLAGS) -o cat_grep examples/cat_grep.cc plumber.o
	$(CC) $(CCFLAGS) -o ctrl-c examples/ctrl-c.cc
	$(CC) $(CCFLAGS) -o regular examples/regular.cc



# Cleanup
clean: clean-tmp
	rm -f shell ctrl-c regular cat_grep

clean-tmp:
	rm -f lex.yy.* y.tab.* y.tab.* *.o *.tmp.*
	rm -rf *.dSYM

.PHONY: examples
