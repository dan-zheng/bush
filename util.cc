#include <string.h>
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
