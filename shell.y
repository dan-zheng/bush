
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

%union	{ char   *string_val; }

%{
  #include <stdio.h>
  #include "main.h"
  #include "trace.h"
  #include "global.h"
  #include "command.h"

  void yyerror(const char*);
  int yylex();

  int error = 0;
%}

%%

  goal: commands;

  commands: command | commands command;

  command: partials redirects background LF {
      if (error) {
        DBG_WARN("Command execution blocked by error.\n");
        error = 0;
        prompt();
      } else {
        DBG_VERBOSE("Yacc: Execute command\n");
        CompoundCommand::current -> execute();
        prompt();
      }
    }  |
    LF {
      prompt();
    } |
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
      CompoundCommand::current -> push(SimpleCommand::current);
    }
    ;
  cmd_word:
    WORD {
      DBG_VERBOSE("Yacc: Insert command \"%s\"\n", $1);
      SimpleCommand::current = new SimpleCommand();
      SimpleCommand::current -> push($1);
    }
    ;

  // ----------------------------------------------------------------------- //
  // Argument list                                                           //
  //                                                                         //
  //   [arg]*                                                                //
  //                                                                         //
  // ----------------------------------------------------------------------- //
  arguments: argument arguments |;
  argument:
    WORD {
      DBG_VERBOSE("Yacc: Insert argument \"%s\"\n", $1);
      SimpleCommand::current -> push($1);
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
  redirects: redirect redirects |;
  redirect:
  	GT WORD {
      char *out = CompoundCommand::current -> out;
      char *err = CompoundCommand::current -> err;

      if (out) { free(out); }

      DBG_VERBOSE("Yacc: Redirect stdout to \"%s\"\n", $2);
      CompoundCommand::current -> out = $2;

      if (out && out == err) {
        DBG_VERBOSE("Yacc: Redirect stderr to \"%s\"\n", $2);
        CompoundCommand::current -> err = $2;
      }
  	} |
    GTGT WORD {
      char *out = CompoundCommand::current -> out;
      char *err = CompoundCommand::current -> err;

      if (out) { free(out); }

      DBG_VERBOSE("Yacc: Append stdout to \"%s\"\n", $2);
      CompoundCommand::current -> append =  1;
  		CompoundCommand::current -> out    = $2;

      if (out && out == err) {
        DBG_VERBOSE("Yacc: Redirect stderr to \"%s\"\n", $2);
        CompoundCommand::current -> err = $2;
      }
    } |
    GTAMP WORD {
      char *out = CompoundCommand::current -> out;
      char *err = CompoundCommand::current -> err;

      if (out) { free(out); }

      DBG_VERBOSE("Yacc: Redirect stdout and stderr to \"%s\"\n", $2);
      CompoundCommand::current -> out = $2;
      CompoundCommand::current -> err = $2;
    } |
    GTGTAMP WORD {

      char *out = CompoundCommand::current -> out;
      char *err = CompoundCommand::current -> err;

      if (out) { free(out); }

      DBG_VERBOSE("Yacc: Append stdout and stderr to \"%s\"\n", $2);
      CompoundCommand::current -> append =  1;
      CompoundCommand::current -> out    = $2;
      CompoundCommand::current -> err    = $2;
    } |
    LT WORD {

      if (CompoundCommand::current -> in) {
        fprintf(stderr, RED("Error: Ambiguous input redirect.\n"));
        error = 1;
      }

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
  fprintf(stderr, "-" SH_NAME ": %s\n", s);
  CompoundCommand::current -> clear();
  prompt();
}

#if 0
  main() { yyparse(); }
#endif
