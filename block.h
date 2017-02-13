/*
 *  Basic block.
 */

#ifndef BLOCK_H
#define BLOCK_H

#include "symbol.h"
#include "gen.h"

#define Instr struct instr 

typedef struct block
{
  int number;
  int ref;
  struct block *prev;
  struct block *next;
  Instr *head;
  Instr *tail;
} Block;

typedef struct blockList
{
  Block *block;
  struct blockList *child;
} BlockList;

extern Block *current;
extern Block *funcBeg;
extern Block *funcEnd;

extern BlockList *continueList;
extern BlockList *breakList;

/* Create a basic block. */
Block *blockNew ();

/* Add a global function. */
void addGlobalFunc (Block *block);

/* Convert a block to a ident. */
Ident *blockToIdent (Block *block);

/* Set current basic block. */
void setCurrentBlock (Block *block);

/* Block list push. */
void blockListPush (BlockList **list, Block *block);

/* Block list pop. */
Block *blockListPop (BlockList **list);

#undef Instr 

#endif