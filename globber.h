#ifndef GLOBBER_H_
#define GLOBBER_H_

#include <regex.h>
#include <string.h>
#include <queue>
#include "global.h"
#include "trace.h"

typedef struct dirent DirEntry;
typedef struct stat   DirStat;
typedef std::queue<char*> Queue;

class Globber {
private:
  char        *root;
  char        *glob;
  Queue       *matches;

  void  next_segment(char*);
  void  next_match(char*, regex_t*, Queue*);

  static char*    cwd();
  static regex_t* torgx(const char*);
public:

  Globber(char*);
  ~Globber();

  void  next();

  static char*    join(const char*, const char*);
  static char*    popd(char*);
  static char*    pushd(const char*, const char*);

};

#endif
