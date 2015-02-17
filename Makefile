
#Use GNU compiler
cc = gcc
CC = g++

LEX=lex
YACC=yacc

LFL = -lfl
CFLAGS = -g

# OS detection, since -lfl on OS X is -ll
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LFL = -ll
	CFLAGS += -DOS_X
endif

all: no-clean clean-tmp

no-clean: shell cat_grep ctrl-c regular mocha

lex.yy.o: shell.l
	$(LEX) shell.l
	$(CC) $(CFLAGS) -c lex.yy.c

y.tab.o: shell.y
	$(YACC) -d shell.y
	$(CC) $(CFLAGS) -c y.tab.c

command.o: command.cc
	$(CC) -c command.cc

shell: y.tab.o lex.yy.o command.o
	$(CC) $(CFLAGS) -o shell lex.yy.o y.tab.o command.o $(LFL)

mocha: shell
	-mocha --reporter nyan

cat_grep: examples/cat_grep.cc
	$(CC) $(CFLAGS) -o cat_grep examples/cat_grep.cc

ctrl-c: examples/ctrl-c.cc
	$(CC) $(CFLAGS) -o ctrl-c examples/ctrl-c.cc

regular: examples/regular.cc
	$(CC) $(CFLAGS) -o regular examples/regular.cc

clean: clean-tmp
	rm -f shell ctrl-c regular cat_grep

clean-tmp:
	rm -f lex.yy.* y.tab.* y.tab.* *.o
