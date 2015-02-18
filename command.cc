
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
#include <string.h>
#include <signal.h>

#include <vector>

#include "main.h"
#include "trace.h"
#include "command.h"

SimpleCommand::SimpleCommand(void) {
	args = new std::vector<char*>();
}

SimpleCommand::~SimpleCommand(void) {
	delete args;
}

void
SimpleCommand::pushArgument(char *arg) {
	args -> push_back(arg);
}

CompoundCommand::CompoundCommand() {
	args = new std::vector<SimpleCommand*>();

	bg  = 0;
	nf  = 0;
	in  = NULL;
	out = NULL;
	err = NULL;
}

void
CompoundCommand::pushArgument(SimpleCommand *cmd) {
	args -> push_back(cmd);
}

void
CompoundCommand::clear() {

	args -> erase(args -> begin(), args -> end());

	if (out) free(out);
	if (in)  free(in);
	if (err && err != out) free(err);

	bg   = 0;
	in   = NULL;
	out  = NULL;
	err  = NULL;
}

void
CompoundCommand::print() {

	DBG_INFO("\n\n");
	DBG_INFO("            COMMAND TABLE                \n");
	DBG_INFO("\n");
	DBG_INFO("#   Simple Commands\n");
	DBG_INFO("--- ----------------------------------------------------------\n");

	int csize = args -> size();
	for (int i = 0; i < csize; i++) {
		DBG_INFO("  %-3d ", i);

		int size = args -> at(i) -> args -> size();
		for (int j = 0; j < size; j++) {
			DBG_INFO_N("\"%s\" \t", args -> at(i) -> args -> at(j));
		}
	}

	DBG_INFO_N( "\n\n" );
	DBG_INFO( "Output       Input        Error        Truncate     Background\n" );
	DBG_INFO( "------------ ------------ ------------ ------------ ------------\n" );
	DBG_INFO( "%s%-12s %s%-12s %s%-12s %s%-12s %s%-12s\n",
		out ? COLOR_YELLOW   : COLOR_NONE,
		out ? out   : "default",
		in  ? COLOR_YELLOW   : COLOR_NONE,
		in  ? in    : "default",
		err ? COLOR_YELLOW   : COLOR_NONE,
		err ? err   : "default",
		nf  ? COLOR_YELLOW   : COLOR_NONE,
		nf  ? "YES" : "NO",
		bg  ? COLOR_YELLOW   : COLOR_NONE,
		bg  ? "YES" : "NO");
	DBG_INFO( "\n\n" );
}

void
CompoundCommand::execute() {
	// Don't do anything if there are no simple commands
	if (!args -> empty()) {

		// Print contents of Command data structure
		print();

		// Add execution here
		// For every simple command fork a new process
		// Setup i/o redirection
		// and call exec

		// Clear to prepare for next command
		clear();

	}

	// Print new prompt
	prompt();
}
