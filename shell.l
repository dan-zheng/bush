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

%}

char        ([^ |><\t\n]|\\[^\n\t])

%%

\n 	        { return LF;         }

">>&"       { return GTGTAMP;    }

">>"        { return GTGT;       }

">&"        { return GTAMP;      }

">"         { return GT;         }

"<"         { return LT;         }

"&"         { return AMP;        }

"|"         { return PIPE;       }

[ \t] 	    { /* discard */      }

\"[^\"]*\"  {
              yylval.string_val = Path::trim(yytext, '"');
              return WORD;
            }

{char}+     {
            	yylval.string_val = strdup(yytext);
            	return WORD;
            }
