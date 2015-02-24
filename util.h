// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// util.h                                                                    //
// This file contains definitions for utility functions, ones that do        //
// little but important things, but don't seem to fit anywhere else.         //
//                                                                           //
// ------------------------------------------------------------------------- //

#ifndef UTIL_H_
#define UTIL_H_

#include <string>

// ------------------------------------------------------------------------- //
// Static Util class, used as a container for utility functions.             //
// ------------------------------------------------------------------------- //
class Util {
public:
  // Removes backslashes from escape sequences.
  // Does not support interpreting excape sequences such as \n or \t
  static void  unescape(char*);
  // Replaces all occurences of given string in a std::string
  static void  replace(std::string*, const char*, const char*);
};

#endif
