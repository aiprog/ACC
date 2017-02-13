/*
 *  Symbol table.
 */

#ifndef SYMBOL_H
#define SYMBOL_H
 
#include "error.h"
#include "table.h"
#include "type.h"

#define Type struct type

enum
{
  CT_INT = 1, CT_STR
};

typedef struct symbolTable
{
  HashTable *type;
  HashTable *ident;
  struct symbolTable *child;
} SymbolTable;

typedef struct offset
{
  int value;
  int direction;
} Offset;

typedef struct ident
{
  char *lexeme;
  int temp;
  Type *type;
  int reqSize;
  struct ident *address;
  int isStatic;
  int isGlobal;
  int isDefined;
  int isLvalue;
  union
  {
    int value;
    char *label;
  } location;
  int constType;
  union
  {
    int integer;
    char *string;
  } value;
  Position position;
  struct ident *next;
} Ident;

extern SymbolTable *top;
extern SymbolTable *unvisiable;
extern Ident *globalData;
extern Ident *func;
extern Offset offset;

/* Create a identifier. */
Ident *identNew (char *lexeme, Type *type, int isLvalue, 
                 int constType, Position *position);

/* Initialize the symbol table system. */
void symbolTableInitialize ();

/* Add global data. */
void symbolTableAddGlobalData (Ident *ident);

/* Test whether it is on global scope. */
int symbolTableGlobal ();

/* Test whether the current symbol table is empty. */
int symbolTableEmpty ();

/* Enter a scope. */
void symbolTablePush ();                                              

/* Leave a scope. */
void symbolTablePop ();

/* Insert a type to current symbol table. */
void symbolTableInsertType (char *typeName, Type *type);

/* Search for a type from current or all visible symbol tables. */
Type *symbolTableSearchType (char *typeName, int scope);

/* Insert a identifier to current symbol table. */
void symbolTableInsertIdent (char *identName, Ident *ident);

/* Search for a identifier from current or all visible symbol tables. */
Ident *symbolTableSearchIdent (char *identName, int scope);

#undef Type

#endif