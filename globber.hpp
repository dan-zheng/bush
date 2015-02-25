#ifndef GLOBBER_H_
#define GLOBBER_H_

#include <regex.h>
#include <string.h>
#include <queue>
#include "trace.hpp"
#include "path.hpp"

typedef regex_t Regex;
typedef struct dirent DirEntry;
typedef struct stat   DirStat;
typedef struct std::queue<Path*> Queue;

class Globber {
private:
  Path    *root;
  Path    *glob;
  Queue   *matches;

  void   glob_segment(char*);
  void   glob_regex(Path*, Regex*, Queue*);
  void   glob_single(Path*, char*, Queue*);

  static Path*  cwd();
  static Regex* toRegex(const char*);

public:

  Globber(const char*);
  ~Globber();

  void run();
};

#endif
