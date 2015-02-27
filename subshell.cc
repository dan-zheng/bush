#include <string>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "path.hpp"
#include "trace.hpp"
#include "plumber.hpp"
#include "globber.hpp"
#include "subshell.hpp"


char *Subshell::me;

void
Subshell::init(char *path) {
  // Capture the path of the executable
  Path *executable = new Path(path),
       *root       = Globber::cwd();
  Subshell::me     = executable -> resolve(root);
  delete executable;
  delete root;
  DBG_INFO("Subshell::init(): me: %s\n", Subshell::me);
}

char*
Subshell::run(char* arg) {

  // Make the comminication pipe
  int ipipe[2];
  int opipe[2];
  if (pipe(ipipe) == -1 || pipe(opipe) == -1) {
    COMPLAIN("subshell: pipe: %s\n", strerror(errno));
    return NULL;
  }

  // Write subshell arg into the pipe and close it
  dprintf(ipipe[1], "%s\n", arg);
  close(ipipe[1]);

  // Fork
  int pid = fork();

  if (pid == -1) {
    COMPLAIN("subshell: fork: %s\n", strerror(errno));
    return NULL;
  }

  int _in =  dup(0);
  int _out = dup(1);

  dup2(ipipe[0], 0);
  close(ipipe[0]);

  dup2(opipe[1], 1);
  close(opipe[1]);

  if (pid == 0) {
    execlp(Subshell::me, Subshell::me, NULL);
    COMPLAIN("subshell: exec: %s\n", strerror(errno));
  }

  dup2(_in,  0);
  dup2(_out, 1);

  close(_in);
  close(_out);
  close(ipipe[0]);
  close(opipe[1]);

  if (pid != -1) { waitpid(pid, 0, 0); }

  char buffer[SUBSHELL_BUFFER_SIZE];
  std::string *output = new std::string();

  int count;
  while ((count = read(opipe[0], buffer, SUBSHELL_BUFFER_SIZE))) {
    output -> append(buffer, count);
  }

  close(opipe[0]);
  char *result = strdup(output -> c_str());
  delete output;

  DBG_INFO("Subshell::run(): result: %s\n", result);
  return result;
}
