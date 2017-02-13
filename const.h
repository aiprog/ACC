/*
 *  Constants.
 */

#ifndef CONST_H
#define CONST_H

#include "symbol.h"

extern BasicType charT;
extern BasicType ucharT;
extern BasicType shortT;
extern BasicType ushortT;
extern BasicType intT;
extern BasicType uintT;

extern PtrType strT;

extern Ident intIdent;

extern Ident intZero;
extern Ident intOne;

Ident *intZeroNew ();
Ident *intOneNew ();

#endif