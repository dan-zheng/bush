// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// global.h                                                                  //
// This file contains global configuration macros such as debug level.       //
//                                                                           //
// ------------------------------------------------------------------------- //

#ifndef GLOBAL_H_
#define GLOBAL_H_

// ------------------------------------------------------------------------- //
// Name of this shell as displayed in prompt.                                //
// ------------------------------------------------------------------------- //
#define SH_NAME "myshell"

// ------------------------------------------------------------------------- //
// Feature levels: allows turning off experimental features so that a more   //
// version of the shell can be submitted for grading without those features  //
// interfering with the output.                                              //
// ------------------------------------------------------------------------- //
#define FLVL_PART1 0
#define FLVL_PART2 1
#define FLVL_PART3 2
#define FLVL_EXTRA 3

// Default feature level to maximum
#ifndef FEATURE_LEVEL
  #define FEATURE_LEVEL 4
#endif

#endif
