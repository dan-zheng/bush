// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// main.h                                                                    //
// This file contains logic for globally available functions such as         //
// signal handler and prompt().                                              //
//                                                                           //
// ------------------------------------------------------------------------- //
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "main.hpp"
#include "trace.hpp"
#include "command.hpp"
#include "builtin.hpp"
#include "plumber.hpp"

#ifndef OS_X
#include "lib/tty.h"
#endif

// Forward declarations
SimpleCommand   *SimpleCommand::current;
CompoundCommand *CompoundCommand::current;

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

  // Initialize the global CompoundCommand instance
  CompoundCommand::current = new CompoundCommand();

  // Do the YACC magic...
  prompt();
  yyparse();
  return 0;
}
