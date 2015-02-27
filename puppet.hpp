#ifndef PUPPET_H_
#define PUPPET_H_

#include "command.hpp"

#define PUPPET_BUFFER_SIZE 1024

class Puppet {
private:
  char     *executable;

  int       ipipe[2];
  int       opipe[2];
  int       epipe[2];
  int       def[3];

  bool      finalized;

  static Path *root;
public:

  Puppet(const char*);
  ~Puppet();

  size_t  write(char*);
  size_t  write(char*, size_t);

  char*   read(int);
  size_t  read(int, char*, size_t);

  int   run();

  static void init();
};

#endif
