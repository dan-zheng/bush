#ifndef UTIL_H_
#define UTIL_H_

#include <string>

class Util {
public:
  static void  unescape(char*);
  static void  replace(std::string*, const char*, const char*);
};

#endif
