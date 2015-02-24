// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// env.h                                                                     //
// This file contains logic dealing with environment vars.                   //
//                                                                           //
// ------------------------------------------------------------------------- //

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <pwd.h>

#include "env.h"
#include "global.h"
#include "trace.h"

// Environment variable container
extern char **environ;

// ------------------------------------------------------------------------- //
// COMMAND HANDLER: `setenv`: Sets an environment variable.                  //
// ------------------------------------------------------------------------- //
void
Env::set(char* args[]) {
  DBG_VERBOSE("Env::set() : %s=%s\n", args[1], args[2] ? args[2] : "(empty)");
  setenv(args[1], args[2] ? args[2] : "", 1);
}

// ------------------------------------------------------------------------- //
// COMMAND HANDLER: `unsetenv`: Deletes an environment variable.             //
// ------------------------------------------------------------------------- //
void
Env::unset(char* args[]) {
  DBG_VERBOSE("Env::unset() : %s\n", args[1]);
  unsetenv(args[1]);
}

// ------------------------------------------------------------------------- //
// COMMAND HANDLER: `prntenv`: Prints all environment variables.             //
// ------------------------------------------------------------------------- //
void
Env::print(char* args[]) {
  DBG_VERBOSE("Env::print() : (no arguments)\n");
  char** p = environ;
  while (*p) { dprintf(1, "%s\n", *p++); };
}

// ------------------------------------------------------------------------- //
// Replaces ${...} sequences with respective environment variable values.    //
// ------------------------------------------------------------------------- //
int
Env::expand(char** str) {
  if (!*str) return 0;

  char        *po     = *str,       // Copy start
              *ps     = po,         // Sequence start
              *pe;                  // Sequence end
  std::string *result = new std::string();

  while ((ps = strstr(ps, "${"))) {
    // Found sequence start, copy stuff before it to result
    result -> append(po, ps - po);
    po = ps;

    // Look for sequence end
    if (!(pe = strstr(ps, "}"))) break;

    // Copy the name of the environment variable and get its value
    char *varname = strndup(ps + 2, pe - ps - 2);
    char *env     = getenv(varname);

    // What if what you're looking for is not there...
    if (!env) {
      COMPLAIN("%s: Undefined variable.", varname);
      free(varname);
      delete result;
      return 1;
    }

    // Copy the value of the variable to result
    result -> append(env);
    po = pe + 1;
    DBG_VERBOSE("Env::expand(): %s: %s\n", varname, env);
    free(varname);
    ps = pe + 1;
  }

  // Copy whatever left to result
  result -> append(po);

  // Clean up, convert result to C string and return
  free(*str);
  *str = strdup(result -> c_str());
  delete result;
  return 0;
}

// ------------------------------------------------------------------------- //
// Replaces ~ with respective user's home directory path.                    //
// ------------------------------------------------------------------------- //
int
Env::tilde(char** str) {
  // Only handle strings where first character is '~'
  if (!*str) return 0;
  if (**str != '~') return 0;

  // Store user's password entry...
  register struct passwd *pw;

  char        *ps     = *str,               // Start of username
              *pe     = strstr(*str, "/");  // End of username
  std::string *result = new std::string();

  if (!pe) { pe = ps + strlen(*str); }      // No '/' after '~', entire string
                                            // is assumed to be the username

  // No username specified, get currently logged-in user
  if (!pe || pe - ps == 1) {
    pw = getpwuid(geteuid());
  }
  // Copy the username and get user entry
  else {
    char *uname = strndup(ps + 1, pe - ps - 1);
    pw = getpwnam(uname);
    // Just in case the user isn't there...
    if (!pw) {
      COMPLAIN("Unknown user: %s", uname);
      free(uname);
      return 1;
    }
    free(uname);
  }

  // Copy home path to result and append the rest of argument
  result -> append(pw -> pw_dir);
  if (pe) { result -> append(pe); }

  // Clean up, convert result to C string and return
  free(*str);
  *str = strdup(result -> c_str());
  delete result;
  return 0;
}
