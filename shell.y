
/*
 * CS-252 Spring 2015
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * You must extend it to understand the complete shell grammar.
 * cmd [arg]* [ | cmd [arg]* ]* [ [> filename] [< filename] [ >& filename] [>> filename] [>>& filename] ]* [&]
 *
 */

%token	<string_val> WORD

%token 	NOTOKEN LF GT LT AMP GTGT GTAMP GTGTAMP PIPE

%union	{
	char   *string_val;
}

%{
  #include <stdio.h>
  #include "main.h"
  #include "trace.h"
  #include "command.h"

  void yyerror(const char*);
  int yylex();
%}

%%

  goal: commands;

  commands: command | commands command;

  command: partials redirects background LF {
      DBG_VERBOSE("Yacc: Execute command\n");
      CompoundCommand::current -> execute();
    }  |
    LF |
    error LF { yyerrok; }
  ;

  // ----------------------------------------------------------------------- //
  // Partial command list                                                    //
  //                                                                         //
  //   cmd [arg]* [ | cmd [arg]* ]*                                          //
  //                                                                         //
  // ----------------------------------------------------------------------- //
  partials: partial | partial PIPE partials;


  // ----------------------------------------------------------------------- //
  // Partial command                                                         //
  //                                                                         //
  //   cmd [arg]*                                                            //
  //                                                                         //
  // ----------------------------------------------------------------------- //
  partial:
    cmd_word arguments {
      CompoundCommand::current -> pushArgument(SimpleCommand::current);
    }
    ;
  cmd_word:
    WORD {
      DBG_VERBOSE("Yacc: insert command \"%s\"\n", $1);
      SimpleCommand::current = new SimpleCommand();
      SimpleCommand::current -> pushArgument($1);
    }
    ;

  // ----------------------------------------------------------------------- //
  // Argument list                                                           //
  //                                                                         //
  //   [arg]*                                                                //
  //                                                                         //
  // ----------------------------------------------------------------------- //
  arguments: argument | argument arguments |;
  argument:
    WORD {
      DBG_VERBOSE("Yacc: insert argument \"%s\"\n", $1);
      SimpleCommand::current -> pushArgument($1);
    }
    ;

  // ----------------------------------------------------------------------- //
  // IO Redirects                                                            //
  //                                                                         //
  //   [                                                                     //
  //     [>  filename]                                                       //
  //     [<  filename]                                                       //
  //     [>& filename]                                                       //
  //     [>> filename]                                                       //
  //     [>>& filename]                                                      //
  //   ]*                                                                    //
  //                                                                         //
  // ----------------------------------------------------------------------- //
  redirects: redirect | redirect redirects |;
  redirect:
  	GT WORD {
      DBG_VERBOSE("Yacc: Redirect stdout to \"%s\"\n", $2);
      CompoundCommand::current -> nf  = 1;
  		CompoundCommand::current -> out = $2;
  	} |
    GTGT WORD {
      DBG_VERBOSE("Yacc: Append stdout to \"%s\"\n", $2);
      CompoundCommand::current -> nf  = 0;
  		CompoundCommand::current -> out = $2;
    } |
    GTAMP WORD {
      DBG_VERBOSE("Yacc: Redirect stdout and stderr to \"%s\"\n", $2);
      CompoundCommand::current -> nf  = 1;
      CompoundCommand::current -> out = $2;
      CompoundCommand::current -> err = $2;
    } |
    GTGTAMP WORD {
      DBG_VERBOSE("Yacc: Append stdout and stderr to \"%s\"\n", $2);
      CompoundCommand::current -> nf  = 0;
      CompoundCommand::current -> out = $2;
      CompoundCommand::current -> err = $2;
    } |
    LT WORD {
      DBG_VERBOSE("Yacc: Redirect stdin from \"%s\"\n", $2);
      CompoundCommand::current -> in  = $2;
    }
  	;

  // ----------------------------------------------------------------------- //
  // Background flag                                                         //
  //                                                                         //
  //   [&]                                                                   //
  //                                                                         //
  // ----------------------------------------------------------------------- //
  background:
    AMP {
      DBG_VERBOSE("Yacc: Enabling command backround flag\n");
      CompoundCommand::current -> bg = 1;
    } |
    /* empty */
    ;
%%

void yyerror(const char *s) {
  WARN("%s", s);
  prompt();
}

#if 0
  main() { yyparse(); }
#endif
