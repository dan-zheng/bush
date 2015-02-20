#include <unistd.h>

#include "trace.h"
#include "command.h"

#ifndef OS_X
#include "lib/tty.h"
#endif

SimpleCommand   *SimpleCommand::current;
CompoundCommand *CompoundCommand::current;

int
yyparse(void);

void
prompt(void) {
  if (isatty(0)) { printf(LGREEN("bush> ")); }
  fflush(stdout);
}

int
main(int argc, char **argv) {

  if (isatty(0)) {
    printf(DGRAY(
    "  ______     __  __     ______     __  __    \n"
    " /\\  == \\   /\\ \\/\\ \\   /\\  ___\\   /\\ \\_\\ \\   \n"
    " \\ \\  __<   \\ \\ \\_\\ \\  \\ \\___  \\  \\ \\  __ \\  \n"
    "  \\ \\_____\\  \\ \\_____\\  \\/\\_____\\  \\ \\_\\ \\_\\ \n"
    "   \\/_____/   \\/_____/   \\/_____/   \\/_/\\/_/ \n"
    "\n"));

    printf(DGRAY("BUSH - Blatantly Useless Shell\n"));
    printf(DGRAY("Copyright (C) Denis Luchkin-Zhou (build %s)\n"), __TIMESTAMP__);

    // Print enabled debug levels
    DBG_ERR_N(DGRAY("\nDEBUG FLAGS : "));
    DBG_ERR_N(LRED("ERROR"));
    DBG_ERR_N(" ");
    DBG_WARN_N(YELLOW("WARNING"));
    DBG_WARN_N(" ");
    DBG_INFO_N(LBLUE("INFO"));
    DBG_INFO_N(" ");
    DBG_VERBOSE_N(DGRAY("VERBOSE"));
    DBG_ERR_N("\n\n");

  }

  CompoundCommand::current = new CompoundCommand();
  prompt();
  yyparse();
  return 0;
}
