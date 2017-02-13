#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opcode.h"
#include "gen.h"
#include "const.h"

extern Offset offset;

static int tempNum;

/* Create a oprand. */
static Oprand *oprandNew (Ident *ident)
{
  Oprand *oprand;

  oprand = malloc (sizeof (Oprand));
  oprand->ident = ident;
  oprand->next = NULL;

  return oprand;
}

/* Create a instruction. */
Instr *instrNew (int opcode, Ident *opr1, Ident *opr2, Ident *opr3)
{
  Instr *instr;

  instr = malloc (sizeof (Instr));
  instr->opcode = opcode;
  instr->next = NULL;

  if (opr1)
    instr->oprand = oprandNew (opr1);
  if (opr2)
    instr->oprand->next = oprandNew (opr2);
  if (opr3)
    instr->oprand->next->next = oprandNew (opr3);

  if (!current->head)
  {
    instr->prev = NULL;
    current->head = instr;
    current->tail = instr;
  }
  else
  {
    instr->prev = current->tail;
    current->tail->next = instr;
    current->tail = instr;
  }

  return instr;
}

/* Test whether it is a temporary identifier. */
/*int isTempIdent (Ident *ident)
{
  return ident->temp;
}*/

/* Append a oprand to the instruction. */
void instrAppendOprand (Instr *instr, Ident *opr)
{
  Oprand *ite;

  if (!instr->oprand)
    instr->oprand = oprandNew (opr);
  else
  {
    ite = instr->oprand;
    while (ite->next)
      ite = ite->next;
    ite->next = oprandNew (opr);
  }
}

/* Create a temporary variable name. */
static char *tempNew ()
{
  char buffer[20];

  sprintf (buffer, "t%d", tempNum++);

  return strdup (buffer);
}

/* Create a temporary identifier. */
static Ident *tempIdentNew (int storage, Type *type, int isLvalue)
{
  Ident *ident;

  ident = identNew (tempNew (), type, isLvalue, 0, NULL);
  ident->temp = 1;
  if (storage)
  {
    offset.value -= 4;
    ident->location.value = offset.value;
  }

  return ident;
}

/* Integral promotion for integers. */
static Ident *integralPromotion (Ident *ident)
{
  Ident *temp;

  if (GKIN (ident->type) >= TK_INT)
    return ident;

  temp = tempIdentNew (1, TTYPE (&intT), 0);
  temp->isLvalue = 0;
  temp->constType = ident->constType;
  temp->value = ident->value;

  instrNew (OP_CAST, temp, &intIdent, ident);

  return temp;
}

/* Automatic conversion for arrays and functions. 
Ident *arrayFuncConv (Ident *ident)
{
  PtrType *ptrType;
  Ident *temp;

  if (GKIN (ident->type) != TK_ARRAY && GKIN (ident->type) != TK_FUNC)
    return ident;

  if ()
  ptrType = ptrTypeNew (0);
  if (GKIN (ident->type) == TK_ARRAY)
    GPTD (ptrType) = GELE (ident->type);
  else
    GPTD (ptrType) = ident->type;

  temp = malloc (sizeof (Ident));
  memcpy (temp, ident, sizeof (Ident));
  temp->type = TTYPE (ptrType);
  temp->isLvalue = 0;

  return temp;
}*/

Ident *arrayFuncConv (AstNode *node)
{
  PtrType *ptrType;
  Ident *ident, *temp;

  ident = node->ident;

  if (GKIN (ident->type) != TK_ARRAY && GKIN (ident->type) != TK_FUNC)
    return ident;

  ptrType = ptrTypeNew (0);
  if (GKIN (ident->type) == TK_ARRAY)
    GPTD (ptrType) = GELE (ident->type);
  else
    GPTD (ptrType) = ident->type;

  if (node->opcode == OP_ID)
  {
    temp = tempIdentNew (node->instr, TTYPE (ptrType), 0);
    if (node->instr)
      instrNew (OP_ADDR, temp, ident, NULL);
  }
  else
  {
    temp = malloc (sizeof (Ident));
    memcpy (temp, ident, sizeof (Ident));
    temp->type = TTYPE (ptrType);
    temp->isLvalue = 0;
  }

  node->ident = temp;

  return temp;
}

#define ID(x)   ((x)->ident)
#define TY(x)   ((x)->ident->type)
#define KIN(x)  (GKIN (TY (x)))

#define OPR1    (node->oprand)
#define OPR2    (node->oprand->next)
#define OPR3    (node->oprand->next->next)

#define ID1     (ID (OPR1))
#define ID2     (ID (OPR2))

#define TY1     (TY (OPR1))
#define TY2     (TY (OPR2))

#define KIN1    (KIN (OPR1))
#define KIN2    (KIN (OPR2))

static Type *subGenArrayAddress (AstNode *node, AstNode **head)
{
  Ident *ident, *prod;
  Type *type;

  if (node->opcode != OP_ARR)
  {
    if (genExprValue (node))
      return NULL;


    *head = node;

    return ID (node)->type;
  }

  OPR1->instr = OPR2->instr = node->instr;

  if (!(type = subGenArrayAddress (OPR1, head)) || genExprValue (OPR2))
    return NULL;

  if (GKIN (type) != TK_PTR && GKIN (type) != TK_ARRAY)
  {
    sprintf (buffer, "subscripted value is neither array nor pointer");
    errorPrint (EK_ERROR, buffer, &node->position);

    return NULL;
  }
  else if (!ISINT (TY2))
  {
    sprintf (buffer, "array subscript is not an integer");
    errorPrint (EK_ERROR, buffer, &node->position);

    return NULL;
  }

  ID2 = integralPromotion (ID2);

  if (GKIN (type) == TK_PTR)
    type = GPTD (type);
  else
    type = GELE (type);

  ident = identNew (NULL, TTYPE (&intT), 0, CT_INT, NULL);
  ident->value.integer = GSZE (type);

  prod = tempIdentNew (node->instr, TY2, 0);

  if (ID2->constType)      
  {
    prod->constType = CT_INT;
    prod->value.integer = ID2->value.integer * ident->value.integer;
  }
  else if (node->instr)
    instrNew (OP_MUL, prod, ID2, ident);

  if (OPR1->opcode != OP_ARR)
  {
    ID (node) = prod;

    return type;
  }

  if (ID1->constType && prod->constType)
  {
    ID1->value.integer += prod->value.integer;
    ID (node) = ID1;

    return type;
  }

  ident = tempIdentNew (node->instr, TTYPE (&intT), 0);
  if (node->instr)
    instrNew (OP_ADD, ident, ID1, prod);

  ID (node) = ident;

  return type;
}

/* Generate the address for a array element. */
static int genArrayAddress (AstNode *node)
{
  AstNode *temp;
  Ident *ident, *head;
  Type *type;
  PtrType *ptrType;

  if (!(type = subGenArrayAddress (node, &temp)))
    return 1;
  
  head = arrayFuncConv (temp);

  ptrType = ptrTypeNew (0);
  GPTD (ptrType) = type;

    ident = tempIdentNew (node->instr, TTYPE (ptrType), 0);
    if (node->instr)
      instrNew (OP_ADD, ident, head, ID (node));

    node->ident = ident;

  return 0;
}

/* Generate the address for a struct member. */
static int genDotAddress (AstNode *node)
{
  PtrType *type;
  Ident *ident, *sum, *temp;

  if (OPR1->opcode != OP_ID)
  {
    OPR1->instr = node->instr;
    if (genExprValue (OPR1))
      return 1;
  }

  if (KIN1 != TK_STRUCT)
  {
    sprintf (buffer, "request for member '%s' in something not a structure",
             ID2->lexeme);
    errorPrint (EK_ERROR, buffer, &node->position);

    return 1;
  }

  if (!(temp = searchMember (TSTRUCT (TY1), ID2->lexeme)))
  {
    sprintf (buffer, "‘struct '%s' has no member named '%s'",
             GNEW (TY1)->lexeme, ID2->lexeme);
    errorPrint (EK_ERROR, buffer, &node->position);

    return 1;
  }

  type = ptrTypeNew (0);
  GPTD (type) = TY (OPR1);

  ident = tempIdentNew (node->instr, TTYPE (type), 0);
  if (node->instr)
    instrNew (OP_ADDR, ident, ID (OPR1), NULL);
  ID1 = ident;

  ident = identNew (NULL, TTYPE (&intT), 0, CT_INT, NULL);
  ident->value.integer = temp->location.value;

  type = ptrTypeNew (0);
  GPTD (type) = temp->type;

  sum = tempIdentNew (1, TTYPE (type), 0);
  if (node->instr)
    instrNew (OP_ADD, sum, ID1, ident);

  node->ident = sum;

  return 0;
}

/* Generate the address for a struct member. */
static int genPtrAddress (AstNode *node)
{
  Ident *ident, *sum, *temp;
  PtrType *type;

  OPR1->instr = node->instr;
  if (genExprValue (OPR1))
    return 1;

  ID1 = arrayFuncConv (OPR1);

  if (KIN1 != TK_PTR || GKIN (GPTD (TY1)) != TK_STRUCT)
  {
    sprintf (buffer, "request for member '%s' in something not a structure",
             ID2->lexeme);
    errorPrint (EK_ERROR, buffer, &node->position);

    return 1;

  }

  if (!(temp = searchMember (TSTRUCT (GPTD (TY1)), ID2->lexeme)))
  {
    sprintf (buffer, "‘struct '%s' has no member named '%s'",
             GNEW (GPTD (TY1))->lexeme, ID2->lexeme);
    errorPrint (EK_ERROR, buffer, &node->position);

    return 1;
  }

  ident = identNew (NULL, TTYPE (&intT), 0, CT_INT, NULL);
  ident->value.integer = temp->location.value;

  type = ptrTypeNew (0);
  GPTD (type) = temp->type;

  sum = tempIdentNew (node->instr, TTYPE (type), 0);
  if (node->instr)
    instrNew (OP_ADD, sum, ID1, ident);

  node->ident = sum;

  return 0;
}

/* Check the oprand types of a assignment. */
int checkAssType (Type *type1, Type *type2, Position *pos)
{
  if (GKIN (type1) == TK_ARRAY || GKIN (type1) == TK_FUNC)
  {
    sprintf (buffer, "assignment to expression with wrong type");
    errorPrint (EK_ERROR, buffer, pos);

    return 1;

  }

  if ((!ISSCA (type1) || !ISSCA (type2)) && 
      (GKIN (type1) != TK_STRUCT || !typeEqual (type1, type2)))
  {
    sprintf (buffer, "incompatible oprand types of '='");
    errorPrint (EK_ERROR, buffer, pos);

    return 1;
  }

  if (!typeEqual (type1, type2))
  {
    sprintf (buffer, "assignment between different types");
    errorPrint (EK_WARN, buffer, pos);
  }

  return 0;
}

int checkRetType (Type *type1, Type *type2, Position *pos)
{
  if (!ISSCA (type1) || !ISSCA (type2))
  {
    sprintf (buffer, "wrong return type");
    errorPrint (EK_ERROR, buffer, pos);

    return 1;
  }

  if (!typeEqual (type1, type2))
  {
    sprintf (buffer, "return between different types");
    errorPrint (EK_WARN, buffer, pos);
  }

  return 0;
}

/* Generate value instructions for a expression. */
int genExprValue (AstNode *node)
{
  AstNode *argu;
  Ident *ident, *sum, *diff, *prod, *quot, *para, *temp;
  Ident *label1, *label2, *label3;;
  Instr *instr;
  PtrType *ptrType;
  Type *type;
  Block *block1, *block2, *block3;
  Oprand *head, *tail;
  int num, opcode;

  switch (node->opcode)
  { 
    case OP_ID:
      /* Empty. */
      break;
    case OP_CONST:
      if (ID (node)->constType == CT_STR)
      {
        ident = tempIdentNew (node->instr, TTYPE (&strT), 0);
        if (node->instr)
          instrNew (OP_ADDR, ident, ID (node), NULL);
        node->ident = ident;
      }

      break;
    case OP_ARR:
      if (genArrayAddress (node))
        return 1;

      ident = tempIdentNew (1, GPTD (TY (node)), 0);
      if (node->instr)
        instrNew (OP_DER, ident, node->ident, NULL);

      node->ident = ident;

      break;
    case OP_CAL:
      if (genExprValue (OPR1))
        return 1;

      if (KIN1 == TK_FUNC)
        type = TY1;
      else if (KIN1 == TK_PTR && GKIN (GPTD (TY1)) == TK_FUNC)
        type = GPTD (TY1);
      else
      {
        sprintf (buffer, "called object is not a function or function pointer");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

      head = oprandNew (ID1);
      tail = head;

      para = GPAR (type);
      argu = OPR2;

      num = 1;
      while (para && argu)
      {
        if (genExprValue (argu))
          return 1;

        argu->ident = arrayFuncConv (argu);
        argu->ident = integralPromotion (argu->ident);

        if (KIN (argu) == TK_STRUCT)
        {
          sprintf (buffer, "wrong type for argument %d", num);
          errorPrint (EK_ERROR, buffer, &node->position);

          return 1;
        }

        if (para->lexeme[0] != '?' && 
            (!ISINT (TY (argu)) || !ISINT (para->type)) && 
            !typeEqual (para->type, TY (argu)))
        {
          sprintf (buffer, "incompatible type for argument %d", num);
          errorPrint (EK_WARN, buffer, &node->position);
        }

        tail->next = oprandNew (ID (argu));
        tail = tail->next;

        if (para->lexeme[0] != '?')
          para = para->next;
        argu = argu->next;

        ++num;
      }

      if ((para && para->lexeme[0] != '?') || argu)
      {
        sprintf (buffer, "wrong argument number");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

      if (node->instr)
      {
        ident = tempIdentNew (1, GRET (type), 0);
        instr = instrNew (OP_CALASS, ident, NULL, NULL);
        instr->oprand->next = head;

        node->ident = ident;
      }
      else
      {
        instr = instrNew (OP_CAL, NULL, NULL, NULL);
        instr->oprand = head;
      }

      break;
    case OP_DOT:
      if (genDotAddress (node))
        return 1;

        ident = tempIdentNew (node->instr, GPTD (TY (node)), 0);
        if (node->instr)
          instrNew (OP_DER, ident, ID (node), NULL);

        node->ident = ident;

      break;
    case OP_PTR:
      if (genPtrAddress (node))
        return 1;
  
        ident = tempIdentNew (node->instr, GPTD (TY (node)), 0);
        if (node->instr)
          instrNew (OP_DER, ident, ID (node), NULL);

        node->ident = ident;

      break;
    case OP_ADDR:
      if (OPR1->opcode == OP_DER)
      {
        OPR1->instr = node->instr;
        if (genExprValue (OPR1->oprand))
          return 1;

        ID (OPR1->oprand) = arrayFuncConv (OPR1->oprand);

        if (KIN (OPR1->oprand) != TK_PTR)
        {
          sprintf (buffer, "invalid oprand type of unary '*'");
          errorPrint (EK_ERROR, buffer, &OPR1->position);

          return 1;
        }

          node->ident = ID (OPR1->oprand);
      }
      else if (OPR1->opcode == OP_ID && (KIN1 == TK_ARRAY || KIN1 == TK_FUNC))
      {
        OPR1->instr = node->instr;
        if (genExprValue (OPR1))
          return 1;

          node->ident = arrayFuncConv (OPR1);
      }
      else if (OPR1->opcode == OP_ARR)
      {
        OPR1->instr = node->instr;
        if (genArrayAddress (OPR1))
          return 1;

          node->ident = ID1;
      }
      else if (ID1->constType == CT_STR)
      {
        ident = tempIdentNew (node->instr, TTYPE (&strT), 0);
        if (node->instr)
          instrNew (OP_ADDR, ident, ID (node), NULL);
        node->ident = ident;
      }
      else
      {
        OPR1->instr = node->instr;
        if (genExprValue (OPR1))
          return 1;

        if (!ID1->isLvalue)
        {
          sprintf (buffer, "lvalue required as unary '&' operand");
          errorPrint (EK_ERROR, buffer, &node->position);

          return 1;
        }

        ptrType = ptrTypeNew (0);
        GPTD (ptrType) = TY1;

        ident = tempIdentNew (node->instr, TTYPE (ptrType), 0);
        if (node->instr)
          instrNew (OP_ADDR, ident, ID1, NULL);

        node->ident = ident;
      }

      break;
    case OP_DER:
      if (OPR1->opcode == OP_ADDR)
      {
        opcode = OPR1->oprand->opcode;
        if (opcode == OP_ID || opcode == OP_ARR || opcode == OP_DER)
        {
          OPR1->oprand->instr = node->instr;
          if (genExprValue (OPR1->oprand))
            return 1;

            node->ident = ID (OPR1->oprand);
        }

        return 0;
      }

      OPR1->instr = node->instr;
      if (genExprValue (OPR1))
        return 1;

      ID1 = arrayFuncConv (OPR1);

      if (KIN1 != TK_PTR)
      {
        sprintf (buffer, "invalid oprand type of unary '*'");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

 
        ident = tempIdentNew (node->instr, GPTD (TY1), 1);
        if (node->instr)
          instrNew (OP_DER, ident, ID1, NULL);

        node->ident = ident;

      break;
    case OP_UADD:
      OPR1->instr = node->instr;
      if (genExprValue (OPR1))
        return 1;

      if (!ISINT (TY1))
      {
        sprintf (buffer, "invalid oprand type of unary '+'");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

        node->ident = ID1;

      break;
    case OP_USUB:
      OPR1->instr = node->instr;
      if (genExprValue (OPR1))
        return 1;

      if (!ISINT (TY1))
      {
        sprintf (buffer, "invalid oprand type of unary '-'");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

   
        ID1 = integralPromotion (ID1);
        ident = tempIdentNew (node->instr, TY1, 0);

        if (node->instr)
          instrNew (OP_USUB, ident, ID1, NULL);

        node->ident = ident;

      break;
    case OP_NOT:
      OPR1->instr = node->instr;
      if (genExprValue (OPR1))
        return 1;

      ID1 = arrayFuncConv (OPR1);

      if (!ISSCA (TY1))
      {
        sprintf (buffer, "invalid oprand type of unary '!'");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

      ID1 = integralPromotion (ID1);
      ident = tempIdentNew (node->instr, TTYPE (&intT), 0);
      node->ident = ident;

      if (node->instr)
      {
        block1 = blockNew ();
        block2 = blockNew ();
        block3 = blockNew ();

        label1 = blockToIdent (block1);
        label2 = blockToIdent (block2);
        label3 = blockToIdent (block3);

        instrNew (OP_IFNOT, ID1, label2, NULL);
        block2->ref = 1;
        setCurrentBlock (block1);
        instrNew (OP_ASS, ident, intZeroNew (), NULL);
        instrNew (OP_GOTO, label3, NULL, NULL);
        block3->ref = 1;
        setCurrentBlock (block2);
        instrNew (OP_ASS, ident, intOneNew (), NULL);
        setCurrentBlock (block3);

      }

      break;
    case OP_SIZE:
      OPR1->instr = node->instr;
      if (genExprValue (OPR1))
        return 1;
     
      ident = identNew (NULL, TTYPE (&intT), 0, CT_INT, NULL);
      ident->value.integer = GSZE (TY1);

      node->ident = ident;

      break;
    case OP_CAST:
      OPR1->instr = node->instr;
      if (genExprValue (OPR2))
        return 1;

      ID2 = arrayFuncConv (OPR2);

      if (KIN1 == TK_STRUCT || KIN1 == TK_ARRAY)
      {
        sprintf (buffer, "illegal destination type");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

      if (KIN2 == TK_STRUCT)
      {
        sprintf (buffer, "illegal source type");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

 
        if (KIN1 == TK_PTR)
          ID2 = integralPromotion (ID2);

        if (GSZE (TY1) == GSZE (TY2))
        {
          ident = malloc (sizeof (Ident));
          memcpy (ident, ID2, sizeof (Ident));
          ident->type = TY1;
          ident->isLvalue = 0;
        }
        else
        {
          ident = tempIdentNew (node->instr, TY1, 0);
          ident->value = ID2->value;

          if (node->instr)
            instrNew (OP_CAST, ident, ID1, ID2);
        }

        node->ident = ident;

      break;
    case OP_MUL:
      OPR1->instr = OPR2->instr = node->instr;
      if (genExprValue (OPR1) || genExprValue (OPR2))
        return 1;

      if (!ISINT (TY1) || !ISINT (TY2))
      {
        sprintf (buffer, "invalid oprand type of '*'");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

   
        ID1 = integralPromotion (ID1);
        ID2 = integralPromotion (ID2);

        ident = tempIdentNew (node->instr, typeMax (TY1, TY2), 0);
        if (node->instr)
          instrNew (OP_MUL, ident, ID1, ID2);

        node->ident = ident;


      break;
    case OP_DIV:
      OPR1->instr = OPR2->instr = node->instr;
      if (genExprValue (OPR1) || genExprValue (OPR2))
        return 1;

      if (!ISINT (TY1) || !ISINT (TY2))
      {
        sprintf (buffer, "invalid oprand type of '/'");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }


        ID1 = integralPromotion (ID1);
        ID2 = integralPromotion (ID2);

        ident = tempIdentNew (node->instr, typeMax (TY1, TY2), 0);
        if (node->instr)
          instrNew (OP_DIV, ident, ID1, ID2);

        node->ident = ident;

      break;
    case OP_MOD:
      OPR1->instr = OPR2->instr = node->instr;
      if (genExprValue (OPR1) || genExprValue (OPR2))
        return 1;

      if (!ISINT (TY1) || !ISINT (TY2))
      {
        sprintf (buffer, "invalid oprand type of '%%'");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }


        ID1 = integralPromotion (ID1);
        ID2 = integralPromotion (ID2);

        ident = tempIdentNew (node->instr, typeMax (TY1, TY2), 0);
        if (node->instr)
          instrNew (OP_MOD, ident, ID1, ID2);

        node->ident = ident;

      break;
    case OP_ADD:
      OPR1->instr = OPR2->instr = node->instr;
      if (genExprValue (OPR1) || genExprValue (OPR2))
        return 1;

      ID1 = arrayFuncConv (OPR1);
      ID2 = arrayFuncConv (OPR2);

      if (ISINT (TY1))
      {
        temp = ID1;
        ID1 = ID2;
        ID2 = temp;
      }

      if ((!ISSCA (TY1)) || !ISINT (TY2))
      {
        sprintf (buffer, "invalid oprand type of '+'");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }
      else if (ISINT (TY1))
      {
   
          ID1 = integralPromotion (ID1);
          ID2 = integralPromotion (ID2);

          ident = tempIdentNew (node->instr, typeMax (TY1, TY2), 0);
          if (node->instr)
            instrNew (OP_ADD, ident, ID1, ID2);

          node->ident = ident;
      }
      else 
      {
  
          if (ID2->constType)
          {
            ID2->value.integer *= GSZE (GPTD (TY1));
            sum = tempIdentNew (1, TY1, 0);
            if (node->instr)
              instrNew (OP_ADD, sum, ID1, ID2);
          }
          else
          {
            ID2 = integralPromotion (ID2);

            ident = identNew (NULL, TTYPE (&intT), 0, CT_INT, NULL);
            ident->value.integer = GSZE (GPTD (TY1));

            prod = tempIdentNew (node->instr, TY2, 0);
            if (node->instr) 
              instrNew (OP_MUL, prod, ID2, ident);

            sum = tempIdentNew (node->instr, TY1, 0);
            if (node->instr)
              instrNew (OP_ADD, sum, ID1, prod);
          }

          node->ident = sum;
      }

      break;
    case OP_SUB:
      OPR1->instr = OPR2->instr = node->instr;
      if (genExprValue (OPR1) || genExprValue (OPR2))
        return 1;

      ID1 = arrayFuncConv (OPR1);
      ID2 = arrayFuncConv (OPR2);

      if (ISINT (TY1) && ISINT (TY2))
      {

          ID1 = integralPromotion (ID1);
          ID2 = integralPromotion (ID2);

          ident = tempIdentNew (node->instr, typeMax (TY1, TY2), 0);
          if (node->instr)
            instrNew (OP_SUB, ident, ID1, ID2);

          node->ident = ident;
      }
      else if (KIN1 == TK_PTR && ISINT (TY2))
      {

          if (ID2->constType)
          {
            ID2->value.integer *= GSZE (GPTD (TY1));
            diff = tempIdentNew (node->instr, TY1, 0);
            if (node->instr)
              instrNew (OP_SUB, diff, ID1, ID2);
          }
          else
          {
            ID2 = integralPromotion (ID2);

            ident = identNew (NULL, TTYPE (&intT), 0, CT_INT, NULL);
            ident->value.integer = GSZE (GPTD (TY1));

            prod = tempIdentNew (node->instr, TY2, 0);
            if (node->instr) 
              instrNew (OP_MUL, prod, ID2, ident);

            diff= tempIdentNew (node->instr, TY1, 0);
            if (node->instr)
              instrNew (OP_SUB, diff, ID1, prod);
          }

          node->ident = diff;
      }
      else if (KIN1 == TK_PTR && KIN2 == TK_PTR && typeEqual (TY1, TY2))
      {

          diff = tempIdentNew (node->instr, TTYPE (&intT), 0);
          if (node->instr)
            instrNew (OP_SUB, diff, ID1, ID2);

          ident = identNew (NULL, TTYPE (&intT), 0, CT_INT, NULL);
          ident->value.integer = GSZE (GPTD (TY1));

          quot = tempIdentNew (node->instr, TTYPE (&intT), 0);
          if (node->instr)
            instrNew (OP_DIV, quot, diff, ident);

          node->ident = quot;
      }
      else
      {
        sprintf (buffer, "invalid oprand type of '-'");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

      break;
    case OP_LT:
      OPR1->instr = OPR2->instr = node->instr;
      if (genExprValue (OPR1) || genExprValue (OPR2))
        return 1;

      ID1 = arrayFuncConv (OPR1);
      ID2 = arrayFuncConv (OPR2);

      if (!ISSCA (TY1) || !ISSCA (TY2))
      {
        sprintf (buffer, "invalid oprand type of '<'");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

      ID1 = integralPromotion (ID1);
      ID2 = integralPromotion (ID2);

      if ((!ISINT (TY1) || !ISINT (TY2)) && !typeEqual (TY1, TY2))
      {
        sprintf (buffer, "comparsion between different types");
        errorPrint (EK_WARN, buffer, &node->position);

        return 1;
      }


        ident = tempIdentNew (node->instr, TTYPE (&intT), 0); 
        node->ident = ident;

        block1 = blockNew ();
        block2 = blockNew ();
        block3 = blockNew ();

        label2 = blockToIdent (block2);
        label3 = blockToIdent (block3);

        if (node->instr)
        {
          instrNew (OP_IFLT, ID1, ID2, label2);
          block2->ref = 1;
        }

        setCurrentBlock (block1);
        if (node->instr)
        {
          instrNew (OP_ASS, ident, intZeroNew (), NULL);
          instrNew (OP_GOTO, label3, NULL, NULL);
          block3->ref = 1;
        }

        setCurrentBlock (block2);
        if (node->instr)
          instrNew (OP_ASS, ident, intOneNew (), NULL);

        setCurrentBlock (block3);

      break;
    case OP_GT:
      OPR1->instr = OPR2->instr = node->instr;
      if (genExprValue (OPR1) || genExprValue (OPR2))
        return 1;

      ID1 = arrayFuncConv (OPR1);
      ID2 = arrayFuncConv (OPR2);

      if (!ISSCA (TY1) || !ISSCA (TY2))
      {
        sprintf (buffer, "invalid oprand type of '>'");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

      ID1 = integralPromotion (ID1);
      ID2 = integralPromotion (ID2);

      if ((!ISINT (TY1) || !ISINT (TY2)) && !typeEqual (TY1, TY2))
      {
        sprintf (buffer, "comparsion between different types");
        errorPrint (EK_WARN, buffer, &node->position);

        return 1;
      }


        ident = tempIdentNew (node->instr, TTYPE (&intT), 0); 
        node->ident = ident;

        block1 = blockNew ();
        block2 = blockNew ();
        block3 = blockNew ();

        label2 = blockToIdent (block2);
        label3 = blockToIdent (block3);

        if (node->instr)
        {
          instrNew (OP_IFGT, ID1, ID2, label2);
          block2->ref = 1;
        }

        setCurrentBlock (block1);
        if (node->instr)
        {
          instrNew (OP_ASS, ident, intZeroNew (), NULL);
          instrNew (OP_GOTO, label3, NULL, NULL);
           block3->ref = 1;
        }

        setCurrentBlock (block2);
        if (node->instr)
          instrNew (OP_ASS, ident, intOneNew (), NULL);

        setCurrentBlock (block3);

      break;
    case OP_LE:
      OPR1->instr = OPR2->instr = node->instr;
      if (genExprValue (OPR1) || genExprValue (OPR2))
        return 1;

      ID1 = arrayFuncConv (OPR1);
      ID2 = arrayFuncConv (OPR2);

      if (!ISSCA (TY1) || !ISSCA (TY2))
      {
        sprintf (buffer, "invalid oprand type of '<='");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

      ID1 = integralPromotion (ID1);
      ID2 = integralPromotion (ID2);

      if ((!ISINT (TY1) || !ISINT (TY2)) && !typeEqual (TY1, TY2))
      {
        sprintf (buffer, "comparsion between different types");
        errorPrint (EK_WARN, buffer, &node->position);

        return 1;
      }

        ident = tempIdentNew (node->instr, TTYPE (&intT), 0); 
        node->ident = ident;

        block1 = blockNew ();
        block2 = blockNew ();
        block3 = blockNew ();

        label2 = blockToIdent (block2);
        label3 = blockToIdent (block3);

        if (node->instr)
        {
          instrNew (OP_IFLE, ID1, ID2, label2);
          block2->ref = 1;
        }

        setCurrentBlock (block1);

        if (node->instr)
        {
          instrNew (OP_ASS, ident, intZeroNew (), NULL);
          instrNew (OP_GOTO, label3, NULL, NULL);
          block3->ref = 1;
        }

        setCurrentBlock (block2);
        if (node->instr)
          instrNew (OP_ASS, ident, intOneNew (), NULL);

        setCurrentBlock (block3);

      break;
    case OP_GE:
      OPR1->instr = OPR2->instr = node->instr;
      if (genExprValue (OPR1) || genExprValue (OPR2))
        return 1;

      ID1 = arrayFuncConv (OPR1);
      ID2 = arrayFuncConv (OPR2);

      if (!ISSCA (TY1) || !ISSCA (TY2))
      {
        sprintf (buffer, "invalid oprand type of '>='");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

      ID1 = integralPromotion (ID1);
      ID2 = integralPromotion (ID2);

      if ((!ISINT (TY1) || !ISINT (TY2)) && !typeEqual (TY1, TY2))
      {
        sprintf (buffer, "comparsion between different types");
        errorPrint (EK_WARN, buffer, &node->position);

        return 1;
      }


        ident = tempIdentNew (node->instr, TTYPE (&intT), 0); 
        node->ident = ident;

        block1 = blockNew ();
        block2 = blockNew ();
        block3 = blockNew ();

        label2 = blockToIdent (block2);
        label3 = blockToIdent (block3);

        if (node->instr)
        {
          instrNew (OP_IFGE, ID1, ID2, label2);
          block2->ref = 1;
        }

        setCurrentBlock (block1);

        if (node->instr)
        {
          instrNew (OP_ASS, ident, intZeroNew (), NULL);
          instrNew (OP_GOTO, label3, NULL, NULL);
           block3->ref = 1;
        }

        setCurrentBlock (block2);
        if (node->instr)
          instrNew (OP_ASS, ident, intOneNew (), NULL);

        setCurrentBlock (block3);
  
      break;
    case OP_EQ:
      OPR1->instr = OPR2->instr = node->instr;
      if (genExprValue (OPR1) || genExprValue (OPR2))
        return 1;

      ID1 = arrayFuncConv (OPR1);
      ID2 = arrayFuncConv (OPR2);

      if (!ISSCA (TY1) || !ISSCA (TY2))
      {
        sprintf (buffer, "invalid oprand type of '=='");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

      ID1 = integralPromotion (ID1);
      ID2 = integralPromotion (ID2);

      if ((!ISINT (TY1) || !ISINT (TY2)) && !typeEqual (TY1, TY2))
      {
        sprintf (buffer, "comparsion between different types");
        errorPrint (EK_WARN, buffer, &node->position);

        return 1;
      }

        ident = tempIdentNew (node->instr, TTYPE (&intT), 0); 
        node->ident = ident;

        block1 = blockNew ();
        block2 = blockNew ();
        block3 = blockNew ();

        label2 = blockToIdent (block2);
        label3 = blockToIdent (block3);

        if (node->instr)
        {
          instrNew (OP_IFEQ, ID1, ID2, label2);
          block2->ref = 1;
        }

        setCurrentBlock (block1);

        if (node->instr)
        {
          instrNew (OP_ASS, ident, intZeroNew (), NULL);
          instrNew (OP_GOTO, label3, NULL, NULL);
           block3->ref = 1;
        }

        if (node->instr)
        {
          setCurrentBlock (block2);
          instrNew (OP_ASS, ident, intOneNew (), NULL);

          setCurrentBlock (block3);
        }

      break;
    case OP_NE:
      OPR1->instr = OPR2->instr = node->instr;
      if (genExprValue (OPR1) || genExprValue (OPR2))
        return 1;

      ID1 = arrayFuncConv (OPR1);
      ID2 = arrayFuncConv (OPR2);

      if (!ISSCA (TY1) || !ISSCA (TY2))
      {
        sprintf (buffer, "invalid oprand type of '!='");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

      ID1 = integralPromotion (ID1);
      ID2 = integralPromotion (ID2);

      if ((!ISINT (TY1) || !ISINT (TY2)) && !typeEqual (TY1, TY2))
      {
        sprintf (buffer, "comparsion between different types");
        errorPrint (EK_WARN, buffer, &node->position);

        return 1;
      }


        ident = tempIdentNew (node->instr, TTYPE (&intT), 0); 
        node->ident = ident;

        block1 = blockNew ();
        block2 = blockNew ();
        block3 = blockNew ();

        label2 = blockToIdent (block2);
        label3 = blockToIdent (block3);

        if (node->instr)
        {
          instrNew (OP_IFNE, ID1, ID2, label2);
          block2->ref = 1;

        setCurrentBlock (block1);
        instrNew (OP_ASS, ident, intZeroNew (), NULL);
        instrNew (OP_GOTO, label3, NULL, NULL);
         block3->ref = 1;

        setCurrentBlock (block2);
        instrNew (OP_ASS, ident, intOneNew (), NULL);

        setCurrentBlock (block3);
      }
      break;
    case OP_AND:
      if (OPR1->opcode == OP_CONST && KIN1 == TK_INT)
      {
        if (ID1->value.integer)
        {
          OPR2->instr = node->instr;
          if (genExprValue (OPR2))
            return 1;

          ID2 = arrayFuncConv (OPR2);

          if (!ISSCA (TY2))
          {
            sprintf (buffer, "invalid oprand type of '&&'");
            errorPrint (EK_ERROR, buffer, &node->position);

            return 1;
          }

          ID2 = integralPromotion (ID2);

          if (OPR2->opcode == OP_AND || OPR2->opcode == OP_OR)
          {
              node->ident = ID2;
          }
          else
          {
            ident = tempIdentNew (node->instr, TTYPE (&intT), 0);
            node->ident = ident;

            if (node->instr)
            {
              block1 = blockNew ();
            block2 = blockNew ();

            label1 = blockToIdent (block1);
            label2 = blockToIdent (block2);

            instrNew (OP_IFNOT, ID2, label1, NULL);
            block1->ref = 1;

            instrNew (OP_ASS, ident, intOneNew (), NULL);
            instrNew (OP_GOTO, label2, NULL, NULL);
             block2->ref = 1;

            setCurrentBlock (block1);
            instrNew (OP_ASS, ident, intZeroNew (), NULL);

            setCurrentBlock (block2);
          }
          }
        }
        else
        {
          OPR2->instr = 0;
          if (genExprValue (OPR2))
            return 1;

          ID2 = arrayFuncConv (OPR2);

          if (!ISSCA (TY2))
          {
            sprintf (buffer, "invalid oprand type of '&&'");
            errorPrint (EK_ERROR, buffer, &node->position);

            return 1;
          }

            node->ident = intZeroNew ();
        } 
      }
      else if (OPR2->opcode == OP_CONST && KIN2 == TK_INT)
      {
        if (ID2->value.integer)
        {
          OPR1->instr = node->instr;
          if (genExprValue (OPR1))
            return 1;

          ID1 = arrayFuncConv (OPR1);

          if (!ISSCA (TY1))
          {
            sprintf (buffer, "invalid oprand type of '&&'");
            errorPrint (EK_ERROR, buffer, &node->position);

            return 1;
          }

          ID1 = integralPromotion (ID1);

          if (OPR1->opcode == OP_AND || OPR1->opcode == OP_OR)
          {
              node->ident = ID1;
          }
          else
          {
            ident = tempIdentNew (node->instr, TTYPE (&intT), 0);
            node->ident = ident;

            if (node->instr)
            {
              block1 = blockNew ();
            block2 = blockNew ();

            label1 = blockToIdent (block1);
            label2 = blockToIdent (block2);

            instrNew (OP_IFNOT, ID1, label1, NULL);
            block1->ref = 1;

            instrNew (OP_ASS, ident, intOneNew (), NULL);
            instrNew (OP_GOTO, label2, NULL, NULL);
             block2->ref = 1;

            setCurrentBlock (block1);
            instrNew (OP_ASS, ident, intZeroNew (), NULL);

            setCurrentBlock (block2);
          }
          } 
        }
        else
        {
          OPR1->instr = 0;
          if (genExprValue (OPR1))
            return 1;

          ID1 = arrayFuncConv (OPR1);

          if (!ISSCA (TY1))
          {
            sprintf (buffer, "invalid oprand type of '&&'");
            errorPrint (EK_ERROR, buffer, &node->position);

            return 1;
          }

            node->ident = intZeroNew ();
        } 
      }
      else
      {
        block1 = blockNew ();
        block2 = blockNew ();

        label1 = blockToIdent (block1);
        label2 = blockToIdent (block2);

        if (genExprJump (node, NULL, block1))
          return 1;

        ident = tempIdentNew (node->instr, TTYPE (&intT), 0);
        node->ident = ident;

        if (node->instr)
        {
          instrNew (OP_ASS, ident, intOneNew (), NULL);
        instrNew (OP_GOTO, label2, NULL, NULL);
         block2->ref = 1;

        setCurrentBlock (block1);
        instrNew (OP_ASS, ident, intZeroNew (), NULL);

        setCurrentBlock (block2);
       }
      }

      break;
    case OP_OR:
      if (OPR1->opcode == OP_CONST && KIN1 == TK_INT)
      {
        if (ID1->value.integer)
        {
          OPR2->instr = 0;
          if (genExprValue (OPR2))
            return 1;

          ID2 = arrayFuncConv (OPR2);

          if (!ISSCA (TY2))
          {
            sprintf (buffer, "invalid oprand type of '||'");
            errorPrint (EK_ERROR, buffer, &node->position);

            return 1;
          }

          node->ident = intOneNew ();
        }
        else
        {
          OPR2->instr = node->instr;
          if (genExprValue (OPR2))
            return 1;

          ID2 = arrayFuncConv (OPR2);

          if (!ISSCA (TY2))
          {
            sprintf (buffer, "invalid oprand type of '||'");
            errorPrint (EK_ERROR, buffer, &node->position);

            return 1;
          }

          ID2 = integralPromotion (ID2);

          if (OPR2->opcode == OP_AND || OPR2->opcode == OP_OR)
          {
              node->ident = ID2;
          }
          else
          {
            ident = tempIdentNew (node->instr, TTYPE (&intT), 0);
            node->ident = ident;

            if (node->instr)
            {
            block1 = blockNew ();
            block2 = blockNew ();

            label1 = blockToIdent (block1);
            label2 = blockToIdent (block2);

            instrNew (OP_IFNOT, ID2, label1, NULL);
            block1->ref = 1;

            instrNew (OP_ASS, ident, intOneNew (), NULL);
            instrNew (OP_GOTO, label2, NULL, NULL);
             block2->ref = 1;

            setCurrentBlock (block1);
            instrNew (OP_ASS, ident, intZeroNew (), NULL);

            setCurrentBlock (block2);
          }
          } 
        }
      }
      else if (OPR2->opcode == OP_CONST && KIN2 == TK_INT)
      {
        if (ID2->value.integer)
        {
          OPR1->instr = 0;
          if (genExprValue (OPR1))
            return 1;

          ID1 = arrayFuncConv (OPR1);

          if (!ISSCA (TY1))
          {
            sprintf (buffer, "invalid oprand type of '&&'");
            errorPrint (EK_ERROR, buffer, &node->position);

            return 1;
          }

            node->ident = intOneNew ();
        } 
        else
        {
          OPR1->instr = node->instr;
          if (genExprValue (OPR1))
            return 1;

          ID1 = arrayFuncConv (OPR1);

          if (!ISSCA (TY1))
          {
            sprintf (buffer, "invalid oprand type of '&&'");
            errorPrint (EK_ERROR, buffer, &node->position);

            return 1;
          }

          ID1 = integralPromotion (ID1);

          if (OPR1->opcode == OP_AND || OPR1->opcode == OP_OR)
          {
              node->ident = ID1;
          }
          else
          {
            ident = tempIdentNew (node->instr, TTYPE (&intT), 0);
            node->ident = ident;

            if (node->instr)
            {
            block1 = blockNew ();
            block2 = blockNew ();

            label1 = blockToIdent (block1);
            label2 = blockToIdent (block2);

            instrNew (OP_IFNOT, ID1, label1, NULL);
            block1->ref = 1;

            instrNew (OP_ASS, ident, intOneNew (), NULL);
            instrNew (OP_GOTO, label2, NULL, NULL);
             block2->ref = 1;

            setCurrentBlock (block1);
            instrNew (OP_ASS, ident, intZeroNew (), NULL);

            setCurrentBlock (block2);
          }
          } 
        }
      }
      else
      {
        block1 = blockNew ();
        block2 = blockNew ();

        label1 = blockToIdent (block1);
        label2 = blockToIdent (block2);

        if (genExprJump (node, NULL, block1))
          return 1;

    
          ident = tempIdentNew (node->instr, TTYPE (&intT), 0);
          node->ident = ident;

          if (node->instr)
          {
            instrNew (OP_ASS, ident, intOneNew (), NULL);
          instrNew (OP_GOTO, label2, NULL, NULL);
           block2->ref = 1;

          setCurrentBlock (block1);
          instrNew (OP_ASS, ident, intZeroNew (), NULL);

          setCurrentBlock (block2);
        }
      }

      break;
    case OP_ASS:
      OPR2->instr = 1;
      OPR1->instr = 1;
      if (genExprValue (OPR2))
        return 1;

      ID2 = arrayFuncConv (OPR2);

      if (OPR1->opcode == OP_ARR)
      {
        if (genArrayAddress (OPR1))
          return 1;

        if (checkAssType (GPTD (TY1), TY2, &node->position))
          return 1;

        instrNew (OP_DASS, ID1, ID2, NULL);

    
        ident = tempIdentNew (1, GPTD (TY1), 0);
          if (node->instr)
            instrNew (OP_DER, ident, ID1, NULL);

          node->ident = ident;
      }
      else if (OPR1->opcode == OP_DOT)
      {
        if (genDotAddress (OPR1))
          return 1;

        if (checkAssType (GPTD (TY1), TY2, &node->position))
          return 1;

        instrNew (OP_DASS, ID1, ID2, NULL);

          ident = tempIdentNew (1, GPTD (TY1), 0);

          if (node->instr)
            instrNew (OP_DER, ident, ID1, NULL);

          node->ident = ident;
      
      }
      else if (OPR1->opcode == OP_PTR)
      {
        if (genPtrAddress (OPR1))
          return 1;

        if (checkAssType (GPTD (TY1), TY2, &node->position))
          return 1;

        instrNew (OP_DASS, ID1, ID2, NULL);


          ident = tempIdentNew (1, GPTD (TY1), 0);
          if (node->instr)
            instrNew (OP_DER, ident, ID1, NULL);

          node->ident = ident;
      }
      else if (OPR1->opcode == OP_DER)
      {
        if (genExprValue (OPR1->oprand))
          return 1;

        ID (OPR1->oprand) = arrayFuncConv (OPR1->oprand);

        if (KIN (OPR1->oprand) != TK_PTR)
        {
          sprintf (buffer, "invalid oprand type of '*'");
          errorPrint (EK_ERROR, buffer, &OPR1->position);

          return 1;
        }

        if (checkAssType (GPTD (TY (OPR1->oprand)), TY2, &node->position))
          return 1;

        instrNew (OP_DASS, ID (OPR1->oprand), ID2, NULL);

    
          ident = tempIdentNew (1, GPTD (TY (OPR1->oprand)), 0);

          if (node->instr)
            instrNew (OP_DER, ident, ID (OPR1->oprand), NULL);

          node->ident = ident;
  
      }
      else
      {
        if (OPR1->opcode == OP_ID && (KIN1 == TK_FUNC || KIN1 == TK_ARRAY))
        {
          sprintf (buffer, "assignment to expression with wrong type");
          errorPrint (EK_ERROR, buffer, &node->position);

          return 1;
        }

        if (genExprValue (OPR1))
          return 1;

        ID1 = arrayFuncConv (OPR1);

        if (!ID1->isLvalue)
        {
          sprintf (buffer, "lvalue required as left operand of '='");
          errorPrint (EK_ERROR, buffer, &node->position);

          return 1;
        }

        if (checkAssType (TY1, TY2, &node->position))
          return 1;

          instrNew (OP_ASS, ID1, ID2, NULL);


          ident = malloc (sizeof (Ident));
          memcpy (ident, ID1, sizeof (Ident));
          ident->isLvalue = 0;

          node->ident = ident;
      }

      break;
    case OP_COM:
      OPR1->instr = 0;
      OPR2->instr = node->instr;
      if (genExprValue (OPR1) || genExprValue (OPR2))
        return 1;

        ident = malloc (sizeof (Ident));
        memcpy (ident, ID2, sizeof (Ident));
        ident->isLvalue = 0;

        node->ident = ident;
  }

  return 0;
}

/* Generate jump instructions for a relational expression. */
static int genRelJump (AstNode *node, Block *tru, Block *fal)
{
  Ident *tLabel, *fLabel;
  int value, opcode, opposite;

  if (tru)
    tLabel = blockToIdent (tru);
  else
    tLabel = NULL;

  if (fal)
    fLabel = blockToIdent (fal);
  else
    fLabel = NULL;

  OPR1->instr = OPR2->instr = node->instr;
  if (genExprValue (OPR1) || genExprValue (OPR2))
    return 1;

  ID1 = arrayFuncConv (OPR1);
  ID2 = arrayFuncConv (OPR2);

  switch (node->opcode)
  {
    case OP_LT:
      value = ID1->value.integer < ID2->value.integer;
      opcode = OP_IFLT;
      opposite = OP_IFGE;

      break;
    case OP_GT:
      value = ID1->value.integer > ID2->value.integer;
      opcode = OP_IFGT;
      opposite = OP_IFLE;

      break;
    case OP_LE:
      value = ID1->value.integer <= ID2->value.integer;
      opcode = OP_IFLE;
      opposite = OP_IFGT;

      break;
    case OP_GE:
      value = ID1->value.integer >= ID2->value.integer;
      opcode = OP_IFGE;
      opposite = OP_IFLT;

      break;
    case OP_EQ:
      value = ID1->value.integer == ID2->value.integer;
      opcode = OP_IFEQ;
      opposite = OP_IFNE;

      break;
    case OP_NE:
      value = ID1->value.integer < ID2->value.integer;
      opcode = OP_IFNE;
      opposite = OP_IFEQ;
  }
  
  if ((!ISINT (TY1) && KIN1 != TK_PTR) || (!ISINT (TY2) && KIN2 != TK_PTR))
  {
    sprintf (buffer, "invalid oprand type of '%s'", opToStr[node->opcode]);
    errorPrint (EK_ERROR, buffer, &node->position);

    return 1;
  }

  if (ID1->constType && ID2->constType)
  {
    if (value)
    {
      if (tLabel && node->instr)
      {
        instrNew (OP_GOTO, tLabel, NULL, NULL);
        tru->ref = 1;
      }
    }
    else if (fLabel && node->instr)
    {
      instrNew (OP_GOTO, fLabel, NULL, NULL);
      fal->ref = 1;
    }
  }
  else
  {
    ID1 = integralPromotion (ID1);
    ID2 = integralPromotion (ID2);

    if ((!ISINT (TY1) || !ISINT (TY2)) && !typeEqual (TY1, TY2))
    {
      sprintf (buffer, "comparsion between different types");
      errorPrint (EK_WARN, buffer, &node->position);

      return 1;
    }

    if (!tLabel && node->instr)
    {
      instrNew (opposite, ID1, ID2, fLabel);
      fal->ref = 1;
      setCurrentBlock (blockNew ());
    }
    else if (!fLabel && node->instr)
    {
      instrNew (opcode, ID1, ID2, tLabel);
      tru->ref = 1;
      setCurrentBlock (blockNew ());
    }
    else if (node->instr)
    {
      instrNew (opcode, ID1, ID2, tLabel);
      tru->ref = 1;
      setCurrentBlock (blockNew ());

      instrNew (OP_GOTO, fLabel, NULL, NULL);
      fal->ref = 1;
      setCurrentBlock (blockNew ());
    }
  }

  return 0;
}

/* Generate jump instructions for a expression. */
int genExprJump (AstNode *node, Block *tru, Block *fal)
{
  Ident *tLabel, *fLabel;
  Block *block1, *block2;
  int flag;

  if (tru)
    tLabel = blockToIdent (tru);
  else
    tLabel = NULL;

  if (fal)
    fLabel = blockToIdent (fal);
  else
    fLabel = NULL;

  switch (node->opcode)
  {
    case OP_ID:
    case OP_CONST:
    case OP_ARR:
    case OP_CAL:
    case OP_DOT:
    case OP_PTR:
    case OP_ADDR:
    case OP_DER:
    case OP_UADD:
    case OP_USUB:
    case OP_SIZE:
    case OP_CAST:
    case OP_MUL:
    case OP_DIV:
    case OP_MOD:
    case OP_ADD:
    case OP_SUB:
    case OP_ASS:
    case OP_COM:
      if (genExprValue (node))
        return 1;

      ID (node) = arrayFuncConv (node);

      if (!ISSCA (TY (node)))
      {
        sprintf (buffer, "scalar type is required");
        errorPrint (EK_ERROR, buffer, &node->position);

        return 1;
      }

      if (ID (node)->constType)
      {
        if (ID (node)->value.integer && tLabel)
        {
          if (node->instr)
          {instrNew (OP_GOTO, tLabel, NULL, NULL);
            tru->ref = 1;

          block1 = blockNew ();
          setCurrentBlock (block1);
         }
        }

        if (ID (node)->value.integer && fLabel)
        {
          if (node->instr)
          {
            instrNew (OP_GOTO, tLabel, NULL, NULL);
            tru->ref = 1;

          block1 = blockNew ();
          setCurrentBlock (block1);
        }
        }
      }
      else
      {
        //ID (node) = integralPromotion (ID (node));

        if (!tLabel)
        {
          tLabel = fLabel;
          fLabel = NULL;

          if (node->instr)
          {
          instrNew (OP_IFNOT, ID (node), tLabel, NULL);
           fal->ref = 1;
          }
        }
        else if (node->instr)
        {
          instrNew (OP_IF, ID (node), tLabel, NULL);
          tru->ref = 1;
        }

        if (node->instr)
        {
        block1 = blockNew ();
        setCurrentBlock (block1);
      }

        if (fLabel && node->instr)
        {
          instrNew (OP_GOTO, fLabel, NULL, NULL);
          fal->ref = 1;

          block2 = blockNew ();
          setCurrentBlock (block2);
        }
      }

      break;
    case OP_NOT:
      OPR1->instr = node->instr;
      if (genExprJump (OPR1, fal, tru))
        return 1;

      break;
    case OP_LT:
    case OP_GT:
    case OP_LE:
    case OP_GE:
    case OP_EQ:
    case OP_NE:
      if (genRelJump (node, tru, fal))
        return 1;

      break;
    case OP_AND:
      flag = 0;
      if (!fal)
      {
        flag = 1;
        fal = blockNew ();
      }

      OPR1->instr = node->instr;
      if (genExprJump (OPR1, NULL, fal))
        return 1;

      if (flag)
      {
        OPR2->instr = node->instr;
        if (genExprJump (OPR2, tru, NULL))
          return 1;

        if (node->instr)
        setCurrentBlock (fal);
      }
      else 
      {
        OPR2->instr = node->instr;
        if (genExprJump (OPR2, tru, fal))
        return 1;
      }

      break;
    case OP_OR:
      flag = 0;
      if (!tru)
      {
        flag = 1;
        tru = blockNew ();
      }

      OPR1->instr = node->instr;
      if (genExprJump (OPR1, tru, NULL))
        return 1;

      if (flag)
      {
        OPR2->instr = node->instr;
        if (genExprJump (OPR2, NULL, fal))
          return 1;

        if (node->instr)
        setCurrentBlock (tru);
      }
      else 
      {
        OPR2->instr = node->instr;

        if (genExprJump (OPR2, tru, fal))
        return 1;
      }
  }

  return 0;
}

/* Return the string form of a identifier. */
static char *identToStr (Ident *ident)
{
  if (ident->constType == CT_INT)
  {
    sprintf (buffer, "%d", ident->value.integer);
    ident->lexeme = strdup (buffer);
  }

  return ident->lexeme;
}

#undef  OPR1
#undef  OPR2
#undef  OPR3

/* Define for genPrint only. */
#define OPR1  (inrIte->oprand->ident)
#define OPR2  (inrIte->oprand->next->ident)
#define OPR3  (inrIte->oprand->next->next->ident)

#define LEX1  (identToStr (inrIte->oprand->ident))
#define LEX2  (identToStr (inrIte->oprand->next->ident))
#define LEX3  (identToStr (inrIte->oprand->next->next->ident))

/* Print intermediate code. */
void genPrint (Block *block)
{
  Block *blkIte;
  Instr *inrIte;
  Oprand *opdIte;
  int opcode, kind;
  char *name;

  for (blkIte = block; blkIte; blkIte = blkIte->next)
  {
    printf ("b%d:\n", blkIte->number);
    for (inrIte = blkIte->head; inrIte; inrIte = inrIte->next)
    {
      opcode = inrIte->opcode;
      switch (opcode)
      {
        case OP_CALASS:
          opdIte = inrIte->oprand;

          printf ("\t%s = %s (", identToStr (opdIte->ident), 
                  identToStr (opdIte->next->ident));
          for (opdIte = opdIte->next->next; opdIte;)
          {
            printf ("%s", identToStr (opdIte->ident));
            if ((opdIte = opdIte->next))
              printf (", ");
          }
          printf (")\n");

          break;
        case OP_CAL:
          opdIte = inrIte->oprand;

          printf ("\t%s (", identToStr (opdIte->ident));
          for (opdIte = opdIte->next; opdIte;)
          {
            printf ("%s", identToStr (opdIte->ident));
            if ((opdIte = opdIte->next))
              printf (", ");
          }
          printf (")\n");

          break;
        case OP_ADDR:
        case OP_DER:
        case OP_USUB:
        case OP_NOT:
          if (opcode == OP_ADDR && OPR2->constType)
            name = OPR2->location.label;
          else
            name = LEX2;

          printf ("\t%s = %s%s\n", LEX1, opToStr[opcode], name);

          break;
        case OP_CAST:
          kind = GKIN (inrIte->oprand->ident->type);
          printf ("\t%s = (%s) %s\n", LEX1, tyToStr[kind], LEX3);

          break;
        case OP_MUL:
        case OP_DIV:
        case OP_MOD:
        case OP_ADD:
        case OP_SUB:
          printf ("\t%s = %s %s %s\n", LEX1, LEX2, opToStr[opcode], LEX3);

          break;
        case OP_ASS:
          printf ("\t%s = %s\n", LEX1, LEX2);

          break;
        case OP_DASS:
          printf ("\t*%s = %s\n", LEX1, LEX2);

          break;
        case OP_IF:
          printf ("\tif (%s) ", LEX1);
          printf ("goto b%d\n", ((Block *) OPR2->lexeme)->number);

          break;
        case OP_IFNOT:
          printf ("\tif (!%s) ", LEX1);
          printf ("goto b%d\n", ((Block *) OPR2->lexeme)->number);

          break;
        case OP_IFLT:
        case OP_IFGT:
        case OP_IFLE:
        case OP_IFGE:
        case OP_IFEQ:
        case OP_IFNE:
          printf ("\tif (%s %s %s) ", LEX1, opToStr[opcode], LEX2); 
          printf ("goto b%d\n", ((Block *) OPR3->lexeme)->number);

          break;
        case OP_GOTO:
          printf ("\tgoto b%d\n", ((Block *) OPR1->lexeme)->number);

          break;
        case OP_RETURN:
          printf ("\treturn %s\n", LEX1);

          break;
        case OP_RET:
          printf ("\tret\n");
      }
    }
  }
}
