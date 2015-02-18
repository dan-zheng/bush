
/*
 *
 * CS-252 Spring 2015
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 * cmd [arg]* [ | cmd [arg]* ]* [ [> filename] [< filename] [ >& filename] [>> filename] [>>& filename] ]* [&]
 */

%{

#include <string.h>
#include "y.tab.h"

static void yyunput(int, char*);

void myunputc(int c) { unput(c); }

%}

%%

\n 	        { return LF; }

[ \t] 	    { /* discard */ }


">>&"       { return AMP; }

">>"        { return GTGT; }

">&"        { return GTAMP; }

">"         { return GT; }

"<"         { return LT; }

"&"         { return AMP; }

"|"         { return PIPE; }

[^ \t\n][^ \t\n]* {
	yylval.string_val = strdup(yytext);
	return WORD;
}
