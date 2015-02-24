// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// util.cc                                                                   //
// This file contains logic for utility functions, ones that do little but   //
// important things, but don't seem to fit anywhere else.                    //
//                                                                           //
// ------------------------------------------------------------------------- //

#include <string.h>
#include <string>
#include "trace.h"
#include "util.h"

// ------------------------------------------------------------------------- //
// Removes backslashes from escape sequences.                                //
// Does not support interpreting excape sequences such as \n or \t.          //
// ------------------------------------------------------------------------- //
void
Util::unescape(char* str) {
  char *pw = str, *pr = str;
  while (*pr) {
    if (*pr == '\\') { pr++; }
    *pw++ = *pr++;
  }
  *pw = 0;
}

// ------------------------------------------------------------------------- //
// Replaces all occurences of `find` in `str` with `replace`.                //
// ------------------------------------------------------------------------- //
void
Util::replace(std::string *str, const char *find, const char *replace) {
  int pos  = 0,
      flen = strlen(find),
      rlen = strlen(replace);
  while ((pos = str->find(find, pos)) != std::string::npos) {
    str -> replace(pos, flen, replace);
    pos += rlen;
  }
}
