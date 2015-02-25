# --------------------------------------------------------------------------- #
# Variables, Additional Flags & OS Detection  														    #
# --------------------------------------------------------------------------- #

# CLI commands
CC       = g++
LEX      = lex
YACC     = yacc

# Platform-specific libraries
#LFL     = -lfl -L./lib -ltty
LFL      = -lfl

# Debug logging level (0 - None, 4 - Max)
DEBUG    = 4
FEATURES = 4
CCFLAGS  = -g $(CFLAGS) -DDEBUG=$(DEBUG) -DFEATURE_LEVEL=$(FEATURES)

# OS detection, since -lfl on OS X is -ll
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LFL = -ll
	CCFLAGS += -DOS_X
endif

# --------------------------------------------------------------------------- #
# shell: aliases & additional files                       								    #
# --------------------------------------------------------------------------- #
# Aliases
re:    clean shell
all:   shell
shell: y.tab.o lex.yy.o main.o builtin.o command.o plumber.o env.o globber.o path.o
	$(CC) $(CCFLAGS) -o shell *.o $(LFL)

env.o: 		 env.cc env.hpp trace.hpp
	$(CC) $(CCFLAGS) -c env.cc
main.o:    main.cc main.hpp trace.hpp plumber.hpp command.hpp builtin.hpp main.hpp
	$(CC) $(CCFLAGS) -c main.cc
path.o:    path.cc path.hpp trace.hpp
	$(CC) $(CCFLAGS) -c path.cc
lex.yy.o:  shell.l
	$(LEX) shell.l
	$(CC) -x c++ $(CCFLAGS) -c lex.yy.c
y.tab.o:   shell.y
	$(YACC) -d shell.y
	$(CC) -x c++ $(CCFLAGS) -c y.tab.c
globber.o: globber.cc globber.hpp trace.hpp
	$(CC) $(CCFLAGS) -c globber.cc
builtin.o: builtin.cc builtin.hpp trace.hpp env.o globber.o
	$(CC) $(CCFLAGS) -c builtin.cc
command.o: command.cc command.hpp trace.hpp plumber.hpp main.hpp builtin.hpp
	$(CC) $(CCFLAGS) -c command.cc
plumber.o: plumber.cc plumber.hpp trace.hpp plumber.hpp
	$(CC) $(CCFLAGS) -c plumber.cc

# --------------------------------------------------------------------------- #
# examples: example executables.                          								    #
# --------------------------------------------------------------------------- #
.PHONY: examples
examples: cat_grep ctrl-c regular
cat_grep: examples/cat_grep.cc plumber.o
	$(CC) $(CCFLAGS) -o cat_grep examples/cat_grep.cc plumber.o
ctrl-c:   examples/ctrl-c.cc
	$(CC) $(CCFLAGS) -o ctrl-c examples/ctrl-c.cc
regular:  examples/regular.cc
	$(CC) $(CCFLAGS) -o regular examples/regular.cc

# --------------------------------------------------------------------------- #
# clean: get rid of all that messy mess                    								    #
# --------------------------------------------------------------------------- #
clean:
	rm -f shell ctrl-c regular cat_grep
	rm -f *.yy.* *.tab.* *.tab.* *.o *.tmp.*
	rm -rf *.dSYM
