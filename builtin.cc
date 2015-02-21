#include <stdlib.h>
#include <map>

#include "global.h"
#include "trace.h"
#include "builtin.h"

FuncMap BuiltIn::map;

void
BuiltIn::init() {
  DBG_VERBOSE("BuiltIn::init()\n");
  BuiltIn::reg("cd", __cd);
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
  DBG_INFO("BuiltIn::cd() : %s\n", args[1]);
  if (args[1]) {
    if (chdir(args[1]) < 0) { COMPLAIN("cd: %s: No such file or directory.", args[1]); }
  } else {
    chdir(getenv("HOME"));
  }
}
