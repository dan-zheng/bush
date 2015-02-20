
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef OS_X
#include <wait.h>
#else
#include <sys/wait.h>
#endif

#include "../plumber.h"

const char * usage = ""
"Usage:\n"
"    cat_grep file-name word outfile\n"
"\n"
"    It does something similar to the shell command:\n"
"        csh> cat file | grep word > outfile\n"
"\n"
"Example:\n"
"    cat_grep command.cc Command out\n"
"    cat out\n\n";

const char *cat = "cat";
const char *grep = "grep";

int main(int argc, char **argv, char **envp)
{
	if (argc < 4) {
		fprintf(stderr, "%s", usage );
		exit(1);
	}

	// Save default input, output, and error because we will
	// change them during redirection and we will need to restore them
	// at the end.

	Plumber::capture();

	//////////////////  cat //////////////////////////

	// Input:    Plumber::defin
	// Output:   pipe
	// Error:    Plumber::deferr

	Plumber::redirect(PLB_NONE, PLB_PIPE, PLB_PIPE);
	
	// Create new process for "cat"
	int pid = fork();
	if ( pid == -1 ) {
		perror( "cat_grep: fork\n");
		exit( 2 );
	}

	if (pid == 0) {
		//Child
		// close file descriptors that are not needed
		Plumber::clear();

		// You can use execvp() instead if the arguments are stored in an array
		execlp(cat, cat, argv[1], (char *) 0);

		// exec() is not suppose to return, something went wrong
		perror( "cat_grep: exec cat");
		exit( 2 );
	}

	//////////////////  grep //////////////////////////

	// Input:    pipe
	// Output:   outfile
	// Error:    Plumber::deferr

	// Redirect output to utfile
	int outfd = creat( argv[ 3 ], 0666 );

	if ( outfd < 0 ) {
		perror( "cat_grep: creat outfile" );
		exit( 2 );
	}

	Plumber::redirect(PLB_PIPE, outfd, outfd);

	close( outfd );

	pid = fork();
	if (pid == -1 ) {
		perror( "cat_grep: fork");
		exit( 2 );
	}

	if (pid == 0) {
		//Child

		// close file descriptors that are not needed
		Plumber::clear();

		// You can use execvp() instead if the arguments are stored in an array
		execlp(grep, cat, argv[2], (char *) 0);

		// exec() is not suppose to return, something went wrong
		perror( "cat_grep: exec grep");
		exit( 2 );

	}

	// Restore input, output, and error
	Plumber::restore();

	// Wait for last process in the pipe line
	waitpid( pid, 0, 0 );

	exit( 2 );
}
