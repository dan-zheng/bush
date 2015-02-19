#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "global.h"
#include "trace.h"
#include "plumber.h"

int Plumber::defin;
int Plumber::defout;
int Plumber::deferr;
int Plumber::iopipe[2];

void
Plumber::capture() {
  defin  = dup(0);
  defout = dup(1);
  deferr = dup(2);

  if (pipe(iopipe) == -1) {
    perror("Plumber::makePipe()");
    exit(2);
  }
}

void
Plumber::restore() {
  dup2(defin,  0);
  dup2(defout, 1);
  dup2(deferr, 2);
  Plumber::clear();
}

void
Plumber::clear() {
  close(defin);
  close(defout);
  close(deferr);
  close(iopipe[0]);
  close(iopipe[1]);
}

void
Plumber::redirect(int in, int out, int err) {

  switch (in) {
    case PLB_NONE: { dup2(defin,     0); } break;
    case PLB_PIPE: { dup2(iopipe[0], 0); } break;
    default:       { dup2(in,        0); } break;
  }

  switch (out) {
    case PLB_NONE: { dup2(defout,    1); } break;
    case PLB_PIPE: { dup2(iopipe[1], 1); } break;
    default:       { dup2(out,       1); } break;
  }

  switch (err) {
    case PLB_NONE: { dup2(deferr,    2); } break;
    case PLB_PIPE: { dup2(iopipe[1], 2); } break;
    default:       { dup2(err,       2); } break;
  }

}
