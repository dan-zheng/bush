// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// builtin.cc                                                                //
// This file contains logic dealing with builtin shell commands, such as     //
// `cd` or `setenv`.                                                         //
//                                                                           //
// ------------------------------------------------------------------------- //

#include <stdlib.h>
#include <map>

#include "env.hpp"
#include "path.hpp"
#include "trace.hpp"
#include "builtin.hpp"
#include "globber.hpp"

// Forward declaration
FuncMap BuiltIn::map;

// ------------------------------------------------------------------------- //
// Initializes BuiltIn class and registers builtin commands.                 //
// ------------------------------------------------------------------------- //
void
BuiltIn::init() {
  DBG_VERBOSE("BuiltIn::init()\n");
  BuiltIn::reg("cd", __cd);
  BuiltIn::reg("setenv", Env::set);
  BuiltIn::reg("unsetenv", Env::unset);
  BuiltIn::reg("printenv", Env::print);
  #if DEBUG > DBG_LVL_NONE
  BuiltIn::reg("debug", __debug);
  #endif
}

// ------------------------------------------------------------------------- //
// Registers a builtin command with the specified handler.                   //
//   name :  Command name, such as "cd"                                      //
//   fn   :  Command handler, a function pointer.                            //
// ------------------------------------------------------------------------- //
void
BuiltIn::reg(const char* name, BuiltInFunc fn) {
  map[name] = fn;
}

// ------------------------------------------------------------------------- //
// Gets a command handler by name. Performs no error checking.               //
//   name :  Command name, such as "cd"                                      //
// ------------------------------------------------------------------------- //
BuiltInFunc
BuiltIn::get(const char* name) {
  return map[name];
}

// ------------------------------------------------------------------------- //
// Special handler for `exit` command that is called directly.               //
// ------------------------------------------------------------------------- //
void
BuiltIn::_exit() {
  DBG_INFO("BuiltIn::exit() : Exiting.\n");
  exit(2);
}

// ------------------------------------------------------------------------- //
// COMMAND HANDLER: `cd [ dir ]`                                             //
// ------------------------------------------------------------------------- //
void
__cd(char* args[]) {
  DBG_INFO("__cd() : %s\n", args[1]);
  if (args[1]) {
    if (chdir(args[1]) < 0) {
      COMPLAIN("cd: %s: No such file or directory.", args[1]);
    }
  } else {
    chdir(getenv("HOME"));
  }
}

// ------------------------------------------------------------------------- //
// COMMAND HANDLER: `debug [ arg ]*`                                         //
// ------------------------------------------------------------------------- //
#if DEBUG > DBG_LVL_NONE
void
__debug(char* args[]) {
  /*
  Path *p = new Path(args[1]);
  char *s = p -> str();
  printf("%s\n", s);
  free(s);
  delete p;
  */
  Path *p = new Path(args[1]);
  p -> push(args[2]);
  p -> reduce();
  char *s = p -> str();
  printf("%s\n", s);
  free(s);
  delete p;

}
#endif
