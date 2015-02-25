// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// builtin.h                                                                 //
// This file contains definitions for logic dealing with builtin shell       //
// commands, such as `cd` or `setenv`.                                       //
//                                                                           //
// ------------------------------------------------------------------------- //

#ifndef BUILTIN_H_
#define BUILTIN_H_

#include <map>
#include <string.h>


// ------------------------------------------------------------------------- //
// Comparator for builtin command map.                                       //
// ------------------------------------------------------------------------- //
struct StringComp :
  public std::binary_function<const char*, const char*, bool> {
    bool operator()(char const *a, char const *b) const {
      return strcmp(a, b) < 0;
    }
};

// ------------------------------------------------------------------------- //
// Shorthand for function pointer handling builtin commands.                 //
// ------------------------------------------------------------------------- //
typedef void (*BuiltInFunc)(char**);

// ------------------------------------------------------------------------- //
// Shorthand for internal map storing builtin command handlers.              //
// ------------------------------------------------------------------------- //
typedef std::map<const char*, BuiltInFunc, StringComp> FuncMap;

// ------------------------------------------------------------------------- //
// Static class that acts as a builtin command index.                        //
// ------------------------------------------------------------------------- //
class BuiltIn {
private:
  // Map that stores command handlers
  static FuncMap map;
public:

  // Initializes the BuiltIn class
  static void init();

  // Registering/Finding stored command handlers
  static void reg(const char*, BuiltInFunc);
  static BuiltInFunc get(const char*);

  // `exit` is considered specian since it runs before executing
  // "child" commands.
  static void _exit();
};

// ------------------------------------------------------------------------- //
// Handler for the builtin command `cd`.                                     //
// ------------------------------------------------------------------------- //
void __cd(char**);

// ------------------------------------------------------------------------- //
// Handler for the builtin command `debug`. Disabled in production.          //
// ------------------------------------------------------------------------- //
#if DEBUG > DBG_LVL_NONE
void __debug(char**);
#endif

#endif
