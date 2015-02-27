/* ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// shell.l                                                                   //
// This file contains lexical analysis rules for the shell commands.         //
//                                                                           //
// ------------------------------------------------------------------------- */

%{

#include <string.h>
#include "path.hpp"
#include "y.tab.h"

#ifdef LTTY_A_

  #include <errno.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <signal.h>
  #include <sys/wait.h>
  #include "lib/tty.h"

  static
  void yyunput(int, char*);
  void myunputc(int c) { unput(c); }

  int read(FILE *f) {
    static char *pr;
    if (!isatty(0)) { return getc(f); }
    if (!pr || !*pr) { pr = ttygetline(TTY_MAX_LINEBUFFER); }
    return *pr++;
  }

  #undef getc
  #define getc(f) read(f)

#endif

%}

char                         ([^ |><\t\n]|\\[^\n\t])
fizchar                      ([^\[\]])

%%

\n 	                         { return LF;         }

">>&"                        { return GTGTAMP;    }

">>"                         { return GTGT;       }

">&"                         { return GTAMP;      }

">"                          { return GT;         }

"<"                          { return LT;         }

"&"                          { return AMP;        }

"|"                          { return PIPE;       }

[ \t] 	                     { /* discard */      }

\[\[{fizchar}*\]\]           {
                               char *tmp = Path::trim(yytext, '[');
                               yylval.string_val = Path::trim(tmp, ']');
                               free(tmp);
                               return FIZ;
                             }

\`.*\`                       {
                               yylval.string_val = Path::trim(yytext, '`');
                               return SUBSHELL;
                             }

\"[^\"]*\"                   {
                               yylval.string_val = Path::trim(yytext, '"');
                               return WORD;
                             }

{char}+                      {
                               yylval.string_val = strdup(yytext);
                               return WORD;
                             }
