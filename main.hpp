// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// main.h                                                                    //
// This file contains definitions for globally available functions as well   //
// as actual actions that parser takes when it encounters the rules defined  //
// in the shell.y file.                                                      //
//                                                                           //
// ------------------------------------------------------------------------- //

#ifndef MAIN_H_
#define MAIN_H_

#include <signal.h>
#include "command.hpp"

// ------------------------------------------------------------------------- //
// Parser class that does the actual parser work.                            //
// ------------------------------------------------------------------------- //
class Parser {
private:
  int               error;           // Flag indicating an error.
  SimpleCommand    *partial;         // Partial command currently being parsed.
  CompoundCommand  *command;         // Full command currently being parsed.

public:
  Parser();                          // Constructor.
  ~Parser();                         // Destructor, calls clear().

  void clear();                      // Clears all partials and their content.

  void execute();                    // Executes the current command.
  void newline();                    // Handles the empty command.
  void background();                 // Makes the command run in background.
  void partial_end();                // Pushes the partial into the command.
  void partial_make(char*);          // Creates a partial.
  void partial_arg(char*);           // Pushes an argument into the partial.
  void subshell_arg(char*);          // Pushes a subshell into the partial.
  void fiz_arg(char*);               // Pushes a FIZ script into the partial.

  void in_file(char*);               // Specifies input file redirection.
  void out_file(char*, int, int);    // Specifies output file redirection.
};

// ------------------------------------------------------------------------- //
// Global instance of Parser mostly just for shell.y to access.              //
// ------------------------------------------------------------------------- //
extern Parser *parser;

extern struct sigaction sigAction;

// ------------------------------------------------------------------------- //
// Called to start the YACC parsing process.                                 //
// ------------------------------------------------------------------------- //
int  yylex();

// ------------------------------------------------------------------------- //
// Handles the YACC errors.                                                  //
// ------------------------------------------------------------------------- //
void yyerror(const char*);

// ------------------------------------------------------------------------- //
// Handles the signal specified by its argument.                             //
// ------------------------------------------------------------------------- //
void signal(int);

// ------------------------------------------------------------------------- //
// Prints an input prompt (only when in TTY mode).                           //
// ------------------------------------------------------------------------- //
void prompt(void);

// ------------------------------------------------------------------------- //
// YACC parse function definition.                                           //
// ------------------------------------------------------------------------- //
int yyparse(void);

#endif
