#ifndef BUILTIN_H_
#define BUILTIN_H_

#include <map>

typedef void (*BuiltInFunc)(char**);

class BuiltIn {
private:
  static std::map<const char*, BuiltInFunc> map;
public:

  static void init();

  static void reg(const char*, BuiltInFunc);
  static BuiltInFunc get(const char*);

  // Builtin functions here
  static void _exit(char**);
  static void _cd(char**);
};

#endif
