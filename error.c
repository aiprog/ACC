#include <stdio.h>
#include <string.h>

#include "error.h"

extern char *yytext;

static FILE *ptr;
static char name[SIZE];
static const char *mesgType[3] = { "NOTE", "WARNING", "ERROR" };

char buffer[SIZE];
Position position;

/* Initialize the error system. */
void errorInitialize (char *file)
{
  strcpy (name, file);
  ptr = fopen (name, "r");

  position.line = 1;
  position.column = 0;
  position.begin = position.offset = 0;
}

/* Update position informations. */
void errorUpdatePosition ()
{
  int i;

  for (i = 0; yytext[i] != 0; ++i)
  {
    ++position.offset;
    if (yytext[i] == '\n')
    {
      position.column = 0;
      ++position.line;
      position.begin += position.offset;
      position.offset = 0;
    }
    else 
    {
      if (yytext[i] == '\t')
        position.column = (position.column / 8 + 1) * 8;
      else
        ++position.column;
    }
  }
}

/* Print error messages. */
void errorPrint (int kind, char *message, Position *position)
{
  char buffer[SIZE];
  
  fseek (ptr, position->begin, SEEK_SET);
  fgets (buffer, SIZE, ptr);

  if (buffer[strlen (buffer) - 1] == '\n')
    buffer[strlen (buffer) - 1] = 0;

  printf ("%s:%d:%d:", name, position->line, position->column);
  printf ("%s: %s\n", mesgType[kind], message);
  printf ("%s\n%*s\n", buffer, position->column, "^");
}