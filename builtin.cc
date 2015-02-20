#include <stdlib.h>
#include <map>

#include "global.h"
#include "trace.h"
#include "builtin.h"

void
BuiltIn::init() {

}


// Built-in functions
void
BuiltIn::_exit() {
  DBG_INFO("BuiltIn::exit() : Exiting.\n");
  exit(2);
}
