#ifndef PATH_H_
#define PATH_H_

#include <regex.h>
#include <string.h>
#include <queue>
#include "global.h"
#include "trace.h"

//#define PATH_DISABLE_DBG
#ifdef PATH_DISABLE_DBG

#undef DBG_VERBOSE
#undef DBG_VERBOSE_N

#define DBG_VERBOSE(...)
#define DBG_VERBOSE_N(...)

#endif

typedef struct dirent DirEntry;
typedef struct stat   DirStat;
typedef std::queue<char*> Queue;

class Path {
private:

public:
  static char*     cwd();

  static char*        join(char*, char*);
  static char*        popd(char**);
  static char*        pushd(char*, char*);

  static regex_t*     glob2rgx(char*);
  static char**       glob(char*);
};

#endif
