// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// puppet.cc                                                                 //
// This file contains the logic for the Puppet class that executes any       //
// program in a child process, while providing an interface for parent       //
// to access its exit status and read its output.                            //
//                                                                           //
// ------------------------------------------------------------------------- //
#include <string>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "path.hpp"
#include "trace.hpp"
#include "plumber.hpp"
#include "globber.hpp"
#include "puppet.hpp"
#include "command.hpp"

// Forward declarations
Path *Puppet::_root;
Path *Puppet::_self;

// ------------------------------------------------------------------------- //
// Constructor. Creates a puppet instance from an executable path.           //
// ------------------------------------------------------------------------- //
Puppet::Puppet(const char* exe) {
  if (!Puppet::_root) {
    PANIC("You must call Puppet::init() first!\n");
  }

  finalized = false;
  _status   = -1;

  // Compute executable path
  Path *path = new Path(exe);
  executable = path -> resolve(Puppet::_root);
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

// ------------------------------------------------------------------------- //
// Destructor. Closes read ends of stdout and stderr pipes.                  //
// ------------------------------------------------------------------------- //
Puppet::~Puppet() {
  free(executable);
  close(opipe[0]);
  close(epipe[0]);
}

// ------------------------------------------------------------------------- //
// Writes a string to the stdin of the puppet process.                       //
// Returns pointer to itself for easy chaining.                              //
// ------------------------------------------------------------------------- //
Puppet*
Puppet::write(const char *str) {
  if (finalized) {
    COMPLAIN("puppet: write: Cannot write to a finalized puppet.\n");
    _status = -1;
    return this;
  }
  dprintf(ipipe[1], "%s", str);
  return this;
}

// ------------------------------------------------------------------------- //
// Reads entire content of the puppet's stdout or stderr depending on the    //
// value of `type`. Possible values are IO_OUT or IO_ERR. Does not close     //
// pipe after reading, but lseeks to the beginning. Blocks until the puppet  //
// closes the pipe that is being read from.                                  //
// ------------------------------------------------------------------------- //
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
  while ((count = ::read(fd, buffer, PUPPET_BUFFER_SIZE))) {
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

// ------------------------------------------------------------------------- //
// Reads entire content of the puppet's stdout, splits it by likes and       //
// pushes them into a SimpleCommand as arguments.                            //
// ------------------------------------------------------------------------- //
int
Puppet::readTo(SimpleCommand *partial) {
  char *output = read(IO_OUT);
  char *ps = output,
       *pe = output;
  while (*pe) {
    if (*pe == '\n') {
      char *one = strndup(ps, pe - ps);
      partial -> push(one);
      ps = pe + 1;
    }
    pe++;
  }
  if (pe) { partial -> push(strdup(pe)); }
  free(output);
  return _status;
}

// ------------------------------------------------------------------------- //
// Executes the puppet program. Takes care of piping and forking.            //
// Returns pointer to itself for easy chaining.                              //
// ------------------------------------------------------------------------- //
Puppet*
Puppet::run() {
  if (finalized) {
    COMPLAIN("puppet: finalized: Cannot run a finalized puppet.");
    _status = -1;
    return this;
  }

  // Prevent repeated execution
  finalized = true;

  // Close the writing end of input pipe
  close(ipipe[1]);

  // Fork
  int pid = fork();
  if (pid == -1) {
    COMPLAIN("puppet: fork: %s", strerror(errno));
    _status = -1;
    return this;
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
    exit(EXIT_FAILURE);
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

  // Wait for process to exit and return its exit _status
  if (pid != -1) { waitpid(pid, &_status, 0); }
  return this;
}

// ------------------------------------------------------------------------- //
// Returns the exit status of the puppet process.                            //
// ------------------------------------------------------------------------- //
int
Puppet::status() {
  return _status;
}

// ------------------------------------------------------------------------- //
// Initializes the path resolution mechanism of the Puppet.                  //
// Essentially, it will set cwd() as the root path for the resolution.       //
// ------------------------------------------------------------------------- //
void
Puppet::init(char *path) {
  Puppet::_self = new Path(path);
  if (Puppet::_self -> isAbsolute()) {
    // CWD cannot be used as root here
    Puppet::_root = Puppet::_self -> clone() -> pushd("..");
  }
  else {
    // Relative path, so CWD can be used as root
    Puppet::_root = Globber::cwd() -> push(Puppet::_self) -> pushd("..");
  }
  printf("root = %s\n", Puppet::_root -> str());
}

// ------------------------------------------------------------------------- //
// Creates an puppet instance of the parent process.                         //
// ------------------------------------------------------------------------- //
Puppet*
Puppet::self() {
  char *path = Puppet::_self -> str();
  Puppet *p = new Puppet(pathm);
  free(path);
  return p;
}
