#include "trace.h"
#include "command.h"

SimpleCommand   *SimpleCommand::current;
CompoundCommand *CompoundCommand::current;

int
yyparse(void);

void
prompt(void) {
  printf(LGREEN("myshell> "));
  fflush(stdout);
}

int
main(int argc, char **argv) {


  DBG_ERR("Error debug level enabled.\n");
  DBG_WARN("Warning debug level enabled.\n");
  DBG_INFO("Info debug level enabled.\n");
  DBG_VERBOSE("Verbose debug level enabled.\n");

  CompoundCommand::current = new CompoundCommand();
  prompt();
  yyparse();
  return 0;
}
