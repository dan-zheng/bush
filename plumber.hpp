// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// plumber.h                                                                 //
// This file contains definitions for Plumber, the pipe master of the shell. //
//                                                                           //
// ------------------------------------------------------------------------- //
#ifndef PLUMBER_H_
#define PLUMBER_H_

// ------------------------------------------------------------------------- //
// Shorthands for open() flags.                                              //
// ------------------------------------------------------------------------- //
#define F_READ   O_RDONLY
#define F_FLAGS  S_IRUSR  | S_IWUSR | S_IXUSR
#define F_TRUNC  O_WRONLY | O_CREAT | O_TRUNC
#define F_APPEND O_WRONLY | O_CREAT | O_APPEND

// ------------------------------------------------------------------------- //
// Plumber redirect constants.                                               //
//   PLB_NONE : No redirect; or redirect to file if one is specified.        //
//   PLB_PIPE : Redirect to an appropriate pipe held by Plumber.             //
// ------------------------------------------------------------------------- //
#define PLB_NONE  -1
#define PLB_PIPE  -2

// ------------------------------------------------------------------------- //
// Plumber the pipe master.                                                  //
// ------------------------------------------------------------------------- //
class Plumber {
private:
  static int       _def[3];              // Default I/O handles
  static int       _file[3];             // File I/O handles
  static int       _ipipe[2];            // Input pipe
  static int       _opipe[2];            // Output pipe
public:
  static void init();                   // Initializes the Plumber

  static void capture();                // Captures current I/O handles
  static void restore();                // Restores captured I/O handles
  static void clear();                  // Clears handles managed by Plumber
  static void push();                   // Push a new pipe into the Plumber
  static void redirect(int, int, int);  // Redirect I/O as specified

  static int  file(char*, char*, char*, int); // Specify standard I/O files

  static int  std(int);                 // Get a default I/O handle
};

#endif
