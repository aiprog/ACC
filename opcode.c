#include "opcode.h"

#define NULL  ((void *) 0)

const char *opToStr[41] = 
{
  NULL,    NULL,       "[]",     "()",     ".",      "->",     "&",
  "*",     "+",        "-",      "!",      NULL,     NULL,     "*",
  "/",     "%",        "+",      "-",      "<",      ">",      "<=",
  ">=",    "==",       "!=",     "=",      ",",      "&&",     "||",
  NULL,    NULL,       NULL,     "<",      ">",      "<=",     ">=",
  "==",    "!=",       NULL,     NULL,    NULL,      NULL
};