/* Code generation. */

#ifndef ASM_H
#define ASM_H

struct block;
struct ident;

typedef struct funcList
{
  struct ident *ident;
  struct block *block;
  struct funcList *next;
} FuncList;

enum
{
  EAX, EBX, ECX, EDX, EDI, ESI 
};

extern FuncList *globalFunc;

void addGlobalFunc (struct block *block);

void initReg ();

void emitFunc (FuncList *list);

void emitFuncs ();

void emitData ();

#endif