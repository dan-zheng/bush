// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// plumber.cc                                                                //
// This file contains logic for Plumber, the pipe master of the shell.       //
//                                                                           //
// ------------------------------------------------------------------------- //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <sys/stat.h>

#include "trace.hpp"
#include "plumber.hpp"

// Forward declarations
int Plumber::_def[3];
int Plumber::_file[3];
int Plumber::_ipipe[2];
int Plumber::_opipe[2];

// ------------------------------------------------------------------------- //
// Initializes the pipe and performs an initial capture.                     //
// ------------------------------------------------------------------------- //
void
Plumber::init() {
  DBG_VERBOSE("Plubmer::init()\n");

  // Capture current stdin, stdout and stderr
  _def[0]  = dup(0);
  _def[1]  = dup(1);
  _def[2]  = dup(2);

  // Initialize pipes
  if (pipe(_opipe) == -1 || pipe(_ipipe) == -1) {
    perror("Plumber::capture()");
    exit(EXIT_FAILURE);
  }
}

// ------------------------------------------------------------------------- //
// Captures current standard I/O state.                                      //
// ------------------------------------------------------------------------- //
void
Plumber::capture() {
  DBG_VERBOSE("Plubmer::capture()\n");

  // Close the previous capture
  close(_def[0]);
  close(_def[1]);
  close(_def[2]);

  // Capture current stdin, stdout and stderr
  _def[0]  = dup(0);
  _def[1]  = dup(1);
  _def[2]  = dup(2);

  // Duplicate those to file redirects (default no file)
  _file[0] = dup(0);
  _file[1] = dup(1);
  _file[2] = dup(2);
}

// ------------------------------------------------------------------------- //
// Restores last captured standard I/O state, closing all pipes and files.   //
// ------------------------------------------------------------------------- //
void
Plumber::restore() {
  DBG_VERBOSE("Plubmer::restore()\n");

  dup2(_def[0],  0);
  dup2(_def[1], 1);
  dup2(_def[2], 2);

  Plumber::clear();
}

// ------------------------------------------------------------------------- //
// Closes all stored I/O states, pipes and files.                            //
// ------------------------------------------------------------------------- //
void
Plumber::clear() {
  DBG_VERBOSE("Plubmer::clear()\n");

  close(_def[0]);
  close(_def[1]);
  close(_def[2]);
  close(_file[0]);
  close(_file[1]);
  close(_ipipe[0]);
  close(_ipipe[1]);
  close(_opipe[0]);
  close(_opipe[1]);
}

// ------------------------------------------------------------------------- //
// Pushes a new pipe into Plumber.                                           //
// This function makes output pipe become input pipe, and creates a new      //
// output pipe. Call this function before executing a new command.           //
// ------------------------------------------------------------------------- //
void
Plumber::push() {
  DBG_VERBOSE("Plubmer::push()\n");

  // Close the input end of output pipe to signify end of stream
  close(_ipipe[1]);

  // Move output pipe to input pipe
  _ipipe[0] = _opipe[0];
  _ipipe[1] = _opipe[1];

  // Create a new output pipe
  if (pipe(_opipe) == -1) {
    perror("Plumber::swap()");
    exit(2);
  }
}

// ------------------------------------------------------------------------- //
// Redirects standard I/O as specified by arguments.                         //
// ------------------------------------------------------------------------- //
void
Plumber::redirect(int in, int out, int err) {
  DBG_VERBOSE("Plubmer::redirect()\n");

  // Redirect stdin
  switch (in) {
    case PLB_NONE: { dup2(_file[0],  0); } break;
    case PLB_PIPE: { dup2(_ipipe[0], 0); } break;
    default:       { dup2(in,        0); } break;
  }

  // Redirect stdout
  switch (out) {
    case PLB_NONE: { dup2(_file[1],  1); } break;
    case PLB_PIPE: { dup2(_opipe[1], 1); } break;
    default:       { dup2(out,       1); } break;
  }

  // Redirect stderr
  switch (err) {
    case PLB_NONE: { dup2(_file[2],  2); } break;
    case PLB_PIPE: { dup2(_opipe[1], 2); } break;
    default:       { dup2(err,       2); } break;
  }
}

// ------------------------------------------------------------------------- //
// Sets up files for redirecting I/O.                                        //
// If any argument is not null, a file will be opened, and all output to the //
// corresponding std I/O will be read from/sent to the file instead of the   //
// std as appropriate.                                                       //
// ------------------------------------------------------------------------- //
int
Plumber::file(char* in, char* out, char* err, int append) {

  // Flag indicating error
  int f = 0;

  // stdin
  _file[0] = _def[0];
  if (in) {
    DBG_VERBOSE("Plumber::file() : stdin -> %s\n", in);
    _file[0] = open(in, F_READ, F_FLAGS);
    // Check if we ran into problems
    if (_file[0] == -1) {
      COMPLAIN("%s: %s\n", in, strerror(errno));
      f = 1;
    }
  }

  // stdout
  _file[1] = _def[1];
  if (out) {
    DBG_VERBOSE("Plumber::file() : stdout -> %s\n", out);
    _file[1] = open(out, append ? (F_APPEND) : (F_TRUNC), F_FLAGS);
    // Check if we ran into problems
    if (_file[1] == -1) {
      COMPLAIN("%s: %s\n", out, strerror(errno));
      f = 1;
    }
  }

  // stderr
  // XXX Per project constraints, oes not support redirecting to a file
  // different than stdout. Easy enough to change this, though.
  _file[2] = _def[2];
  if (err) {
    DBG_VERBOSE("Plumber::file() : stderr -> %s\n", err);
    _file[2] = _file[1];
  }

  // Return the error flag (1 if there is something wrong)
  return f;
}

// ------------------------------------------------------------------------- //
// Gets the corresponding std I/O from the previousy captured state.         //
// ------------------------------------------------------------------------- //
int
Plumber::std(int i) {
  return _def[i];
}
