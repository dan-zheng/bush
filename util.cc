#include <string.h>
#include <string>
#include "trace.h"
#include "util.h"

void
Util::unescape(char* str) {
  char *pw = str, *pr = str;
  while (*pr) {
    if (*pr == '\\') { pr++; }
    *pw++ = *pr++;
  }
  *pw = 0;
}

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
