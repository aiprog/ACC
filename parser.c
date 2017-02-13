/*  c.y
 *
 *  ANSI C Yacc grammar
 */

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "opcode.h"
#include "gen.h"
#include "const.h"
#include "asm.h"

#define GID(x)    ((x)->ident)
#define CNODE(x)  ((x)->opcode == OP_CONST && GID (x)->constType == CT_INT)

static int lcNum;
static const char *string[2] = { "declaration", "definition" };

extern int yylex ();

int dataSeg;


%}

%start translation_unit

%token <id> IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token <id> '[' ']' '(' ')' '.' '+' '-' '&' '*' '!' '/' '%' 
%token <id> '<' '>' '=' ',' ':' ';'
%token <id> PTR_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP
%token <id> EXTERN STATIC
%token <id> CHAR SHORT INT SIGNED UNSIGNED CONST VOLATILE VOID
%token <id> STRUCT ELLIPSIS
%token <id> IF ELSE WHILE DO FOR CONTINUE BREAK RETURN

%type <id> unary_operator

%type <decl> declaration_specifiers M1 storage_class_specifier type_specifier
%type <decl> type_qualifier struct_specifier specifier_qualifier_list 
%type <decl> type_qualifier declarator pointer direct_declarator 
%type <decl> type_qualifier_list abstract_declarator type_name
%type <decl> direct_abstract_declarator

%type <node> primary_expression expression postfix_expression unary_expression
%type <node> cast_expression multiplicative_expression additive_expression 
%type <node> relational_expression equality_expression logical_and_expression 
%type <node> logical_or_expression assignment_expression expression
%type <node> constant_expression expression_statement

%%

primary_expression
  : IDENTIFIER
    {
      $$ = astNodeNew (OP_ID, NULL, NULL, &$1.position);

      if (!(GID ($$) = symbolTableSearchIdent ($1.lexeme, 1)))
      {
        sprintf (buffer, "'%s' undeclared", $1.lexeme);
        errorPrint (EK_ERROR, buffer, &$1.position);

        return 1;
      }

      free ($1.lexeme);
    }
  | CONSTANT
    {
      $$ = astNodeNew (OP_CONST, NULL, NULL, &$1.position);
      GID ($$) = identNew (NULL, TTYPE (&intT), 0, CT_INT, &$1.position);
      GID ($$)->value.integer = $1.value;
    }
  | STRING_LITERAL
    {
      $$ = astNodeNew (OP_CONST, NULL, NULL, &$1.position);
      GID ($$) = identNew (NULL, TTYPE (&strT), 0, CT_STR, &$1.position);
      GID ($$)->location.label = lcNew ();
      GID ($$)->value.string = $1.lexeme;

      symbolTableAddGlobalData (GID ($$));
    }
  | '(' expression ')'
    {
      $$ = $2;
    }
  ;

postfix_expression
  : primary_expression
  | postfix_expression '[' expression ']'
    {
      $$ = astNodeNew (OP_ARR, $1, $3, &$2.position);
    }
  | postfix_expression '(' ')'
    {
      $$ = astNodeNew (OP_CAL, $1, NULL, &$2.position);
    }
  | postfix_expression '(' 
    {
      $$ = $1;
      $1 = astNodeNew (OP_CAL, $1, NULL, &$2.position);
    }
    argument_expression_list ')'
    {
      $$ = $1;
    }
  | postfix_expression '.' IDENTIFIER
    {
      $$ = astNodeNew (OP_ID, NULL, NULL, &$3.position);
      GID ($$) = identNew ($3.lexeme, NULL, 0, 0, NULL);

      $$ = astNodeNew (OP_DOT, $1, $$, &$2.position);
    }
  | postfix_expression PTR_OP IDENTIFIER
    {
      $$ = astNodeNew (OP_ID, NULL, NULL, &$3.position);
      GID ($$) = identNew ($3.lexeme, NULL, 0, 0, NULL);

      $$ = astNodeNew (OP_PTR, $1, $$, &$2.position);
    }
  ;

argument_expression_list
  : assignment_expression
    {
      $<node>0->next = $1;
      $<node>0 = $1;
    }
  | argument_expression_list ',' assignment_expression
    {
      $<node>0->next = $3;
      $<node>0 = $3;
    }
  ;

unary_expression
  : postfix_expression
  | unary_operator cast_expression
    {
      switch ($1.lexeme[0])
      {
        case '&':
          $$ = astNodeNew (OP_ADDR, $2, NULL, &$1.position);

          break;
        case '*':
          $$ = astNodeNew (OP_DER, $2, NULL, &$1.position);

          break;
        case '+':
          $$ = $2;

          break;
        case '-':
          if (CNODE ($2))
          {
            GID ($2)->value.integer = -GID ($2)->value.integer;
            $$ = $2;
          }
          else
            $$ = astNodeNew (OP_USUB, $2, NULL, &$1.position);

          break;
        case '!':
          if (CNODE ($2))
          {
            GID ($2)->value.integer = -GID ($2)->value.integer;
            $$ = $2;
          }
          else
            $$ = astNodeNew (OP_NOT, $2, NULL, &$1.position);
      }
    }
  | SIZEOF unary_expression
    {
      $$ = astNodeNew (OP_SIZE, $2, NULL, &$1.position);
    }
  | SIZEOF '(' type_name ')'
    {
      $$ = astNodeNew (OP_CONST, NULL, NULL, &$1.position);
      GID ($$) = identNew (NULL, TTYPE (&intT), 0, CT_INT, NULL);
      GID ($$)->value.integer = GSZE ($3.type);
    }
  ;

unary_operator
  : '&' { $$.lexeme = strdup ("&"); $$.position = $1.position; }
  | '*' { $$.lexeme = strdup ("*"); $$.position = $1.position; }
  | '+' { $$.lexeme = strdup ("+"); $$.position = $1.position; }
  | '-' { $$.lexeme = strdup ("-"); $$.position = $1.position; }
  | '!' { $$.lexeme = strdup ("!"); $$.position = $1.position; }
  ;

cast_expression
  : unary_expression
  | '(' type_name ')' cast_expression
    {
      if (CNODE ($4))
      {
        if (GKIN ($2.type) == TK_STRUCT || GKIN ($2.type) == TK_ARRAY)
        {
          sprintf (buffer, "illegal destination type");
          errorPrint (EK_ERROR, buffer, &$1.position);

          return 1;
        }

        GID ($4)->type = $2.type;
        $$ = $4;
      }
      else
      {
        $$ = astNodeNew (OP_ID, NULL, NULL, &$1.position);
        GID ($$) = identNew (NULL, $2.type, 0, 0, NULL);

        $$ = astNodeNew (OP_CAST, $$, $4, &$1.position);
      }
    }
  ;

multiplicative_expression
  : cast_expression
  | multiplicative_expression '*' cast_expression
    {
      if (CNODE ($1) && CNODE ($3))
      {
        GID ($1)->value.integer *= GID ($3)->value.integer;
        $$ = $1;
      }
      else
        $$ = astNodeNew (OP_MUL, $1, $3, &$2.position);
    }
  | multiplicative_expression '/' cast_expression
    {
      if (CNODE ($1) && CNODE ($3))
      {
        GID ($1)->value.integer /= GID ($3)->value.integer;
        $$ = $1;
      }
      else
        $$ = astNodeNew (OP_DIV, $1, $3, &$2.position);
    }
  | multiplicative_expression '%' cast_expression
    {
      if (CNODE ($1) && CNODE ($3))
      {
        GID ($1)->value.integer %= GID ($3)->value.integer;
        $$ = $1;
      }
      else
        $$ = astNodeNew (OP_MOD, $1, $3, &$2.position);
    }
  ;

additive_expression
  : multiplicative_expression
  | additive_expression '+' multiplicative_expression
    {
      if (CNODE ($1) && CNODE ($3))
      {
        if (ISINT (GID ($1)->type))
        {
          $$ = $1;
          $1 = $3;
          $3 = $$;
        }

        if (!ISINT (GID ($3)->type))
        {
          sprintf (buffer, "invalid oprand type of '+'");
          errorPrint (EK_ERROR, buffer, &$2.position);

          return 1;
        }

        if (GKIN (GID ($1)->type) == TK_PTR)
          GID ($1)->value.integer += 
          GSZE (GPTD (GID ($1)->type)) * GID ($3)->value.integer;
        else
          GID ($1)->value.integer += GID ($3)->value.integer;
        $$ = $1;
      }
      else
        $$ = astNodeNew (OP_ADD, $1, $3, &$2.position);
    }
  | additive_expression '-' multiplicative_expression
    {
      if (CNODE ($1) && CNODE ($3))
      {
        if (ISINT (GID ($1)->type))
        {
          $$ = $1;
          $1 = $3;
          $3 = $$;
        }

        if (GKIN (GID ($3)->type) == TK_PTR)
        {
          if (!typeEqual (GID ($1)->type, GID ($3)->type))
          {
            sprintf (buffer, "invalid oprand type of '-'");
            errorPrint (EK_ERROR, buffer, &$2.position);

            return 1;
          }
          else
          {
            GID ($1)->value.integer = 
            (GID ($1)->value.integer - GID ($3)->value.integer) / 
            GSZE (GPTD (GID ($1)->type));

            GID ($1)->type = TTYPE (&intT);
          }
        }
        else if (GKIN (GPTD (GID ($1)->type)) == TK_PTR)
          GID ($1)->value.integer -= 
          GSZE (GPTD (GID ($1)->type)) * GID ($3)->value.integer;
        else
          GID ($1)->value.integer -= GID ($3)->value.integer;
        $$ = $1;
      }
      else
        $$ = astNodeNew (OP_SUB, $1, $3, &$2.position);
    }
  ;

relational_expression
  : additive_expression
  | relational_expression '<' additive_expression
    {
      if (CNODE ($1) && CNODE ($3))
      {
        GID ($1)->value.integer = 
        GID ($1)->value.integer < GID ($3)->value.integer;
        $$ = $1;
      }
      else
        $$ = astNodeNew (OP_LT, $1, $3, &$2.position);
    }
  | relational_expression '>' additive_expression
    {
      if (CNODE ($1) && CNODE ($3))
      {
        GID ($1)->value.integer = 
        GID ($1)->value.integer > GID ($3)->value.integer;
        $$ = $1;
      }
      else
        $$ = astNodeNew (OP_GT, $1, $3, &$2.position);
    }
  | relational_expression LE_OP additive_expression
    {
      if (CNODE ($1) && CNODE ($3))
      {
        GID ($1)->value.integer = 
        GID ($1)->value.integer <= GID ($3)->value.integer;
        $$ = $1;
      }
      else
        $$ = astNodeNew (OP_LE, $1, $3, &$2.position);
    }
  | relational_expression GE_OP additive_expression
    {
      if (CNODE ($1) && CNODE ($3))
      {
        GID ($1)->value.integer = 
        GID ($1)->value.integer >= GID ($3)->value.integer;
        $$ = $1;
      }
      else
        $$ = astNodeNew (OP_GE, $1, $3, &$2.position);
    }
  ;

equality_expression
  : relational_expression
  | equality_expression EQ_OP relational_expression
    {
      if (CNODE ($1) && CNODE ($3))
      {
        GID ($1)->value.integer = 
        GID ($1)->value.integer == GID ($3)->value.integer;
        $$ = $1;
      }
      else
        $$ = astNodeNew (OP_EQ, $1, $3, &$2.position);
    }
  | equality_expression NE_OP relational_expression
    {
      if (CNODE ($1) && CNODE ($3))
      {
        GID ($1)->value.integer = 
        GID ($1)->value.integer != GID ($3)->value.integer;
        $$ = $1;
      }
      else
        $$ = astNodeNew (OP_NE, $1, $3, &$2.position);
    }
  ;

logical_and_expression
  : equality_expression
  | logical_and_expression AND_OP equality_expression
    {
      if (CNODE ($1) && CNODE ($3))
      {
        GID ($1)->value.integer = 
        GID ($1)->value.integer && GID ($3)->value.integer;
        $$ = $1;
      }
      else
        $$ = astNodeNew (OP_AND, $1, $3, &$2.position);
    }
  ;

logical_or_expression
  : logical_and_expression
  | logical_or_expression OR_OP logical_and_expression
    {
      if (CNODE ($1) && CNODE ($3))
      {
        GID ($1)->value.integer = 
        GID ($1)->value.integer || GID ($3)->value.integer;
        $$ = $1;
      }
      else
        $$ = astNodeNew (OP_OR, $1, $3, &$2.position);
    }
  ;

assignment_expression
  : logical_or_expression
  | unary_expression '=' assignment_expression
    {
      $$ = astNodeNew (OP_ASS, $1, $3, &$2.position);
    }
  ;

expression
  : assignment_expression
  | expression ',' assignment_expression
    {
      markTopExpr ($1);
      $$ = astNodeNew (OP_COM, $1, $3, &$2.position);
    }
  ;

constant_expression 
  : logical_or_expression
  ;

M1
  :
  {
    int onlySign, noType;

    onlySign = $<decl>0.sign && !$<decl>0.type;
    if ((noType = !$<decl>0.sign && !$<decl>0.type))
      errorPrint (EK_WARN, "type defaults to 'int'", &$<decl>0.position);

    if (onlySign || noType)
    {
      $$.type = TTYPE (basicTypeNew (TK_INT));

      if ($<decl>0.sign == 1)
        ++$$.type->kind;
    }
    else
      $$.type = $<decl>0.type;

    $$.store = $<decl>0.store;
    $$.type->qualifier = $<decl>0.qualifier;
  }
  ;

declaration
  : declaration_specifiers ';'
    {
      if (!$1.type || GKIN ($1.type) != TK_STRUCT)
        errorPrint (EK_WARN, "empty declaration", &$2.position);
    }
  | declaration_specifiers M1 declarator_list ';'
  ;

declaration_specifiers
  : storage_class_specifier 
    {
      $$.type = NULL;
      $$.sign = 0;
      $$.store = $1.store;
      $$.qualifier = 0;
      $$.position = $1.position;
    }
  | declaration_specifiers storage_class_specifier
    { 
      if ($1.store)
      {
        sprintf (buffer, "multiple storage classes in declaration");
        errorPrint (EK_ERROR, buffer, &$2.position);

        return 1;
      }
      else
        $$.store = $2.store;

      $$.type = $1.type;
      $$.sign = $1.sign;
      $$.qualifier = $1.qualifier;
      $$.position = $2.position;
    }
  | type_specifier
    {
      $$.type = $1.type;
      $$.sign = $1.sign;
      $$.store = 0;
      $$.qualifier = 0;
      $$.position = $1.position;
    }         
  | declaration_specifiers type_specifier
    {
      if ($1.sign && $2.sign)
      {
        sprintf (buffer, "too many 'signed' and 'unsigned' in declaration");
        errorPrint (EK_ERROR, buffer, &$2.position);

        return 1;
      }

      if ($1.type && $2.type)
      {
        sprintf (buffer, "two or more data types in declaration");
        errorPrint (EK_ERROR, buffer, &$2.position);
        return 1;
      }

      if (($1.sign && $2.type && !ISINT ($2.type)) || 
          ($2.sign && $1.type && !ISINT ($1.type)))
      {
        sprintf (buffer, "'signed' or 'unsigned' used with non-integer types");
        errorPrint (EK_ERROR, buffer, &$2.position);

        return 1;
      }

      if ($2.sign)
      {
        $$.type = $1.type;
        $$.sign = $2.sign;
        $$.position = $1.position;
      }
      else
      {
        $$.type = $2.type;
        $$.sign = $1.sign;
        $$.position = $2.position;
      }

      $$.store = $1.store;
      $$.qualifier = $1.qualifier;
    }
  | type_qualifier
    {
      $$.type = NULL;
      $$.sign = 0;
      $$.store = 0;
      $$.qualifier = $1.qualifier;
      $$.position = $1.position;
    }
  | declaration_specifiers type_qualifier
    {
      $$.type = $1.type;   
      $$.sign = $1.sign;
      $$.store = $1.store;
      SBIT ($$.qualifier, $2.qualifier);
      $$.position = $1.position; 
    } 
  ;

declarator_list
  : declarator
    {
      *$1.unknown = $<decl>0.type;

      patchIdent ($1.ident, $<decl>0.store, 0);
      if (checkIdentType ($1.ident))
        return 1;
      patchArraySize ($1.ident->type);
      if (identAction ($1.ident))
        return 1;
    }
  | declarator_list ',' declarator
    {
      *$3.unknown = $<decl>0.type;

      patchIdent ($3.ident, $<decl>0.store, 0);
      if (checkIdentType ($3.ident))
        return 1;
      patchArraySize ($3.ident->type);
      if (identAction ($3.ident))
        return 1;
    }
  ;

storage_class_specifier
  : EXTERN  
    {
      if (!symbolTableGlobal ())
      {
        sprintf (buffer, "store class specifier in local scope");
        errorPrint (EK_ERROR, buffer, &$1.position);

        return 1;
      }

      $$.store = ST_EXTERN;
      $$.position = $1.position;
    }
  | STATIC  
    {
      if (!symbolTableGlobal ())
      {
        sprintf (buffer, "store class specifier in local scope");
        errorPrint (EK_ERROR, buffer, &$1.position);

        return 1;
      }
      
      $$.store = ST_STATIC;
      $$.position = $1.position;
    }
  ;

type_specifier
  : VOID
    {
      $$.type = TTYPE (basicTypeNew (TK_VOID));
      $$.sign = 0;
      $$.position = $1.position;
    }
  | CHAR
    {
      $$.type = TTYPE (basicTypeNew (TK_CHAR));
      $$.sign = 0;
      $$.position = $1.position;
    }
  | SHORT
    {
      $$.type = TTYPE (basicTypeNew (TK_SHORT));
      $$.sign = 0;
      $$.position = $1.position;
    }
  | INT
    {
      $$.type = TTYPE (basicTypeNew (TK_INT));
      $$.sign = 0;
      $$.position = $1.position;
    }
  | SIGNED
    {
      $$.type = NULL;
      $$.sign = -1;
      $$.position = $1.position;
    }
  | UNSIGNED
    {
      $$.type = NULL;
      $$.sign = 1;
      $$.position = $1.position;
    }
  | struct_specifier
    {
      $$.type = $1.type;
      $$.sign = 0;
      $$.position = $1.position;
    }
  ;

struct_specifier
  : STRUCT IDENTIFIER
    {
      NewType *type;

      type = TNEW (symbolTableSearchType ($2.lexeme, 0));
      if (type && type->table)
      {
        sprintf (buffer, "redefinition of 'struct %s'", $2.lexeme);
        errorPrint (EK_ERROR, buffer, &$2.position);
        errorPrint (EK_NOTE, "originally defined here", &type->position);

        return 1;
      }
      else if (!type)
      {
        type = newTypeNew (0, $2.lexeme, NULL, &$2.position);
        symbolTableInsertType ($2.lexeme, TTYPE (type));
      }
      else if (!type->table)
        free ($2.lexeme);

      $<decl>$.type = TTYPE (type);
      $<decl>$.offset = offset;

      offset.value = 0;
      offset.direction = 1;

      symbolTablePush ();
    } 
    '{' struct_declaration_list '}'
    {
      if (symbolTableEmpty ())
        errorPrint (EK_WARN, "struct has no members", &$2.position);

      TNEW ($<decl>3.type)->size = offset.value;
      TNEW ($<decl>3.type)->table = top;

      $$.type = TTYPE (structTypeNew (TNEW ($<decl>3.type)));
      $$.position = $<decl>3.position;
      offset = $<decl>3.offset;

      symbolTablePop ();
    }
  | STRUCT 
    {
      $<decl>$.offset = offset;
      offset.value = 0;
      offset.direction = 1;

      symbolTablePush ();
    }
    '{' struct_declaration_list '}'
    {
      NewType *type;

      if (symbolTableEmpty ())
        errorPrint (EK_WARN, "struct has no members", &$1.position);

      type = newTypeNew (offset.value, NULL, top, &$1.position);
      $$.type = TTYPE (structTypeNew (type));
      $$.position = $1.position;
      offset = $<decl>2.offset;

      symbolTablePop ();
    }
  | STRUCT IDENTIFIER
    {
      NewType *type;

      type = TNEW (symbolTableSearchType ($2.lexeme, 1));
      if (!type)
      {
        type = newTypeNew (0, $2.lexeme, NULL, &$2.position);
        symbolTableInsertType ($2.lexeme, TTYPE (type));
      }
      else
        free ($2.lexeme);

      $$.type = TTYPE (structTypeNew (type));
      $$.position = $2.position;
    }
  ;

struct_declaration_list
  : struct_declaration
  | struct_declaration_list struct_declaration
  ;

struct_declaration
  : specifier_qualifier_list M1 declarator_list ';'
  ;

specifier_qualifier_list
  : specifier_qualifier_list type_specifier 
    {
      if ($1.sign && $2.sign)
      {
        sprintf (buffer, "too many 'signed' and 'unsigned' in declaration");
        errorPrint (EK_ERROR, buffer, &$2.position);

        return 1;
      }

      if ($1.type && $2.type)
      {
        sprintf (buffer, "two or more data types in declaration");
        errorPrint (EK_ERROR, buffer, &$2.position);
        return 1;
      }

      if (($1.sign && $2.type && !ISINT ($2.type)) || 
          ($2.sign && $1.type && !ISINT ($1.type)))
      {
        sprintf (buffer, "'signed' or 'unsigned' used with non-integer types");
        errorPrint (EK_ERROR, buffer, &$2.position);

        return 1;
      }

      if ($2.sign)
      {
        $$.type = $1.type;
        $$.sign = $2.sign;
        $$.position = $1.position;
      }
      else
      {
        $$.type = $2.type;
        $$.sign = $1.sign;
        $$.position = $2.position;
      }

      $$.store = 0;
      $$.qualifier = $1.qualifier;
    }
  | type_specifier
    {
      $$.type = $1.type;
      $$.sign = $1.sign;
      $$.store = 0;
      $$.qualifier = 0;
      $$.position = $1.position;
    }  
  | specifier_qualifier_list type_qualifier 
    {
      $$.type = $1.type;  
      $$.sign = $1.sign;
      $$.store = 0;
      $$.qualifier = $1.qualifier;
      SBIT ($$.qualifier, $2.qualifier);
      $$.position = $1.position;  
    } 
  | type_qualifier
    {
      $$.type = NULL;
      $$.sign = 0;
      $$.store = 0;
      $$.qualifier = $1.qualifier;
      $$.position = $1.position;
    }
  ;

type_qualifier
  : CONST
  {
    SBIT ($$.qualifier, QU_CONST);
    $$.position = $1.position;
  }
  | VOLATILE
  {
    SBIT ($$.qualifier, QU_VOLATILE);
    $$.position = $1.position;
  }
  ;

declarator
  : pointer direct_declarator
    {
      *$2.unknown = $1.type;
      $$.unknown = $1.unknown;
      $$.ident = $2.ident;
    }
  | direct_declarator
    {
      $$.unknown = $1.unknown;
      $$.ident = $1.ident;
    }
  ;

direct_declarator
  : IDENTIFIER
    {
      $$.ident = identNew ($1.lexeme, NULL, 1, 0, &$1.position);  
      $$.unknown = &$$.ident->type;
    }
  | '(' declarator ')'
    {
      $$.unknown = $2.unknown;
      $$.ident = $2.ident;
    }
  | direct_declarator '[' constant_expression ']'
    {
      if (genExprValue ($3))
        return 1;

      if ($3->ident->constType != CT_INT)
      {
        sprintf (buffer, "nonconstant length array '%s'", $1.ident->lexeme);
        errorPrint (EK_ERROR, buffer, &$1.ident->position);

        return 1;
      }
      else if ($3->ident->value.integer <= 0)
      {
        sprintf (buffer, "nonpositive length array '%s'", $1.ident->lexeme);
        errorPrint (EK_ERROR, buffer, &$1.ident->position);

        return 1;
      }

      *$1.unknown = TTYPE (arrayTypeNew ($3->ident->value.integer));
      $$.unknown = &GELE (*$1.unknown);
      $$.ident = $1.ident;
    }
  | direct_declarator '(' 
    {
      $$.type = TTYPE (funcTypeNew (NULL));
      *$1.unknown = $$.type;
      $$.parameter = &GPAR ($$.type);
    }
    parameter_type_list ')'
    {
      $$.unknown = &GRET ($3.type);
      $$.ident = $1.ident;  
    }
  | direct_declarator '(' ')'
    {
      *$1.unknown = TTYPE (funcTypeNew (NULL));
      $$.unknown = &GRET (*$1.unknown);
      $$.ident = $1.ident;
    }
  ;

pointer
  : '*' 
    {
      $$.type = TTYPE (ptrTypeNew (0));
      $$.position = $1.position;
      $$.unknown = &GPTD ($$.type);
    }
  | '*' type_qualifier_list
    {
      $$.type = TTYPE (ptrTypeNew ($2.qualifier));
      $$.position = $1.position;
      $$.unknown = &GPTD ($$.type);
    }
  | '*' pointer
    {
      *$2.unknown = TTYPE (ptrTypeNew (0));

      $$.type = $2.type;
      $$.position = $2.position;
      $$.unknown = &GPTD (*$2.unknown);
    } 
  | '*' type_qualifier_list pointer 
    {
      *$2.unknown = TTYPE (ptrTypeNew ($2.qualifier));

      $$.type = $3.type;
      $$.position = $3.position;
      $$.unknown = &GPTD (*$2.unknown);
    }
  ;

type_qualifier_list
  : type_qualifier
    {
      SBIT ($$.qualifier, $1.qualifier);
    }
  | type_qualifier_list type_qualifier
    {
      SBIT ($$.qualifier, $1.qualifier);
      SBIT ($$.qualifier, $2.qualifier);
    }
  ;

parameter_type_list
  : parameter_list
  | parameter_list ',' ELLIPSIS
    {
      *$<decl>0.parameter = identNew ("?", NULL, 0, 0, &$3.position);
    }
  ;

parameter_list
  : parameter_declaration
  | parameter_list ',' 
    {
      $<decl>$.parameter = $<decl>0.parameter;
    } 
    parameter_declaration
    {
      $<decl>0.parameter = $<decl>3.parameter;
    }
  ;

parameter_declaration
  : declaration_specifiers M1 declarator 
    {
      *$3.unknown = $2.type;

      if (patchParameterIdent ($3.ident, $2.store))
        return 1;
      patchArraySize ($3.ident->type);

      *($<decl>0.parameter) = $3.ident;
      $<decl>0.parameter = &(*($<decl>0.parameter))->next;
    }
  | declaration_specifiers M1 abstract_declarator
    {
      *$3.unknown = $2.type;

      if (patchParameterIdent ($3.ident, $2.store))
        return 1;
      patchArraySize ($3.ident->type);

      *($<decl>0.parameter) = $3.ident;
      $<decl>0.parameter = &(*($<decl>0.parameter))->next;
    }
  | declaration_specifiers M1
    {
      Ident *ident;

      ident = identNew (NULL, $2.type, 0, 0, &$1.position);
      if (patchParameterIdent (ident, $2.store))
        return 1;

      *($<decl>0.parameter) = ident;
      $<decl>0.parameter = &(*($<decl>0.parameter))->next;
    }
  ;

type_name
  : specifier_qualifier_list M1
    {
      $$.type = $2.type;
    }
  | specifier_qualifier_list M1 abstract_declarator
    {
      *$3.unknown = $2.type;
      $$.type = $3.ident->type;
    }
  ;

abstract_declarator
  : pointer
    {
      $$.unknown = &GPTD ($1.type);
      $$.ident = identNew (NULL, $1.type, 0, 0, &$1.position);
    }
  | direct_abstract_declarator
    {
      $$.unknown = $1.unknown;
      $$.ident = $1.ident;
    }
  | pointer direct_abstract_declarator
    {
      *$2.unknown = $1.type;
      $$.unknown = $1.unknown;
      $$.ident = $2.ident;
    }
  ;

direct_abstract_declarator
  : '(' abstract_declarator ')'
    {
      $$.unknown = $2.unknown;
      $$.ident = $2.ident;
    }
  | '[' constant_expression ']'
    {
      ArrayType *type;

      if (genExprValue ($2))
        return 1;

      if ($2->ident->constType != CT_INT)
      {
        sprintf (buffer, "nonconstant length array");
        errorPrint (EK_ERROR, buffer, &$1.position);

        return 1;
      }
      else if ($2->ident->value.integer <= 0)
      {
        sprintf (buffer, "nonpositive length array");
        errorPrint (EK_ERROR, buffer, &$1.position);

        return 1;
      }

      type = arrayTypeNew ($2->ident->value.integer);
      $$.unknown = &GELE (type);
      $$.ident = identNew (NULL, TTYPE (type), 0, 0, &$1.position);
    }
  | direct_abstract_declarator '[' constant_expression ']'
    {
      if (genExprValue ($3))
        return 1;

      if ($3->ident->constType != CT_INT)
      {
        sprintf (buffer, "nonconstant length array");
        errorPrint (EK_ERROR, buffer, &$2.position);

        return 1;
      }
      else if ($3->ident->value.integer <= 0)
      {
        sprintf (buffer, "nonpositive length array");
        errorPrint (EK_ERROR, buffer, &$2.position);

        return 1;
      }

      *$1.unknown = TTYPE (arrayTypeNew ($3->ident->value.integer));
      $$.unknown = &GELE (*$1.unknown);
      $$.ident = identNew (NULL, *$1.unknown, 0, 0, &$1.position);
    }
  | '(' ')'
    {
      FuncType *type;

      type = funcTypeNew (NULL);
      $$.unknown = &GRET (type);
      $$.ident = identNew (NULL, TTYPE (type), 0, 0, &$1.position);
    }
  | '(' 
    {
      $$.type = TTYPE (funcTypeNew (NULL));
      $$.parameter = &GPAR ($$.type);
    }
    parameter_type_list ')'
    {
      $$.unknown = &GRET ($2.type);
      $$.ident = identNew (NULL, $2.type, 0, 0, &$1.position);
    }
  | direct_abstract_declarator '(' ')'
    {
      *$1.unknown = TTYPE (funcTypeNew (NULL));
      $$.unknown = &GRET (*$1.unknown);
      $$.ident = $1.ident;
    }
  | direct_abstract_declarator '(' 
    {
      $$.type = TTYPE (funcTypeNew (NULL));
      *$1.unknown = $$.type;
      $$.parameter = &GPAR ($$.type);
    }
    parameter_type_list ')'
    {
      $$.unknown = &GRET ($$.type);
      $$.ident = $1.ident;
    }
  ;

statement
  : labeled_statement
  | { $<isFunc>$ = 0; } compound_statement
  | expression_statement
    {
      markTopExpr ($1);
      if (genExprValue ($1))
        return 1;
    }
  | selection_statement
  | iteration_statement
  | jump_statement
  ;

labeled_statement
  : IDENTIFIER ':' statement
  ;

M2
  :
    {
      if (!$<isFunc>0)
        symbolTablePush ();
    }
  ;

M3
  :
    {
      symbolTablePop ();
    }
  ;

compound_statement
  : M2 '{' '}' M3
  | M2 '{' statement_list '}' M3
  | M2 '{' declaration_list '}' M3
  | M2 '{' declaration_list statement_list '}' M3
  ;

declaration_list
  : declaration
  | declaration_list declaration
  ;

statement_list
  : statement
  | statement_list statement
  ;

expression_statement
  : ';' { }
  | expression ';'
    {
      $$ = $1;
    }
  ;

M4
  :
    {
      $<block>$ = blockNew ();
      if (genExprJump ($<node>-1, NULL, $<block>$))
        return 1;
    }
  ;

selection_statement
  : IF '(' expression ')' M4 statement
    {
      setCurrentBlock ($<block>5);
    }
  | IF '(' expression ')' M4 statement ELSE 
    {
      $<block>$ = blockNew ();
      instrNew (OP_GOTO, blockToIdent ($<block>$), NULL, NULL);
      $<block>$->ref = 1;
      setCurrentBlock ($<block>5);
    }
    statement
    {
      setCurrentBlock ($<block>8);
    }
  ;

iteration_statement
  : WHILE
    {
      $<block>$ = blockNew ();
      blockListPush (&continueList, $<block>$);
      setCurrentBlock ($<block>$);
    }
    '(' expression 
    {
      $<block>$ = blockNew ();
      blockListPush (&breakList, $<block>$);

      if (genExprJump ($4, NULL, $<block>$))
        return 1;
    }
    ')' statement
    {
      instrNew (OP_GOTO, blockToIdent ($<block>2), NULL, NULL);
      $<block>2->ref = 1;
      setCurrentBlock ($<block>5);

      blockListPop (&continueList);
      blockListPop (&breakList);
    }
  | DO 
    {
      $<block>$ = blockNew ();
      blockListPush (&breakList, $<block>$);
    }
    {
      $<block>$ = blockNew (); 
      blockListPush (&continueList, $<block>$);

      setCurrentBlock ($<block>$);
    } 
    statement WHILE '(' expression ')' ';'
    {
      if (genExprJump ($7, $<block>3, NULL))
        return 1;

      setCurrentBlock ($<block>2);

      blockListPop(&continueList);
      blockListPop (&breakList);
    }
  | FOR '(' expression_statement expression_statement ')' 
    {
      markTopExpr ($3);
      if (genExprValue ($3))
        return 1;

      $<block>$ = blockNew ();
      blockListPush (&continueList, $<block>$);
      setCurrentBlock ($<block>$);
    }
    {
      $<block>$ = blockNew ();
      blockListPush (&breakList, $<block>$);

      if (genExprJump ($4, NULL, $<block>$))
        return 1;
    }
    statement
    {
      instrNew (OP_GOTO, blockToIdent ($<block>6), NULL, NULL); 
      $<block>6->ref = 1;
      setCurrentBlock ($<block>7);

      blockListPop (&continueList);
      blockListPop (&breakList);
    }
  | FOR '(' expression_statement expression_statement expression ')' 
    {
      markTopExpr ($3);
      if (genExprValue ($3))
        return 1;

      $<block>$ = blockNew ();
      setCurrentBlock ($<block>$);
    }
    {
      $<block>$ = blockNew ();
      blockListPush (&breakList, $<block>$);

      if (genExprJump ($4, NULL, $<block>$))
        return 1;
    }
    {
      $<block>$ = blockNew (); //l2
      blockListPush (&continueList, $<block>$);
    }
    statement
    {
      setCurrentBlock ($<block>9);

      markTopExpr ($5);
      if (genExprValue ($5))
        return 1;

      instrNew (OP_GOTO, blockToIdent ($<block>7), NULL, NULL);
      $<block>7->ref = 1;
      setCurrentBlock ($<block>8);

      blockListPop (&continueList);
      blockListPop (&breakList);
    }
  ;

jump_statement
  : CONTINUE ';'
    {
      $<block>$= blockListPop (&continueList);
      if (!$<block>$)
      {
        sprintf (buffer, "continue statement not within a loop");
        errorPrint (EK_ERROR, buffer, &$1.position);

        return 1;
      }

      instrNew (OP_GOTO, blockToIdent ($<block>$), NULL, NULL);
      $<block>$->ref = 1;
    }
  | BREAK ';'
    {
      $<block>$ = blockListPop (&breakList);
      if (!$<block>$)
      {
        sprintf (buffer, "break statement not within a loop");
        errorPrint (EK_ERROR, buffer, &$1.position);

        return 1;
      }

      instrNew (OP_GOTO, blockToIdent ($<block>$), NULL, NULL);   
      $<block>$->ref = 1;
    }
  | RETURN ';'
    {
      if (GKIN (GRET (func->type)) != TK_VOID)
      {
        sprintf (buffer, "return no value, in function returning non-void");
        errorPrint (EK_WARN, buffer, &$1.position);
      }

      instrNew (OP_GOTO, blockToIdent (funcEnd), NULL, NULL);
      funcEnd->ref = 1;
    }
  | RETURN expression ';'
    {
      if (genExprValue ($2))
        return 1;

      $2->ident = arrayFuncConv ($2);

      if (checkRetType (GRET (func->type), $2->ident->type, &$1.position))
        return 1;

      instrNew (OP_RETURN, GID ($2), NULL, NULL);
      instrNew (OP_GOTO, blockToIdent (funcEnd), NULL, NULL);
      funcEnd->ref = 1;
    }
  ;

translation_unit
  : external_declaration
  | translation_unit external_declaration
  ;

external_declaration
  : function_definition
  | declaration
  ;

function_definition
  : declaration_specifiers M1 declarator
    {
      Ident *ite;

      *$3.unknown = $2.type;

      patchIdent ($3.ident, $<decl>0.store, 1);
      if (checkIdentType ($3.ident))
        return 1;
      patchArraySize ($3.ident->type);
      if (identAction ($3.ident))
        return 1;

      symbolTablePush ();

      offset.value = 8;

      ite = GPAR ($3.ident->type);
      while (ite)
      {
        ite->location.value = offset.value;
        offset.value += 4;
       
        symbolTableInsertIdent (ite->lexeme, ite);

        ite = ite->next;
      }

      offset.value = 0;
      offset.direction = -1;
      
      funcBeg = blockNew ();
      funcEnd = blockNew ();

      current = funcBeg;

      func = $3.ident;
    }
    {
      $<isFunc>$ = 1;
    }
    compound_statement
    {
      setCurrentBlock (funcEnd);
      instrNew (OP_RET, NULL, NULL, NULL);
      TFUNC ($3.ident->type)->offset = offset.value;

      //printf ("function %s\n", $3.ident->lexeme);
     // genPrint (funcBeg);
      //printf ("\n\n\n");

      /*if (!dataSeg)
      {
        emitData ();
        dataSeg = 1;
      }
      emitFunc (funcBeg);*/

      addGlobalFunc (funcBeg);
      func = NULL;
    }
  ; 

%%

void yyerror(char *message)
{
  errorPrint (EK_ERROR, message, &position);
}

void patchIdent (Ident *ident, int store, int isFuncDef)
{
  if((GKIN (ident->type) != TK_FUNC && !TBIT (store, ST_EXTERN)) || isFuncDef)
    ident->isDefined = 1;
  else
    ident->isDefined = 0;

  if (symbolTableGlobal ())
    ident->isGlobal = 1;
  else
    ident->isGlobal = 0;

  if (TBIT (store, ST_STATIC))
    ident->isStatic= 1;
  else
    ident->isStatic = 0;
}

int checkIdentType (Ident *ident)
{
  Ident *parameter, *ite;
  Position *position;
  Type *type;
  char *lexeme;
  int kind;

  lexeme = ident->lexeme;
  type = ident->type;
  kind = GKIN (type);
  position = &ident->position;
  switch (kind)
  {
    case TK_VOID:
      if (lexeme)
        sprintf (buffer, "'%s' declared void", lexeme);
      else
        sprintf (buffer, "type name declared void");
      errorPrint (EK_ERROR, buffer, position);

      return 1;
    case TK_ARRAY:
      type = GELE (type);
      while (GKIN (type) == TK_ARRAY)
        type = GELE (type);

      kind = GKIN (type);
      if (kind == TK_VOID)
      {
        if (lexeme)
          sprintf (buffer, "'%s' declared array of voids", lexeme);
        else
          sprintf (buffer, "type name declared array of voids");
        errorPrint (EK_ERROR, buffer, position);

        return 1;
      }
      else if (kind == TK_FUNC)
      {
        if (lexeme)
          sprintf (buffer, "'%s' declared array of functions", lexeme);
        else
          sprintf (buffer, "type name declared array of functions");
        errorPrint (EK_ERROR, buffer, position);

        return 1;
      }
      else if (kind == TK_STRUCT && !GNEW (type)->table)
      {
        if (lexeme)
          sprintf (buffer, "'%s' declared array of incomplete type", lexeme);
        else
          sprintf (buffer, "type name declared array of incomplete type");   
        errorPrint (EK_ERROR, buffer, position);

        return 1;
      }

      break;
    case TK_STRUCT:
      if (ident->isDefined && !(GNEW (type)->table))
      {
        if (lexeme)
          sprintf (buffer, "'%s' declared incomplete type", lexeme);
        else
          sprintf (buffer, "type name declared incomplete type");   
        errorPrint (EK_ERROR, buffer, position);

        return 1;
      }

      break;
    case TK_FUNC:
      type = GRET (type);
      kind = GKIN (type);
      if (kind == TK_ARRAY)
      {
        if (lexeme)
          sprintf (buffer, "'%s' declared function returning an array", lexeme);
        else
          sprintf (buffer, "type name declared function returning an array");
        errorPrint (EK_ERROR, buffer, position);

        return 1;
      }
      else if (kind == TK_FUNC)
      {
        if (lexeme)
          sprintf (buffer, "'%s' declared function returning a function",
                   lexeme);
        else
          sprintf (buffer, "type name declared function returning a function");
        errorPrint (EK_ERROR, buffer, position);

        return 1;
      }
      else if (kind == TK_STRUCT)
      {
        if (lexeme)
          sprintf (buffer, "'%s' declared function returning a struct", lexeme);
        else
          sprintf (buffer, "type name declared fucntion returning a struct");
        errorPrint (EK_ERROR, buffer, position);

        return 1;
      }

      parameter = GPAR (ident->type);
      while (parameter && (!parameter->lexeme || parameter->lexeme[0] != '?'))
      {
        if (ident->isDefined && !parameter->lexeme)
        {
          sprintf (buffer, "parameter name omitted");
          errorPrint (EK_ERROR, buffer, &parameter->position);

          return 1;
        }
        else if (GKIN (parameter->type) == TK_STRUCT)
        {
          sprintf (buffer, "parameter declared struct");
          errorPrint (EK_ERROR, buffer, &parameter->position);

          return 1;
        }
        else if (checkIdentType (parameter))
          return 1;

        ite = GPAR (ident->type);
        while (parameter->lexeme && ite != parameter && 
               (ite->lexeme || strcmp (ite->lexeme, parameter->lexeme)))
          ite = ite->next;

        if (parameter->lexeme && ite != parameter)
        {
          sprintf (buffer, "redefinition of parameter '%s'",
                   parameter->lexeme);
          errorPrint (EK_ERROR, buffer, &parameter->position);
          sprintf (buffer, "previous definition of '%s' was here'", 
                   ite->lexeme);
          errorPrint (EK_NOTE, buffer, &ite->position);

          return 1;
        }

        parameter = parameter->next;
      }
  }

  return 0;
}

void patchArraySize (Type *type)
{
  Ident *parameter;

  switch (GKIN (type))
  {
    case TK_ARRAY:
      patchArraySize (GELE (type));
      TARRAY (type)->size = GLEN (type) * GSZE (GELE (type));

      break;
    case TK_FUNC:
      patchArraySize (GRET (type));
      parameter = GPAR (type);
      while (parameter && parameter->lexeme[0] != '?')
      {
        patchArraySize (parameter->type);
        parameter = parameter->next;
      }

      break;
    case TK_PTR:
      patchArraySize (GPTD (type));
  }
}

int identAction (Ident *ident)
{
  Ident *prev;
  char *lexeme;

  lexeme = ident->lexeme;
  prev = symbolTableSearchIdent (lexeme, 0);
  if (!prev)
  {
    if (ident->isDefined)
    {
      if (ident->isGlobal)
      {
        ident->location.label = strdup (lexeme);
        if (GKIN (ident->type) != TK_FUNC)
            symbolTableAddGlobalData (ident);
      }
      else if (offset.direction == 1)
      {
        ident->location.value = offset.value;
        offset.value += GSZE (ident->type);
      }
      else
      {
        offset.value -= GSZE (ident->type);
        ident->location.value = offset.value;
      }
    }

    symbolTableInsertIdent (lexeme, ident);
  }
  else 
  {
    if (prev->isDefined && ident->isDefined)
    {
      sprintf (buffer, "redefinition of '%s'", lexeme);
      errorPrint (EK_ERROR, buffer, &ident->position);
      sprintf (buffer, "originally defined here");
      errorPrint (EK_NOTE, buffer, &prev->position);

      return 1;
    }
    else if (!typeEqual (prev->type, ident->type))              
    {
      sprintf (buffer, "conflicting types for '%s'", lexeme);
      errorPrint (EK_ERROR, buffer, &ident->position);
      sprintf (buffer, "previous %s of '%s' was here",
               string[prev->isDefined], lexeme);
      errorPrint (EK_NOTE, buffer, &prev->position);

      return 1;
    }
    else if (!prev->isStatic && ident->isStatic)
    {
      sprintf (buffer, "static %s of '%s' follows non-static %s", 
                string[ident->isDefined], lexeme, string[prev->isDefined]);
      errorPrint (EK_ERROR, buffer, &ident->position);
      sprintf (buffer, "previous %s of '%s' was here", 
               string[prev->isDefined], lexeme);
      errorPrint (EK_NOTE, buffer, &prev->position);

      return 1;
    }

    if (ident->isDefined)
    {
      if (prev->isGlobal)
      {
        prev->location.label = strdup (lexeme);
        if (prev->type->kind != TK_FUNC)
          symbolTableAddGlobalData (prev);
        else
          GPAR (prev->type) = GPAR (ident->type);
      }
      else if (offset.direction == 1)
      {
        ident->location.value = offset.value;
        offset.value += GSZE (ident->type);
      }
      else
      {
        offset.value -= GSZE (ident->type);
        ident->location.value = offset.value;
      }
    }
  }

  return 0;
}

int patchParameterIdent (Ident *ident, int store)
{
  PtrType *ptrType;
  Type *type;

  type = NULL;
  if (GKIN (ident->type) == TK_ARRAY)
    type = GELE (ident->type);
  else if (GKIN (ident->type) == TK_FUNC)
    type = ident->type;

  if (type)
  {
    ptrType = ptrTypeNew (0);
    GPTD (ptrType) = type;
    ident->type = TTYPE (ptrType);
  }

  ident->isStatic = 0;
  ident->isGlobal = 0;
  ident->isDefined = 1;

  if (store)
  {
    if (ident->lexeme)
      sprintf (buffer, "storage class specified for parameter '%s'",
               ident->lexeme);
    else
      sprintf (buffer, "storage class specified for unnamed parameter");
    errorPrint (EK_ERROR, buffer, &ident->position);

    return 1;
  }

  return 0;
}

char *lcNew ()
{
  char buffer[20];

  sprintf (buffer, "str%d", lcNum++);

  return strdup (buffer);
}

void markTopExpr (AstNode *node)
{
  node->instr = 0;
}