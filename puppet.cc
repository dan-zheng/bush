#include <string>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "path.hpp"
#include "trace.hpp"
#include "plumber.hpp"
#include "globber.hpp"
#include "puppet.hpp"

Path *Puppet::root;

Puppet::Puppet(const char* exe) {
  if (!Puppet::root) {
    PANIC("You must call Puppet::init() first!\n");
  }

  finalized = false;

  // Compute executable path
  Path *path = new Path(exe);
  executable = path -> resolve(Puppet::root);
  delete path;

  // Create pipes
  if (pipe(ipipe) == -1 ||
      pipe(opipe) == -1 ||
      pipe(epipe) == -1) {
    COMPLAIN("Puppet: pipe: %s\n", strerror(errno));
    finalized = true;
  }

  // Capture current output state
  def[0] = dup(0);
  def[1] = dup(1);
  def[2] = dup(2);
}

Puppet::~Puppet() {
  free(executable);
  close(opipe[0]);
  close(epipe[0]);
}

size_t
Puppet::write(char *str) {
  if (finalized) {
    COMPLAIN("puppet: write: Cannot write to a finalized puppet.\n");
    return 0;
  }
  dprintf(ipipe[1], "%s", str);
}

size_t
Puppet::write(char *buffer, size_t count) {
  if (finalized) {
    COMPLAIN("puppet: write: Cannot write to a finalized puppet.\n");
    return 0;
  }
  return s_write(ipipe[1], buffer, count);
}

char*
Puppet::read(int type) {
  // Figure out which stream to read
  int fd;
  switch (type) {
    case IO_OUT: fd = opipe[0]; break;
    case IO_ERR: fd = epipe[0]; break;
    default: COMPLAIN("puppet: read: bad stream type [%d]", type);
  }

  // Temporary buffer and std::string for dynamic growth
  char buffer[PUPPET_BUFFER_SIZE];
  std::string *temp = new std::string();

  // Read the entire buffer
  int count;
  while ((count = s_read(fd, buffer, PUPPET_BUFFER_SIZE))) {
    temp -> append(buffer, count);
  }

  // Rewind so that it can be read again if needed
  // Output ends of output pipes are closed by destructor
  lseek(fd, 0, SEEK_SET);

  // Convert to C string and delete temporary std::string
  char *result = strdup(temp -> c_str());
  delete temp;

  return result;
}

size_t
Puppet::read(int type, char *buffer, size_t count) {
  // Figure out which stream to read
  int fd;
  switch (type) {
    case IO_OUT: fd = opipe[0]; break;
    case IO_ERR: fd = epipe[0]; break;
    default: COMPLAIN("puppet: read: bad stream type [%d]", type);
  }
  return s_read(fd, buffer, count);
}

int
Puppet::run() {
  if (finalized) {
    COMPLAIN("puppet: finalized: Cannot run a finalized puppet.");
    return -1;
  }

  // Prevent repeated execution
  finalized = true;

  // Close the writing end of input pipe
  close(ipipe[1]);

  // Fork
  int pid = fork();
  if (pid == -1) {
    COMPLAIN("puppet: fork: %s", strerror(errno));
    return -1;
  }

  // Redirect IO
  dup2(ipipe[0], 0);
  dup2(opipe[1], 1);
  dup2(epipe[1], 2);
  close(ipipe[0]);
  close(opipe[1]);
  close(epipe[1]);

  // Execute
  if (pid == 0) {
    execlp(executable, executable, NULL);
    COMPLAIN("puppet: exec: %s", strerror(errno));
  }

  // Restore IO
  dup2(def[0], 0);
  dup2(def[1], 1);
  dup2(def[2], 2);

  // Close unused descriptors
  close(def[0]);
  close(def[1]);
  close(def[2]);
  close(ipipe[0]);
  close(opipe[1]);
  close(epipe[1]);

  // Wait for process to exit and return its exit status
  int status;
  if (pid != -1) { waitpid(pid, &status, 0); }
  return status;
}

void
Puppet::init() {
  Puppet::root  = Globber::cwd();
}
