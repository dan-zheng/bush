#include <string.h>
#include "util.h"

void
Util::unescape(char* str) {
  char *pw = str;
  char *pr = str;
  while (*pr) {
    if (*pr == '\\') { pr++; }
    *pw++ = *pr++;
  }
  *pw = 0;
}
