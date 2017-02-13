#include <stdio.h>
#include <stdlib.h>

#include "symbol.h"
#include "type.h"

const char *tyToStr[12] =
{
  "char",  "uchar",  "short",  "ushort",  "int",   "uint",
  "void",
};

/* Create a BasicType. */
BasicType *basicTypeNew (int kind)
{
  BasicType *type;

  type = malloc (sizeof (BasicType));
  type->kind = kind;
  type->qualifier = 0;

  switch (kind)
  {
    case TK_CHAR:
      type->size = 1;

      break;
    case TK_SHORT:
      type->size = 2;

      break;
    case TK_INT:
      type->size = 4;

    break;
    case TK_VOID:
      type->size = 0;
  }

  return type;
}

/* Create a NewType. */
NewType *newTypeNew (int size, char *lexeme, SymbolTable *table, Position *pos)
{
  NewType *type;

  type = malloc (sizeof (NewType));
  type->kind = TK_NEW;
  type->size = size;
  type->qualifier = 0;
  type->lexeme = lexeme;
  type->table = table;
  type->position = *pos;

  return type;
}

/* Create a StructType. */
StructType *structTypeNew (NewType *type)
{
  StructType *structType;

  structType = malloc (sizeof (StructType));
  structType->kind = TK_STRUCT;
  structType->size = 0;
  structType->qualifier = 0;
  structType->type = type;

  return structType;
}

/* Search for a member in the struct. */
Ident *searchMember (StructType *type, char *lexeme)
{
  SymbolTable *temp;
  Ident *ident;

  temp = top;
  top = GNEW (type)->table;
  ident = symbolTableSearchIdent (lexeme, 0);
  top = temp;

  return ident;
}

/* Create a ArrayType. */
ArrayType *arrayTypeNew (int length)
{
  ArrayType *type;

  type = malloc (sizeof (ArrayType));
  type->kind = TK_ARRAY;
  type->qualifier = 0;
  type->length = length;

  return type;
}

/* Create a FuncType. */
FuncType *funcTypeNew (Ident *parameter)
{
  FuncType *type;

  type = malloc (sizeof (FuncType));
  type->kind = TK_FUNC;
  type->size = 4;
  type->qualifier = 0;
  type->parameter = parameter;

  return type;
}

/* Create a PtrType. */
PtrType *ptrTypeNew (int qualifier)
{
  PtrType *type;

  type = malloc (sizeof (PtrType));
  type->kind = TK_PTR;
  type->size = 4;
  type->qualifier = qualifier;

  return type;
}

/* Test whether two types are equal. */
int typeEqual (Type *type1, Type *type2)
{
  Ident *ite1, *ite2;

  if (GKIN (type1) != GKIN (type2))
    return 0;

  switch (GKIN (type1))
  {
    case TK_NEW:
      return type1 == type2;

    case TK_STRUCT:
      return typeEqual (TTYPE (GNEW (type1)), TTYPE (GNEW (type2)));

    case TK_ARRAY:
      if (GLEN (type1) != GLEN (type2))
        return 0;
      else if (!typeEqual (GELE (type1), GELE (type2)))
        return 0;

      break;
    case TK_FUNC:
      if (!typeEqual (GRET (type1), GRET (type2)))
        return 0;

      ite1 = GPAR (type1);
      ite2 = GPAR (type2);

      while (ite1 && (!ite1->lexeme || ite1->lexeme[0] != '?') && 
             ite2 && (!ite2->lexeme || ite2->lexeme[0] != '?'))
      {
        if (!typeEqual (ite1->type, ite2->type))
          return 0;

        ite1 = ite1->next;
        ite2 = ite2->next;
      }

      if ((ite1 || ite2) && 
          (!ite1->lexeme || ite1->lexeme[0] != '?' ||
           !ite1->lexeme || ite2->lexeme[0] != '?'))
        return 0;

      break;
    case TK_PTR:
      return typeEqual (GPTD (type1), GPTD (type2));
  }

  return 1;
}

/* Get the maximum of two types. */
Type *typeMax (Type *type1, Type *type2)
{
  if (GKIN (type1) > GKIN (type2))
    return type1;
  else
    return type2;
}

/* Print a type in human readable way. */
void typePrint (Type *type)
{
  Ident *ite;
  printf ("{\n");
  
  if (TBIT (GQUL (type), QU_CONST))
    printf ("const ");

  if (TBIT (GQUL (type), QU_VOLATILE))
    printf ("volatile ");

  switch (GKIN (type))
  {
    case TK_CHAR:
      printf ("char\n");

      break;
    case TK_UCHAR:
      printf ("uchar\n");

      break;
    case TK_SHORT:
      printf ("short\n");

      break;
    case TK_USHORT:
      printf ("ushort\n");

      break;
    case TK_INT:
      printf ("int\n");

      break;
    case TK_UINT:
      printf ("uint\n");

      break;
    case TK_VOID:
      printf ("void\n");

      break;
    case TK_STRUCT:
      printf ("struct %s\n", GLEX (type));

      break;
    case TK_ARRAY:
      printf ("array length: %d type:\n", GLEN (type));
      typePrint (GELE (type));

      break;
    case TK_FUNC:
      printf ("func retrun: \n");
      typePrint (GRET (type));

      ite = GPAR (type);
      while (ite)
      {
        if (ite->lexeme[0] != '?')
        {
          printf ("parameter %s: \n", ite->lexeme);
          typePrint (ite->type);
        }
        else
          printf ("parameter ...\n");

        ite = ite->next;
      }

      break;
    case TK_PTR:
      printf ("pointer type:\n");
      typePrint (GPTD (type));
  }
  printf ("}\n");
}