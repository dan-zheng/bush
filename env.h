// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// env.h                                                                     //
// This file contains definitions for logic dealing with environment vars.   //
//                                                                           //
// ------------------------------------------------------------------------- //

#ifndef ENV_H_
#define ENV_H_

// ------------------------------------------------------------------------- //
// Static class with methods for `*env` command handlers.                    //
// ------------------------------------------------------------------------- //
class Env {
public:
  static void set(char**);      // Sets an environment variable
  static void unset(char**);    // Deletes an environment variable
  static void print(char**);    // Prints all environment variables
  static int  expand(char**);   // Replaces ${...} with environment variable
                                // values
  static int  tilde(char**);    // Replaces ~.../ with home dir path
};

#endif
