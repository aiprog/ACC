/*
 *  Common structs for lexer.c and parser.c.
 */

#ifndef COMMON_H
#define COMMON_H

#include "error.h"
#include "symbol.h"
#include "ast.h"
#include "block.h"

typedef struct decl
{
  Type *type;
  int sign;
  int store;
  int qualifier;
  Offset offset;
  Type **unknown;
  Position position;
  Ident *ident;
  Ident **parameter;
} Decl;

typedef struct id
{
  char *lexeme;
  int value;
  Position position;
} Id;

typedef union 
{ 
  Decl decl; 
  AstNode *node; 
  Id id; 
  Block *block; 
  int isFunc; 
} YYSTYPE;

extern YYSTYPE yylval;

#endif