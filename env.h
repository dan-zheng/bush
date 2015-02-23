#ifndef ENV_H_
#define ENV_H_

class Env {
public:
  static void set(char**);
  static void unset(char**);
  static void print(char**);
  static int  expand(char**);
  static int  tilde(char**);
};

#endif
