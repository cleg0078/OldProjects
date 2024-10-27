#ifndef lint
static char yysccsid[] = "@(#)yaccpar     1.7 (Berkeley) 09/09/90";
#endif
#define YYBYACC 1
#line 2 "fpc.y"
#include<stdio.h>
#include<stdlib.h>
#include"string.h"
#include"ListId.h"
#include"FPCStr.h"
#define BestType(a,b) (a>b?a:b)
#define Push(a) if(stackptr<200) Stack[stackptr++]=a;
#define Pop() stackptr>0?Stack[--stackptr]:-1

#define YYSTYPE FPCval

YYSTYPE yylval;
int tmp,tmp2,actoper_type;
int nbtmp,nbligne,nbfct;
int NBFUNCTION,NBIDENT,NBSYMBOL;
int nberror,opt;
ListIdent IdentHead[1000];
Symbole Symtab[3500];
Function FctHead[256];
int Stack[200],stackptr;
int StackAddr[200],stackptrAddr;

void error(int,char *,char *);
void warning(int,char *,char *);

void BuildPseudo(int,int,int,int);
int GenerateTMP(int);
int BuildOper(int,int,int);
int BuilUnaryOper(int,int,int);

#line 37 "y_tab.c"
#define OR 257
#define AND 258
#define NOT 259
#define AFFECTATION 260
#define INFERIEUR 261
#define SUPERIEUR 262
#define INFERIEUR_EGAL 263
#define SUPERIEUR_EGAL 264
#define EGALITE 265
#define DIFFERENCE 266
#define PLUS 267
#define MOINS 268
#define UNARY_PLUS 269
#define UNARY_MOINS 270
#define MULTIPLIER 271
#define DIVISER 272
#define MODULO 273
#define TYPE_BOOL 274
#define TYPE_ENTIER 275
#define TYPE_REAL 276
#define PBEGIN 277
#define PEND 278
#define PARENTHESE_OUVERTE 279
#define PARENTHESE_FERMEE 280
#define FIN_INSTRUCTION 281
#define VIRGULE 282
#define DOUBLE_POINT 283
#define IF 284
#define THEN 285
#define ELSE 286
#define GOTO 287
#define WHILE 288
#define PROGRAM 289
#define FUNCTION 290
#define PROCEDURE 291
#define VAR 292
#define CONSTANTE 293
#define IDENTIFICATEUR 294
#define WRITE 295
#define WRITELN 296
#define JUMP_EQ 297
#define COS 298
#define SIN 299
#define LN 300
#define EXP 301
#define SQRT 302
#define ABS 303
#define POINT 304
#define PUSHADDR 305
#define POPJUMP 306
#define PUSHARG 307
#define POPARG 308
#define RETURN 309
#define UNARY 310
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    1,    1,    4,    2,    5,    2,    6,    2,    3,    3,
    7,    7,    7,    9,    8,   10,    8,   11,    8,    8,
   12,   12,   12,   13,   13,   15,   15,   15,   17,   17,
   18,   17,   16,   16,   20,   16,   21,   22,   16,   16,
   16,   16,   16,   16,   16,   24,   23,   25,   25,   19,
   19,   14,   14,   14,   14,   14,   14,   14,   14,   14,
   14,   14,   14,   14,   14,   14,   14,   14,   14,   14,
   14,   14,   14,   14,   14,   14,   26,   26,   26,   28,
   29,   27,   31,   32,   30,   33,   33,   33,   33,   35,
   34,    0,    0,
};
short yylen[] = {                                         2,
    0,    2,    0,    4,    0,    4,    0,    4,    1,    3,
    0,    2,    2,    0,    3,    0,    3,    0,    3,    3,
    0,    2,    2,    1,    3,    0,    2,    2,    0,    2,
    0,    5,    1,    1,    0,    4,    0,    0,    5,    1,
    1,    3,    4,    1,    4,    1,    4,    1,    3,    3,
    1,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    2,    2,    2,    4,    4,
    4,    4,    4,    4,    1,    4,    1,    1,    3,    0,
    0,   14,    0,    0,   14,    0,    2,    2,    2,    0,
    7,    2,    5,
};
short yydefred[] = {                                      0,
    0,    1,    0,    0,    0,    0,   92,    0,    7,    3,
    5,    2,    0,    0,    0,    0,    0,   89,   90,    0,
    0,   87,   88,   93,    9,    0,    0,    0,   26,   83,
   80,    8,    0,    4,    6,    0,    0,    0,   10,   28,
   26,   91,    0,   33,    0,   37,   78,    0,    0,   46,
    0,   27,   34,   40,   41,    0,   11,   11,    0,   77,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   75,    0,   21,    0,    0,    0,    0,
    0,   42,   79,   68,   67,   66,    0,    0,   21,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   13,   18,   14,   16,   84,
    0,   81,   52,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   64,   65,   58,    0,    0,
    0,    0,    0,    0,   36,    0,   23,   43,    0,    0,
   47,    0,   45,    0,    0,    0,    0,    0,    0,   76,
   69,   70,   71,   73,   72,   74,    0,   39,    0,    0,
   19,   15,   17,    0,    0,    0,   31,    0,    1,    1,
    0,    0,    0,   32,   26,   26,    0,    0,    0,    0,
   85,   82,
};
short yydgoto[] = {                                       3,
    6,   12,   26,   16,   17,   15,   80,  121,  155,  156,
  154,  111,  149,  150,   36,   52,  145,  181,   53,  109,
   75,  146,   54,   55,  113,   74,   22,   38,  159,   23,
   37,  157,   13,    4,   29,
};
short yysindex[] = {                                   -273,
 -264,    0,    0, -280, -238, -270,    0, -199,    0,    0,
    0,    0, -255, -226, -197, -197, -197,    0,    0, -193,
 -172,    0,    0,    0,    0, -261, -209, -194,    0,    0,
    0,    0, -168,    0,    0,  473, -131, -130,    0,    0,
    0,    0, -276,    0,  739,    0,    0, -129, -122,    0,
 -108,    0,    0,    0,    0, -132,    0,    0,  493,    0,
 -138,  739,  739,  739,  739, -107, -106,  -99,  -95,  -93,
  -92,  -91,  758,    0,  739,    0,  739,  739,  739, -151,
 -113,    0,    0,    0,    0,    0,  826, -138,    0,  739,
  739,  739,  739,  739,  739,  739,  739,  739,  739,  739,
  739,  739,  739,  739,  739,  739,  739,  739, -119,  758,
  605,  758, -243,  846,  758,    0,    0,    0,    0,    0,
  -88,    0,    0,  633,  866,  886,  906,  926,  946,  966,
  685,  685, -232, -232, -232,    0,    0,    0,  685,  685,
 -214, -214, -214,  766,    0,  766,    0,    0,  -81,  758,
    0,  739,    0, -150, -105,  -84,  -70, -137,  -69,    0,
    0,    0,    0,    0,    0,    0,  -71,    0,  739,  758,
    0,    0,    0,  -76,  -88,  -72,    0,  758,    0,    0,
  766, -262, -141,    0,    0,    0,  517,  537,  -60,  -59,
    0,    0,
};
short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0, -246,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  385,    0,    0,
  561,    0,    0,    0,    0,  405,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -182,    0,    0,    0,    0,
    0,    0,  358,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  986,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  429,  786,
    0, -188,    0,    0,  449,    0,    0,    0,    0,    0,
  504,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  187,  235, -103,  -54,   -5,    0,    0,    0,  283,  331,
   43,   91,  139,    0,    0,    0,    0,    0,  661,  686,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  581,    0,    0, -173,
    0,    0,    0,    0,  548,    0,    0,  711,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
short yygindex[] = {                                      0,
  -49,    0,  124,    0,    0,    0,  171,   75,    0,    0,
    0,  146,    0,  -37,  -39, -135,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -36,    0,    0,    0,    0,
    0,    0,    0,  228,    0,
};
#define YYTABLESIZE 1259
short yytable[] = {                                      56,
   18,   59,   43,    9,   10,   11,   61,   73,  167,   86,
  168,    9,   10,   11,  185,    1,   47,   60,    2,   32,
   33,   19,   56,    7,   84,   85,   86,   87,   88,    5,
   86,  101,  102,  103,   20,   21,  151,  110,  152,  112,
  114,  115,    8,   86,   86,  184,   98,   99,  100,  101,
  102,  103,  125,  126,  127,  128,  129,  130,  131,  132,
  133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
  143,   34,   33,   77,   77,   77,   77,   24,   77,   77,
   77,   77,   77,   77,   77,   77,   35,   33,   77,   77,
   77,   48,    2,   48,   77,   77,   25,   77,   77,   77,
   30,   77,   77,   77,  116,   77,   49,   56,   49,   56,
   77,   77,   77,   77,  170,   77,   77,   77,   77,   77,
   77,   31,  117,  118,  119,   39,   77,   79,  120,  182,
  183,  178,    9,   10,   11,  186,  117,  118,  119,   27,
   28,   83,  116,  171,   56,  187,  188,   57,   58,   76,
   56,   56,   61,   61,   61,   61,   77,   61,   61,   61,
  117,  118,  119,   61,   61,  144,  122,   61,   61,   61,
   78,   89,   90,   61,   61,   61,   61,   61,   61,   91,
   61,   61,   61,   92,   61,   93,   94,   95,  172,   61,
   61,   61,   61,  158,   61,   61,   61,   61,   61,   61,
  169,   62,   62,   62,   62,   61,   62,   62,   62,  173,
  174,  176,   62,   62,  177,  179,   62,   62,   62,  180,
  191,  192,   62,   62,   62,   62,   62,   62,   81,   62,
   62,   62,  175,   62,  124,   14,    0,    0,   62,   62,
   62,   62,    0,   62,   62,   62,   62,   62,   62,    0,
   63,   63,   63,   63,   62,   63,   63,   63,    0,    0,
    0,   63,   63,    0,    0,   63,   63,   63,    0,    0,
    0,   63,   63,   63,   63,   63,   63,    0,   63,   63,
   63,    0,   63,    0,    0,    0,    0,   63,   63,   63,
   63,    0,   63,   63,   63,   63,   63,   63,   55,   55,
   55,   55,    0,   63,    0,    0,    0,    0,    0,   55,
   55,    0,    0,   55,   55,   55,    0,    0,    0,   55,
   55,   55,   55,   55,   55,    0,   55,   55,   55,    0,
   55,    0,    0,    0,    0,   55,   55,   55,   55,    0,
   55,   55,   55,   55,   55,   55,   56,   56,   56,   56,
    0,   55,    0,    0,    0,    0,    0,   56,   56,    0,
    0,   56,   56,   56,    0,    0,    0,   56,   56,   56,
   56,   56,   56,    0,   56,   56,   56,    0,   56,    0,
    0,    0,    0,   56,   56,   56,   56,    0,   56,   56,
   56,   56,   56,   56,   57,   57,   57,   57,    0,   56,
    0,    0,    0,    0,    0,   57,   57,    0,    0,   57,
   57,   57,    0,    0,    0,   57,   57,   57,   57,   57,
   57,    0,   57,   57,   57,    0,   57,    0,    0,    0,
    0,   57,   57,   57,   57,    0,   57,   57,   57,   57,
   57,   57,   59,   59,   59,   59,    0,   57,    0,    0,
    0,    0,    0,   59,   59,    0,    0,    0,    0,    0,
    0,    0,    0,   59,   59,   59,   59,   59,   59,    0,
   59,   59,   59,    0,   59,    0,    0,    0,    0,   59,
   59,   59,   59,    0,   59,   59,   59,   59,   59,   59,
   60,   60,   60,   60,    0,   59,    0,    0,    0,    0,
    0,   60,   60,    0,    0,    0,    0,    0,    0,    0,
    0,   60,   60,   60,   60,   60,   60,    0,   60,   60,
   60,    0,   60,    0,    0,    0,    0,   60,   60,   60,
   60,    0,   60,   60,   60,   60,   60,   60,   53,   53,
   53,   53,    0,   60,    0,    0,    0,    0,    0,   53,
   53,    0,    0,    0,    0,    0,    0,    0,    0,   53,
   53,   53,   53,   53,   53,    0,   53,   53,   53,    0,
   53,    0,    0,    0,    0,   53,   53,   53,   53,    0,
   53,   53,   53,   53,   53,   53,   54,   54,   54,   54,
    0,   53,    0,    0,    0,    0,    0,   54,   54,    0,
    0,    0,    0,    0,    0,    0,    0,   54,   54,   54,
   54,   54,   54,   35,   54,   54,   54,    0,   54,    0,
    0,    0,    0,   54,   54,   54,   54,    0,   54,   54,
   54,   54,   54,   54,   35,   35,   35,    0,   35,   54,
   77,   35,   35,   35,   77,   35,    0,    0,    0,    0,
   35,   35,   35,   35,    0,    0,    0,    0,    0,    0,
   51,   77,   77,    0,    0,   77,   35,    0,   77,    0,
   77,    0,   77,    0,    0,    0,    0,   77,   77,   77,
   77,   51,   51,   51,   29,   51,    0,    0,   51,    0,
   51,    0,   51,   77,    0,    0,    0,   51,   51,   51,
   51,    0,    0,    0,   50,   29,   29,   29,    0,   29,
    0,    0,   29,   51,   29,    0,   29,    0,    0,    0,
    0,   29,   29,   29,   29,   50,   50,   50,   40,   50,
    0,    0,   50,    0,   50,    0,   50,   29,    0,    0,
    0,   50,   50,   50,   50,    0,    0,    0,   40,   41,
   42,   43,    0,   44,    0,    0,   45,   50,    0,   12,
   46,    0,    0,    0,    0,   47,   48,   49,   50,   41,
   82,   43,   40,   44,    0,    0,   45,   12,   12,   12,
   46,   51,    0,   12,    0,   47,   48,   49,   50,    0,
    0,    0,   40,   41,  189,   43,    0,   44,    0,    0,
   45,   51,    0,   20,   46,    0,    0,    0,    0,   47,
   48,   49,   50,   41,  190,   43,   44,   44,    0,    0,
   45,   20,   20,   20,   46,   51,    0,   20,    0,   47,
   48,   49,   50,    0,    0,    0,   30,   44,   44,    0,
    0,   44,    0,    0,   44,   51,   44,    0,   44,    0,
    0,    0,    0,   44,   44,   44,   44,   30,   30,   30,
  147,   30,    0,   62,   30,    0,    0,    0,   30,   44,
    0,   63,   64,   30,   30,   30,   30,    0,    0,    0,
    0,    0,    0,   65,  148,    0,    0,    0,  147,   30,
    0,   62,    0,    0,    0,    0,    0,   47,   66,   63,
   64,    0,   67,   68,   69,   70,   71,   72,    0,    0,
    0,   65,  160,    0,    0,    0,   22,    0,    0,   22,
    0,    0,    0,    0,    0,   47,   66,   22,   22,    0,
   67,   68,   69,   70,   71,   72,    0,    0,    0,   22,
   22,   24,    0,    0,   24,   98,   99,  100,  101,  102,
  103,    0,    0,   22,   22,  106,  107,  108,   22,   22,
   22,   22,   22,   22,   24,   24,   25,   24,    0,   25,
    0,    0,    0,    0,    0,    0,    0,    0,   24,   24,
    0,    0,    0,   24,   24,   24,   24,   24,   24,   25,
   25,    0,   25,    0,    0,    0,    0,   62,    0,    0,
    0,    0,    0,   25,   25,   63,   64,    0,   25,   25,
   25,   25,   25,   25,   96,   97,    0,   65,   98,   99,
  100,  101,  102,  103,  104,  105,    0,    0,  106,  107,
  108,   47,   66,    0,    0,    0,   67,   68,   69,   70,
   71,   72,   41,    0,   43,    0,   44,    0,    0,   45,
    0,    0,    0,   46,    0,    0,    0,    0,   47,   48,
   49,   50,   38,    0,   38,    0,   38,    0,    0,   38,
    0,    0,    0,   38,   51,    0,    0,    0,   38,   38,
   38,   38,   96,   97,    0,    0,   98,   99,  100,  101,
  102,  103,  104,  105,   38,    0,  106,  107,  108,    0,
    0,    0,   96,   97,    0,  123,   98,   99,  100,  101,
  102,  103,  104,  105,    0,    0,  106,  107,  108,    0,
    0,    0,   96,   97,    0,  153,   98,   99,  100,  101,
  102,  103,  104,  105,    0,    0,  106,  107,  108,    0,
    0,    0,   96,   97,    0,  161,   98,   99,  100,  101,
  102,  103,  104,  105,    0,    0,  106,  107,  108,    0,
    0,    0,   96,   97,    0,  162,   98,   99,  100,  101,
  102,  103,  104,  105,    0,    0,  106,  107,  108,    0,
    0,    0,   96,   97,    0,  163,   98,   99,  100,  101,
  102,  103,  104,  105,    0,    0,  106,  107,  108,    0,
    0,    0,   96,   97,    0,  164,   98,   99,  100,  101,
  102,  103,  104,  105,    0,    0,  106,  107,  108,    0,
    0,    0,   96,   97,    0,  165,   98,   99,  100,  101,
  102,  103,  104,  105,    0,    0,  106,  107,  108,    0,
    0,    0,   75,   75,    0,  166,   75,   75,   75,   75,
   75,   75,   75,   75,    0,    0,   75,   75,   75,
};
short yycheck[] = {                                      36,
  256,   41,  279,  274,  275,  276,   43,   45,  144,  256,
  146,  274,  275,  276,  277,  289,  293,  294,  292,  281,
  282,  277,   59,  304,   62,   63,   64,   65,   65,  294,
  277,  264,  265,  266,  290,  291,  280,   75,  282,   77,
   78,   79,  281,  290,  291,  181,  261,  262,  263,  264,
  265,  266,   90,   91,   92,   93,   94,   95,   96,   97,
   98,   99,  100,  101,  102,  103,  104,  105,  106,  107,
  108,  281,  282,  256,  257,  258,  259,  304,  261,  262,
  263,  264,  265,  266,  267,  268,  281,  282,  271,  272,
  273,  280,  292,  282,  277,  278,  294,  280,  281,  282,
  294,  284,  285,  286,  256,  288,  280,  144,  282,  146,
  293,  294,  295,  296,  152,  298,  299,  300,  301,  302,
  303,  294,  274,  275,  276,  294,  309,  260,  280,  179,
  180,  169,  274,  275,  276,  277,  274,  275,  276,   16,
   17,  280,  256,  294,  181,  185,  186,  279,  279,  279,
  187,  188,  256,  257,  258,  259,  279,  261,  262,  263,
  274,  275,  276,  267,  268,  285,  280,  271,  272,  273,
  279,  279,  279,  277,  278,  279,  280,  281,  282,  279,
  284,  285,  286,  279,  288,  279,  279,  279,  294,  293,
  294,  295,  296,  282,  298,  299,  300,  301,  302,  303,
  282,  256,  257,  258,  259,  309,  261,  262,  263,  294,
  281,  281,  267,  268,  286,  292,  271,  272,  273,  292,
  281,  281,  277,  278,  279,  280,  281,  282,   58,  284,
  285,  286,  158,  288,   89,    8,   -1,   -1,  293,  294,
  295,  296,   -1,  298,  299,  300,  301,  302,  303,   -1,
  256,  257,  258,  259,  309,  261,  262,  263,   -1,   -1,
   -1,  267,  268,   -1,   -1,  271,  272,  273,   -1,   -1,
   -1,  277,  278,  279,  280,  281,  282,   -1,  284,  285,
  286,   -1,  288,   -1,   -1,   -1,   -1,  293,  294,  295,
  296,   -1,  298,  299,  300,  301,  302,  303,  256,  257,
  258,  259,   -1,  309,   -1,   -1,   -1,   -1,   -1,  267,
  268,   -1,   -1,  271,  272,  273,   -1,   -1,   -1,  277,
  278,  279,  280,  281,  282,   -1,  284,  285,  286,   -1,
  288,   -1,   -1,   -1,   -1,  293,  294,  295,  296,   -1,
  298,  299,  300,  301,  302,  303,  256,  257,  258,  259,
   -1,  309,   -1,   -1,   -1,   -1,   -1,  267,  268,   -1,
   -1,  271,  272,  273,   -1,   -1,   -1,  277,  278,  279,
  280,  281,  282,   -1,  284,  285,  286,   -1,  288,   -1,
   -1,   -1,   -1,  293,  294,  295,  296,   -1,  298,  299,
  300,  301,  302,  303,  256,  257,  258,  259,   -1,  309,
   -1,   -1,   -1,   -1,   -1,  267,  268,   -1,   -1,  271,
  272,  273,   -1,   -1,   -1,  277,  278,  279,  280,  281,
  282,   -1,  284,  285,  286,   -1,  288,   -1,   -1,   -1,
   -1,  293,  294,  295,  296,   -1,  298,  299,  300,  301,
  302,  303,  256,  257,  258,  259,   -1,  309,   -1,   -1,
   -1,   -1,   -1,  267,  268,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  277,  278,  279,  280,  281,  282,   -1,
  284,  285,  286,   -1,  288,   -1,   -1,   -1,   -1,  293,
  294,  295,  296,   -1,  298,  299,  300,  301,  302,  303,
  256,  257,  258,  259,   -1,  309,   -1,   -1,   -1,   -1,
   -1,  267,  268,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  277,  278,  279,  280,  281,  282,   -1,  284,  285,
  286,   -1,  288,   -1,   -1,   -1,   -1,  293,  294,  295,
  296,   -1,  298,  299,  300,  301,  302,  303,  256,  257,
  258,  259,   -1,  309,   -1,   -1,   -1,   -1,   -1,  267,
  268,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  277,
  278,  279,  280,  281,  282,   -1,  284,  285,  286,   -1,
  288,   -1,   -1,   -1,   -1,  293,  294,  295,  296,   -1,
  298,  299,  300,  301,  302,  303,  256,  257,  258,  259,
   -1,  309,   -1,   -1,   -1,   -1,   -1,  267,  268,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  277,  278,  279,
  280,  281,  282,  256,  284,  285,  286,   -1,  288,   -1,
   -1,   -1,   -1,  293,  294,  295,  296,   -1,  298,  299,
  300,  301,  302,  303,  277,  278,  279,   -1,  281,  309,
  256,  284,  285,  286,  260,  288,   -1,   -1,   -1,   -1,
  293,  294,  295,  296,   -1,   -1,   -1,   -1,   -1,   -1,
  256,  277,  278,   -1,   -1,  281,  309,   -1,  284,   -1,
  286,   -1,  288,   -1,   -1,   -1,   -1,  293,  294,  295,
  296,  277,  278,  279,  256,  281,   -1,   -1,  284,   -1,
  286,   -1,  288,  309,   -1,   -1,   -1,  293,  294,  295,
  296,   -1,   -1,   -1,  256,  277,  278,  279,   -1,  281,
   -1,   -1,  284,  309,  286,   -1,  288,   -1,   -1,   -1,
   -1,  293,  294,  295,  296,  277,  278,  279,  256,  281,
   -1,   -1,  284,   -1,  286,   -1,  288,  309,   -1,   -1,
   -1,  293,  294,  295,  296,   -1,   -1,   -1,  256,  277,
  278,  279,   -1,  281,   -1,   -1,  284,  309,   -1,  256,
  288,   -1,   -1,   -1,   -1,  293,  294,  295,  296,  277,
  278,  279,  256,  281,   -1,   -1,  284,  274,  275,  276,
  288,  309,   -1,  280,   -1,  293,  294,  295,  296,   -1,
   -1,   -1,  256,  277,  278,  279,   -1,  281,   -1,   -1,
  284,  309,   -1,  256,  288,   -1,   -1,   -1,   -1,  293,
  294,  295,  296,  277,  278,  279,  256,  281,   -1,   -1,
  284,  274,  275,  276,  288,  309,   -1,  280,   -1,  293,
  294,  295,  296,   -1,   -1,   -1,  256,  277,  278,   -1,
   -1,  281,   -1,   -1,  284,  309,  286,   -1,  288,   -1,
   -1,   -1,   -1,  293,  294,  295,  296,  277,  278,  279,
  256,  281,   -1,  259,  284,   -1,   -1,   -1,  288,  309,
   -1,  267,  268,  293,  294,  295,  296,   -1,   -1,   -1,
   -1,   -1,   -1,  279,  280,   -1,   -1,   -1,  256,  309,
   -1,  259,   -1,   -1,   -1,   -1,   -1,  293,  294,  267,
  268,   -1,  298,  299,  300,  301,  302,  303,   -1,   -1,
   -1,  279,  280,   -1,   -1,   -1,  256,   -1,   -1,  259,
   -1,   -1,   -1,   -1,   -1,  293,  294,  267,  268,   -1,
  298,  299,  300,  301,  302,  303,   -1,   -1,   -1,  279,
  280,  256,   -1,   -1,  259,  261,  262,  263,  264,  265,
  266,   -1,   -1,  293,  294,  271,  272,  273,  298,  299,
  300,  301,  302,  303,  279,  280,  256,  282,   -1,  259,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  293,  294,
   -1,   -1,   -1,  298,  299,  300,  301,  302,  303,  279,
  280,   -1,  282,   -1,   -1,   -1,   -1,  259,   -1,   -1,
   -1,   -1,   -1,  293,  294,  267,  268,   -1,  298,  299,
  300,  301,  302,  303,  257,  258,   -1,  279,  261,  262,
  263,  264,  265,  266,  267,  268,   -1,   -1,  271,  272,
  273,  293,  294,   -1,   -1,   -1,  298,  299,  300,  301,
  302,  303,  277,   -1,  279,   -1,  281,   -1,   -1,  284,
   -1,   -1,   -1,  288,   -1,   -1,   -1,   -1,  293,  294,
  295,  296,  277,   -1,  279,   -1,  281,   -1,   -1,  284,
   -1,   -1,   -1,  288,  309,   -1,   -1,   -1,  293,  294,
  295,  296,  257,  258,   -1,   -1,  261,  262,  263,  264,
  265,  266,  267,  268,  309,   -1,  271,  272,  273,   -1,
   -1,   -1,  257,  258,   -1,  280,  261,  262,  263,  264,
  265,  266,  267,  268,   -1,   -1,  271,  272,  273,   -1,
   -1,   -1,  257,  258,   -1,  280,  261,  262,  263,  264,
  265,  266,  267,  268,   -1,   -1,  271,  272,  273,   -1,
   -1,   -1,  257,  258,   -1,  280,  261,  262,  263,  264,
  265,  266,  267,  268,   -1,   -1,  271,  272,  273,   -1,
   -1,   -1,  257,  258,   -1,  280,  261,  262,  263,  264,
  265,  266,  267,  268,   -1,   -1,  271,  272,  273,   -1,
   -1,   -1,  257,  258,   -1,  280,  261,  262,  263,  264,
  265,  266,  267,  268,   -1,   -1,  271,  272,  273,   -1,
   -1,   -1,  257,  258,   -1,  280,  261,  262,  263,  264,
  265,  266,  267,  268,   -1,   -1,  271,  272,  273,   -1,
   -1,   -1,  257,  258,   -1,  280,  261,  262,  263,  264,
  265,  266,  267,  268,   -1,   -1,  271,  272,  273,   -1,
   -1,   -1,  257,  258,   -1,  280,  261,  262,  263,  264,
  265,  266,  267,  268,   -1,   -1,  271,  272,  273,
};
#define YYFINAL 3
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 310
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"OR","AND","NOT","AFFECTATION",
"INFERIEUR","SUPERIEUR","INFERIEUR_EGAL","SUPERIEUR_EGAL","EGALITE",
"DIFFERENCE","PLUS","MOINS","UNARY_PLUS","UNARY_MOINS","MULTIPLIER","DIVISER",
"MODULO","TYPE_BOOL","TYPE_ENTIER","TYPE_REAL","PBEGIN","PEND",
"PARENTHESE_OUVERTE","PARENTHESE_FERMEE","FIN_INSTRUCTION","VIRGULE",
"DOUBLE_POINT","IF","THEN","ELSE","GOTO","WHILE","PROGRAM","FUNCTION",
"PROCEDURE","VAR","CONSTANTE","IDENTIFICATEUR","WRITE","WRITELN","JUMP_EQ",
"COS","SIN","LN","EXP","SQRT","ABS","POINT","PUSHADDR","POPJUMP","PUSHARG",
"POPARG","RETURN","UNARY",
};
char *yyrule[] = {
"$accept : program",
"start_declarations :",
"start_declarations : start_declarations declarations",
"$$1 :",
"declarations : TYPE_ENTIER $$1 declarations_var FIN_INSTRUCTION",
"$$2 :",
"declarations : TYPE_REAL $$2 declarations_var FIN_INSTRUCTION",
"$$3 :",
"declarations : TYPE_BOOL $$3 declarations_var FIN_INSTRUCTION",
"declarations_var : IDENTIFICATEUR",
"declarations_var : declarations_var VIRGULE IDENTIFICATEUR",
"start_input_parameters :",
"start_input_parameters : start_input_parameters input_parameters",
"start_input_parameters : start_input_parameters error",
"$$4 :",
"input_parameters : TYPE_ENTIER $$4 IDENTIFICATEUR",
"$$5 :",
"input_parameters : TYPE_REAL $$5 IDENTIFICATEUR",
"$$6 :",
"input_parameters : TYPE_BOOL $$6 IDENTIFICATEUR",
"input_parameters : input_parameters VIRGULE input_parameters",
"start_getparameters :",
"start_getparameters : start_getparameters getparameters",
"start_getparameters : start_getparameters error",
"getparameters : expressions_logiq",
"getparameters : getparameters VIRGULE expressions_logiq",
"start_instructions :",
"start_instructions : start_instructions instructions",
"start_instructions : start_instructions error",
"then_else :",
"then_else : THEN instructions",
"$$7 :",
"then_else : THEN instructions ELSE $$7 instructions",
"instructions : FIN_INSTRUCTION",
"instructions : expressions_arith",
"$$8 :",
"instructions : IF expressions_logiq $$8 then_else",
"$$9 :",
"$$10 :",
"instructions : WHILE $$9 expressions_logiq $$10 instructions",
"instructions : write",
"instructions : writeln",
"instructions : PBEGIN start_instructions PEND",
"instructions : IDENTIFICATEUR PARENTHESE_OUVERTE start_getparameters PARENTHESE_FERMEE",
"instructions : RETURN",
"instructions : RETURN PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE",
"writeln : WRITELN",
"write : WRITE PARENTHESE_OUVERTE write_expr PARENTHESE_FERMEE",
"write_expr : expressions_logiq",
"write_expr : write_expr VIRGULE expressions_logiq",
"expressions_arith : terme AFFECTATION expressions_logiq",
"expressions_arith : terme",
"expressions_logiq : PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE",
"expressions_logiq : expressions_logiq PLUS expressions_logiq",
"expressions_logiq : expressions_logiq MOINS expressions_logiq",
"expressions_logiq : expressions_logiq MULTIPLIER expressions_logiq",
"expressions_logiq : expressions_logiq DIVISER expressions_logiq",
"expressions_logiq : expressions_logiq MODULO expressions_logiq",
"expressions_logiq : expressions_logiq DIFFERENCE expressions_logiq",
"expressions_logiq : expressions_logiq OR expressions_logiq",
"expressions_logiq : expressions_logiq AND expressions_logiq",
"expressions_logiq : expressions_logiq INFERIEUR expressions_logiq",
"expressions_logiq : expressions_logiq SUPERIEUR expressions_logiq",
"expressions_logiq : expressions_logiq INFERIEUR_EGAL expressions_logiq",
"expressions_logiq : expressions_logiq SUPERIEUR_EGAL expressions_logiq",
"expressions_logiq : expressions_logiq EGALITE expressions_logiq",
"expressions_logiq : MOINS expressions_logiq",
"expressions_logiq : PLUS expressions_logiq",
"expressions_logiq : NOT expressions_logiq",
"expressions_logiq : COS PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE",
"expressions_logiq : SIN PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE",
"expressions_logiq : LN PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE",
"expressions_logiq : SQRT PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE",
"expressions_logiq : EXP PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE",
"expressions_logiq : ABS PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE",
"expressions_logiq : terme",
"expressions_logiq : IDENTIFICATEUR PARENTHESE_OUVERTE start_getparameters PARENTHESE_FERMEE",
"terme : IDENTIFICATEUR",
"terme : CONSTANTE",
"terme : PARENTHESE_OUVERTE terme PARENTHESE_FERMEE",
"$$11 :",
"$$12 :",
"procedure : PROCEDURE IDENTIFICATEUR $$11 PARENTHESE_OUVERTE start_input_parameters PARENTHESE_FERMEE $$12 FIN_INSTRUCTION VAR start_declarations PBEGIN start_instructions PEND FIN_INSTRUCTION",
"$$13 :",
"$$14 :",
"function : FUNCTION IDENTIFICATEUR $$13 PARENTHESE_OUVERTE start_input_parameters PARENTHESE_FERMEE $$14 FIN_INSTRUCTION VAR start_declarations PBEGIN start_instructions PEND FIN_INSTRUCTION",
"start_proc_fct :",
"start_proc_fct : start_proc_fct procedure",
"start_proc_fct : start_proc_fct function",
"start_proc_fct : start_proc_fct error",
"$$15 :",
"prog_bloc : VAR start_declarations start_proc_fct PBEGIN $$15 start_instructions PEND",
"program : prog_bloc POINT",
"program : PROGRAM IDENTIFICATEUR FIN_INSTRUCTION prog_bloc POINT",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#ifdef YYSTACKSIZE
#ifndef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#endif
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 600
#define YYMAXDEPTH 600
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 341 "fpc.y"

#include"lexyy.c"
#include"listid.c"

void BuildPseudo(int oper,int oper1,int oper2,int res)
{
 Symtab[NBSYMBOL].Oper=oper;
 Symtab[NBSYMBOL].Res=res;
 Symtab[NBSYMBOL].Oper1=oper1;
 Symtab[NBSYMBOL].Oper2=oper2;
/*
 fprintf(stderr,"Oper=%d  RES=%d  NBSYMBOL=%d\n",oper,res,NBSYMBOL);
*/
 NBSYMBOL++;
}

int GenerateTMP(int type)
{
 char buff[16];
 sprintf(buff,"#tmp%d",nbtmp++);
 return(InsertIdent(IdentHead,buff,IDENTIFICATEUR,type,NBFUNCTION) );
}

int BuildOper(int oper,int pl1,int pl2)
{
 int res;
 res=GenerateTMP(BestType(IdentHead[pl1].opertype,
                          IdentHead[pl2].opertype));   
 BuildPseudo(oper,pl1,pl2,res);
 return(res);
}

int BuildUnaryOper(int oper,int pl1)
{
 int res;
 res=GenerateTMP(IdentHead[pl1].opertype);
 BuildPseudo(oper,pl1,-1,res);
 return(res);
}

void CheckUsedTime()
{
 int i,oper,oper1,oper2,res;

 for(i=0;i<NBSYMBOL;i++)
    {
     oper = Symtab[i].Oper;
     oper1=Symtab[i].Oper1; oper2=Symtab[i].Oper2;
     res=Symtab[i].Res;

     if( oper1!=-1 && oper!=GOTO )
        IdentHead[oper1].usedtime++;

     if( oper2!=-1 && oper!=GOTO && oper!=JUMP_EQ)
        IdentHead[oper2].usedtime++;
 
     if( res !=-1 && oper !=GOTO && oper!=JUMP_EQ )
        IdentHead[res].usedtime++;
    }
}

void yyerror(char *str)
{
 nberror++;

 fprintf(stderr,"\n%d: %s Syntax error\n",nbligne,yytext);

 if(nberror>4)
    exit(0);
}

void warning(int nb,char *s1,char *s2)
{
 fprintf(stderr,"\n%d: Warning %s",nb,s1);
 if( s2 )
    fprintf(stderr,"%s",s2);
  fprintf(stderr,"\n");
}

void error(int nb,char *s1,char *s2)
{
 fprintf(stderr,"\n%d: %s",nb,s1);
 if( s2 )
    fprintf(stderr,"%s",s2);

 fprintf(stderr,"\n");
 nberror++;
 if(nberror>4)
    exit(0);
}

void DestroyIdent(int pl)
{
 int i;

 if( NBIDENT >0 )
    NBIDENT--;

 strcpy(IdentHead[pl].nom,IdentHead[NBIDENT].nom);
 IdentHead[pl].type = IdentHead[NBIDENT].type;
 
 IdentHead[pl].usedtime = IdentHead[NBIDENT].usedtime;
 IdentHead[pl].eval = IdentHead[NBIDENT].eval;
 IdentHead[pl].fval = IdentHead[NBIDENT].fval;
 IdentHead[pl].opertype = IdentHead[NBIDENT].opertype;
 
 for(i=0;i<NBSYMBOL;i++)
     if( Symtab[i].Oper != GOTO && Symtab[i].Oper != JUMP_EQ )
       {
        if(Symtab[i].Oper1 == NBIDENT ) 
           Symtab[i].Oper1=pl; 

        if(Symtab[i].Oper2 == NBIDENT )
           Symtab[i].Oper2=pl; 
        if(Symtab[i].Res == NBIDENT )
           Symtab[i].Res = pl;
       }
     else
       if( Symtab[i].Oper == JUMP_EQ )
         if(Symtab[i].Oper1 == NBIDENT )
            Symtab[i].Oper1=pl;
}

void main(int argc,char **argv)
{
 extern char *optarg;
 extern int optind;
 int bflag, ch, fd;
 FILE *in,*out;
 char *name;
 InterIdent inter_ident;
 int i;

 fprintf(stderr,"Fast Pascal Compiler @1996-1998 C.Legendre\n");
 if( argc < 2)
   {
    fprintf(stderr,"No Input File\n");
    exit(0);
   }

 bflag = 0;
 opt=1;
 name=argv[1];

 in = fopen(name,"rt");
 if( !in )
   {
    fprintf(stderr,"Unable To Open Source File: %s\n",argv[1]);
    exit(0);
   }

 NBFUNCTION=0;
 NBSYMBOL=NBIDENT=0;
 nbligne=1;
 nberror=stackptr=stackptrAddr=0;
 nbtmp=nbfct=0;

 memset(IdentHead,0,1000*sizeof(ListIdent));
 memset(Symtab,0,3500*sizeof(Symbole));
 memset(FctHead,0,256*sizeof(Function));

 yyin = in;

 /* Prepare the Jump To The Main Function */
 BuildPseudo(GOTO,-1,-1,-1);

 yyparse();

 fclose(in);
 if( nberror )
   {
    fprintf(stderr,"\nCompiling Error.\n");
    exit(0);
   }
 else
   fprintf(stderr,"\nAll Ok\n");

 out =fopen("FPC.obj","wb");
 if( !out )
   {
    fprintf(stderr,"\nUnable To Generate Object File\n");
    exit(0);
   } 
 
 if( opt )
   {
    fprintf(stderr,"Optimizing...\nPhase I:  Deleting Unused ID\n");
    CheckUsedTime();
    for(i=0;i<NBIDENT;i++)
       {
    /*    fprintf(stderr,"NOM=%s   USEDTIME=%d\n",IdentHead[i].nom,IdentHead[i].usedtime);
    */     
         if( IdentHead[i].usedtime == 0 )
           DestroyIdent(i--);
       }
   }

 fwrite(&NBIDENT,1,sizeof(int),out);
 fwrite(&NBSYMBOL,1,sizeof(int),out);

 for(i=0;i<NBIDENT;i++)
    {
     inter_ident.opertype = IdentHead[i].opertype; 
     inter_ident.eval = IdentHead[i].eval;
     inter_ident.fval = IdentHead[i].fval;
     inter_ident.nbf = IdentHead[i].fct;
     strncpy(inter_ident.nom,IdentHead[i].nom,32);
     fwrite(&inter_ident,1,sizeof(InterIdent),out);
    }       

 for(i=0;i<NBSYMBOL;i++)
    {
     fwrite(&Symtab[i],1,sizeof(InterSymbol),out);
/*
     fprintf(stderr,"Oper=%d  Oper1=%d Oper2=%d Res=%d\n",Symtab[i].Oper,Symtab[i].Oper1,Symtab[i].Oper2,Symtab[i].Res);
*/
    }
 fclose(out);
 fprintf(stderr,"Now Generating Object File...OK\n");
 fprintf(stderr,"Stat:  %d Data Lines    %d Code Lines\n",NBIDENT,NBSYMBOL);

 exit(0); 
}

#line 825 "y_tab.c"
#define YYABORT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("yydebug: state %d, reading %d (%s)\n", yystate,
                    yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("yydebug: state %d, shifting to state %d\n",
                    yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
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
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("yydebug: state %d, error recovery shifting\
 to state %d\n", *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("yydebug: error recovery discarding state %d\n",
                            *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("yydebug: state %d, error recovery discards token %d (%s)\n",
                    yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("yydebug: state %d, reducing by rule %d (%s)\n",
                yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 3:
#line 60 "fpc.y"
{ actoper_type=TYPE_ENTIER; }
break;
case 5:
#line 61 "fpc.y"
{ actoper_type=TYPE_REAL; }
break;
case 7:
#line 62 "fpc.y"
{ actoper_type=TYPE_BOOL; }
break;
case 9:
#line 65 "fpc.y"
{ if( InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION) < 0 ) error(nbligne,"Duplicate Id ",yylval.nom); }
break;
case 10:
#line 67 "fpc.y"
{ if( InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION) < 0 )  error(nbligne,"Duplicate Id ",yylval.nom); }
break;
case 14:
#line 77 "fpc.y"
{ actoper_type=TYPE_ENTIER; }
break;
case 15:
#line 77 "fpc.y"
{
                                                                      if( (tmp=InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION)) < 0 ) 
                                                                          error(nbligne,"Duplicate Id ",yylval.nom);  
                                                                       FctHead[NBFUNCTION].arg[FctHead[NBFUNCTION].nbarg++]=tmp;
             
                                                                     }
break;
case 16:
#line 83 "fpc.y"
{ actoper_type=TYPE_REAL; }
break;
case 17:
#line 83 "fpc.y"
{ 
                                                                   if( (tmp=InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION)) < 0 ) 
	                                                               error(nbligne,"Duplicate Id ",yylval.nom); 
  	                                                            FctHead[NBFUNCTION].arg[FctHead[NBFUNCTION].nbarg++]=tmp;
                                                                  }
break;
case 18:
#line 88 "fpc.y"
{ actoper_type=TYPE_BOOL; }
break;
case 19:
#line 88 "fpc.y"
{
                                                                  if( (tmp=InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION)) < 0 )
                                                                     error(nbligne,"Duplicate Id ",yylval.nom);
                                                                   FctHead[NBFUNCTION].arg[FctHead[NBFUNCTION].nbarg++]=tmp;
                                                                  }
break;
case 24:
#line 102 "fpc.y"
{ BuildPseudo(PUSHARG,-1,-1,yyvsp[0].place); }
break;
case 25:
#line 103 "fpc.y"
{ BuildPseudo(PUSHARG,-1,-1,yyvsp[0].place); }
break;
case 30:
#line 112 "fpc.y"
{ tmp2=0; }
break;
case 31:
#line 114 "fpc.y"
{ tmp2=1; BuildPseudo(GOTO,-1,-1,-1); Push(NBSYMBOL);
                    }
break;
case 32:
#line 116 "fpc.y"
{ Push(NBSYMBOL); }
break;
case 35:
#line 122 "fpc.y"
{ Push(NBSYMBOL); BuildPseudo(JUMP_EQ,yyvsp[0].place,-1,-1); }
break;
case 36:
#line 123 "fpc.y"
{
                          int res1,res2,res3;
                          if( tmp2==1 ) /* ELSE */
                            {
                             res1=Pop(); res2=Pop(); res3=Pop();
                             Symtab[res2-1].Res=res1;
                             Symtab[res3].Res=res2;
                            }
                          else
                            { 
                             res1=Pop(); 
                             Symtab[res1].Res = NBSYMBOL;
                            }
                         }
break;
case 37:
#line 137 "fpc.y"
{ Push(NBSYMBOL); }
break;
case 38:
#line 137 "fpc.y"
{ Push(NBSYMBOL); BuildPseudo(JUMP_EQ,yyvsp[-1].place,-1,-1); }
break;
case 39:
#line 138 "fpc.y"
{ int res=Pop();  
                              Symtab[res].Res=NBSYMBOL+1;
                              Symtab[res].Oper1 = Symtab[res-1].Res;
                              BuildPseudo(GOTO,-1,-1,Pop());
                            }
break;
case 43:
#line 147 "fpc.y"
{
                tmp=FindFunction(FctHead,yyvsp[-3].nom);
                if( tmp < 0 )
                   error(nbligne,"Undeclared Function:",yyvsp[-3].nom);

                BuildPseudo(PUSHADDR,-1,-1,NBSYMBOL+2);
                BuildPseudo(GOTO,-1,-1,FctHead[tmp].addr);
               }
break;
case 44:
#line 155 "fpc.y"
{ if( FctHead[NBFUNCTION-1].ret != 0 )
                           error(nbligne,"Missing Return Value",NULL);
                        BuildPseudo(POPJUMP,-1,-1,-1); 
                      }
break;
case 45:
#line 160 "fpc.y"
{
                        if( FctHead[NBFUNCTION-1].ret == 0 )
                           error(nbligne,"Procedure Can't Use A Return Value",NULL);
                        BuildPseudo(PUSHARG,-1,-1,yyvsp[-1].place);
                       }
break;
case 46:
#line 167 "fpc.y"
{ BuildPseudo(WRITELN,-1,-1,-1); }
break;
case 48:
#line 175 "fpc.y"
{ 
                                  if( yyvsp[0].type==PROCEDURE )
			             error(nbligne,"Invalid Procedure Affectation",NULL);
                                  BuildPseudo(WRITE,yyvsp[0].place,-1,-1); 
                                }
break;
case 49:
#line 181 "fpc.y"
{
                                 if( yyvsp[0].type == PROCEDURE ) 
                                    error(nbligne,"Invalide Procedure Affectation",NULL);
                                 BuildPseudo(WRITE,yyvsp[0].place,-1,-1);
                                }
break;
case 50:
#line 189 "fpc.y"
{ if( yyvsp[-2].type != IDENTIFICATEUR )
                                                       error(nbligne,"Invalid lvalue Assignment",NULL);
                                                    else
                                                      {/*             ORIGINAL CODE                     */
                                                       /* BuildPseudo(AFFECTATION,$3.place,0,$1.place); */
                                                       /*     But We've include a small Optimization    */
                                                       if( yyvsp[0].type== PROCEDURE )
                                                          error(nbligne,"Invalid Procedure Assignment\n",NULL);

                                                       if( NBSYMBOL>1 && strncmp(IdentHead[yyvsp[0].place].nom,"#tmp",4) == 0 && opt)
                                                          Symtab[NBSYMBOL-1].Res = yyvsp[-2].place;  
                                                       else
                                                          BuildPseudo(AFFECTATION,yyvsp[0].place,-1,yyvsp[-2].place);
                                                      }
                                                  }
break;
case 51:
#line 204 "fpc.y"
{ yyval.type = yyvsp[0].type; yyval.place=yyvsp[0].place; }
break;
case 52:
#line 208 "fpc.y"
{ yyval.place=yyvsp[-1].place; }
break;
case 53:
#line 209 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
break;
case 54:
#line 210 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
break;
case 55:
#line 211 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
break;
case 56:
#line 212 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
break;
case 57:
#line 213 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
break;
case 58:
#line 214 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
break;
case 59:
#line 215 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
break;
case 60:
#line 216 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
break;
case 61:
#line 217 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
break;
case 62:
#line 218 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
break;
case 63:
#line 219 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
break;
case 64:
#line 220 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
break;
case 65:
#line 221 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
break;
case 66:
#line 222 "fpc.y"
{ yyval.place=BuildUnaryOper(UNARY_MOINS,yyvsp[0].place); }
break;
case 67:
#line 223 "fpc.y"
{ yyval.place=BuildUnaryOper(UNARY_PLUS,yyvsp[0].place); }
break;
case 68:
#line 224 "fpc.y"
{ yyval.place=BuildUnaryOper(NOT,yyvsp[0].place); }
break;
case 69:
#line 225 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); }
break;
case 70:
#line 226 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); }
break;
case 71:
#line 227 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); }
break;
case 72:
#line 228 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); }
break;
case 73:
#line 229 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); }
break;
case 74:
#line 230 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); }
break;
case 75:
#line 231 "fpc.y"
{ yyval.type = yyvsp[0].type; yyval.place=yyvsp[0].place;}
break;
case 76:
#line 233 "fpc.y"
{
               tmp=FindFunction(FctHead,yyvsp[-3].nom);
               if( tmp < 0 )
                  error(nbligne,"Undeclared Function or Procedure",yyvsp[-3].nom);

               BuildPseudo(PUSHADDR,-1,-1,NBSYMBOL+2);
               BuildPseudo(GOTO,-1,-1,FctHead[tmp].addr);
               if( FctHead[tmp].ret !=0 )
                 { 
                  int tmp2;
                  tmp2=GenerateTMP(TYPE_REAL);
                  BuildPseudo(POPARG,-1,-1,tmp2);
                  if(tmp2<0 ) error(nbligne,"Can't Alloc TMP\n","");
                  yyval.place=tmp2;
                  yyval.type = FUNCTION; 
                 }
               else 
                {
                 yyval.type=PROCEDURE;     
                 yyval.place = tmp;
                }
              }
break;
case 77:
#line 257 "fpc.y"
{ yyval.type=IDENTIFICATEUR; if( (yyval.place=FindIdent(IdentHead,yylval.nom,NBFUNCTION)) < 0 ) error(nbligne,"Undeclared Id ",yylval.nom); }
break;
case 78:
#line 258 "fpc.y"
{ yyval.type=yyvsp[0].type;  
                    tmp=InsertIdent(IdentHead,yylval.nom,yyvsp[0].type,yyvsp[0].type==CONSTANTE_ENTIERE?TYPE_ENTIER:TYPE_REAL,NBFUNCTION); 
                    if( tmp >-1 )                         
                       if(yyvsp[0].type == CONSTANTE_ENTIERE )
                          IdentHead[tmp].eval = yyvsp[0].eval;
                       else
                          IdentHead[tmp].fval = yyvsp[0].fval;
                    yyval.place=tmp;
                  }
break;
case 79:
#line 267 "fpc.y"
{ yyval.type=yyvsp[-1].type; yyval.place=yyvsp[-1].place; }
break;
case 80:
#line 273 "fpc.y"
{ 
                         tmp=FindFunction(FctHead,yylval.nom);
                         if( tmp>=0 ) error(nbligne,"Duplicate Procedure Name: ",yylval.nom);
                         tmp=InsertFunction(FctHead,yylval.nom,NBSYMBOL);
                         FctHead[NBFUNCTION].ret=0;
                         NBFUNCTION++;
                       }
break;
case 81:
#line 283 "fpc.y"
{ /* Now Build The Argument Stack In INVERSE ORDER */
             for(tmp=FctHead[NBFUNCTION].nbarg-1; tmp>=0 ; tmp-- )
                 BuildPseudo(POPARG,-1,-1,FctHead[NBFUNCTION].arg[tmp]);
           }
break;
case 82:
#line 290 "fpc.y"
{ BuildPseudo(POPJUMP,-1,-1,-1); }
break;
case 83:
#line 295 "fpc.y"
{
                 tmp=FindFunction(FctHead,yylval.nom);
                 if( tmp>=0 )
                    error(nbligne,"Duplicate Procedure Name: ",yylval.nom);                         tmp=InsertFunction(FctHead,yylval.nom,NBSYMBOL);
                 FctHead[NBFUNCTION].ret=1;
                 NBFUNCTION++;
                }
break;
case 84:
#line 305 "fpc.y"
{ /* Now Build The Argument Stack In INVERSE ORDER */
             for(tmp=FctHead[NBFUNCTION].nbarg-1; tmp>=0 ; tmp-- )
                 BuildPseudo(POPARG,-1,-1,FctHead[NBFUNCTION].arg[tmp]);
           }
break;
case 85:
#line 313 "fpc.y"
{ /* Assuming No Fonction return: It's For Stack underflow */
                               BuildPseudo(POPJUMP,-1,-1,-1);
                             }
break;
case 90:
#line 326 "fpc.y"
{ Symtab[0].Res=NBSYMBOL; }
break;
#line 1337 "y_tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("yydebug: after reduction, shifting from state 0 to\
 state %d\n", YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("yydebug: state %d, reading %d (%s)\n",
                        YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("yydebug: after reduction, shifting from state %d \
to state %d\n", *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
