#include <stdlib.h>
#include <map>

#include "global.h"
#include "trace.h"
#include "builtin.h"

std::map<const char*, BuiltInFunc> BuiltIn::map;

void
BuiltIn::init() {
  DBG_VERBOSE("BuiltIn::init()\n");
  //BuiltIn::reg("exit", &BuiltIn::_exit);
  //BuiltIn::reg("cd", &BuiltIn::_cd);

}

void
BuiltIn::reg(const char* name, BuiltInFunc fn) {
  map[name] = fn;
}

BuiltInFunc
BuiltIn::get(const char* name) {
  return map[name];
}


// Built-in functions
void
BuiltIn::_exit(char**) {
  DBG_INFO("BuiltIn::exit() : Exiting.\n");
  exit(2);
}

void
BuiltIn::_cd(char** args) {
  DBG_WARN("BuiltIn::cd() : Not implemented.\n");
}
