#include <map>

#ifndef BUILTIN_H_
#define BUILTIN_H_

typedef void (*Func)(char**);

class BuiltIn {
public:

  static void init();

  // Builtin functions here
  static void _exit();

};



#endif
