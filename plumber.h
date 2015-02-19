#ifndef PLUMBER_H_
#define PLUMBER_H_

#define PLB_NONE  -1
#define PLB_PIPE  -2

class Plumber {
private:
  static int       defin;
  static int       defout;
  static int       deferr;
  static int       iopipe[2];

public:

  static void capture();
  static void restore();
  static void clear();
  static void redirect(int, int, int);
};

#endif
