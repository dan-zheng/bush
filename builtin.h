#ifndef BUILTIN_H_
#define BUILTIN_H_

#include <map>
#include <string.h>

struct StringComp : public std::binary_function<const char*, const char*, bool> {
  bool operator()(char const *a, char const *b) const {
    return strcmp(a, b) < 0;
  }
};

typedef void (*BuiltInFunc)(char**);
typedef std::map<const char*, BuiltInFunc, StringComp> FuncMap;

class BuiltIn {
private:
  static FuncMap map;
public:

  static void init();

  static void reg(const char*, BuiltInFunc);
  static BuiltInFunc get(const char*);

  static void _exit();
};

void __cd(char**);

#endif
