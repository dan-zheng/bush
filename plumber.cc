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

int Plumber::rootPID;
int Plumber::def[3];
int Plumber::file[3];
int Plumber::ipipe[2];
int Plumber::opipe[2];

void
Plumber::init() {
  DBG_VERBOSE("Plubmer::init()\n");
  rootPID = getpid();
}

void
Plumber::capture() {
  DBG_VERBOSE("Plubmer::capture()\n");

  def[0]  = dup(0);
  def[1]  = dup(1);
  def[2]  = dup(2);

  file[0] = dup(0);
  file[1] = dup(1);
  file[2] = dup(2);

  if (pipe(opipe) == -1 || pipe(ipipe) == -1) {
    perror("Plumber::capture()");
    exit(2);
  }
}

void
Plumber::restore() {
  DBG_VERBOSE("Plubmer::restore()\n");

  dup2(def[0],  0);
  dup2(def[1], 1);
  dup2(def[2], 2);
  Plumber::clear();
}

void
Plumber::clear() {
  DBG_VERBOSE("Plubmer::clear()\n");

  close(def[0]);
  close(def[1]);
  close(def[2]);
  close(file[0]);
  close(file[1]);
  close(ipipe[0]);
  close(ipipe[1]);
  close(opipe[0]);
  close(opipe[1]);
}

void
Plumber::push() {
  DBG_VERBOSE("Plubmer::swap()\n");

  close(ipipe[1]);

  ipipe[0] = opipe[0];
  ipipe[1] = opipe[1];

  if (pipe(opipe) == -1) {
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
    case PLB_NONE: { dup2(def[0],   0); } break;
    case PLB_PIPE: { dup2(ipipe[0], 0); } break;
    case PLB_FILE: { dup2(file[0],  0); } break;
    default:       { dup2(in,       0); } break;
  }

  switch (out) {
    case PLB_NONE: { dup2(def[1],   1); } break;
    case PLB_PIPE: { dup2(opipe[1], 1); } break;
    case PLB_FILE: { dup2(file[1],  1); } break;
    default:       { dup2(out,      1); } break;
  }

  switch (err) {
    case PLB_NONE: { dup2(def[2],   2); } break;
    case PLB_PIPE: { dup2(opipe[1], 2); } break;
    case PLB_FILE: { dup2(file[2],  2); } break;
    default:       { dup2(err,      2); } break;
  }

}

int
Plumber::in(char* path) {
  file[0] = def[0];
  if (path) {
		DBG_VERBOSE("Plumber::in() : Opening input file: %s\n", path);
    file[0] = open(path, F_READ, F_FLAGS);
    if (file[0] == -1) { fprintf(stderr, "-" SH_NAME ": %s: %s\n", path, strerror(errno)); }
    return file[0];
  }
  return 0;
}

int
Plumber::out(char* path, int append) {
  if (path) {
    DBG_VERBOSE("Plumber::out() : Opening output file: %s\n", path);
    file[1] = open(path, append ? (F_APPEND) : (F_TRUNC), F_FLAGS);
    if (file[1] == -1) { fprintf(stderr, "-" SH_NAME ": %s: %s\n", path, strerror(errno)); }
    return file[1];
  }
  return 0;
}

int
Plumber::err(char* path, int append) {
  if (path) {
    DBG_VERBOSE("Plumber::err() : Redirecting stderr to output file: %s\n", path);
    return file[2] = file[1];
  }
  return 0;
}

int
Plumber::std(int i) {
  return def[i];
}
