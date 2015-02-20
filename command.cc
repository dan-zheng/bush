
/*
 * CS252: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include <cerrno>
#include <vector>

#include "global.h"
#include "main.h"
#include "trace.h"
#include "command.h"
#include "builtin.h"
#include "plumber.h"

#define HIGHLIGHT  COLOR_YELLOW
#define F_READ   O_RDONLY
#define F_FLAGS  S_IRUSR | S_IWUSR | S_IXUSR
#define F_TRUNC  O_WRONLY | O_CREAT | O_TRUNC
#define F_APPEND O_WRONLY | O_CREAT | O_APPEND

// ------------------------------------------------------------------------- //
// class SimpleCommand { ... }                                               //
// ------------------------------------------------------------------------- //
SimpleCommand::SimpleCommand() {
	args = new std::vector<char*>();
}

SimpleCommand::~SimpleCommand() {
	delete args;
}

void
SimpleCommand::print() {
	int size = args -> size();
	for (int j = 0; j < size - 1; j++) {
		DBG_INFO_N("\"%s\" ", args -> at(j));
	}
}

int
SimpleCommand::execute() {
	int pid = fork();

	// Fail
	if (pid == -1) {
		perror(LRED("SimpleCommand::execute() "));
		exit(2);
	}

	// Success
	if (pid == 0) {
		// Close unnecessary descriptors
		Plumber::clear();

		// Execute the program
		execvp(first(), &args->front());

		// Panic here
		if (errno != ENOENT) {
			perror(LRED("SimpleCommand::execute() "));
		}
		else {
			fprintf(stderr, "-%s: %s: command not found\n", SH_NAME, first());
		}
		exit(2);
	} else {
		return pid;
	}
}

void
SimpleCommand::push(char *arg) {
	args -> push_back(arg);
}

char*
SimpleCommand::first() {
	if (args -> size() == 0) { return NULL; }
	return args -> at(0);
}

char*
SimpleCommand::last() {
	if (args -> size() == 0) { return NULL; }
	return args -> at(args -> size() - 1);
}

// ------------------------------------------------------------------------- //
// class CompoundCommand { ... }                                             //
// ------------------------------------------------------------------------- //
CompoundCommand::CompoundCommand() {
	args = new std::vector<SimpleCommand*>();

	bg     = 0;
	append = 0;
	in     = NULL;
	out    = NULL;
	err    = NULL;
}

void
CompoundCommand::push(SimpleCommand *cmd) {
	cmd -> args -> push_back(NULL);
	args -> push_back(cmd);
}

SimpleCommand*
CompoundCommand::first() {
	return args -> at(0);
}

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

void
CompoundCommand::print() {

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
	DBG_INFO(" Output       Input        Error        Background   Append File\n" );
	DBG_INFO("------------ ------------ ------------ ------------ ------------\n" );
	DBG_INFO(" %s%-12s %s%-12s %s%-12s %s%-12s %s%-12s %s\n",
		out      ? HIGHLIGHT   : COLOR_NONE, out    ? out   : "default",
		in       ? HIGHLIGHT   : COLOR_NONE, in     ? in    : "default",
		err      ? HIGHLIGHT   : COLOR_NONE, err    ? err   : "default",
		bg       ? HIGHLIGHT   : COLOR_NONE, bg     ? "YES" : "NO",
		append   ? HIGHLIGHT   : COLOR_NONE, append ? "YES" : "NO",
		COLOR_NONE);
	DBG_INFO("\n");
	DBG_INFO("\n");
}

void
CompoundCommand::execute() {

	// Handle exit()
	if (!strcmp(first() -> first(), "exit")) { BuiltIn::_exit(); }

	// Empty command, skip.
	if (args -> empty()) {
		DBG_VERBOSE("CompoundCommand::execute() : Skipping empty command.");
		clear();
		return;
	}

	// Print contents of Command data structure
	print();

	#ifndef PARSER_ONLY

	// Capture the I/O state
	Plumber::capture();

	// Open input file
	int fdin  = PLB_NONE;
	if (in) {
		DBG_VERBOSE("CompoundCommand::execute() : Opening input file: %s\n", in);
		fdin = open(in, F_READ, F_FLAGS);
		if (fdin == -1) {
			DBG_ERR("CompoundCommand::execute() : Failed to open input file.\n");
			perror(in);
			clear();
			return;
		}
	}

	// Open output file
	int fdout = PLB_NONE;
	if (out) {
		DBG_VERBOSE("CompoundCommand::execute() : Opening output file: %s\n", out);
		fdout = open(out, append ? (F_APPEND) : (F_TRUNC), F_FLAGS);
		if (fdout == -1) {
			DBG_ERR("CompoundCommand::execute() : Failed to open output file.\n");
			perror(in);
			clear();
			return;
		}
	}

	// Open error file
	int fderr = PLB_NONE;
	if (err && fdout) { fderr = fdout; }

	// Redirect for the first command
	Plumber::redirect(fdin, fdout, fderr);

	// Execute first command
	int pid = first() -> execute();

	// Unless &, wait for child to finish
	if (!bg) { waitpid(pid, 0, 0); }

	#else



	#endif

	// Restore I/O state
	Plumber::restore();

	// Clear to prepare for next command
	clear();


}
