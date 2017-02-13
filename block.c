#include <stdlib.h>

#include "block.h"

static int blockNum;

Block *current;
Block *funcBeg;
Block *funcEnd;

BlockList *continueList;
BlockList *breakList;

/* Create a basic block. */
Block *blockNew ()
{
  Block *block;

  block = malloc (sizeof (Block));
  block->number = blockNum++;
  block->ref = 0;
  block->prev = block->next = NULL;
  block->head = block->tail = NULL;

  return block;
}


/* Convert a block to a ident. */
Ident *blockToIdent (Block *block)
{
  Ident *ident;

  ident = identNew ((char *) block, NULL, 0, 0, NULL);

  return ident;
}

/* Set current basic block. */
void setCurrentBlock (Block *block)
{
  current->next = block;
  block->prev = current;
  current = block;
}

/* Block list push. */
void blockListPush (BlockList **list, Block *block)
{
  BlockList *top;

  top = malloc (sizeof (BlockList));
  top->block = block;
  top->child = *list;

  *list = top;
}

/* Block list pop. */
Block *blockListPop (BlockList **list)
{
  Block *block;

  if (*list)
  {
    block = (*list)->block;
    if ((*list)->child)
      *list = (*list)->child;
    else
      *list = NULL;
  }
  else
    block = NULL;

  return block;
}