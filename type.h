/*
 *  Type system.
 */

#ifndef TYPE_H
#define TYPE_H

#include "error.h"
#include "symbol.h"

#define SymbolTable struct symbolTable
#define Ident       struct ident

#define SBIT(x, y)  ((x) |= (y))
#define TBIT(x, y)  ((x) & (y))

#define TTYPE(x)    ((Type *) (x))
#define TBASIC(x)   ((BasicType *) (x))
#define TNEW(x)     ((NewType *) (x))
#define TSTRUCT(x)  ((StructType *) (x))
#define TARRAY(x)   ((ArrayType *) (x))
#define TFUNC(x)    ((FuncType *) (x))
#define TPTR(x)     ((PtrType *) (x))

#define GNEW(x)     (TSTRUCT (x)->type)
#define GLEX(x)     (TNEW (GNEW (x))->lexeme)
#define GLEN(x)     (TARRAY (x)->length)
#define GELE(x)     (TARRAY (x)->type)
#define GRET(x)     (TFUNC (x)->type)
#define GPAR(x)     (TFUNC (x)->parameter)
#define GPTD(x)     (TPTR (x)->type)

#define GKIN(x)     ((x)->kind)
#define GSZE(x)     (GKIN (x) == TK_STRUCT ? GNEW (x)->size : (x)->size)
#define GQUL(x)     ((x)->qualifier)

#define ISINT(x)    (GKIN (x) <= TK_UINT)
#define ISSCA(x)    (ISINT (x) || GKIN (x) == TK_PTR)
#define ISSIG(x)    (ISINT (x) && GKIN (x) % 2 == 0)

enum
{
  ST_EXTERN = 1, ST_STATIC
};

enum
{
  QU_CONST = 1, QU_VOLATILE
};

enum
{
  TK_CHAR, TK_UCHAR, TK_SHORT,  TK_USHORT, TK_INT,  TK_UINT, 
  TK_VOID, TK_NEW,   TK_STRUCT, TK_ARRAY,  TK_FUNC, TK_PTR
};

extern const char *tyToStr[12];

#define TYPE_COMMON \
  int kind;         \
  int size;         \
  int qualifier;

typedef struct type
{
  TYPE_COMMON
} Type;

typedef struct basicType
{
  TYPE_COMMON
} BasicType;

typedef struct newType
{
  TYPE_COMMON
  char *lexeme;
  SymbolTable *table;
  Position position;
} NewType;

typedef struct structType
{
  TYPE_COMMON
  NewType *type;
} StructType;

typedef struct arrayType
{
  TYPE_COMMON
  int length;
  Type *type; 
} ArrayType;

typedef struct funcType
{
  TYPE_COMMON
  int offset;
  Type *type;
  Ident *parameter;
} FuncType;

typedef struct ptrType
{
  TYPE_COMMON
  Type *type;
} PtrType;

/* Create a BasicType. */
BasicType *basicTypeNew (int kind);

/* Create a NewType. */
NewType *newTypeNew (int size, char *lexeme, SymbolTable *table, Position *pos);

/* Create a StructType. */
StructType *structTypeNew (NewType *type);

/* Search for a member in the struct. */
Ident *searchMember (StructType *type, char *lexeme);

/* Create a ArrayType. */
ArrayType *arrayTypeNew (int length);

/* Create a FuncType. */
FuncType *funcTypeNew (Ident *parameter);

/* Create a PtrType. */
PtrType *ptrTypeNew (int qualifier);

/* Test whether two types are equal. */
int typeEqual (Type *type1, Type *type2);

/* Get the maximum of two types. */
Type *typeMax (Type *type1, Type *type2);

/* Print a type in human readable way. */
void typePrint (Type *type);

#undef SymbolTable
#undef Ident

#endif