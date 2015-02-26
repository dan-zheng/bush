#ifndef GLOBBER_H_
#define GLOBBER_H_

#include <regex.h>
#include <string.h>
#include <list>
#include "command.hpp"
#include "trace.hpp"
#include "path.hpp"

typedef regex_t Regex;
typedef struct dirent DirEntry;
typedef struct stat   DirStat;
typedef struct std::list<Path*> MatchList;

class Globber {
private:
  Path        *root;
  Path        *glob;
  MatchList   *matches;

  void   glob_segment(char*);
  void   glob_regex(Path*, Regex*, MatchList*);
  void   glob_single(Path*, char*, MatchList*);

  static Path*  cwd();
  static Regex* toRegex(const char*);

public:

  Globber(const char*);
  ~Globber();

  void run();
  void output(ArgList*);
  int  count();
};

#endif
