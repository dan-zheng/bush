// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// main.cc                                                                   //
// This file contains the logic for globally available functions as well     //
// as actual actions that parser takes when it encounters the rules defined  //
// in the shell.y file.                                                      //
//                                                                           //
// ------------------------------------------------------------------------- //
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "env.hpp"
#include "main.hpp"
#include "path.hpp"
#include "trace.hpp"
#include "globber.hpp"
#include "command.hpp"
#include "builtin.hpp"
#include "plumber.hpp"

#ifndef OS_X
#include "lib/tty.h"
#endif

// Forward declaration
Parser *parser;

// ------------------------------------------------------------------------- //
// Constructor. Nothing fancy here.                                          //
// ------------------------------------------------------------------------- //
Parser::Parser() {
  error   = 0;
  command = new CompoundCommand();
}

// ------------------------------------------------------------------------- //
// Destructor, calls clear() under the hood.                                 //
// ------------------------------------------------------------------------- //
Parser::~Parser() {
  clear();
}

// ------------------------------------------------------------------------- //
// Clears all partials and their content.                                    //
// ------------------------------------------------------------------------- //
void
Parser::clear() {
  command -> clear();
  if (partial) { delete partial; }
  partial = NULL;
}

// ------------------------------------------------------------------------- //
// Executes the current command, then clears the command afterwards.         //
// ------------------------------------------------------------------------- //
void
Parser::execute() {
  if (error) {
    DBG_WARN("Command execution blocked by error.\n");
    command -> clear();
    error = 0;
    prompt();
  } else {
    DBG_VERBOSE("Yacc: Execute command\n");
    command -> execute();
    prompt();
  }
}

// ------------------------------------------------------------------------- //
// Handles the empty command because we can't exactly execute() those.       //
// ------------------------------------------------------------------------- //
void
Parser::newline() {
  if (!error) {
    prompt();
  }
}

// ------------------------------------------------------------------------- //
// Makes the command run in background.                                      //
// ------------------------------------------------------------------------- //
void
Parser::background() {
  DBG_VERBOSE("Yacc: Enabling command backround flag\n");
  command -> bg = 1;
}

// ------------------------------------------------------------------------- //
// Finalizes the partial and pushes it into the command.                     //
// ------------------------------------------------------------------------- //
void
Parser::partial_end() {
  command -> push(partial);
  partial = NULL;
}

// ------------------------------------------------------------------------- //
// Creates a partial and pushes in its first argument right away.            //
// ------------------------------------------------------------------------- //
void
Parser::partial_make(char *arg) {
  DBG_VERBOSE("Yacc: Insert command \"%s\"\n", arg);
  partial = new SimpleCommand();
  partial -> push(arg);
}

// ------------------------------------------------------------------------- //
// Pushes an argument into the partial.                                      //
// ------------------------------------------------------------------------- //
void
Parser::partial_arg(char *arg) {
  Path::unescape(arg);
  if (Env::expand(&arg) || Env::tilde(&arg)) { error = 1; }
  // Glob if it looks like a glob pattern
  if (strstr(arg, "*") || strstr(arg, "?")) {
    Globber *g = new Globber(arg);
    g -> run();
    // If there are matches, insert them into arguments
    if (g -> count()) {
      g -> output(partial -> args);
      free(arg);
    }
    // No matches, so just insert the pattern
    else {
      DBG_WARN("Parser::partial_arg(): No wildcard match.\n");
      partial -> push(arg);
    }
    delete g;
  } else {
    DBG_VERBOSE("Yacc: Insert argument \"%s\"\n", arg);
    partial -> push(arg);
  }
}

// ------------------------------------------------------------------------- //
// Specifies input file redirection (more specifically, `< filename`).       //
// ------------------------------------------------------------------------- //
void
Parser::in_file(char *file) {
  if (command -> in) {
    COMPLAIN("Ambiguous input redirect.");
    error = 1;
  }
  else {
    DBG_VERBOSE("Yacc: Redirect stdin from \"%s\"\n", file);
    command -> in  = file;
  }
}

// ------------------------------------------------------------------------- //
// Specifies output file redirection (`[>|>>|>&|>>&] filename`).             //
// ------------------------------------------------------------------------- //
void
Parser::out_file(char *file, int err, int append) {
  if (command -> out) {
    COMPLAIN("Ambiguous output redirect.");
    error = 1;
  }
  else {
    DBG_VERBOSE("Yacc: Redirect stdout to \"%s\"\n", file);
    command -> append = append;
    command -> out = file;
    if (err) { command -> err = file; }
  }
}


// ------------------------------------------------------------------------- //
// Handles the signal specified by its first argument (n).                   //
// ------------------------------------------------------------------------- //
void
signal(int n) {
  switch (n) {
    case SIGINT:
    case SIGCHLD: {
      int status;
      int pid = wait(&status);
      if (pid != -1) { kill(getpid(), n); }
    } break;
  }
}

// ------------------------------------------------------------------------- //
// YACC error handler.                                                       //
// ------------------------------------------------------------------------- //
void
yyerror(const char *s) {
  COMPLAIN("%s", s);
  parser -> clear();
  prompt();
}

// ------------------------------------------------------------------------- //
// Prints an input prompt (only when in TTY mode).                           //
// ------------------------------------------------------------------------- //
void
prompt(void) {
  if (isatty(0)) { printf(LGREEN("%s> "), SH_NAME); }
  fflush(stdout);
}

// ------------------------------------------------------------------------- //
// main() function for the shell.                                            //
// ------------------------------------------------------------------------- //
int
main(int argc, char **argv) {

  // Handle signals if feature level is above FL_PART3
  #if FEATURE_LEVEL >= FL_PART3
  sigset(SIGINT,  signal);
  sigset(SIGCHLD, signal);
  #endif

  // If in TTY, print enabled debug flags for reference
  // This block is no-op if DEBUG is set to DBG_LVL_NONE
  if (isatty(0)) {
    printf(LGRAY(SH_BANNER));

    printf(LGRAY("BUSH - Blatantly Useless Shell\n"));
    printf(LGRAY("Copyright (C) Denis Luchkin-Zhou (build %s)\n"), __TIMESTAMP__);

    // Print enabled debug levels
    DBG_ERR_N(DGRAY("DEBUG FLAGS : "));
    DBG_ERR_N(LRED("ERROR"));
    DBG_ERR_N(" ");
    DBG_WARN_N(YELLOW("WARNING"));
    DBG_WARN_N(" ");
    DBG_INFO_N(LBLUE("INFO"));
    DBG_INFO_N(" ");
    DBG_VERBOSE_N(DGRAY("VERBOSE"));
    DBG_ERR_N("\n\n");
  }

  // Initialize Plumber and BuiltIn
  Plumber::init();
  BuiltIn::init();
  parser = new Parser();

  // Do the YACC magic...
  prompt();
  yyparse();
  return 0;
}
