#include <stdio.h>

#include "error.h"
#include "symbol.h"
#include "asm.h"

extern int yyparse ();
extern FILE *yyin;

int main (int argc, char **argv)
{
  symbolTableInitialize ();
  errorInitialize (argv[1]);

  yyin = fopen (argv[1], "r");
  yyparse ();

   emitData ();
  emitFuncs ();

  return 0;
}








