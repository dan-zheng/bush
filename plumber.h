#ifndef PLUMBER_H_
#define PLUMBER_H_

#define PLB_NONE  -1
#define PLB_PIPE  -2
#define PLB_FILE  -3

class Plumber {
private:
  static int       def[3];
  static int       file[3];
  static int       ipipe[2];
  static int       opipe[2];

public:

  static int  rootPID;


  static void init();

  static void capture();
  static void restore();
  static void clear();
  static void push();
  static void redirect(int, int, int);

  static int  in (char*);
  static int  out(char*, int);
  static int  err(char*, int);

  static int  std(int);
};

#endif
