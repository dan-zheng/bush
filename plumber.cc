#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <sys/stat.h>

#include "global.h"
#include "trace.h"
#include "plumber.h"

#define F_READ   O_RDONLY
#define F_FLAGS  S_IRUSR | S_IWUSR | S_IXUSR
#define F_TRUNC  O_WRONLY | O_CREAT | O_TRUNC
#define F_APPEND O_WRONLY | O_CREAT | O_APPEND

int Plumber::_def[3];
int Plumber::_file[3];
int Plumber::_ipipe[2];
int Plumber::_opipe[2];

void
Plumber::init() {
  DBG_VERBOSE("Plubmer::init()\n");
}

void
Plumber::capture() {
  DBG_VERBOSE("Plubmer::capture()\n");

  _def[0]  = dup(0);
  _def[1]  = dup(1);
  _def[2]  = dup(2);

  _file[0] = dup(0);
  _file[1] = dup(1);
  _file[2] = dup(2);

  if (pipe(_opipe) == -1 || pipe(_ipipe) == -1) {
    perror("Plumber::capture()");
    exit(2);
  }
}

void
Plumber::restore() {
  DBG_VERBOSE("Plubmer::restore()\n");

  dup2(_def[0],  0);
  dup2(_def[1], 1);
  dup2(_def[2], 2);
  Plumber::clear();
}

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

void
Plumber::push() {
  DBG_VERBOSE("Plubmer::swap()\n");

  close(_ipipe[1]);

  _ipipe[0] = _opipe[0];
  _ipipe[1] = _opipe[1];

  if (pipe(_opipe) == -1) {
    perror("Plumber::swap()");
    exit(2);
  }
}

void
Plumber::redirect(int in, int out, int err) {
  DBG_VERBOSE("Plubmer::redirect()\n");

  close(0);
  close(1);
  close(2);

  switch (in) {
    case PLB_NONE: { dup2(_file[0],  0); } break;
    case PLB_PIPE: { dup2(_ipipe[0], 0); } break;
    default:       { dup2(in,        0); } break;
  }

  switch (out) {
    case PLB_NONE: { dup2(_file[1],  1); } break;
    case PLB_PIPE: { dup2(_opipe[1], 1); } break;
    default:       { dup2(out,       1); } break;
  }

  switch (err) {
    case PLB_NONE: { dup2(_file[2],  2); } break;
    case PLB_PIPE: { dup2(_opipe[1], 2); } break;
    default:       { dup2(err,       2); } break;
  }

}

int
Plumber::file(char* in, char* out, char* err, int append) {

  int f = 0;

  // In
  _file[0] = _def[0];
  if (in) {
    DBG_VERBOSE("Plumber::file() : stdin -> %s\n", in);
    _file[0] = open(in, F_READ, F_FLAGS);
    if (_file[0] == -1) { COMPLAIN("%s: %s\n", in, strerror(errno)); f = 1; }
  }

  // Out
  if (out) {
    DBG_VERBOSE("Plumber::file() : stdout -> %s\n", out);
    _file[1] = open(out, append ? (F_APPEND) : (F_TRUNC), F_FLAGS);
    if (_file[1] == -1) { COMPLAIN("%s: %s\n", out, strerror(errno)); f = 1; }
  }

  // Err
  if (err) {
    DBG_VERBOSE("Plumber::file() : stderr -> %s\n", err);
    return _file[2] = _file[1];
  }

  return f;
}

int
Plumber::std(int i) {
  return _def[i];
}
