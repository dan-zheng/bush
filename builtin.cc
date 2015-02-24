#include <stdlib.h>
#include <map>

#include "env.h"
#include "path.h"
#include "global.h"
#include "trace.h"
#include "builtin.h"

FuncMap BuiltIn::map;

void
BuiltIn::init() {
  DBG_VERBOSE("BuiltIn::init()\n");
  BuiltIn::reg("cd", __cd);
  BuiltIn::reg("setenv", Env::set);
  BuiltIn::reg("unsetenv", Env::unset);
  BuiltIn::reg("printenv", Env::print);
  BuiltIn::reg("debug", __debug);
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
__debug(char* args[]) {
  //char* result = Path::join(args[1], args[2]);
  //DBG_INFO("Path::join(): path: %s\n", result);
  //free(result);
  //Path::popdir("");
  //Path::glob2rgx(&args[1]);
  //DBG_INFO("Path::glob2rgx(): %s\n", args[1]);
  Queue *list = Path::glob(args[1]);
  while (!list->empty()) {
    char* match = list->front();
    list->pop();

    printf("%s\n", match);
    free(match);
  }
}
