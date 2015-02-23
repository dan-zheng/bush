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
CCFLAGS  = $(CFLAGS) -DDEBUG=$(DEBUG) -DFEATURE_LEVEL=$(FEATURES)

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
all: shell
shell: y.tab.o lex.yy.o main.o util.o builtin.o command.o plumber.o env.o
	$(CC) $(CCFLAGS) -o shell *.o $(LFL)

env.o: 		 env.cc env.h global.h trace.h
	$(CC) $(CCFLAGS) -c env.cc
main.o:    main.cc main.h global.h trace.h plumber.h command.h builtin.h main.h
	$(CC) $(CCFLAGS) -c main.cc
util.o:    util.cc util.h global.h trace.h
	$(CC) $(CCFLAGS) -c util.cc
lex.yy.o:  shell.l
	$(LEX) shell.l
	$(CC) -x c++ $(CCFLAGS) -c lex.yy.c
y.tab.o:   shell.y
	$(YACC) -d shell.y
	$(CC) -x c++ $(CCFLAGS) -c y.tab.c
builtin.o: builtin.cc builtin.h global.h trace.h env.o
	$(CC) $(CCFLAGS) -c builtin.cc
command.o: command.cc command.h global.h trace.h plumber.h main.h builtin.h
	$(CC) $(CCFLAGS) -c command.cc
plumber.o: plumber.cc plumber.h global.h trace.h plumber.h
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
	rm -f lex.yy.* *.tab.c *.tab.h *.o *.tmp.*
	rm -rf *.dSYM
