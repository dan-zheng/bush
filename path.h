#ifndef PATH_H_
#define PATH_H_

#include <string.h>
#include "global.h"
#include "trace.h"

class Path {
private:

public:
  static char* cwd();

  static char* join(char*, char*);
  static char* popd(char**);
  static void  pushd(char**, char*);

  static void  glob2rgx(char**);
};

#endif
