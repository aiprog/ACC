/* Code generation. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "opcode.h"
#include "gen.h"
#include "asm.h"

static const char *postfix[5] = { NULL, "b", "w", NULL, "l" };

static Ident *reg[ESI + 1];
static int lock[ESI + 1];
FuncList *globalFunc;

char pushBuffer[100];

/* Add a global function. */
void addGlobalFunc (Block *block)
{
  FuncList *list;

  list = malloc (sizeof (FuncList));
  list->ident = func;
  list->block = block;
  list->next = globalFunc;

  globalFunc = list;
}

void initReg ()
{
  memset (reg, 0, (ESI + 1) * sizeof (Ident *));
  memset (lock, 0, (ESI + 1) * sizeof (int));
}

void lockReg (int num)
{
  lock[num] = 1;
}

void unlockReg (int num)
{
  lock[num] = 0;
}

void emptyReg (int num)
{
  if (num >= EAX && num <= ESI)
    reg[num] = NULL;
}

int isEmptyReg (int num)
{
  return reg[num] == NULL;
}

void occupyReg (Ident *ident, int num)
{
  reg[num] = ident->address;
}

char *regName (int num, int size)
{
  switch (num)
  {
    case EAX:
      if (size == 1)
       return "%al";
      else if (size == 2)
        return "%ax";
      else
        return "%eax";

      break;
    case EBX:
      if (size == 1)
        return "bl";
      else if (size == 2)
        return "%bx";
      else
        return "%ebx";

      break;
    case ECX:
      if (size == 1)
        return "%cl";
      else if (size == 2)
        return "%cx";
      else
        return "%ecx";

      break;
    case EDX:
      if (size == 1)
        return "%dl";
      else if (size == 2)
        return "%dx";
      else
        return "%edx";
      break;

    case EDI:
      if (size == 2)
        return "%di";
      else
        return "%edi";

      break;
    case ESI:
      if (size == 2)
        return "%si";
      else
        return "%esi";
  }

  return NULL;
}

char *varName (Ident *ident)
{

  if (ident->isGlobal)
    sprintf (buffer, "%s", ident->lexeme);
  else if (ident->constType == CT_STR)
    sprintf (buffer, "%s", ident->location.label);
  else if (ident->constType == CT_INT)
    sprintf (buffer, "$%d", ident->value.integer);
  else
    sprintf (buffer, "%d(%%ebp)", ident->location.value);

  return strdup (buffer);
}

void genMove (int sign, int size1, int size2)
{
  if (size1 >= size2)
    sprintf (buffer, "mov%s", postfix[size2]);
  else if (sign)
    sprintf (buffer, "movs%s%s", postfix[size1], postfix[size2]);
  else
    sprintf (buffer, "movz%s%s", postfix[size1], postfix[size2]);
}

void emitMove (Ident *srcIdent, Ident *destIdent, int srcReg, int destReg)
{
  int sign, size1, size2;
  char *name1, *name2;

  if (srcIdent)
  {
    sign = ISSIG (srcIdent->type);
    size1 = GSZE (srcIdent->type);
  }
  if (destIdent)
  {
    sign = ISSIG (destIdent->type);
    size2 = GSZE (destIdent->type);
  }

  if (!srcIdent)
    size1 = size2;
  if (!destIdent)
    size2 = size1;

  if (srcReg == -1)
    name1 = varName (srcIdent);
  else
    name1 = regName (srcReg, size1);

  if (destReg == -1)
    name2 = varName (destIdent);
  else
    name2 = regName (destReg, size2);

  genMove (sign, size1, size2);

  printf ("\t%s %s, %s\n", buffer, name1, name2);
}

void readVar (Ident *ident, int num)
{
  emitMove (ident, NULL, -1, num);
}

void writeVar (Ident *ident, int num)
{
  emitMove (NULL, ident, num, -1);
}

void spillReg (int num)
{
  if (reg[num])
    writeVar (reg[num], num);
}

int getReg (int size)
{
  int i;

  for (i = 0; i <= EDX; ++i)
  {
    if (!reg[i])
      return i;
  }

  if (size != 1)
  {
    while (i <= ESI)
    {
      if (!reg[i])
        return i;

      ++i;
    }
  }

  for (i = 0; lock[i]; ++i)
    ;

  spillReg (i);

  return i;
}

int getReadReg (int size)
{
  return getReg (size);
}

int getWriteReg (int size)
{
  return getReg (size);
}

int isInReg (Ident *ident)
{
  int i;

  for (i = 0; i <= ESI; ++i)
  {
    if (reg[i] == ident->address)
      return i;
  }

  return -1;
}

void xchgIdent (Ident **ident1, Ident **ident2)
{
  Ident *temp;

  temp = *ident1;
  *ident1 = *ident2;
  *ident2 = temp;
}

#define OPR1  (inrIte->oprand)
#define OPR2  (inrIte->oprand->next)
#define OPR3  (inrIte->oprand->next->next)

#define ID1   (OPR1->ident)
#define ID2   (OPR2->ident)
#define ID3   (OPR3->ident)

#define TY1   (ID1->type)
#define TY2   (ID2->type)
#define TY3   (ID3->type)

void pushArgu (Oprand *oprand, int *size)
{
  int num;
  char *name;

  if (!oprand)
    return;

  pushArgu (oprand->next, size);

  num = 0;
  if ((num = isInReg (oprand->ident)) != -1)
  {
    emptyReg (num);
    name = regName (num, 4);
  }
  else
    name = varName (oprand->ident);

  *size += GSZE (oprand->ident->type);
 
   sprintf (pushBuffer + strlen (pushBuffer), "\tpushl %s\n", name);
}

void emitFunc (FuncList *list)
{
  Block *block, *blkIte;
  Instr *inrIte;
  int sign, opcode, num1, num2;
  int size1;
  char *name1, *name2;
  int size;
  Ident *func;

  block = list->block;
  func = list->ident;

  if (!func->isStatic)
    printf (".globl %s\n", func->lexeme);
  printf ("%s:\n", func->lexeme);

  printf ("\tenter $%d, $0\n", -TFUNC (func->type)->offset);
  printf ("\tpopl %%ebx\n\tpopl %%esi\n\tpopl %%edi\n");

  for (blkIte = block; blkIte; blkIte = blkIte->next)
  {
    initReg ();

    if (blkIte->ref)
      printf ("b%d:\n", blkIte->number);
    
    for (inrIte = blkIte->tail; inrIte; inrIte = inrIte->prev)
    {
      opcode = inrIte->opcode;
      if ((opcode == OP_ASS || opcode == OP_CAST) && ID2->temp)
        ID2->reqSize = GSZE (TY1);
      else if (opcode == OP_DASS && ID2->temp)
        ID2->reqSize = GSZE (GPTD (TY1));
    }

    for (inrIte = blkIte->head; inrIte; inrIte = inrIte->next)
    {
      opcode = inrIte->opcode;
      switch (opcode)
      {
        case OP_ADD:
        case OP_SUB:
          if (opcode == OP_ADD && ID2->constType)
            xchgIdent (&ID2, &ID3);

          if ((num1 = isInReg (ID2)) == -1)
          {
            num1 = getWriteReg (ID1->reqSize);
            readVar (ID2, num1);
          }
          occupyReg (ID1, num1);
          name1 = regName (num1, 4);

          if ((num2 = isInReg (ID3)) != -1)
          {
            emptyReg (num2);
            name2 = regName (num2, 4);
          }
          else
            name2 = varName (ID3);

          if (opcode == OP_ADD)
            printf ("\taddl %s, %s\n", name2, name1);
          else
            printf ("\tsubl %s, %s\n", name2, name1);

          break;
        case OP_MUL:
          num1 = isInReg (ID2);
          num2 = isInReg (ID3);

          if (num2 == EAX)
          {
            xchgIdent (&ID2, &ID3);
            num2 = num1;
            num1 = EAX;
          }

          if (num1 != EAX)
          {
            spillReg (EAX);
            occupyReg (ID1, EAX);

            emitMove (ID2, NULL, num1, EAX);
            emptyReg (num1);
          }

          if (num2 == EDX)
            name2 = "edx";
          else
          {
            spillReg (EDX);

            if (ID3->constType)
            {
              readVar (ID3, EDX);
              num2 = EDX;
            }

            if (num2 != -1)
            {
              name2 = regName (num2, 4);
              emptyReg (num2);
            }
            else
              name2 = varName (ID3);
          }
          emptyReg (EDX);

          if (ISSIG (TY1))
            printf ("\timull %s\n", name2);
          else
            printf ("\tmull %s\n", name2);

          break;
        case OP_DIV:
        case OP_MOD:
          num1 = isInReg (ID2);
          num2 = isInReg (ID3);

          if (num1 != EAX)
          {
            spillReg (EAX);
            if (num2 == EAX)
              num2 = -1;
            occupyReg (ID1, EAX);
            emitMove (ID2, NULL, num1, EAX);
            emptyReg (num1);
          }

          spillReg (EDX);
          if (num2 == EDX)
            num2 = -1;
          occupyReg (ID1, EDX);

          if (ID3->constType)
          {
            lockReg (EAX);
            lockReg (EDX);
            num2 = getReadReg (4);
            unlockReg (EAX);
            unlockReg (EDX);
            readVar (ID3, num2);
          }

          if (num2 != -1)
          {
            name2 = regName (num2, 4);
            emptyReg (num2);
          }
          else
            name2 = varName (ID3);
    
          if (opcode == OP_DIV)
          {
            occupyReg (ID1, EAX);
            emptyReg (EDX);
          }
          else
          {
            occupyReg (ID1, EDX);
            emptyReg (EAX);
          }

          if (ISSIG (TY1))
            printf ("\tcdq\n");
          else
            printf ("\tmovl $0, %%edx\n");

          if (ISSIG (TY1))
            printf ("\tidivl %s\n", name2);
          else
            printf ("\tdivl %s\n", name2);

          break;
        case OP_ASS:
        case OP_CAST:
          if (opcode == OP_CAST)
            xchgIdent (&ID2, &ID3);

          if (ID1->temp)
          {
            num1 = getWriteReg (ID1->reqSize);
            occupyReg (ID1, num1);
          }
          else
            num1 = -1;

          num2 = isInReg (ID2);

          if (num1 == -1 && num2 == -1 && !ID2->constType)
          {
            num2 = getReadReg (GSZE (TY1));
            readVar (ID2, num2);
          }
          emptyReg (num2);

          emitMove (ID2, ID1, num2, num1);

          break;
        case OP_DASS:
          if ((num1 = isInReg (ID1)) == -1)
          {
            num1 = getReadReg (4);
            readVar (ID1, num1);
            emptyReg (num1);
          }
          name1 = regName (num1, 4);

          if (!ID2->constType && (num2 = isInReg (ID2)) == -1)
          {
            lockReg (num1);
            num2 = getReadReg (GSZE (GPTD (TY1)));
            unlockReg (num1);
            readVar (ID2, num2);
          }

          if (num2 != -1)
          {
            name2 = regName (num2, GSZE (GPTD (TY1)));
            emptyReg (num2);
          }
          else
            name2 = varName (ID2);

          genMove (1, GSZE (GPTD (TY1)), GSZE (GPTD (TY1)));

          printf ("\t%s %s, (%s)\n", buffer, name2, name1);

          break;
        case OP_ADDR:
          num1 = getWriteReg (ID1->reqSize);
          occupyReg (ID1, num1);

          printf ("\tleal %s, %s\n", varName (ID2), regName (num1, 4));

          break;
        case OP_DER:
          size1 = GSZE (TY1);
          num1 = getWriteReg (ID1->reqSize);
          occupyReg (ID1, num1);
          name1 = regName (num1, size1);

          if (ID2->constType)
            name2 = varName (ID2);
          else
          {
            lockReg (num1);
            if ((num2 = isInReg (ID2)) == -1)
            {
              num2 = getReadReg (4);
              readVar (ID2, num2);  
            }
            unlockReg (num1);
            emptyReg (num2);
            name2 = regName (num2, 4);
          }

          genMove (ISSIG (TY1), size1, size1);

          printf ("\t%s (%s), %s\n", buffer, name2, name1);

          break;
        case OP_IF:
        case OP_IFNOT:
          if ((num1 = isInReg (ID1)) != -1)
          {
            emptyReg (num1);
            name1 = regName (num1, GSZE (TY1));
          }
          else
            name1 = varName (ID1);

          printf ("\tcmp%s $0, %s\n", postfix[GSZE (TY1)], name1);
          if (opcode == OP_IF)
            printf ("\tjne b%d\n", ((Block *) ID2->lexeme)->number);
          else
            printf ("\tje b%d\n", ((Block *) ID2->lexeme)->number);

          break;
        case OP_IFLT:
        case OP_IFGT:
        case OP_IFLE:
        case OP_IFGE:
        case OP_IFEQ:
        case OP_IFNE:
          if (GKIN (TY1) == TK_UINT || GKIN (TY1) == TK_PTR ||
              GKIN (TY2) == TK_UINT || GKIN (TY2) == TK_PTR)
            sign = 0;
          else
            sign = 1;

          if ((num2 = isInReg (ID2)) != -1)
          {
            name2 = regName (num2, 4);
            emptyReg (num2);
          }
          else
            name2 = varName (ID2);

          if ((num1 = isInReg (ID1)) != -1)
          {
            name1 = regName (num1, 4);
            emptyReg (num1);
          }
          else if ((!ID2->constType && num2 == -1) || ID1->constType)
          {
            num1 = getReadReg (4);
            emptyReg (num1);
            readVar (ID1, num1);
            name1 = regName (num1, 4);
          }
          else
            name1 = varName (ID1);

          if (opcode == OP_IFLT)
          {
            if (sign)
              sprintf (buffer, "%s", "jl");
            else
              sprintf (buffer, "%s", "jb");
          }
          else if (opcode == OP_IFGT)
          {
            if (sign)
              sprintf (buffer, "%s", "jg");
            else
              sprintf (buffer, "%s", "ja");
          }
          else if (opcode == OP_IFLE)
          {
            if (sign)
              sprintf (buffer, "%s", "jle");
            else
              sprintf (buffer, "%s", "jbe");
          }
          else if (opcode == OP_IFGE)
          {
            if (sign)
              sprintf (buffer, "%s", "jge");
            else
              sprintf (buffer, "%s", "jae");
          }
          else if (opcode == OP_IFEQ)
            sprintf (buffer, "%s", "je");
          else if (opcode == OP_IFNE)
            sprintf (buffer, "%s", "jne");

          printf ("\tcmpl %s, %s\n", name2, name1);
          printf ("\t%s b%d\n", buffer, ((Block *) ID3->lexeme)->number);

          break;
        case OP_GOTO:
          printf ("\tjmp b%d\n", ((Block *) ID1->lexeme)->number);

          break;
        case OP_CAL:
          pushBuffer[0] = 0;
          size = 0;
          pushArgu (OPR2, &size);

          if (!isEmptyReg (EAX))
            printf ("\tpushl %%eax\n");


          if (!isEmptyReg (ECX))
            printf ("\tpushl %%ecx\n");
  

          if (!isEmptyReg (EDX))
            printf ("\tpushl %%edx\n");

          printf ("%s", pushBuffer);
    

          if (GKIN (TY1) == TK_FUNC)
            printf ("\tcall %s\n", ID1->lexeme);
          else 
          {
            if ((num1 = isInReg (ID1)) != -1)
            {
              name1 = regName (num1, 4);
              emptyReg (num1);
            }
            else
              name1 = varName (ID1);

            printf ("\tcall *%s\n", name1);
          }

          printf ("\taddl $%d, %%esp\n", size);

          if (!isEmptyReg (EDX))
            printf ("\tpopl %%edx\n");

          if (!isEmptyReg (ECX))
            printf ("\tpopl %%ecx\n");

          if (!isEmptyReg (EAX))
            printf ("\tpopl %%eax\n");

          break;
        case OP_CALASS:
          pushBuffer[0] = 0;
          size = 0;
          pushArgu (OPR3, &size);

          if (!isEmptyReg (EAX))
            printf ("\tpushl %%eax\n");


          if (!isEmptyReg (ECX))
            printf ("\tpushl %%ecx\n");
  

          if (!isEmptyReg (EDX))
            printf ("\tpushl %%edx\n");

          printf ("%s", pushBuffer);

          if (GKIN (TY2) == TK_FUNC)
            printf ("\tcall %s\n", ID2->lexeme);
          else 
          {
            if ((num1 = isInReg (ID2)) != -1)
            {
              name1 = regName (num1, 4);
              emptyReg (num1);
            }
            else
              name1 = varName (ID2);

            printf ("\tcall *%s\n", name1);
          }

          printf ("\taddl $%d, %%esp\n", size);

          spillReg (EBX);
          occupyReg (ID1, EBX);
          emitMove (NULL, ID1, EAX, EBX);

          if (!isEmptyReg (EDX))
            printf ("\tpopl %%edx\n");

          if (!isEmptyReg (ECX))
            printf ("\tpopl %%ecx\n");

          if (!isEmptyReg (EAX))
            printf ("\tpopl %%eax\n");

          break;
        case OP_RETURN:
          emitMove (ID1, NULL, -1, EAX);

          break;
        case OP_RET:
          printf ("\tpopl %%edi\n\tpopl %%esi\n\tpopl %%ebx\n");
          printf ("\tleave\n\tret\n");
      }
    }
  }
  printf ("\n");
}

void emitFuncs ()
{
  FuncList *ite;

  for (ite = globalFunc; ite; ite = ite->next)
    emitFunc (ite);
}

void emitData ()
{
  printf ("# Code auto-generated by ACC\n\n");
  Ident *ident;
  printf (".data\n\n");
  for (ident = globalData; ident; ident = ident->next)
  {
    if (ident->constType)
      printf ("%s:\n\t.string %s\n", ident->location.label, 
              ident->value.string);
    else 
    {
      if (ident->isStatic)
        printf (".lcomm ");
      else
        printf (".comm ");
      printf ("%s, %d\n", ident->lexeme, GSZE (ident->type));
    }
  }
  printf ("\n.text\n\n");
}