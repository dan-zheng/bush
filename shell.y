
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

  command: simple_command;

  simple_command:
  	command_and_args iomodifier_opt LF {
  		DBG_VERBOSE("Yacc: Execute command\n");
  		CompoundCommand::current -> execute();
  	} |
    LF |
    error LF { yyerrok; }
  	;

  command_and_args:
  	command_word arg_list {
  		CompoundCommand::current -> pushArgument(SimpleCommand::current);
  	}
  	;

  arg_list:
  	arg_list argument
  	| /* can be empty */
  	;

  argument:
  	WORD {
      DBG_VERBOSE("Yacc: insert argument \"%s\"\n", $1);
  	  SimpleCommand::current -> pushArgument($1);
  	}
  	;

  command_word:
  	WORD {
      DBG_VERBOSE("Yacc: insert command \"%s\"\n", $1);
      SimpleCommand::current = new SimpleCommand();
      SimpleCommand::current -> pushArgument($1);
  	}
  	;

  iomodifier_opt:
  	GT WORD {
      DBG_VERBOSE("Yacc: insert output \"%s\"\n", $2);
  		CompoundCommand::current -> out = $2;
  	}
  	| /* can be empty */
  	;

%%

void yyerror(const char *s) {
  WARN("%s", s);
  prompt();
}

#if 0
  main() { yyparse(); }
#endif
