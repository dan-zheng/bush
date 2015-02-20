#include <unistd.h>

#include "trace.h"
#include "command.h"

SimpleCommand   *SimpleCommand::current;
CompoundCommand *CompoundCommand::current;

int
yyparse(void);

void
prompt(void) {
  if (isatty(0)) { printf(LGREEN("myshell> ")); }
  fflush(stdout);
}

int
main(int argc, char **argv) {

  // Print enabled debug levels
  DBG_ERR_N("  ENABLED DEBUG LEVELS : ");
  DBG_ERR_N(LRED("ERROR"));
  DBG_ERR_N(" ");
  DBG_WARN_N(YELLOW("WARNING"));
  DBG_WARN_N(" ");
  DBG_INFO_N(LBLUE("INFO"));
  DBG_INFO_N(" ");
  DBG_VERBOSE_N(DGRAY("VERBOSE"));
  DBG_ERR_N("\n\n");

  CompoundCommand::current = new CompoundCommand();
  prompt();
  yyparse();
  return 0;
}
