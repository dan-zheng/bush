// ------------------------------------------------------------------------- //
// Plumber: heard you have pipes that need fixing..?                         //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
// ------------------------------------------------------------------------- //
#ifndef PLUMBER_H_
#define PLUMBER_H_

#define PLB_NONE  -1
#define PLB_PIPE  -2

// Plumber: takes care of I/O, pipes and redirection.
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

  static int  file(char*, char*, char*, int);               // Specify an input file

  static int  std(int);                 // Get a default I/O handle
};

#endif
