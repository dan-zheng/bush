#include <stdlib.h>
#include <unistd.h>
#include <map>

#include "global.h"
#include "trace.h"
#include "builtin.h"

extern char **environ;
FuncMap BuiltIn::map;

void
BuiltIn::init() {
  DBG_VERBOSE("BuiltIn::init()\n");
  BuiltIn::reg("cd", __cd);
  BuiltIn::reg("setenv", __setenv);
  BuiltIn::reg("unsetenv", __unsetenv);
  BuiltIn::reg("printenv", __printenv);
}

void
BuiltIn::reg(const char* name, BuiltInFunc fn) {
  map[name] = fn;
}

BuiltInFunc
BuiltIn::get(const char* name) {
  return map[name];
}

void
BuiltIn::_exit() {
  DBG_INFO("BuiltIn::exit() : Exiting.\n");
  exit(2);
}

// Built-in functions
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

void
__setenv(char* args[]) {
  DBG_VERBOSE("__setenv() : %s=%s\n", args[1], args[2] ? args[2] : "(empty)");
  setenv(args[1], args[2] ? args[2] : "", 1);
}

void
__unsetenv(char* args[]) {
  DBG_VERBOSE("__unsetenv() : %s\n", args[1]);
  unsetenv(args[1]);
}

void
__printenv(char* args[]) {
  DBG_VERBOSE("__printenv() : (no arguments)\n");
  char** p = environ;
  while (*p) {
    printf("%s\n", *p++);
  };
}
