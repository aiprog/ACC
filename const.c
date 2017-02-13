/*
 *  Constants.
 */

#include <stdlib.h>
#include <string.h>

#include "const.h"

BasicType charT   = { TK_CHAR,   1, 0 };
BasicType ucharT  = { TK_UCHAR,  1, 0 };
BasicType shortT  = { TK_SHORT,  2, 0 };
BasicType ushortT = { TK_USHORT, 2, 0 };
BasicType intT    = { TK_INT,    4, 0 };
BasicType uintT   = { TK_UINT,   4, 0 };

PtrType strT = { TK_PTR, 4, 0, TTYPE (&charT) };

Ident intIdent = { NULL, 0, TTYPE (&intT), 4 };

Ident intZero = { NULL, 0, TTYPE (&intT), 4, NULL, 0, 0, 0, 0, { 0 }, CT_INT, { 0 } };
Ident intOne  = { NULL, 0, TTYPE (&intT), 4, NULL, 0, 0, 0, 0, { 0 }, CT_INT, { 1 } };

Ident *intZeroNew ()
{
  Ident *ident;

  ident = malloc (sizeof (Ident));
  memcpy (ident, &intZero, sizeof (Ident));

  return ident;
}

Ident *intOneNew ()
{
  Ident *ident;

  ident = malloc (sizeof (Ident));
  memcpy (ident, &intOne, sizeof (Ident));

  return ident;
}