// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// command.h                                                                 //
// This file contains definitions for logic dealing with shell command       //
// representation and execution.                                             //
//                                                                           //
// ------------------------------------------------------------------------- //

#include <vector>

#ifndef COMMAND_H_
#define COMMAND_H_

typedef std::vector<char*>          ArgList;

// ------------------------------------------------------------------------- //
// An atomic executable shell command with its arguments.                    //
// ------------------------------------------------------------------------- //
class SimpleCommand {
public:
  ArgList     *args;

  SimpleCommand();
  ~SimpleCommand();

  void  print();        // Intended to be called from CompoundCommand::print()
  int   execute();      // Intended to be called from CompoundCommand::execute()

  void  push(char*);    // Pushes an argument into *args
  char* first();        // Gets the first argument
  char* last();         // Gets the last argument
};

typedef std::vector<SimpleCommand*> PartialList;

class CompoundCommand {
public:

  PartialList  *args;    // SimpleCommands that make up this CompoundCommand

  int      bg;           // Flag to indicate whether the process should
                         // run in background
  int      append;       // Flag to indicate whether the output files
                         // should be overwritten

  char   *in;            // Path to the input file
  char   *out;           // Path to the output file
  char   *err;           // Path to the error file

  CompoundCommand();
  ~CompoundCommand();

  void  print();         // Prints contents of the command to stdout
  void  execute();       // Executes the command
  void  clear();         // Deletes all contents of the command

  void           push(SimpleCommand*);  // Pushes a SimpleCommand into *args
  SimpleCommand* first();               // Gets the first SimpleCommand
};

#endif
