#ifndef SUBSHELL_H_
#define SUBSHELL_H_

#define SUBSHELL_BUFFER_SIZE 1024

class Subshell {
private:
  static char *me;

public:
  static char* run(char*);
  static void  init(char*);

};

#endif
