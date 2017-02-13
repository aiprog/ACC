/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20140715

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#define YYPREFIX "yy"

#define YYPURE 0

#line 7 "parser.c"

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


#line 44 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define IDENTIFIER 257
#define CONSTANT 258
#define STRING_LITERAL 259
#define SIZEOF 260
#define PTR_OP 261
#define LE_OP 262
#define GE_OP 263
#define EQ_OP 264
#define NE_OP 265
#define AND_OP 266
#define OR_OP 267
#define EXTERN 268
#define STATIC 269
#define CHAR 270
#define SHORT 271
#define INT 272
#define SIGNED 273
#define UNSIGNED 274
#define CONST 275
#define VOLATILE 276
#define VOID 277
#define STRUCT 278
#define ELLIPSIS 279
#define IF 280
#define ELSE 281
#define WHILE 282
#define DO 283
#define FOR 284
#define CONTINUE 285
#define BREAK 286
#define RETURN 287
#define YYERRCODE 256
typedef short YYINT;
static const YYINT yylhs[] = {                           -1,
   16,   16,   16,   16,   18,   18,   18,   31,   18,   18,
   18,   30,   30,   19,   19,   19,   19,    1,    1,    1,
    1,    1,   20,   20,   21,   21,   21,   21,   22,   22,
   22,   23,   23,   23,   23,   23,   24,   24,   24,   25,
   25,   26,   26,   27,   27,   17,   17,   28,    3,   32,
   32,    2,    2,    2,    2,    2,    2,   33,   33,    4,
    4,    5,    5,    5,    5,    5,    5,    5,   34,    7,
   36,    7,    7,   35,   35,   37,    8,    8,    8,    8,
    6,    6,    9,    9,   11,   11,   11,   39,   11,   11,
   10,   10,   10,   10,   12,   12,   38,   38,   40,   42,
   40,   41,   41,   41,   14,   14,   13,   13,   13,   15,
   15,   15,   15,   43,   15,   15,   44,   15,   45,   48,
   45,   45,   45,   45,   45,   46,   52,   53,   47,   47,
   47,   47,   55,   55,   54,   54,   29,   29,   56,   49,
   57,   49,   58,   59,   50,   60,   61,   50,   62,   63,
   50,   64,   65,   66,   50,   51,   51,   51,   51,    0,
    0,   67,   67,   69,   70,   68,
};
static const YYINT yylen[] = {                            2,
    1,    1,    1,    3,    1,    4,    3,    0,    5,    3,
    3,    1,    3,    1,    2,    2,    4,    1,    1,    1,
    1,    1,    1,    4,    1,    3,    3,    3,    1,    3,
    3,    1,    3,    3,    3,    3,    1,    3,    3,    1,
    3,    1,    3,    1,    3,    1,    3,    1,    0,    2,
    4,    1,    2,    1,    2,    1,    2,    1,    3,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    0,    6,
    0,    5,    2,    1,    2,    4,    2,    1,    2,    1,
    1,    1,    2,    1,    1,    3,    4,    0,    5,    3,
    1,    2,    2,    3,    1,    2,    1,    3,    1,    0,
    4,    3,    3,    2,    2,    3,    1,    1,    2,    3,
    3,    4,    2,    0,    4,    3,    0,    5,    1,    0,
    2,    1,    1,    1,    1,    3,    0,    0,    4,    5,
    5,    6,    1,    2,    1,    2,    1,    2,    0,    6,
    0,    9,    0,    0,    7,    0,    0,    9,    0,    0,
    8,    0,    0,    0,   10,    2,    2,    2,    3,    1,
    2,    1,    1,    0,    0,    6,
};
static const YYINT yydefred[] = {                         0,
   60,   61,   63,   64,   65,   66,   67,   81,   82,   62,
    0,    0,    0,   52,   54,   56,   68,  163,  160,  162,
    0,    0,  161,   50,    0,   53,   55,   57,    0,    0,
   85,    0,    0,    0,    0,    0,    0,    0,   78,   80,
    0,    0,   74,    0,   95,   93,    0,  165,    0,    0,
    0,    0,   51,    0,    0,   77,   79,   72,   75,   86,
   96,   94,  127,    1,    2,    3,    0,    0,   20,   21,
   18,   19,   22,    0,    5,    0,   23,   25,    0,    0,
    0,    0,    0,    0,    0,   90,    0,   59,   70,   58,
    0,  166,    0,    0,   16,    0,    0,    0,    0,    0,
   46,   15,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   87,
    0,    0,    0,   99,   76,    0,    0,    0,    0,    4,
    0,    0,    0,    7,    0,   10,   11,   26,   27,   28,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   89,    0,    0,  137,    0,  143,  146,    0,    0,
    0,    0,  128,    0,    0,  122,  133,  135,  119,  127,
  123,  124,  125,    0,    0,   17,    0,    0,    0,  106,
    0,   24,   47,   45,    6,   12,    0,    0,  102,    0,
  103,   98,    0,    0,    0,    0,  147,    0,  156,  157,
  158,    0,  129,    0,  138,  121,  128,  136,  128,  134,
    0,    0,  113,    0,    0,    0,    0,    0,    9,    0,
  101,  126,    0,    0,    0,    0,  159,  130,  131,  128,
  111,  110,    0,    0,  116,    0,   13,  139,    0,    0,
    0,  132,  115,  112,    0,    0,    0,    0,  149,    0,
  118,    0,    0,    0,  150,  152,  141,  145,    0,    0,
  153,    0,    0,  151,  154,  142,  148,    0,  155,
};
static const YYINT yydgoto[] = {                         12,
   74,  121,   25,   14,   15,   16,   17,   41,   44,   35,
   36,   47,  214,   97,  181,   75,  165,   76,   99,   78,
   79,   80,   81,   82,   83,  100,  101,   85,  166,  187,
  135,   18,   37,   29,   42,   22,   43,  122,   87,  123,
  124,  193,  215,  236,  168,  169,   92,  170,  171,  172,
  173,   93,  203,  174,  175,  246,  262,  196,  247,  197,
  225,  255,  260,  261,  265,  268,   19,   20,   48,   63,
};
static const YYINT yysindex[] = {                       626,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -256,  626,  360,    0,    0,    0,    0,    0,    0,    0,
    0,  -98,    0,    0,    2,    0,    0,    0,  -91,  716,
    0,    2,  -14,    0,  -36,  -32,   19,  716,    0,    0,
  716,  -73,    0,   20,    0,    0,  -14,    0,  -32,  299,
   62,    2,    0,  383,    2,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  331,  108,    0,    0,
    0,    0,    0,  299,    0,  -27,    0,    0,  534,  203,
  -40,  -90,  -71,   10,  172,    0,  626,    0,    0,    0,
   88,    0,  156,  108,    0,  716,  251,   95,  247,   10,
    0,    0,  299,  297,   83,   92,  299,  299,  299,  299,
  299,  299,  299,  299,  299,  299,  299,  299,  299,    0,
  626,  304,  303,    0,    0,  -33,  316,   16,  299,    0,
  299,  299,   -7,    0,  299,    0,    0,    0,    0,    0,
  534,  534,  203,  203,  203,  203,  -40,  -40,  -90,  -71,
   47,    0,   91,  317,    0,  361,    0,    0,  362,  345,
  357,  408,    0,  360,  132,    0,    0,    0,    0,    0,
    0,    0,    0,   37,   -2,    0,  299,  495,    9,    0,
   13,    0,    0,    0,    0,    0,   96,   53,    0,   89,
    0,    0,  626,   76,  299,  392,    0,  416,    0,    0,
    0,  150,    0,    2,    0,    0,    0,    0,    0,    0,
   68,  327,    0,  402,  626,   13,  299,  404,    0,  299,
    0,    0,  166,  299,   76,  416,    0,    0,    0,    0,
    0,    0,  414,  364,    0,  626,    0,    0,  418,  178,
  260,    0,    0,    0,  425,   76,  429,  432,    0,  387,
    0,  196,   76,  299,    0,    0,    0,    0,  403,   76,
    0,   76,  428,    0,    0,    0,    0,   76,    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  368,    0,   86,    0,    0,    0,    0,    0,    0,    0,
  137,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  115,    7,    0,   61,    0,    0,    0,    0,
   86,    0,    0,    0,    0,    0,  120,    0,  456,    0,
  650,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  171,    0,    0,  439,  462,
  359,   24,  -38,  400,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  558,    0,    0,  380,   90,
    0,    0,    0,  436,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    6,    0,  461,    0,    0,  391,    0,  475,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  445,  451,  468,  490,  504,  510,  482,  521,   40,   32,
  427,    0,  681,  126,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   86,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  391,  391,    0,    0,  693,  488,    0,
  498,    0,    0,    0,    0,    0,    0,  693,    0,  499,
    0,    0,    0,  391,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  391,    0,    0,    0,    0,  533,    0,  704,    0,    0,
    0,    0,    0,    0,  391,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  492,    0,
    0,    0,    0,    0,    0,  391,    0,    0,    0,    0,
    0,   29,  391,    0,    0,    0,    0,    0,    0,  391,
    0,  391,    0,    0,    0,    0,    0,  391,    0,
};
static const YYINT yygindex[] = {                         0,
    0,   17,   -6,    1,  839,  821,    0,   30,  374,  572,
   -1,    0,   39,  452,  139,    0,  583,    0,  731,  356,
  109,  283,  214,  435,  441,  -35, -108, -132, -196,    0,
    0, -115,  512,    0,  509,    0,  103, -182,    0,    0,
  375,    0,    0,    0,  568,    0,  412,    0,    0,    0,
    0,    0, -191,  398,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  573,    0,    0,    0,
};
#define YYTABLESIZE 1003
static const YYINT yytable[] = {                         73,
   21,  226,   42,   32,   71,   42,   68,   51,   72,   69,
  167,   70,  104,   26,   84,  228,   13,  229,  105,  112,
   42,  113,  183,  184,   30,  155,  186,   33,   13,  241,
   73,   38,  233,   49,   55,   71,  131,   68,  242,   72,
   69,   32,   70,   33,  212,   49,   49,   49,  178,   49,
   58,   58,  218,  245,   42,  178,  155,   33,   50,  210,
   60,  140,   52,  103,   40,   58,  140,   40,  140,   73,
  140,  140,   43,  140,   71,   43,   68,   53,   72,   69,
   41,   70,   40,   41,  234,  185,  188,  140,   33,  128,
   43,  163,  188,  213,   33,  155,   49,   96,   41,  177,
   73,   84,   86,  217,   84,   71,  177,   68,   73,   72,
   69,  237,   70,   71,  151,   68,   40,   72,   69,   84,
   70,   26,  209,   96,   43,   49,  155,   49,  188,  164,
   44,   52,   41,   44,  155,  130,  219,  177,  131,  220,
   73,   84,  164,  177,   59,   71,  125,   68,   44,   72,
   69,  140,   70,  140,   91,   91,   59,  204,   91,   92,
   92,  207,    1,   92,   26,    1,  180,    1,    1,    1,
    1,    1,    1,  116,  117,  131,   73,   73,   73,  177,
   73,   84,   44,   84,    1,    1,    1,    1,   49,  191,
  205,  164,  230,  131,  118,   73,    3,    4,    5,    6,
    7,    8,    9,   10,   11,   91,  238,   14,  227,  131,
   92,   14,   14,   14,   14,   14,    1,   14,  141,  142,
   31,  114,  115,  154,   65,   66,   67,   73,   42,   14,
   14,   14,   14,  106,    1,    2,    3,    4,    5,    6,
    7,    8,    9,   10,   11,  110,  156,  111,  157,  158,
  159,  160,  161,  162,  154,   65,   66,   67,   31,   69,
    8,    9,   49,   14,  120,    1,    2,    3,    4,    5,
    6,    7,    8,    9,   10,   11,  119,  156,  126,  157,
  158,  159,  160,  161,  162,  140,  140,  140,  140,   40,
   40,  129,   73,  154,   65,   66,   67,   71,   43,   68,
  249,   72,   69,   31,   70,   41,   41,  132,  140,   31,
  140,  140,  140,  140,  140,  140,  156,  216,  157,  158,
  159,  160,  161,  162,  154,   65,   66,   67,  216,  147,
  148,   73,  154,   65,   66,   67,   71,  134,   68,  136,
   72,   69,   49,   70,  152,   31,  153,  156,  137,  157,
  158,  159,  160,  161,  162,  156,  176,  157,  158,  159,
  160,  161,  162,   73,   64,   65,   66,   67,   71,  192,
   94,   91,   72,   69,  194,   70,   92,    3,    4,    5,
    6,    7,    8,    9,   10,   11,    1,    1,    1,    1,
    1,    1,    1,   73,  143,  144,  145,  146,   34,   37,
  195,  198,   37,  199,   73,   73,   73,   73,   73,   73,
   73,   73,   73,   73,   73,  200,   23,   37,   24,  231,
   23,   23,   23,   23,   23,   88,   23,  256,   90,  102,
  131,  224,   14,   14,   14,   14,   14,   14,   23,   23,
   73,   23,  232,  263,  235,   71,  131,   68,   73,   72,
   69,   37,   70,   71,  243,   68,  244,   72,   69,  248,
   70,  131,  138,  139,  140,  251,  201,  104,    8,  253,
  104,  254,   23,    8,  155,    8,  257,    8,    8,   29,
    8,   29,   29,   29,  182,   30,  267,   30,   30,   30,
   71,   31,   48,   31,   31,   31,   83,   29,   29,   83,
   29,   97,   32,   30,   30,   32,   30,   89,   33,   31,
   31,   33,   31,  120,   83,  105,   64,   65,   66,   67,
   32,   32,   38,   32,  189,   38,   33,   33,  107,   33,
   34,   29,  144,   34,  178,  213,   33,   30,  108,  107,
   38,  108,  107,   31,   35,  127,   54,   35,   34,   34,
   36,   34,  149,   36,   32,   64,   65,   66,   67,  150,
   33,   39,   35,   35,   39,   35,   91,  221,   36,   36,
  109,   36,  211,  109,   38,  107,  109,   90,   83,   39,
  108,  206,   34,    0,   23,  177,    0,   64,   65,   66,
   67,    0,    0,    0,    0,    0,   35,   49,   49,   49,
    0,    0,   36,    0,   46,    0,    0,    0,    0,    0,
    0,    0,    0,   39,    0,    0,    0,    0,   62,    0,
    0,    0,   37,   37,   37,   37,    0,    1,    2,    3,
    4,    5,    6,    7,    8,    9,   10,   11,    0,    0,
    0,   23,   23,   23,   23,   23,   23,    0,   49,    0,
   98,    0,    3,    4,    5,    6,    7,    8,    9,   10,
   11,    0,    0,    0,   64,   65,   66,   67,    0,    0,
    0,    0,   64,   65,   66,   67,   98,    0,    0,    0,
    0,    0,    0,    0,    0,  133,    0,    0,    0,    0,
    0,    0,    8,    8,    8,    8,    0,    0,    0,  179,
   29,   29,   29,   29,   29,   29,   30,   30,   30,   30,
   30,   30,   31,   31,   31,   31,   31,   31,    0,    0,
    0,    0,  190,   32,   32,   32,   32,   32,   32,   33,
   33,   33,   33,   33,   33,    0,    0,    0,    0,    0,
    0,  208,    0,    0,  202,   38,   38,   38,   38,  179,
    0,   34,   34,   34,   34,   34,   34,    0,    0,  190,
    0,  222,    0,    0,    0,   35,   35,   35,   35,   35,
   35,   36,   36,   36,   36,   36,   36,  223,  208,    0,
   77,    0,    0,    0,   39,   39,   39,   39,    0,    0,
    0,    0,  240,    0,    0,    0,    0,   95,    0,    0,
    0,    0,    0,    0,   77,    0,  239,    0,    0,    0,
    0,    0,    0,  252,    0,    0,    0,    0,    0,    0,
  258,    0,    0,  250,    0,    0,    0,  264,    0,  266,
    0,    0,    0,   28,    0,  269,  259,   77,   77,   77,
   77,   77,   77,   77,   77,   77,   77,   77,   77,   77,
   40,   27,    0,   45,    0,    0,    0,    0,   40,   77,
    0,   57,   40,    0,    0,    0,    0,   61,   39,    0,
    0,    0,    0,    0,   40,    0,   39,    0,    0,   56,
   39,    0,    0,    0,    0,    0,    0,    0,   40,    0,
    0,    0,   39,    1,    2,    3,    4,    5,    6,    7,
    8,    9,   10,   11,    0,    0,   39,   77,    0,    0,
    0,    0,    0,    0,   40,    0,   57,   88,   88,   88,
   88,   88,   88,   88,   88,   88,   88,   88,    0,    0,
    0,    0,   39,    0,   56,    0,    0,    0,    0,    0,
    0,   28,    0,    0,    0,    0,    0,   77,  100,  100,
  100,  100,  100,  100,  100,  100,  100,  100,  100,   27,
  114,  114,  114,  114,  114,  114,  114,  114,  114,  114,
  114,  117,  117,  117,  117,  117,  117,  117,  117,  117,
  117,  117,    0,    0,   28,    3,    4,    5,    6,    7,
    8,    9,   10,   11,    0,    0,    0,    0,    0,    0,
    0,    0,   27,
};
static const YYINT yycheck[] = {                         33,
  257,  198,   41,   40,   38,   44,   40,   40,   42,   43,
  126,   45,   40,   13,   50,  207,    0,  209,   46,   60,
   59,   62,  131,  132,  123,   59,  135,   42,   12,  226,
   33,  123,  215,   35,   41,   38,   44,   40,  230,   42,
   43,   40,   45,   42,  177,   40,   41,   42,   40,   44,
   44,  125,   40,  236,   93,   40,   59,   42,   91,  175,
   41,   33,   44,   91,   41,   59,   38,   44,   40,   33,
   42,   43,   41,   45,   38,   44,   40,   59,   42,   43,
   41,   45,   59,   44,  217,   93,   40,   59,   42,   96,
   59,  125,   40,   41,   42,   59,   91,   68,   59,   91,
   33,   41,   41,   91,   44,   38,   91,   40,   33,   42,
   43,  220,   45,   38,  121,   40,   93,   42,   43,   59,
   45,  121,  125,   94,   93,   40,   59,   42,   40,  123,
   41,   44,   93,   44,   59,   41,   41,   91,   44,   44,
   33,  177,  126,   91,   42,   38,   59,   40,   59,   42,
   43,  123,   45,  125,   40,   41,   54,  164,   44,   40,
   41,  125,   37,   44,  164,   40,  128,   42,   43,   44,
   45,   46,   47,  264,  265,   44,   40,   41,   42,   91,
   44,  217,   93,  123,   59,   60,   61,   62,  190,  151,
   59,  175,  125,   44,  266,   59,  270,  271,  272,  273,
  274,  275,  276,  277,  278,   91,   41,   37,   59,   44,
   91,   41,   42,   43,   44,   45,   91,   47,  110,  111,
  257,  262,  263,  257,  258,  259,  260,   91,  267,   59,
   60,   61,   62,  261,  268,  269,  270,  271,  272,  273,
  274,  275,  276,  277,  278,   43,  280,   45,  282,  283,
  284,  285,  286,  287,  257,  258,  259,  260,  257,  123,
  275,  276,  257,   93,   93,  268,  269,  270,  271,  272,
  273,  274,  275,  276,  277,  278,  267,  280,  123,  282,
  283,  284,  285,  286,  287,  257,  258,  259,  260,  266,
  267,   41,   33,  257,  258,  259,  260,   38,  267,   40,
   41,   42,   43,  257,   45,  266,  267,   61,  280,  257,
  282,  283,  284,  285,  286,  287,  280,  179,  282,  283,
  284,  285,  286,  287,  257,  258,  259,  260,  190,  116,
  117,   33,  257,  258,  259,  260,   38,   41,   40,  257,
   42,   43,  257,   45,   41,  257,   44,  280,  257,  282,
  283,  284,  285,  286,  287,  280,   41,  282,  283,  284,
  285,  286,  287,   33,  257,  258,  259,  260,   38,  279,
   40,  257,   42,   43,   58,   45,  257,  270,  271,  272,
  273,  274,  275,  276,  277,  278,  261,  262,  263,  264,
  265,  266,  267,  257,  112,  113,  114,  115,   25,   41,
   40,   40,   44,   59,  268,  269,  270,  271,  272,  273,
  274,  275,  276,  277,  278,   59,   37,   59,   59,   93,
   41,   42,   43,   44,   45,   52,   47,   41,   55,   74,
   44,   40,  262,  263,  264,  265,  266,  267,   59,   60,
   33,   62,   41,   41,   41,   38,   44,   40,   33,   42,
   43,   93,   45,   38,   41,   40,   93,   42,   43,  282,
   45,   44,  107,  108,  109,   41,   59,   41,   33,   41,
   44,   40,   93,   38,   59,   40,  281,   42,   43,   41,
   45,   43,   44,   45,  129,   41,   59,   43,   44,   45,
  123,   41,   93,   43,   44,   45,   41,   59,   60,   44,
   62,   41,   41,   59,   60,   44,   62,  125,   41,   59,
   60,   44,   62,  123,   59,   41,  257,  258,  259,  260,
   59,   60,   41,   62,  151,   44,   59,   60,   41,   62,
   41,   93,   41,   44,   40,   41,   42,   93,   41,   41,
   59,   44,   44,   93,   41,   94,   38,   44,   59,   60,
   41,   62,  118,   44,   93,  257,  258,  259,  260,  119,
   93,   41,   59,   60,   44,   62,   55,  193,   59,   60,
   37,   62,  175,   41,   93,   42,   44,  204,  123,   59,
   47,  170,   93,   -1,   12,   91,   -1,  257,  258,  259,
  260,   -1,   -1,   -1,   -1,   -1,   93,   40,   41,   42,
   -1,   -1,   93,   -1,   33,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   93,   -1,   -1,   -1,   -1,   47,   -1,
   -1,   -1,  264,  265,  266,  267,   -1,  268,  269,  270,
  271,  272,  273,  274,  275,  276,  277,  278,   -1,   -1,
   -1,  262,  263,  264,  265,  266,  267,   -1,   91,   -1,
   68,   -1,  270,  271,  272,  273,  274,  275,  276,  277,
  278,   -1,   -1,   -1,  257,  258,  259,  260,   -1,   -1,
   -1,   -1,  257,  258,  259,  260,   94,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  103,   -1,   -1,   -1,   -1,
   -1,   -1,  257,  258,  259,  260,   -1,   -1,   -1,  128,
  262,  263,  264,  265,  266,  267,  262,  263,  264,  265,
  266,  267,  262,  263,  264,  265,  266,  267,   -1,   -1,
   -1,   -1,  151,  262,  263,  264,  265,  266,  267,  262,
  263,  264,  265,  266,  267,   -1,   -1,   -1,   -1,   -1,
   -1,  174,   -1,   -1,  162,  264,  265,  266,  267,  178,
   -1,  262,  263,  264,  265,  266,  267,   -1,   -1,  188,
   -1,  194,   -1,   -1,   -1,  262,  263,  264,  265,  266,
  267,  262,  263,  264,  265,  266,  267,  195,  211,   -1,
   50,   -1,   -1,   -1,  264,  265,  266,  267,   -1,   -1,
   -1,   -1,  225,   -1,   -1,   -1,   -1,   67,   -1,   -1,
   -1,   -1,   -1,   -1,   74,   -1,  224,   -1,   -1,   -1,
   -1,   -1,   -1,  246,   -1,   -1,   -1,   -1,   -1,   -1,
  253,   -1,   -1,  241,   -1,   -1,   -1,  260,   -1,  262,
   -1,   -1,   -1,   13,   -1,  268,  254,  107,  108,  109,
  110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
   30,   13,   -1,   33,   -1,   -1,   -1,   -1,   38,  129,
   -1,   41,   42,   -1,   -1,   -1,   -1,   47,   30,   -1,
   -1,   -1,   -1,   -1,   54,   -1,   38,   -1,   -1,   41,
   42,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   68,   -1,
   -1,   -1,   54,  268,  269,  270,  271,  272,  273,  274,
  275,  276,  277,  278,   -1,   -1,   68,  177,   -1,   -1,
   -1,   -1,   -1,   -1,   94,   -1,   96,  268,  269,  270,
  271,  272,  273,  274,  275,  276,  277,  278,   -1,   -1,
   -1,   -1,   94,   -1,   96,   -1,   -1,   -1,   -1,   -1,
   -1,  121,   -1,   -1,   -1,   -1,   -1,  217,  268,  269,
  270,  271,  272,  273,  274,  275,  276,  277,  278,  121,
  268,  269,  270,  271,  272,  273,  274,  275,  276,  277,
  278,  268,  269,  270,  271,  272,  273,  274,  275,  276,
  277,  278,   -1,   -1,  164,  270,  271,  272,  273,  274,
  275,  276,  277,  278,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  164,
};
#define YYFINAL 12
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 287
#define YYUNDFTOKEN 360
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'!'",0,0,0,"'%'","'&'",0,"'('","')'","'*'","'+'","','","'-'","'.'","'/'",0,0,0,
0,0,0,0,0,0,0,"':'","';'","'<'","'='","'>'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,"'['",0,"']'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"IDENTIFIER","CONSTANT",
"STRING_LITERAL","SIZEOF","PTR_OP","LE_OP","GE_OP","EQ_OP","NE_OP","AND_OP",
"OR_OP","EXTERN","STATIC","CHAR","SHORT","INT","SIGNED","UNSIGNED","CONST",
"VOLATILE","VOID","STRUCT","ELLIPSIS","IF","ELSE","WHILE","DO","FOR","CONTINUE",
"BREAK","RETURN",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,"illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : translation_unit",
"primary_expression : IDENTIFIER",
"primary_expression : CONSTANT",
"primary_expression : STRING_LITERAL",
"primary_expression : '(' expression ')'",
"postfix_expression : primary_expression",
"postfix_expression : postfix_expression '[' expression ']'",
"postfix_expression : postfix_expression '(' ')'",
"$$1 :",
"postfix_expression : postfix_expression '(' $$1 argument_expression_list ')'",
"postfix_expression : postfix_expression '.' IDENTIFIER",
"postfix_expression : postfix_expression PTR_OP IDENTIFIER",
"argument_expression_list : assignment_expression",
"argument_expression_list : argument_expression_list ',' assignment_expression",
"unary_expression : postfix_expression",
"unary_expression : unary_operator cast_expression",
"unary_expression : SIZEOF unary_expression",
"unary_expression : SIZEOF '(' type_name ')'",
"unary_operator : '&'",
"unary_operator : '*'",
"unary_operator : '+'",
"unary_operator : '-'",
"unary_operator : '!'",
"cast_expression : unary_expression",
"cast_expression : '(' type_name ')' cast_expression",
"multiplicative_expression : cast_expression",
"multiplicative_expression : multiplicative_expression '*' cast_expression",
"multiplicative_expression : multiplicative_expression '/' cast_expression",
"multiplicative_expression : multiplicative_expression '%' cast_expression",
"additive_expression : multiplicative_expression",
"additive_expression : additive_expression '+' multiplicative_expression",
"additive_expression : additive_expression '-' multiplicative_expression",
"relational_expression : additive_expression",
"relational_expression : relational_expression '<' additive_expression",
"relational_expression : relational_expression '>' additive_expression",
"relational_expression : relational_expression LE_OP additive_expression",
"relational_expression : relational_expression GE_OP additive_expression",
"equality_expression : relational_expression",
"equality_expression : equality_expression EQ_OP relational_expression",
"equality_expression : equality_expression NE_OP relational_expression",
"logical_and_expression : equality_expression",
"logical_and_expression : logical_and_expression AND_OP equality_expression",
"logical_or_expression : logical_and_expression",
"logical_or_expression : logical_or_expression OR_OP logical_and_expression",
"assignment_expression : logical_or_expression",
"assignment_expression : unary_expression '=' assignment_expression",
"expression : assignment_expression",
"expression : expression ',' assignment_expression",
"constant_expression : logical_or_expression",
"M1 :",
"declaration : declaration_specifiers ';'",
"declaration : declaration_specifiers M1 declarator_list ';'",
"declaration_specifiers : storage_class_specifier",
"declaration_specifiers : declaration_specifiers storage_class_specifier",
"declaration_specifiers : type_specifier",
"declaration_specifiers : declaration_specifiers type_specifier",
"declaration_specifiers : type_qualifier",
"declaration_specifiers : declaration_specifiers type_qualifier",
"declarator_list : declarator",
"declarator_list : declarator_list ',' declarator",
"storage_class_specifier : EXTERN",
"storage_class_specifier : STATIC",
"type_specifier : VOID",
"type_specifier : CHAR",
"type_specifier : SHORT",
"type_specifier : INT",
"type_specifier : SIGNED",
"type_specifier : UNSIGNED",
"type_specifier : struct_specifier",
"$$2 :",
"struct_specifier : STRUCT IDENTIFIER $$2 '{' struct_declaration_list '}'",
"$$3 :",
"struct_specifier : STRUCT $$3 '{' struct_declaration_list '}'",
"struct_specifier : STRUCT IDENTIFIER",
"struct_declaration_list : struct_declaration",
"struct_declaration_list : struct_declaration_list struct_declaration",
"struct_declaration : specifier_qualifier_list M1 declarator_list ';'",
"specifier_qualifier_list : specifier_qualifier_list type_specifier",
"specifier_qualifier_list : type_specifier",
"specifier_qualifier_list : specifier_qualifier_list type_qualifier",
"specifier_qualifier_list : type_qualifier",
"type_qualifier : CONST",
"type_qualifier : VOLATILE",
"declarator : pointer direct_declarator",
"declarator : direct_declarator",
"direct_declarator : IDENTIFIER",
"direct_declarator : '(' declarator ')'",
"direct_declarator : direct_declarator '[' constant_expression ']'",
"$$4 :",
"direct_declarator : direct_declarator '(' $$4 parameter_type_list ')'",
"direct_declarator : direct_declarator '(' ')'",
"pointer : '*'",
"pointer : '*' type_qualifier_list",
"pointer : '*' pointer",
"pointer : '*' type_qualifier_list pointer",
"type_qualifier_list : type_qualifier",
"type_qualifier_list : type_qualifier_list type_qualifier",
"parameter_type_list : parameter_list",
"parameter_type_list : parameter_list ',' ELLIPSIS",
"parameter_list : parameter_declaration",
"$$5 :",
"parameter_list : parameter_list ',' $$5 parameter_declaration",
"parameter_declaration : declaration_specifiers M1 declarator",
"parameter_declaration : declaration_specifiers M1 abstract_declarator",
"parameter_declaration : declaration_specifiers M1",
"type_name : specifier_qualifier_list M1",
"type_name : specifier_qualifier_list M1 abstract_declarator",
"abstract_declarator : pointer",
"abstract_declarator : direct_abstract_declarator",
"abstract_declarator : pointer direct_abstract_declarator",
"direct_abstract_declarator : '(' abstract_declarator ')'",
"direct_abstract_declarator : '[' constant_expression ']'",
"direct_abstract_declarator : direct_abstract_declarator '[' constant_expression ']'",
"direct_abstract_declarator : '(' ')'",
"$$6 :",
"direct_abstract_declarator : '(' $$6 parameter_type_list ')'",
"direct_abstract_declarator : direct_abstract_declarator '(' ')'",
"$$7 :",
"direct_abstract_declarator : direct_abstract_declarator '(' $$7 parameter_type_list ')'",
"statement : labeled_statement",
"$$8 :",
"statement : $$8 compound_statement",
"statement : expression_statement",
"statement : selection_statement",
"statement : iteration_statement",
"statement : jump_statement",
"labeled_statement : IDENTIFIER ':' statement",
"M2 :",
"M3 :",
"compound_statement : M2 '{' '}' M3",
"compound_statement : M2 '{' statement_list '}' M3",
"compound_statement : M2 '{' declaration_list '}' M3",
"compound_statement : M2 '{' declaration_list statement_list '}' M3",
"declaration_list : declaration",
"declaration_list : declaration_list declaration",
"statement_list : statement",
"statement_list : statement_list statement",
"expression_statement : ';'",
"expression_statement : expression ';'",
"M4 :",
"selection_statement : IF '(' expression ')' M4 statement",
"$$9 :",
"selection_statement : IF '(' expression ')' M4 statement ELSE $$9 statement",
"$$10 :",
"$$11 :",
"iteration_statement : WHILE $$10 '(' expression $$11 ')' statement",
"$$12 :",
"$$13 :",
"iteration_statement : DO $$12 $$13 statement WHILE '(' expression ')' ';'",
"$$14 :",
"$$15 :",
"iteration_statement : FOR '(' expression_statement expression_statement ')' $$14 $$15 statement",
"$$16 :",
"$$17 :",
"$$18 :",
"iteration_statement : FOR '(' expression_statement expression_statement expression ')' $$16 $$17 $$18 statement",
"jump_statement : CONTINUE ';'",
"jump_statement : BREAK ';'",
"jump_statement : RETURN ';'",
"jump_statement : RETURN expression ';'",
"translation_unit : external_declaration",
"translation_unit : translation_unit external_declaration",
"external_declaration : function_definition",
"external_declaration : declaration",
"$$19 :",
"$$20 :",
"function_definition : declaration_specifiers M1 declarator $$19 $$20 compound_statement",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 1475 "parser.c"

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
#line 1033 "y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    YYERROR_CALL("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 60 "parser.c"
	{
      yyval.node = astNodeNew (OP_ID, NULL, NULL, &yystack.l_mark[0].id.position);

      if (!(GID (yyval.node) = symbolTableSearchIdent (yystack.l_mark[0].id.lexeme, 1)))
      {
        sprintf (buffer, "'%s' undeclared", yystack.l_mark[0].id.lexeme);
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[0].id.position);

        return 1;
      }

      free (yystack.l_mark[0].id.lexeme);
    }
break;
case 2:
#line 74 "parser.c"
	{
      yyval.node = astNodeNew (OP_CONST, NULL, NULL, &yystack.l_mark[0].id.position);
      GID (yyval.node) = identNew (NULL, TTYPE (&intT), 0, CT_INT, &yystack.l_mark[0].id.position);
      GID (yyval.node)->value.integer = yystack.l_mark[0].id.value;
    }
break;
case 3:
#line 80 "parser.c"
	{
      yyval.node = astNodeNew (OP_CONST, NULL, NULL, &yystack.l_mark[0].id.position);
      GID (yyval.node) = identNew (NULL, TTYPE (&strT), 0, CT_STR, &yystack.l_mark[0].id.position);
      GID (yyval.node)->location.label = lcNew ();
      GID (yyval.node)->value.string = yystack.l_mark[0].id.lexeme;

      symbolTableAddGlobalData (GID (yyval.node));
    }
break;
case 4:
#line 89 "parser.c"
	{
      yyval.node = yystack.l_mark[-1].node;
    }
break;
case 6:
#line 97 "parser.c"
	{
      yyval.node = astNodeNew (OP_ARR, yystack.l_mark[-3].node, yystack.l_mark[-1].node, &yystack.l_mark[-2].id.position);
    }
break;
case 7:
#line 101 "parser.c"
	{
      yyval.node = astNodeNew (OP_CAL, yystack.l_mark[-2].node, NULL, &yystack.l_mark[-1].id.position);
    }
break;
case 8:
#line 105 "parser.c"
	{
      yyval.node = yystack.l_mark[-1].node;
      yystack.l_mark[-1].node = astNodeNew (OP_CAL, yystack.l_mark[-1].node, NULL, &yystack.l_mark[0].id.position);
    }
break;
case 9:
#line 110 "parser.c"
	{
      yyval.node = yystack.l_mark[-4].node;
    }
break;
case 10:
#line 114 "parser.c"
	{
      yyval.node = astNodeNew (OP_ID, NULL, NULL, &yystack.l_mark[0].id.position);
      GID (yyval.node) = identNew (yystack.l_mark[0].id.lexeme, NULL, 0, 0, NULL);

      yyval.node = astNodeNew (OP_DOT, yystack.l_mark[-2].node, yyval.node, &yystack.l_mark[-1].id.position);
    }
break;
case 11:
#line 121 "parser.c"
	{
      yyval.node = astNodeNew (OP_ID, NULL, NULL, &yystack.l_mark[0].id.position);
      GID (yyval.node) = identNew (yystack.l_mark[0].id.lexeme, NULL, 0, 0, NULL);

      yyval.node = astNodeNew (OP_PTR, yystack.l_mark[-2].node, yyval.node, &yystack.l_mark[-1].id.position);
    }
break;
case 12:
#line 131 "parser.c"
	{
      yystack.l_mark[-1].node->next = yystack.l_mark[0].node;
      yystack.l_mark[-1].node = yystack.l_mark[0].node;
    }
break;
case 13:
#line 136 "parser.c"
	{
      yystack.l_mark[-3].node->next = yystack.l_mark[0].node;
      yystack.l_mark[-3].node = yystack.l_mark[0].node;
    }
break;
case 15:
#line 145 "parser.c"
	{
      switch (yystack.l_mark[-1].id.lexeme[0])
      {
        case '&':
          yyval.node = astNodeNew (OP_ADDR, yystack.l_mark[0].node, NULL, &yystack.l_mark[-1].id.position);

          break;
        case '*':
          yyval.node = astNodeNew (OP_DER, yystack.l_mark[0].node, NULL, &yystack.l_mark[-1].id.position);

          break;
        case '+':
          yyval.node = yystack.l_mark[0].node;

          break;
        case '-':
          if (CNODE (yystack.l_mark[0].node))
          {
            GID (yystack.l_mark[0].node)->value.integer = -GID (yystack.l_mark[0].node)->value.integer;
            yyval.node = yystack.l_mark[0].node;
          }
          else
            yyval.node = astNodeNew (OP_USUB, yystack.l_mark[0].node, NULL, &yystack.l_mark[-1].id.position);

          break;
        case '!':
          if (CNODE (yystack.l_mark[0].node))
          {
            GID (yystack.l_mark[0].node)->value.integer = -GID (yystack.l_mark[0].node)->value.integer;
            yyval.node = yystack.l_mark[0].node;
          }
          else
            yyval.node = astNodeNew (OP_NOT, yystack.l_mark[0].node, NULL, &yystack.l_mark[-1].id.position);
      }
    }
break;
case 16:
#line 181 "parser.c"
	{
      yyval.node = astNodeNew (OP_SIZE, yystack.l_mark[0].node, NULL, &yystack.l_mark[-1].id.position);
    }
break;
case 17:
#line 185 "parser.c"
	{
      yyval.node = astNodeNew (OP_CONST, NULL, NULL, &yystack.l_mark[-3].id.position);
      GID (yyval.node) = identNew (NULL, TTYPE (&intT), 0, CT_INT, NULL);
      GID (yyval.node)->value.integer = GSZE (yystack.l_mark[-1].decl.type);
    }
break;
case 18:
#line 193 "parser.c"
	{ yyval.id.lexeme = strdup ("&"); yyval.id.position = yystack.l_mark[0].id.position; }
break;
case 19:
#line 194 "parser.c"
	{ yyval.id.lexeme = strdup ("*"); yyval.id.position = yystack.l_mark[0].id.position; }
break;
case 20:
#line 195 "parser.c"
	{ yyval.id.lexeme = strdup ("+"); yyval.id.position = yystack.l_mark[0].id.position; }
break;
case 21:
#line 196 "parser.c"
	{ yyval.id.lexeme = strdup ("-"); yyval.id.position = yystack.l_mark[0].id.position; }
break;
case 22:
#line 197 "parser.c"
	{ yyval.id.lexeme = strdup ("!"); yyval.id.position = yystack.l_mark[0].id.position; }
break;
case 24:
#line 203 "parser.c"
	{
      if (CNODE (yystack.l_mark[0].node))
      {
        if (GKIN (yystack.l_mark[-2].decl.type) == TK_STRUCT || GKIN (yystack.l_mark[-2].decl.type) == TK_ARRAY)
        {
          sprintf (buffer, "illegal destination type");
          errorPrint (EK_ERROR, buffer, &yystack.l_mark[-3].id.position);

          return 1;
        }

        GID (yystack.l_mark[0].node)->type = yystack.l_mark[-2].decl.type;
        yyval.node = yystack.l_mark[0].node;
      }
      else
      {
        yyval.node = astNodeNew (OP_ID, NULL, NULL, &yystack.l_mark[-3].id.position);
        GID (yyval.node) = identNew (NULL, yystack.l_mark[-2].decl.type, 0, 0, NULL);

        yyval.node = astNodeNew (OP_CAST, yyval.node, yystack.l_mark[0].node, &yystack.l_mark[-3].id.position);
      }
    }
break;
case 26:
#line 230 "parser.c"
	{
      if (CNODE (yystack.l_mark[-2].node) && CNODE (yystack.l_mark[0].node))
      {
        GID (yystack.l_mark[-2].node)->value.integer *= GID (yystack.l_mark[0].node)->value.integer;
        yyval.node = yystack.l_mark[-2].node;
      }
      else
        yyval.node = astNodeNew (OP_MUL, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 27:
#line 240 "parser.c"
	{
      if (CNODE (yystack.l_mark[-2].node) && CNODE (yystack.l_mark[0].node))
      {
        GID (yystack.l_mark[-2].node)->value.integer /= GID (yystack.l_mark[0].node)->value.integer;
        yyval.node = yystack.l_mark[-2].node;
      }
      else
        yyval.node = astNodeNew (OP_DIV, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 28:
#line 250 "parser.c"
	{
      if (CNODE (yystack.l_mark[-2].node) && CNODE (yystack.l_mark[0].node))
      {
        GID (yystack.l_mark[-2].node)->value.integer %= GID (yystack.l_mark[0].node)->value.integer;
        yyval.node = yystack.l_mark[-2].node;
      }
      else
        yyval.node = astNodeNew (OP_MOD, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 30:
#line 264 "parser.c"
	{
      if (CNODE (yystack.l_mark[-2].node) && CNODE (yystack.l_mark[0].node))
      {
        if (ISINT (GID (yystack.l_mark[-2].node)->type))
        {
          yyval.node = yystack.l_mark[-2].node;
          yystack.l_mark[-2].node = yystack.l_mark[0].node;
          yystack.l_mark[0].node = yyval.node;
        }

        if (!ISINT (GID (yystack.l_mark[0].node)->type))
        {
          sprintf (buffer, "invalid oprand type of '+'");
          errorPrint (EK_ERROR, buffer, &yystack.l_mark[-1].id.position);

          return 1;
        }

        if (GKIN (GID (yystack.l_mark[-2].node)->type) == TK_PTR)
          GID (yystack.l_mark[-2].node)->value.integer += 
          GSZE (GPTD (GID (yystack.l_mark[-2].node)->type)) * GID (yystack.l_mark[0].node)->value.integer;
        else
          GID (yystack.l_mark[-2].node)->value.integer += GID (yystack.l_mark[0].node)->value.integer;
        yyval.node = yystack.l_mark[-2].node;
      }
      else
        yyval.node = astNodeNew (OP_ADD, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 31:
#line 293 "parser.c"
	{
      if (CNODE (yystack.l_mark[-2].node) && CNODE (yystack.l_mark[0].node))
      {
        if (ISINT (GID (yystack.l_mark[-2].node)->type))
        {
          yyval.node = yystack.l_mark[-2].node;
          yystack.l_mark[-2].node = yystack.l_mark[0].node;
          yystack.l_mark[0].node = yyval.node;
        }

        if (GKIN (GID (yystack.l_mark[0].node)->type) == TK_PTR)
        {
          if (!typeEqual (GID (yystack.l_mark[-2].node)->type, GID (yystack.l_mark[0].node)->type))
          {
            sprintf (buffer, "invalid oprand type of '-'");
            errorPrint (EK_ERROR, buffer, &yystack.l_mark[-1].id.position);

            return 1;
          }
          else
          {
            GID (yystack.l_mark[-2].node)->value.integer = 
            (GID (yystack.l_mark[-2].node)->value.integer - GID (yystack.l_mark[0].node)->value.integer) / 
            GSZE (GPTD (GID (yystack.l_mark[-2].node)->type));

            GID (yystack.l_mark[-2].node)->type = TTYPE (&intT);
          }
        }
        else if (GKIN (GPTD (GID (yystack.l_mark[-2].node)->type)) == TK_PTR)
          GID (yystack.l_mark[-2].node)->value.integer -= 
          GSZE (GPTD (GID (yystack.l_mark[-2].node)->type)) * GID (yystack.l_mark[0].node)->value.integer;
        else
          GID (yystack.l_mark[-2].node)->value.integer -= GID (yystack.l_mark[0].node)->value.integer;
        yyval.node = yystack.l_mark[-2].node;
      }
      else
        yyval.node = astNodeNew (OP_SUB, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 33:
#line 336 "parser.c"
	{
      if (CNODE (yystack.l_mark[-2].node) && CNODE (yystack.l_mark[0].node))
      {
        GID (yystack.l_mark[-2].node)->value.integer = 
        GID (yystack.l_mark[-2].node)->value.integer < GID (yystack.l_mark[0].node)->value.integer;
        yyval.node = yystack.l_mark[-2].node;
      }
      else
        yyval.node = astNodeNew (OP_LT, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 34:
#line 347 "parser.c"
	{
      if (CNODE (yystack.l_mark[-2].node) && CNODE (yystack.l_mark[0].node))
      {
        GID (yystack.l_mark[-2].node)->value.integer = 
        GID (yystack.l_mark[-2].node)->value.integer > GID (yystack.l_mark[0].node)->value.integer;
        yyval.node = yystack.l_mark[-2].node;
      }
      else
        yyval.node = astNodeNew (OP_GT, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 35:
#line 358 "parser.c"
	{
      if (CNODE (yystack.l_mark[-2].node) && CNODE (yystack.l_mark[0].node))
      {
        GID (yystack.l_mark[-2].node)->value.integer = 
        GID (yystack.l_mark[-2].node)->value.integer <= GID (yystack.l_mark[0].node)->value.integer;
        yyval.node = yystack.l_mark[-2].node;
      }
      else
        yyval.node = astNodeNew (OP_LE, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 36:
#line 369 "parser.c"
	{
      if (CNODE (yystack.l_mark[-2].node) && CNODE (yystack.l_mark[0].node))
      {
        GID (yystack.l_mark[-2].node)->value.integer = 
        GID (yystack.l_mark[-2].node)->value.integer >= GID (yystack.l_mark[0].node)->value.integer;
        yyval.node = yystack.l_mark[-2].node;
      }
      else
        yyval.node = astNodeNew (OP_GE, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 38:
#line 384 "parser.c"
	{
      if (CNODE (yystack.l_mark[-2].node) && CNODE (yystack.l_mark[0].node))
      {
        GID (yystack.l_mark[-2].node)->value.integer = 
        GID (yystack.l_mark[-2].node)->value.integer == GID (yystack.l_mark[0].node)->value.integer;
        yyval.node = yystack.l_mark[-2].node;
      }
      else
        yyval.node = astNodeNew (OP_EQ, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 39:
#line 395 "parser.c"
	{
      if (CNODE (yystack.l_mark[-2].node) && CNODE (yystack.l_mark[0].node))
      {
        GID (yystack.l_mark[-2].node)->value.integer = 
        GID (yystack.l_mark[-2].node)->value.integer != GID (yystack.l_mark[0].node)->value.integer;
        yyval.node = yystack.l_mark[-2].node;
      }
      else
        yyval.node = astNodeNew (OP_NE, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 41:
#line 410 "parser.c"
	{
      if (CNODE (yystack.l_mark[-2].node) && CNODE (yystack.l_mark[0].node))
      {
        GID (yystack.l_mark[-2].node)->value.integer = 
        GID (yystack.l_mark[-2].node)->value.integer && GID (yystack.l_mark[0].node)->value.integer;
        yyval.node = yystack.l_mark[-2].node;
      }
      else
        yyval.node = astNodeNew (OP_AND, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 43:
#line 425 "parser.c"
	{
      if (CNODE (yystack.l_mark[-2].node) && CNODE (yystack.l_mark[0].node))
      {
        GID (yystack.l_mark[-2].node)->value.integer = 
        GID (yystack.l_mark[-2].node)->value.integer || GID (yystack.l_mark[0].node)->value.integer;
        yyval.node = yystack.l_mark[-2].node;
      }
      else
        yyval.node = astNodeNew (OP_OR, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 45:
#line 440 "parser.c"
	{
      yyval.node = astNodeNew (OP_ASS, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 47:
#line 448 "parser.c"
	{
      markTopExpr (yystack.l_mark[-2].node);
      yyval.node = astNodeNew (OP_COM, yystack.l_mark[-2].node, yystack.l_mark[0].node, &yystack.l_mark[-1].id.position);
    }
break;
case 49:
#line 460 "parser.c"
	{
    int onlySign, noType;

    onlySign = yystack.l_mark[0].decl.sign && !yystack.l_mark[0].decl.type;
    if ((noType = !yystack.l_mark[0].decl.sign && !yystack.l_mark[0].decl.type))
      errorPrint (EK_WARN, "type defaults to 'int'", &yystack.l_mark[0].decl.position);

    if (onlySign || noType)
    {
      yyval.decl.type = TTYPE (basicTypeNew (TK_INT));

      if (yystack.l_mark[0].decl.sign == 1)
        ++yyval.decl.type->kind;
    }
    else
      yyval.decl.type = yystack.l_mark[0].decl.type;

    yyval.decl.store = yystack.l_mark[0].decl.store;
    yyval.decl.type->qualifier = yystack.l_mark[0].decl.qualifier;
  }
break;
case 50:
#line 484 "parser.c"
	{
      if (!yystack.l_mark[-1].decl.type || GKIN (yystack.l_mark[-1].decl.type) != TK_STRUCT)
        errorPrint (EK_WARN, "empty declaration", &yystack.l_mark[0].id.position);
    }
break;
case 52:
#line 493 "parser.c"
	{
      yyval.decl.type = NULL;
      yyval.decl.sign = 0;
      yyval.decl.store = yystack.l_mark[0].decl.store;
      yyval.decl.qualifier = 0;
      yyval.decl.position = yystack.l_mark[0].decl.position;
    }
break;
case 53:
#line 501 "parser.c"
	{ 
      if (yystack.l_mark[-1].decl.store)
      {
        sprintf (buffer, "multiple storage classes in declaration");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[0].decl.position);

        return 1;
      }
      else
        yyval.decl.store = yystack.l_mark[0].decl.store;

      yyval.decl.type = yystack.l_mark[-1].decl.type;
      yyval.decl.sign = yystack.l_mark[-1].decl.sign;
      yyval.decl.qualifier = yystack.l_mark[-1].decl.qualifier;
      yyval.decl.position = yystack.l_mark[0].decl.position;
    }
break;
case 54:
#line 518 "parser.c"
	{
      yyval.decl.type = yystack.l_mark[0].decl.type;
      yyval.decl.sign = yystack.l_mark[0].decl.sign;
      yyval.decl.store = 0;
      yyval.decl.qualifier = 0;
      yyval.decl.position = yystack.l_mark[0].decl.position;
    }
break;
case 55:
#line 526 "parser.c"
	{
      if (yystack.l_mark[-1].decl.sign && yystack.l_mark[0].decl.sign)
      {
        sprintf (buffer, "too many 'signed' and 'unsigned' in declaration");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[0].decl.position);

        return 1;
      }

      if (yystack.l_mark[-1].decl.type && yystack.l_mark[0].decl.type)
      {
        sprintf (buffer, "two or more data types in declaration");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[0].decl.position);
        return 1;
      }

      if ((yystack.l_mark[-1].decl.sign && yystack.l_mark[0].decl.type && !ISINT (yystack.l_mark[0].decl.type)) || 
          (yystack.l_mark[0].decl.sign && yystack.l_mark[-1].decl.type && !ISINT (yystack.l_mark[-1].decl.type)))
      {
        sprintf (buffer, "'signed' or 'unsigned' used with non-integer types");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[0].decl.position);

        return 1;
      }

      if (yystack.l_mark[0].decl.sign)
      {
        yyval.decl.type = yystack.l_mark[-1].decl.type;
        yyval.decl.sign = yystack.l_mark[0].decl.sign;
        yyval.decl.position = yystack.l_mark[-1].decl.position;
      }
      else
      {
        yyval.decl.type = yystack.l_mark[0].decl.type;
        yyval.decl.sign = yystack.l_mark[-1].decl.sign;
        yyval.decl.position = yystack.l_mark[0].decl.position;
      }

      yyval.decl.store = yystack.l_mark[-1].decl.store;
      yyval.decl.qualifier = yystack.l_mark[-1].decl.qualifier;
    }
break;
case 56:
#line 568 "parser.c"
	{
      yyval.decl.type = NULL;
      yyval.decl.sign = 0;
      yyval.decl.store = 0;
      yyval.decl.qualifier = yystack.l_mark[0].decl.qualifier;
      yyval.decl.position = yystack.l_mark[0].decl.position;
    }
break;
case 57:
#line 576 "parser.c"
	{
      yyval.decl.type = yystack.l_mark[-1].decl.type;   
      yyval.decl.sign = yystack.l_mark[-1].decl.sign;
      yyval.decl.store = yystack.l_mark[-1].decl.store;
      SBIT (yyval.decl.qualifier, yystack.l_mark[0].decl.qualifier);
      yyval.decl.position = yystack.l_mark[-1].decl.position; 
    }
break;
case 58:
#line 587 "parser.c"
	{
      *yystack.l_mark[0].decl.unknown = yystack.l_mark[-1].decl.type;

      patchIdent (yystack.l_mark[0].decl.ident, yystack.l_mark[-1].decl.store, 0);
      if (checkIdentType (yystack.l_mark[0].decl.ident))
        return 1;
      patchArraySize (yystack.l_mark[0].decl.ident->type);
      if (identAction (yystack.l_mark[0].decl.ident))
        return 1;
    }
break;
case 59:
#line 598 "parser.c"
	{
      *yystack.l_mark[0].decl.unknown = yystack.l_mark[-3].decl.type;

      patchIdent (yystack.l_mark[0].decl.ident, yystack.l_mark[-3].decl.store, 0);
      if (checkIdentType (yystack.l_mark[0].decl.ident))
        return 1;
      patchArraySize (yystack.l_mark[0].decl.ident->type);
      if (identAction (yystack.l_mark[0].decl.ident))
        return 1;
    }
break;
case 60:
#line 612 "parser.c"
	{
      if (!symbolTableGlobal ())
      {
        sprintf (buffer, "store class specifier in local scope");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[0].id.position);

        return 1;
      }

      yyval.decl.store = ST_EXTERN;
      yyval.decl.position = yystack.l_mark[0].id.position;
    }
break;
case 61:
#line 625 "parser.c"
	{
      if (!symbolTableGlobal ())
      {
        sprintf (buffer, "store class specifier in local scope");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[0].id.position);

        return 1;
      }
      
      yyval.decl.store = ST_STATIC;
      yyval.decl.position = yystack.l_mark[0].id.position;
    }
break;
case 62:
#line 641 "parser.c"
	{
      yyval.decl.type = TTYPE (basicTypeNew (TK_VOID));
      yyval.decl.sign = 0;
      yyval.decl.position = yystack.l_mark[0].id.position;
    }
break;
case 63:
#line 647 "parser.c"
	{
      yyval.decl.type = TTYPE (basicTypeNew (TK_CHAR));
      yyval.decl.sign = 0;
      yyval.decl.position = yystack.l_mark[0].id.position;
    }
break;
case 64:
#line 653 "parser.c"
	{
      yyval.decl.type = TTYPE (basicTypeNew (TK_SHORT));
      yyval.decl.sign = 0;
      yyval.decl.position = yystack.l_mark[0].id.position;
    }
break;
case 65:
#line 659 "parser.c"
	{
      yyval.decl.type = TTYPE (basicTypeNew (TK_INT));
      yyval.decl.sign = 0;
      yyval.decl.position = yystack.l_mark[0].id.position;
    }
break;
case 66:
#line 665 "parser.c"
	{
      yyval.decl.type = NULL;
      yyval.decl.sign = -1;
      yyval.decl.position = yystack.l_mark[0].id.position;
    }
break;
case 67:
#line 671 "parser.c"
	{
      yyval.decl.type = NULL;
      yyval.decl.sign = 1;
      yyval.decl.position = yystack.l_mark[0].id.position;
    }
break;
case 68:
#line 677 "parser.c"
	{
      yyval.decl.type = yystack.l_mark[0].decl.type;
      yyval.decl.sign = 0;
      yyval.decl.position = yystack.l_mark[0].decl.position;
    }
break;
case 69:
#line 686 "parser.c"
	{
      NewType *type;

      type = TNEW (symbolTableSearchType (yystack.l_mark[0].id.lexeme, 0));
      if (type && type->table)
      {
        sprintf (buffer, "redefinition of 'struct %s'", yystack.l_mark[0].id.lexeme);
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[0].id.position);
        errorPrint (EK_NOTE, "originally defined here", &type->position);

        return 1;
      }
      else if (!type)
      {
        type = newTypeNew (0, yystack.l_mark[0].id.lexeme, NULL, &yystack.l_mark[0].id.position);
        symbolTableInsertType (yystack.l_mark[0].id.lexeme, TTYPE (type));
      }
      else if (!type->table)
        free (yystack.l_mark[0].id.lexeme);

      yyval.decl.type = TTYPE (type);
      yyval.decl.offset = offset;

      offset.value = 0;
      offset.direction = 1;

      symbolTablePush ();
    }
break;
case 70:
#line 715 "parser.c"
	{
      if (symbolTableEmpty ())
        errorPrint (EK_WARN, "struct has no members", &yystack.l_mark[-4].id.position);

      TNEW (yystack.l_mark[-3].decl.type)->size = offset.value;
      TNEW (yystack.l_mark[-3].decl.type)->table = top;

      yyval.decl.type = TTYPE (structTypeNew (TNEW (yystack.l_mark[-3].decl.type)));
      yyval.decl.position = yystack.l_mark[-3].decl.position;
      offset = yystack.l_mark[-3].decl.offset;

      symbolTablePop ();
    }
break;
case 71:
#line 729 "parser.c"
	{
      yyval.decl.offset = offset;
      offset.value = 0;
      offset.direction = 1;

      symbolTablePush ();
    }
break;
case 72:
#line 737 "parser.c"
	{
      NewType *type;

      if (symbolTableEmpty ())
        errorPrint (EK_WARN, "struct has no members", &yystack.l_mark[-4].id.position);

      type = newTypeNew (offset.value, NULL, top, &yystack.l_mark[-4].id.position);
      yyval.decl.type = TTYPE (structTypeNew (type));
      yyval.decl.position = yystack.l_mark[-4].id.position;
      offset = yystack.l_mark[-3].decl.offset;

      symbolTablePop ();
    }
break;
case 73:
#line 751 "parser.c"
	{
      NewType *type;

      type = TNEW (symbolTableSearchType (yystack.l_mark[0].id.lexeme, 1));
      if (!type)
      {
        type = newTypeNew (0, yystack.l_mark[0].id.lexeme, NULL, &yystack.l_mark[0].id.position);
        symbolTableInsertType (yystack.l_mark[0].id.lexeme, TTYPE (type));
      }
      else
        free (yystack.l_mark[0].id.lexeme);

      yyval.decl.type = TTYPE (structTypeNew (type));
      yyval.decl.position = yystack.l_mark[0].id.position;
    }
break;
case 77:
#line 779 "parser.c"
	{
      if (yystack.l_mark[-1].decl.sign && yystack.l_mark[0].decl.sign)
      {
        sprintf (buffer, "too many 'signed' and 'unsigned' in declaration");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[0].decl.position);

        return 1;
      }

      if (yystack.l_mark[-1].decl.type && yystack.l_mark[0].decl.type)
      {
        sprintf (buffer, "two or more data types in declaration");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[0].decl.position);
        return 1;
      }

      if ((yystack.l_mark[-1].decl.sign && yystack.l_mark[0].decl.type && !ISINT (yystack.l_mark[0].decl.type)) || 
          (yystack.l_mark[0].decl.sign && yystack.l_mark[-1].decl.type && !ISINT (yystack.l_mark[-1].decl.type)))
      {
        sprintf (buffer, "'signed' or 'unsigned' used with non-integer types");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[0].decl.position);

        return 1;
      }

      if (yystack.l_mark[0].decl.sign)
      {
        yyval.decl.type = yystack.l_mark[-1].decl.type;
        yyval.decl.sign = yystack.l_mark[0].decl.sign;
        yyval.decl.position = yystack.l_mark[-1].decl.position;
      }
      else
      {
        yyval.decl.type = yystack.l_mark[0].decl.type;
        yyval.decl.sign = yystack.l_mark[-1].decl.sign;
        yyval.decl.position = yystack.l_mark[0].decl.position;
      }

      yyval.decl.store = 0;
      yyval.decl.qualifier = yystack.l_mark[-1].decl.qualifier;
    }
break;
case 78:
#line 821 "parser.c"
	{
      yyval.decl.type = yystack.l_mark[0].decl.type;
      yyval.decl.sign = yystack.l_mark[0].decl.sign;
      yyval.decl.store = 0;
      yyval.decl.qualifier = 0;
      yyval.decl.position = yystack.l_mark[0].decl.position;
    }
break;
case 79:
#line 829 "parser.c"
	{
      yyval.decl.type = yystack.l_mark[-1].decl.type;  
      yyval.decl.sign = yystack.l_mark[-1].decl.sign;
      yyval.decl.store = 0;
      yyval.decl.qualifier = yystack.l_mark[-1].decl.qualifier;
      SBIT (yyval.decl.qualifier, yystack.l_mark[0].decl.qualifier);
      yyval.decl.position = yystack.l_mark[-1].decl.position;  
    }
break;
case 80:
#line 838 "parser.c"
	{
      yyval.decl.type = NULL;
      yyval.decl.sign = 0;
      yyval.decl.store = 0;
      yyval.decl.qualifier = yystack.l_mark[0].decl.qualifier;
      yyval.decl.position = yystack.l_mark[0].decl.position;
    }
break;
case 81:
#line 849 "parser.c"
	{
    SBIT (yyval.decl.qualifier, QU_CONST);
    yyval.decl.position = yystack.l_mark[0].id.position;
  }
break;
case 82:
#line 854 "parser.c"
	{
    SBIT (yyval.decl.qualifier, QU_VOLATILE);
    yyval.decl.position = yystack.l_mark[0].id.position;
  }
break;
case 83:
#line 862 "parser.c"
	{
      *yystack.l_mark[0].decl.unknown = yystack.l_mark[-1].decl.type;
      yyval.decl.unknown = yystack.l_mark[-1].decl.unknown;
      yyval.decl.ident = yystack.l_mark[0].decl.ident;
    }
break;
case 84:
#line 868 "parser.c"
	{
      yyval.decl.unknown = yystack.l_mark[0].decl.unknown;
      yyval.decl.ident = yystack.l_mark[0].decl.ident;
    }
break;
case 85:
#line 876 "parser.c"
	{
      yyval.decl.ident = identNew (yystack.l_mark[0].id.lexeme, NULL, 1, 0, &yystack.l_mark[0].id.position);  
      yyval.decl.unknown = &yyval.decl.ident->type;
    }
break;
case 86:
#line 881 "parser.c"
	{
      yyval.decl.unknown = yystack.l_mark[-1].decl.unknown;
      yyval.decl.ident = yystack.l_mark[-1].decl.ident;
    }
break;
case 87:
#line 886 "parser.c"
	{
      if (genExprValue (yystack.l_mark[-1].node))
        return 1;

      if (yystack.l_mark[-1].node->ident->constType != CT_INT)
      {
        sprintf (buffer, "nonconstant length array '%s'", yystack.l_mark[-3].decl.ident->lexeme);
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[-3].decl.ident->position);

        return 1;
      }
      else if (yystack.l_mark[-1].node->ident->value.integer <= 0)
      {
        sprintf (buffer, "nonpositive length array '%s'", yystack.l_mark[-3].decl.ident->lexeme);
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[-3].decl.ident->position);

        return 1;
      }

      *yystack.l_mark[-3].decl.unknown = TTYPE (arrayTypeNew (yystack.l_mark[-1].node->ident->value.integer));
      yyval.decl.unknown = &GELE (*yystack.l_mark[-3].decl.unknown);
      yyval.decl.ident = yystack.l_mark[-3].decl.ident;
    }
break;
case 88:
#line 910 "parser.c"
	{
      yyval.decl.type = TTYPE (funcTypeNew (NULL));
      *yystack.l_mark[-1].decl.unknown = yyval.decl.type;
      yyval.decl.parameter = &GPAR (yyval.decl.type);
    }
break;
case 89:
#line 916 "parser.c"
	{
      yyval.decl.unknown = &GRET (yystack.l_mark[-2].decl.type);
      yyval.decl.ident = yystack.l_mark[-4].decl.ident;  
    }
break;
case 90:
#line 921 "parser.c"
	{
      *yystack.l_mark[-2].decl.unknown = TTYPE (funcTypeNew (NULL));
      yyval.decl.unknown = &GRET (*yystack.l_mark[-2].decl.unknown);
      yyval.decl.ident = yystack.l_mark[-2].decl.ident;
    }
break;
case 91:
#line 930 "parser.c"
	{
      yyval.decl.type = TTYPE (ptrTypeNew (0));
      yyval.decl.position = yystack.l_mark[0].id.position;
      yyval.decl.unknown = &GPTD (yyval.decl.type);
    }
break;
case 92:
#line 936 "parser.c"
	{
      yyval.decl.type = TTYPE (ptrTypeNew (yystack.l_mark[0].decl.qualifier));
      yyval.decl.position = yystack.l_mark[-1].id.position;
      yyval.decl.unknown = &GPTD (yyval.decl.type);
    }
break;
case 93:
#line 942 "parser.c"
	{
      *yystack.l_mark[0].decl.unknown = TTYPE (ptrTypeNew (0));

      yyval.decl.type = yystack.l_mark[0].decl.type;
      yyval.decl.position = yystack.l_mark[0].decl.position;
      yyval.decl.unknown = &GPTD (*yystack.l_mark[0].decl.unknown);
    }
break;
case 94:
#line 950 "parser.c"
	{
      *yystack.l_mark[-1].decl.unknown = TTYPE (ptrTypeNew (yystack.l_mark[-1].decl.qualifier));

      yyval.decl.type = yystack.l_mark[0].decl.type;
      yyval.decl.position = yystack.l_mark[0].decl.position;
      yyval.decl.unknown = &GPTD (*yystack.l_mark[-1].decl.unknown);
    }
break;
case 95:
#line 961 "parser.c"
	{
      SBIT (yyval.decl.qualifier, yystack.l_mark[0].decl.qualifier);
    }
break;
case 96:
#line 965 "parser.c"
	{
      SBIT (yyval.decl.qualifier, yystack.l_mark[-1].decl.qualifier);
      SBIT (yyval.decl.qualifier, yystack.l_mark[0].decl.qualifier);
    }
break;
case 98:
#line 974 "parser.c"
	{
      *yystack.l_mark[-3].decl.parameter = identNew ("?", NULL, 0, 0, &yystack.l_mark[0].id.position);
    }
break;
case 100:
#line 982 "parser.c"
	{
      yyval.decl.parameter = yystack.l_mark[-2].decl.parameter;
    }
break;
case 101:
#line 986 "parser.c"
	{
      yystack.l_mark[-4].decl.parameter = yystack.l_mark[-1].decl.parameter;
    }
break;
case 102:
#line 993 "parser.c"
	{
      *yystack.l_mark[0].decl.unknown = yystack.l_mark[-1].decl.type;

      if (patchParameterIdent (yystack.l_mark[0].decl.ident, yystack.l_mark[-1].decl.store))
        return 1;
      patchArraySize (yystack.l_mark[0].decl.ident->type);

      *(yystack.l_mark[-3].decl.parameter) = yystack.l_mark[0].decl.ident;
      yystack.l_mark[-3].decl.parameter = &(*(yystack.l_mark[-3].decl.parameter))->next;
    }
break;
case 103:
#line 1004 "parser.c"
	{
      *yystack.l_mark[0].decl.unknown = yystack.l_mark[-1].decl.type;

      if (patchParameterIdent (yystack.l_mark[0].decl.ident, yystack.l_mark[-1].decl.store))
        return 1;
      patchArraySize (yystack.l_mark[0].decl.ident->type);

      *(yystack.l_mark[-3].decl.parameter) = yystack.l_mark[0].decl.ident;
      yystack.l_mark[-3].decl.parameter = &(*(yystack.l_mark[-3].decl.parameter))->next;
    }
break;
case 104:
#line 1015 "parser.c"
	{
      Ident *ident;

      ident = identNew (NULL, yystack.l_mark[0].decl.type, 0, 0, &yystack.l_mark[-1].decl.position);
      if (patchParameterIdent (ident, yystack.l_mark[0].decl.store))
        return 1;

      *(yystack.l_mark[-2].decl.parameter) = ident;
      yystack.l_mark[-2].decl.parameter = &(*(yystack.l_mark[-2].decl.parameter))->next;
    }
break;
case 105:
#line 1029 "parser.c"
	{
      yyval.decl.type = yystack.l_mark[0].decl.type;
    }
break;
case 106:
#line 1033 "parser.c"
	{
      *yystack.l_mark[0].decl.unknown = yystack.l_mark[-1].decl.type;
      yyval.decl.type = yystack.l_mark[0].decl.ident->type;
    }
break;
case 107:
#line 1041 "parser.c"
	{
      yyval.decl.unknown = &GPTD (yystack.l_mark[0].decl.type);
      yyval.decl.ident = identNew (NULL, yystack.l_mark[0].decl.type, 0, 0, &yystack.l_mark[0].decl.position);
    }
break;
case 108:
#line 1046 "parser.c"
	{
      yyval.decl.unknown = yystack.l_mark[0].decl.unknown;
      yyval.decl.ident = yystack.l_mark[0].decl.ident;
    }
break;
case 109:
#line 1051 "parser.c"
	{
      *yystack.l_mark[0].decl.unknown = yystack.l_mark[-1].decl.type;
      yyval.decl.unknown = yystack.l_mark[-1].decl.unknown;
      yyval.decl.ident = yystack.l_mark[0].decl.ident;
    }
break;
case 110:
#line 1060 "parser.c"
	{
      yyval.decl.unknown = yystack.l_mark[-1].decl.unknown;
      yyval.decl.ident = yystack.l_mark[-1].decl.ident;
    }
break;
case 111:
#line 1065 "parser.c"
	{
      ArrayType *type;

      if (genExprValue (yystack.l_mark[-1].node))
        return 1;

      if (yystack.l_mark[-1].node->ident->constType != CT_INT)
      {
        sprintf (buffer, "nonconstant length array");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[-2].id.position);

        return 1;
      }
      else if (yystack.l_mark[-1].node->ident->value.integer <= 0)
      {
        sprintf (buffer, "nonpositive length array");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[-2].id.position);

        return 1;
      }

      type = arrayTypeNew (yystack.l_mark[-1].node->ident->value.integer);
      yyval.decl.unknown = &GELE (type);
      yyval.decl.ident = identNew (NULL, TTYPE (type), 0, 0, &yystack.l_mark[-2].id.position);
    }
break;
case 112:
#line 1091 "parser.c"
	{
      if (genExprValue (yystack.l_mark[-1].node))
        return 1;

      if (yystack.l_mark[-1].node->ident->constType != CT_INT)
      {
        sprintf (buffer, "nonconstant length array");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[-2].id.position);

        return 1;
      }
      else if (yystack.l_mark[-1].node->ident->value.integer <= 0)
      {
        sprintf (buffer, "nonpositive length array");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[-2].id.position);

        return 1;
      }

      *yystack.l_mark[-3].decl.unknown = TTYPE (arrayTypeNew (yystack.l_mark[-1].node->ident->value.integer));
      yyval.decl.unknown = &GELE (*yystack.l_mark[-3].decl.unknown);
      yyval.decl.ident = identNew (NULL, *yystack.l_mark[-3].decl.unknown, 0, 0, &yystack.l_mark[-3].decl.position);
    }
break;
case 113:
#line 1115 "parser.c"
	{
      FuncType *type;

      type = funcTypeNew (NULL);
      yyval.decl.unknown = &GRET (type);
      yyval.decl.ident = identNew (NULL, TTYPE (type), 0, 0, &yystack.l_mark[-1].id.position);
    }
break;
case 114:
#line 1123 "parser.c"
	{
      yyval.decl.type = TTYPE (funcTypeNew (NULL));
      yyval.decl.parameter = &GPAR (yyval.decl.type);
    }
break;
case 115:
#line 1128 "parser.c"
	{
      yyval.decl.unknown = &GRET (yystack.l_mark[-2].decl.type);
      yyval.decl.ident = identNew (NULL, yystack.l_mark[-2].decl.type, 0, 0, &yystack.l_mark[-3].id.position);
    }
break;
case 116:
#line 1133 "parser.c"
	{
      *yystack.l_mark[-2].decl.unknown = TTYPE (funcTypeNew (NULL));
      yyval.decl.unknown = &GRET (*yystack.l_mark[-2].decl.unknown);
      yyval.decl.ident = yystack.l_mark[-2].decl.ident;
    }
break;
case 117:
#line 1139 "parser.c"
	{
      yyval.decl.type = TTYPE (funcTypeNew (NULL));
      *yystack.l_mark[-1].decl.unknown = yyval.decl.type;
      yyval.decl.parameter = &GPAR (yyval.decl.type);
    }
break;
case 118:
#line 1145 "parser.c"
	{
      yyval.decl.unknown = &GRET (yyval.decl.type);
      yyval.decl.ident = yystack.l_mark[-4].decl.ident;
    }
break;
case 120:
#line 1153 "parser.c"
	{ yyval.isFunc = 0; }
break;
case 122:
#line 1155 "parser.c"
	{
      markTopExpr (yystack.l_mark[0].node);
      if (genExprValue (yystack.l_mark[0].node))
        return 1;
    }
break;
case 127:
#line 1171 "parser.c"
	{
      if (!yystack.l_mark[0].isFunc)
        symbolTablePush ();
    }
break;
case 128:
#line 1179 "parser.c"
	{
      symbolTablePop ();
    }
break;
case 137:
#line 1202 "parser.c"
	{ }
break;
case 138:
#line 1204 "parser.c"
	{
      yyval.node = yystack.l_mark[-1].node;
    }
break;
case 139:
#line 1211 "parser.c"
	{
      yyval.block = blockNew ();
      if (genExprJump (yystack.l_mark[-1].node, NULL, yyval.block))
        return 1;
    }
break;
case 140:
#line 1220 "parser.c"
	{
      setCurrentBlock (yystack.l_mark[-1].block);
    }
break;
case 141:
#line 1224 "parser.c"
	{
      yyval.block = blockNew ();
      instrNew (OP_GOTO, blockToIdent (yyval.block), NULL, NULL);
      yyval.block->ref = 1;
      setCurrentBlock (yystack.l_mark[-2].block);
    }
break;
case 142:
#line 1231 "parser.c"
	{
      setCurrentBlock (yystack.l_mark[-1].block);
    }
break;
case 143:
#line 1238 "parser.c"
	{
      yyval.block = blockNew ();
      blockListPush (&continueList, yyval.block);
      setCurrentBlock (yyval.block);
    }
break;
case 144:
#line 1244 "parser.c"
	{
      yyval.block = blockNew ();
      blockListPush (&breakList, yyval.block);

      if (genExprJump (yystack.l_mark[0].node, NULL, yyval.block))
        return 1;
    }
break;
case 145:
#line 1252 "parser.c"
	{
      instrNew (OP_GOTO, blockToIdent (yystack.l_mark[-5].block), NULL, NULL);
      yystack.l_mark[-5].block->ref = 1;
      setCurrentBlock (yystack.l_mark[-2].block);

      blockListPop (&continueList);
      blockListPop (&breakList);
    }
break;
case 146:
#line 1261 "parser.c"
	{
      yyval.block = blockNew ();
      blockListPush (&breakList, yyval.block);
    }
break;
case 147:
#line 1265 "parser.c"
	{
      yyval.block = blockNew (); 
      blockListPush (&continueList, yyval.block);

      setCurrentBlock (yyval.block);
    }
break;
case 148:
#line 1272 "parser.c"
	{
      if (genExprJump (yystack.l_mark[-2].node, yystack.l_mark[-6].block, NULL))
        return 1;

      setCurrentBlock (yystack.l_mark[-7].block);

      blockListPop(&continueList);
      blockListPop (&breakList);
    }
break;
case 149:
#line 1282 "parser.c"
	{
      markTopExpr (yystack.l_mark[-2].node);
      if (genExprValue (yystack.l_mark[-2].node))
        return 1;

      yyval.block = blockNew ();
      blockListPush (&continueList, yyval.block);
      setCurrentBlock (yyval.block);
    }
break;
case 150:
#line 1291 "parser.c"
	{
      yyval.block = blockNew ();
      blockListPush (&breakList, yyval.block);

      if (genExprJump (yystack.l_mark[-2].node, NULL, yyval.block))
        return 1;
    }
break;
case 151:
#line 1299 "parser.c"
	{
      instrNew (OP_GOTO, blockToIdent (yystack.l_mark[-2].block), NULL, NULL); 
      yystack.l_mark[-2].block->ref = 1;
      setCurrentBlock (yystack.l_mark[-1].block);

      blockListPop (&continueList);
      blockListPop (&breakList);
    }
break;
case 152:
#line 1308 "parser.c"
	{
      markTopExpr (yystack.l_mark[-3].node);
      if (genExprValue (yystack.l_mark[-3].node))
        return 1;

      yyval.block = blockNew ();
      setCurrentBlock (yyval.block);
    }
break;
case 153:
#line 1316 "parser.c"
	{
      yyval.block = blockNew ();
      blockListPush (&breakList, yyval.block);

      if (genExprJump (yystack.l_mark[-3].node, NULL, yyval.block))
        return 1;
    }
break;
case 154:
#line 1323 "parser.c"
	{
      yyval.block = blockNew (); /*l2*/
      blockListPush (&continueList, yyval.block);
    }
break;
case 155:
#line 1328 "parser.c"
	{
      setCurrentBlock (yystack.l_mark[-1].block);

      markTopExpr (yystack.l_mark[-5].node);
      if (genExprValue (yystack.l_mark[-5].node))
        return 1;

      instrNew (OP_GOTO, blockToIdent (yystack.l_mark[-3].block), NULL, NULL);
      yystack.l_mark[-3].block->ref = 1;
      setCurrentBlock (yystack.l_mark[-2].block);

      blockListPop (&continueList);
      blockListPop (&breakList);
    }
break;
case 156:
#line 1346 "parser.c"
	{
      yyval.block= blockListPop (&continueList);
      if (!yyval.block)
      {
        sprintf (buffer, "continue statement not within a loop");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[-1].id.position);

        return 1;
      }

      instrNew (OP_GOTO, blockToIdent (yyval.block), NULL, NULL);
      yyval.block->ref = 1;
    }
break;
case 157:
#line 1360 "parser.c"
	{
      yyval.block = blockListPop (&breakList);
      if (!yyval.block)
      {
        sprintf (buffer, "break statement not within a loop");
        errorPrint (EK_ERROR, buffer, &yystack.l_mark[-1].id.position);

        return 1;
      }

      instrNew (OP_GOTO, blockToIdent (yyval.block), NULL, NULL);   
      yyval.block->ref = 1;
    }
break;
case 158:
#line 1374 "parser.c"
	{
      if (GKIN (GRET (func->type)) != TK_VOID)
      {
        sprintf (buffer, "return no value, in function returning non-void");
        errorPrint (EK_WARN, buffer, &yystack.l_mark[-1].id.position);
      }

      instrNew (OP_GOTO, blockToIdent (funcEnd), NULL, NULL);
      funcEnd->ref = 1;
    }
break;
case 159:
#line 1385 "parser.c"
	{
      if (genExprValue (yystack.l_mark[-1].node))
        return 1;

      yystack.l_mark[-1].node->ident = arrayFuncConv (yystack.l_mark[-1].node);

      if (checkRetType (GRET (func->type), yystack.l_mark[-1].node->ident->type, &yystack.l_mark[-2].id.position))
        return 1;

      instrNew (OP_RETURN, GID (yystack.l_mark[-1].node), NULL, NULL);
      instrNew (OP_GOTO, blockToIdent (funcEnd), NULL, NULL);
      funcEnd->ref = 1;
    }
break;
case 164:
#line 1412 "parser.c"
	{
      Ident *ite;

      *yystack.l_mark[0].decl.unknown = yystack.l_mark[-1].decl.type;

      patchIdent (yystack.l_mark[0].decl.ident, yystack.l_mark[-3].decl.store, 1);
      if (checkIdentType (yystack.l_mark[0].decl.ident))
        return 1;
      patchArraySize (yystack.l_mark[0].decl.ident->type);
      if (identAction (yystack.l_mark[0].decl.ident))
        return 1;

      symbolTablePush ();

      offset.value = 8;

      ite = GPAR (yystack.l_mark[0].decl.ident->type);
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

      func = yystack.l_mark[0].decl.ident;
    }
break;
case 165:
#line 1449 "parser.c"
	{
      yyval.isFunc = 1;
    }
break;
case 166:
#line 1453 "parser.c"
	{
      setCurrentBlock (funcEnd);
      instrNew (OP_RET, NULL, NULL, NULL);
      TFUNC (yystack.l_mark[-3].decl.ident->type)->offset = offset.value;

      /*printf ("function %s\n", $3.ident->lexeme);*/
     /* genPrint (funcBeg);*/
      /*printf ("\n\n\n");*/

      /*if (!dataSeg)
      {
        emitData ();
        dataSeg = 1;
      }
      emitFunc (funcBeg);*/

      addGlobalFunc (funcBeg);
      func = NULL;
    }
break;
#line 2735 "y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                yys = yyname[YYTRANSLATE(yychar)];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    YYERROR_CALL("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
