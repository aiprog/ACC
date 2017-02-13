/*
 *  Operators.
 */

#ifndef OPCODE_H
#define OPCODE_H

enum
{
  OP_ID,   OP_CONST,   OP_ARR,   OP_CAL,   OP_DOT,   OP_PTR,   OP_ADDR, 
  OP_DER,  OP_UADD,    OP_USUB,  OP_NOT,   OP_SIZE,  OP_CAST,  OP_MUL, 
  OP_DIV,  OP_MOD,     OP_ADD,   OP_SUB,   OP_LT,    OP_GT,    OP_LE,  
  OP_GE,   OP_EQ,      OP_NE,    OP_ASS,   OP_COM,   OP_AND,   OP_OR,
  OP_DASS, OP_IF,      OP_IFNOT, OP_IFLT,  OP_IFGT,  OP_IFLE,  OP_IFGE,
  OP_IFEQ, OP_IFNE,    OP_GOTO,  OP_RET,   OP_CALASS,OP_RETURN
};

extern const char *opToStr[41];

#endif