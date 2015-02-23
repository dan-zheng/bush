#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include "env.h"
#include "global.h"
#include "trace.h"

extern char **environ;

void
Env::set(char* args[]) {
  DBG_VERBOSE("Env::set() : %s=%s\n", args[1], args[2] ? args[2] : "(empty)");
  setenv(args[1], args[2] ? args[2] : "", 1);
}

void
Env::unset(char* args[]) {
  DBG_VERBOSE("Env::unset() : %s\n", args[1]);
  unsetenv(args[1]);
}

void
Env::print(char* args[]) {
  DBG_VERBOSE("Env::print() : (no arguments)\n");
  char** p = environ;
  while (*p) { printf("%s\n", *p++); };
}

int
Env::expand(char** str) {
  if (!*str) return 0;

  char        *po     = *str,
              *ps     = po,
              *pe;
  std::string *result = new std::string();

  while ((ps = strstr(ps, "${"))) {
    result -> append(po, ps - po);
    po = ps;

    if (!(pe = strstr(ps, "}"))) break;

    char *varname = strndup(ps + 2, pe - ps - 2);
    char *env     = getenv(varname);

    if (!env) {
      COMPLAIN("%s: Undefined variable.", varname);
      free(varname);
      delete result;
      return 1;
    }

    result -> append(env);
    po = pe + 1;
    DBG_VERBOSE("Env::expand(): %s: %s\n", varname, env);
    free(varname);

    ps = pe + 1;
  }

  result -> append(po);

  free(*str);
  *str = strdup(result -> c_str());
  delete result;
  return 0;
}
