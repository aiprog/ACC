#include <stdlib.h>

#include "ast.h"

/* Create a ast node. */
AstNode *astNodeNew (int opcode, AstNode *opr1, AstNode *opr2, Position *pos)
{
  AstNode *node;

  node = malloc (sizeof (AstNode));
  node->instr = 1;
  node->ident = NULL;
  node->opcode = opcode;
  node->oprand = opr1;
  if (pos)
    node->position = *pos;
  node->next = NULL;

  if (opr1)
    opr1->next = opr2;

  return node;
}