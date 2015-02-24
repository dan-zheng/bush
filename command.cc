// ------------------------------------------------------------------------- //
//                                                                           //
// CS252 Lab03 - Shell                                                       //
// Copyright © 2015 Denis Luchkin-Zhou                                       //
//                                                                           //
// command.h                                                                 //
// This file contains logic dealing with shell command representation and    //
// execution.                                                                //
//                                                                           //
// ------------------------------------------------------------------------- //

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#include <cerrno>
#include <vector>

#include "global.h"
#include "main.h"
#include "trace.h"
#include "command.h"
#include "builtin.h"
#include "plumber.h"

// Highlight color when printing non-default command table values
#define HIGHLIGHT  COLOR_YELLOW

// ------------------------------------------------------------------------- //
// SimpleCommand constructor.                                                //
// ------------------------------------------------------------------------- //
SimpleCommand::SimpleCommand() {
	args = new std::vector<char*>();
}

// ------------------------------------------------------------------------- //
// SimpleCommand destructor.                                                 //
// ------------------------------------------------------------------------- //
SimpleCommand::~SimpleCommand() {
	delete args;
}

// ------------------------------------------------------------------------- //
// Prints contents of the SimpleCommand to stdout.                           //
// XXX Intended to be called from CompoundCommand::print()                   //
// ------------------------------------------------------------------------- //
void
SimpleCommand::print() {
	int size = args -> size();
	for (int j = 0; j < size - 1; j++) {
		DBG_INFO_N("\"%s\" ", args -> at(j));
	}
}

// ------------------------------------------------------------------------- //
// Executes the SimpleCommand.                                               //
// XXX Intended to be called from CompoundCommand::execute()                 //
// ------------------------------------------------------------------------- //
int
SimpleCommand::execute() {
	DBG_INFO("SimpleCommand::execute() : %s\n", first());

	#if FEATURE_LEVEL >= FL_PART3

	// If there is a builtin command with that name, run the command handler
	// with arguments. Do not run executable with the same name.
	BuiltInFunc fn = BuiltIn::get(first());
	if (fn) {
		(*fn)(&args->front());
		return -1;
	}

	#endif

	// Fork the process at this point
	int pid = fork();

	// Scream, run and panic if we can't fork
	if (pid == -1) {
		COMPLAIN("SimpleCommand::execute(): %s", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// Fork OK, go ahead
	if (pid == 0) {
		// Close unnecessary descriptors
		Plumber::clear();

		// Execute the program
		execvp(first(), &args->front());

		// We shouldn't be here, panic!
		if (errno != ENOENT) {
			COMPLAIN("SimpleCommand::execute(): %s", strerror(errno));
		}
		else {
			COMPLAIN("%s: command not found", first());
		}
		exit(EXIT_FAILURE);
	}
	else { return pid; }
}

// ------------------------------------------------------------------------- //
// Pushes an argument string into the args array.                            //
// ------------------------------------------------------------------------- //
void
SimpleCommand::push(char *arg) {
	args -> push_back(arg);
}

// ------------------------------------------------------------------------- //
// Gets the first argument, or NULL if there are none.                       //
// ------------------------------------------------------------------------- //
char*
SimpleCommand::first() {
	if (args -> size() == 0) { return NULL; }
	return args -> at(0);
}

// ------------------------------------------------------------------------- //
// Gets the last argument, or NULL if there are none.                        //
// ------------------------------------------------------------------------- //
char*
SimpleCommand::last() {
	if (args -> size() == 0) { return NULL; }
	return args -> at(args -> size() - 1);
}

// ------------------------------------------------------------------------- //
// CompoundCommand constructor.                                              //
// ------------------------------------------------------------------------- //
CompoundCommand::CompoundCommand() {
	args = new std::vector<SimpleCommand*>();

	bg     = 0;
	append = 0;
	in     = NULL;
	out    = NULL;
	err    = NULL;
}

// ------------------------------------------------------------------------- //
// CompoundCommand destructor.                                               //
// ------------------------------------------------------------------------- //
void
CompoundCommand::push(SimpleCommand *cmd) {
	cmd -> args -> push_back(NULL);
	args -> push_back(cmd);
}

// ------------------------------------------------------------------------- //
// Deletes contents of the CompoundCommand.                                  //
// ------------------------------------------------------------------------- //
void
CompoundCommand::clear() {
	args -> erase(args -> begin(), args -> end());

	if (out) free(out);
	if (in)  free(in);
	if (err && err != out) free(err);

	bg     = 0;
	append = 0;
	in     = NULL;
	out    = NULL;
	err    = NULL;
}

// ------------------------------------------------------------------------- //
// Prints contents of the CompoundCommand to stdout.                         //
// ------------------------------------------------------------------------- //
void
CompoundCommand::print() {

	#if FEATURE_LEVEL == FL_PART1

	DBG_INFO("\n");
	DBG_INFO("\n");
	DBG_INFO("            COMMAND TABLE                \n");
	DBG_INFO("\n");
	DBG_INFO("#   Simple Commands\n");
	DBG_INFO("--- ------------------------------------------------------------\n");

	int csize = args -> size();
	for (int i = 0; i < csize; i++) {
		DBG_INFO("%-3d ", i);
		args -> at(i) -> print();
		DBG_INFO_N("\n");
	}

	DBG_INFO("\n");
	DBG_INFO(" Output       Input        Error        Background   File Mode  \n" );
	DBG_INFO("------------ ------------ ------------ ------------ ------------\n" );
	DBG_INFO(" %s%-12s %s%-12s %s%-12s %s%-12s %s%-12s %s\n",
		out      ? HIGHLIGHT   : COLOR_NONE, out    ? out      : "DEFAULT",
		in       ? HIGHLIGHT   : COLOR_NONE, in     ? in       : "DEFAULT",
		err      ? HIGHLIGHT   : COLOR_NONE, err    ? err      : "DEFAULT",
		bg       ? HIGHLIGHT   : COLOR_NONE, bg     ? "YES"    : "NO",
		append   ? HIGHLIGHT   : COLOR_NONE, append ? "APPEND" : "OVERWRITE",
		COLOR_NONE);
	DBG_INFO("\n");
	DBG_INFO("\n");

	#endif
}

// ------------------------------------------------------------------------- //
// Executes the CompoundCommand.                                             //
// ------------------------------------------------------------------------- //
void
CompoundCommand::execute() {

	// Handle `exit` command here.
	if (!strcmp(first() -> first(), "exit")) { BuiltIn::_exit(); }

	// CompoundCommand is empty, no point in executing it
	if (args -> empty()) {
		DBG_VERBOSE("CompoundCommand::execute() : Skipping empty command.");
		clear();
		return;
	}

	// Print contents of Command data structure
	// Prints nothing when DEBUG LEVEL is lower than DBG_LVL_INFO
	print();

	#if FEATURE_LEVEL >= FLVL_PART2

	// Capture the I/O state
	Plumber::capture();

	// Open input/output files
	if (Plumber::file(in, out, err, append)) {
		DBG_ERR("Failed to open one of the files.\n");
		clear();
		return;
	}

	// Number of simple commands
	int argc = args -> size();

	// Execute command s
	int pid  = -1;
	if (argc == 1) {
		// One and only command.. redirect both ends to files
		Plumber::redirect(PLB_NONE, PLB_NONE, PLB_NONE);
		pid = first() -> execute();
	}
	else {
		// Multiple, let's work on it
		for (int i = 0; i < argc; i++) {
			DBG_VERBOSE("Command loop: %d / %d\n", i + 1, argc);

			// Let Plumber lay down pipes
			Plumber::redirect(
				i == 0        ? PLB_NONE : PLB_PIPE,
				i == argc - 1 ? PLB_NONE : PLB_PIPE,
				PLB_NONE
			);

			// Execute and pass on PID
			int _pid = args -> at(i) -> execute();
			if (_pid != -1) { pid = _pid; }

			// Push new pipe into Plumber
			Plumber::push();
		}
	}

	// Unless &, wait for child to finish
	if (pid != -1 && !bg) { waitpid(pid, 0, 0); }

	// Restore I/O state
	Plumber::restore();

	#endif

	// Clear to prepare for next command
	clear();
}

// ------------------------------------------------------------------------- //
// Gets the first SimpleCommand that makes up the CompoundCommand.           //
// ------------------------------------------------------------------------- //
SimpleCommand*
CompoundCommand::first() {
	return args -> at(0);
}
