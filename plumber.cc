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
char *Plumber::_out;
bool  Plumber::_append;
int   Plumber::_def[3];
int   Plumber::_std[3];

// ------------------------------------------------------------------------- //
// Initializes the pipe and performs an initial capture.                     //
// ------------------------------------------------------------------------- //
void
Plumber::capture() {
  DBG_INFO("Plubmer::init()\n");

  // Capture current stdin, stdout and stderr
  _def[0]  = dup(0);
  _def[1]  = dup(1);
  _def[2]  = dup(2);
}

// ------------------------------------------------------------------------- //
// Configures the Plumber to execute the next command.                       //
// ------------------------------------------------------------------------- //
bool
Plumber::setup(char *in, char *out, char *err, int append) {
  // Initialize Plumber for the new command
  Plumber::capture();

  // Set up initial input/error redirects
  if (!Plumber::_iofile(IO_IN,  in,  append)) { return false; }
  if (!Plumber::_iofile(IO_ERR, err, append)) { return false; }

  // Let plumber redirect stderr as necessary
  Plumber::_redirect(IO_ERR);

  // Save output file information
  Plumber::_out    = out;
  Plumber::_append = append;

  return true;
}

// ------------------------------------------------------------------------- //
// Cleans up the old configuration and restores altered IO settings.         //
// ------------------------------------------------------------------------- //
void
Plumber::teardown() {
  DBG_INFO("Plubmer::restore()\n");

  // Restore default IO
  dup2(_def[0], 0);
  dup2(_def[1], 1);
  dup2(_def[2], 2);

  // Close backup IO descriptors
  close(_def[0]);
  close(_def[1]);
  close(_def[2]);

  // Clean output information
  Plumber::_out    = NULL;
  Plumber::_append = false;
}

// ------------------------------------------------------------------------- //
// Pushes a new Plumber state for the next partial command.                  //
// ------------------------------------------------------------------------- //
bool
Plumber::push(int type) {
  DBG_INFO("Plubmer::push()\n");

  Plumber::_redirect(IO_IN);

  // If not pushing to pipe, try to redirect stdout to outfile (if there is one)
  if (type == PLB_NONE) {
    if (!Plumber::_iofile(IO_OUT, Plumber::_out, Plumber::_append)) { return false; }
  }
  // Otherwise, push another pipe and redirect stdout there
  else if (!Plumber::_pushpipe()) { return false; }

  // Apply stdout redirect created above
  Plumber::_redirect(IO_OUT);
  return true;
}


// ------------------------------------------------------------------------- //
// Gets the corresponding std I/O from the previousy captured state.         //
// ------------------------------------------------------------------------- //
int
Plumber::std(int i) {
  return _def[i];
}


// ------------------------------------------------------------------------- //
// Redirects an std IO to the specified file.                                //
// ------------------------------------------------------------------------- //
bool
Plumber::_iofile(int type, char *path, bool append) {

  // Compute open flags for the file
  int flags;
  switch (type) {
    case IO_IN:  { flags = F_READ; } break;
    case IO_ERR:
    case IO_OUT: { flags = append ? F_APPEND : F_TRUNC; } break;
    default: { PANIC("Plumber::_iofile(): Unexpected iofile type: %d\n", type); }
  }

  // If the path is not null, open the file and redirect stdin from it
  if (path) {
    DBG_INFO("Plubmer::_iofile(): [%d] %s\n", type, path);

    _std[type] = open(path, flags, S_IRUSR | S_IWUSR | S_IXUSR);

    // Fail miserably if the file refuses to open
    if (_std[type] == -1) {
      COMPLAIN("%s: %s\n", path, strerror(errno));
      return false;
    }

  }
  // Otherwise, just read from stdin as always
  else {
    DBG_INFO("Plubmer::_iofile(): [%d] default\n", type);
    _std[type] = dup(_def[type]);
  }
  return true;
}

// ------------------------------------------------------------------------- //
// Applies a redirect to the specified std IO.                               //
// ------------------------------------------------------------------------- //
void
Plumber::_redirect(int i) {
  DBG_INFO("Plubmer::apply(): %d\n", i);
  dup2(_std[i], i);
  close(_std[i]);
}

// ------------------------------------------------------------------------- //
// Creates a new pipe and pushes it as the new Plumber state.                //
// ------------------------------------------------------------------------- //
bool
Plumber::_pushpipe() {
  int fdpipe[2];
  if (pipe(fdpipe) == -1) {
    COMPLAIN("%s: %s\n", "pipe", strerror(errno));
    return false;
  }
  _std[1] = fdpipe[1];
  _std[0] = fdpipe[0];
  return true;
}
