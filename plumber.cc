#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "global.h"
#include "trace.h"
#include "plumber.h"

int Plumber::defin;
int Plumber::defout;
int Plumber::deferr;
int Plumber::ipipe[2];
int Plumber::opipe[2];

void
Plumber::capture() {
  defin  = dup(0);
  defout = dup(1);
  deferr = dup(2);

  if (pipe(ipipe) == -1 || pipe(opipe) == -1) {
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
  close(ipipe[0]);
  close(ipipe[1]);
  close(opipe[0]);
  close(opipe[1]);
}

void
Plumber::swap() {
  int tmp;

  tmp = ipipe[0];
  ipipe[0] = opipe[0];
  opipe[0] = tmp;

  tmp = ipipe[1];
  ipipe[1] = opipe[1];
  opipe[1] = tmp;
}

void
Plumber::redirect(int in, int out, int err) {

  switch (in) {
    case PLB_NONE: { dup2(defin,    0); } break;
    case PLB_PIPE: { dup2(ipipe[0], 0); } break;
    default:       { dup2(in,       0); } break;
  }

  switch (out) {
    case PLB_NONE: { dup2(defout,   1); } break;
    case PLB_PIPE: { dup2(opipe[1], 1); } break;
    default:       { dup2(out,      1); } break;
  }

  switch (err) {
    case PLB_NONE: { dup2(deferr,   2); } break;
    case PLB_PIPE: { dup2(opipe[1], 2); } break;
    default:       { dup2(err,      2); } break;
  }

}
