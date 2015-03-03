# --------------------------------------------------------------------------- #
# Variables, Additional Flags & OS Detection  														    #
# --------------------------------------------------------------------------- #

# CLI commands
CC       = g++
LEX      = lex
YACC     = yacc
MAKE     = make

# Platform-specific libraries
LFL      = -lfl

# Debug logging level (0 - None, 4 - Max)
DEBUG    = 0
FEATURES = 4
CCFLAGS  = -g $(CFLAGS) -DDEBUG=$(DEBUG) -DFEATURE_LEVEL=$(FEATURES)

# OS detection, since -lfl on OS X is -ll
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_S),Darwin)
	LFL = -ll
	CCFLAGS += -DOS_X
endif

ifeq ($(UNAME_S),Linux)
ifeq ($(UNAME_M),x86_64)
	CCFLAGS += -DLTTY_A_
	LFL += lib/libtty.a
endif
endif


# --------------------------------------------------------------------------- #
# shell: aliases & additional files                       								    #
# --------------------------------------------------------------------------- #
# Aliases
all:     shell
test:    release
	$(MAKE) -C test-shell
force:   clean shell
debug:   DEBUG = 4
debug:   force
release: DEBUG = 0
release: force

shell: libfiz.a y.tab.o lex.yy.o main.o builtin.o command.o plumber.o env.o globber.o path.o puppet.o
	$(CC) $(CCFLAGS) -o shell *.o libfiz.a $(LFL)

%.o:       %.cc
	$(CC) $(CCFLAGS) -c $^
lex.yy.o:  shell.l
	$(LEX) shell.l
	$(CC) -x c++ $(CCFLAGS) -c lex.yy.c
y.tab.o:   shell.y
	$(YACC) -d shell.y
	$(CC) -x c++ $(CCFLAGS) -c y.tab.c
libfiz.a:  submodules
	$(MAKE) -C fiz-src CFLAGS="-DSTRICT"
	cp fiz-src/libfiz.a ./

# --------------------------------------------------------------------------- #
# clean: get rid of all that messy mess                    								    #
# --------------------------------------------------------------------------- #
.PHONY: submodules clean

submodules:
	git submodule init
	git submodule update
	git submodule foreach git pull origin master

clean:
	rm -f shell ctrl-c regular cat_grep fiz
	rm -f *.yy.* *.tab.* *.tab.* *.o *.tmp.*
	rm -rf *.dSYM
	$(MAKE) clean -C fiz-src
