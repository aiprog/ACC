#include <stdlib.h>

#include "symbol.h"

SymbolTable *top;
SymbolTable *unvisible;
Ident *globalData;
Ident *func;
Offset offset;

extern struct funcList *globalFunc;

/* Create a identifier. */
Ident *identNew (char *lexeme, Type *type, int isLvalue, 
                 int constType, Position *position)
{
  Ident *ident;

  ident = malloc (sizeof (Ident));
  ident->temp = 0;
  ident->reqSize = 4;
  ident->address = ident;
  ident->lexeme = lexeme;
  ident->type = type;
  ident->isLvalue = isLvalue;
  ident->constType = constType;
  if (position)
    ident->position = *position;
  ident->next = NULL;

  return ident;
}

/* Initialize the symbol table system. */
void symbolTableInitialize ()
{
  top = unvisible = NULL;
  globalData = NULL;
  globalFunc = NULL;
  func = NULL;

  symbolTablePush ();
}

/* Add global data. */
void symbolTableAddGlobalData (Ident *ident)
{
  ident->next = globalData;
  globalData = ident;
}

/* Test whether it is on global scope. */
int symbolTableGlobal ()
{
  return !top->child;
}

/* Test whether the current symbol table is empty. */
int symbolTableEmpty ()
{
  return !hashTableSize (top->ident);
}

/* Enter a scope. */
void symbolTablePush ()
{
  SymbolTable *table;

  table = malloc (sizeof (SymbolTable));
  table->type = hashTableNew (1);
  table->ident = hashTableNew (1);
  table->child = top;
  top = table;
}

/* Leave a scope. */
void symbolTablePop ()
{
  SymbolTable *table;

  table = top;
  top = top->child;
  table->child = unvisible;
  unvisible = table;
}

/* Insert a type to current symbol table*/
void symbolTableInsertType (char *typeName, Type *type)
{
  hashTableInsert (top->type, typeName, type);
}

/* Search for a type from current or all visible symbol tables. */
Type *symbolTableSearchType (char *typeName, int scope)
{
  SymbolTable *ite;
  Type *type;

  if (!scope)
    return hashTableSearch (top->type, typeName);
  
  ite = top;
  do
  {
    if ((type = hashTableSearch (ite->type, typeName)))
      return type;
  } while ((ite = ite->child));

  return NULL;
}

/* Insert a identifier to current symbol table. */
void symbolTableInsertIdent (char *identName, Ident *ident)
{
  hashTableInsert (top->ident, identName, ident);
}

/* Search for a identifier from current or all visible symbol tables. */
Ident *symbolTableSearchIdent (char *identName, int scope)
{
  SymbolTable *ite;
  Ident *ident;

  if (!scope)
    return hashTableSearch (top->ident, identName);

  ite = top;
  do
  {
    if ((ident = hashTableSearch (ite->ident, identName)))
      return ident;
  } while ((ite = ite->child));

  return NULL;
}