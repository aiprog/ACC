/*
 *  Abstract syntax tree.
 */

#ifndef  AST_H
#define  AST_H

#include "error.h"
#include "symbol.h"

typedef struct astNode
{
  int instr;
  Ident *ident;
  int opcode;
  struct astNode *oprand;
  Position position;
  struct astNode *next;
} AstNode;

/* Create a ast node. */
AstNode *astNodeNew (int opcode, AstNode *opr1, AstNode *opr2, Position *pos);

#endif

