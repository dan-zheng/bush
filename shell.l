
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

wordchar    ([^ |><\t\n]|\\[^\n\t])

%%

\n 	        { return LF; }

">>&"       { return GTGTAMP; }

">>"        { return GTGT; }

">&"        { return GTAMP; }

">"         { return GT; }

"<"         { return LT; }

"&"         { return AMP; }

"|"         { return PIPE; }

\"[^\"]*\"      {
              yylval.string_val = strdup(yytext + 1);
              if (yylval.string_val[yyleng - 2] == '"') {
                yylval.string_val[yyleng - 2] = 0;
              }
              return WORD;
            }

[ \t] 	    { /* discard */ }

{wordchar}{wordchar}* {
	yylval.string_val = strdup(yytext);
	return WORD;
}
