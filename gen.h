/*
 *  Intermediate code generation.
 */

#ifndef GEN_H
#define GEN_H

#include "symbol.h"
#include "ast.h"
#include "block.h"

#define Block struct block

struct block;

typedef struct oprand
{
  Ident *ident;
  struct oprand *next;
} Oprand;

typedef struct instr
{
  struct instr *prev;
  struct instr *next;
  int opcode;
  Oprand *oprand;
} Instr;

/* Create a instruction. */
Instr *instrNew (int opcode, Ident *opr1, Ident *opr2, Ident *opr3);

/* Test whether it is a temporary identifier. */
int isTempIdnet (Ident *ident);

/* Automatic conversion for arrays and functions. */
Ident *arrayFuncConv (AstNode *node);

/* Append a oprand to the instruction. */
void instrAppendOprand (Instr *instr, Ident *opr);

/* Check the oprand types of a assignment. */
int checkAssType (Type *type1, Type *type2, Position *pos);

int checkRetType (Type *type1, Type *type2, Position *pos);

/* Generate value instructions for a expression. */
int genExprValue (AstNode *node);

/* Generate jump instructions for a expression. */
int genExprJump (AstNode *node, Block *tru, Block *fal);

/* Print intermediate code. */
void genPrint (Block *block);

#undef Block

#endif

