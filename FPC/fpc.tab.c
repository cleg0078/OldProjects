
/*  A Bison parser, made from fpc.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	OR	258
#define	AND	259
#define	NOT	260
#define	AFFECTATION	261
#define	INFERIEUR	262
#define	SUPERIEUR	263
#define	INFERIEUR_EGAL	264
#define	SUPERIEUR_EGAL	265
#define	EGALITE	266
#define	DIFFERENCE	267
#define	PLUS	268
#define	MOINS	269
#define	UNARY_PLUS	270
#define	UNARY_MOINS	271
#define	MULTIPLIER	272
#define	DIVISER	273
#define	MODULO	274
#define	TYPE_BOOL	275
#define	TYPE_ENTIER	276
#define	TYPE_REAL	277
#define	PBEGIN	278
#define	PEND	279
#define	PARENTHESE_OUVERTE	280
#define	PARENTHESE_FERMEE	281
#define	FIN_INSTRUCTION	282
#define	VIRGULE	283
#define	DOUBLE_POINT	284
#define	IF	285
#define	THEN	286
#define	ELSE	287
#define	GOTO	288
#define	WHILE	289
#define	PROGRAM	290
#define	FUNCTION	291
#define	PROCEDURE	292
#define	VAR	293
#define	CONSTANTE	294
#define	IDENTIFICATEUR	295
#define	WRITE	296
#define	WRITELN	297
#define	JUMP_EQ	298
#define	COS	299
#define	SIN	300
#define	LN	301
#define	EXP	302
#define	SQRT	303
#define	ABS	304
#define	POINT	305
#define	PUSHADDR	306
#define	POPJUMP	307
#define	PUSHARG	308
#define	POPARG	309
#define	RETURN	310
#define	UNARY	311

#line 1 "fpc.y"

#include<stdio.h>
#include<stdlib.h>
#include"string.h"
#include"listid.h"
#include"fpcstr.h"
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

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		194
#define	YYFLAG		-32768
#define	YYNTBASE	57

#define YYTRANSLATE(x) ((unsigned)(x) <= 311 ? yytranslate[x] : 93)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     5,    10,    11,    16,    17,    22,    24,
    28,    29,    32,    35,    36,    40,    41,    45,    46,    50,
    54,    55,    58,    61,    63,    67,    68,    71,    74,    75,
    78,    79,    85,    87,    89,    90,    95,    96,    97,   103,
   105,   107,   111,   116,   118,   123,   125,   130,   132,   136,
   140,   142,   146,   150,   154,   158,   162,   166,   170,   174,
   178,   182,   186,   190,   194,   198,   201,   204,   207,   212,
   217,   222,   227,   232,   237,   239,   244,   246,   248,   252,
   253,   254,   269,   270,   271,   286,   287,   290,   293,   296,
   297,   305,   308
};

static const short yyrhs[] = {    -1,
    57,    58,     0,     0,    21,    59,    62,    27,     0,     0,
    22,    60,    62,    27,     0,     0,    20,    61,    62,    27,
     0,    40,     0,    62,    28,    40,     0,     0,    63,    64,
     0,    63,     1,     0,     0,    21,    65,    40,     0,     0,
    22,    66,    40,     0,     0,    20,    67,    40,     0,    64,
    28,    64,     0,     0,    68,    69,     0,    68,     1,     0,
    81,     0,    69,    28,    81,     0,     0,    70,    73,     0,
    70,     1,     0,     0,    31,    73,     0,     0,    31,    73,
    32,    72,    73,     0,    27,     0,    80,     0,     0,    30,
    81,    74,    71,     0,     0,     0,    34,    75,    81,    76,
    73,     0,    78,     0,    77,     0,    23,    70,    24,     0,
    40,    25,    68,    26,     0,    55,     0,    55,    25,    81,
    26,     0,    42,     0,    41,    25,    79,    26,     0,    81,
     0,    79,    28,    81,     0,    82,     6,    81,     0,    82,
     0,    25,    81,    26,     0,    81,    13,    81,     0,    81,
    14,    81,     0,    81,    17,    81,     0,    81,    18,    81,
     0,    81,    19,    81,     0,    81,    12,    81,     0,    81,
     3,    81,     0,    81,     4,    81,     0,    81,     7,    81,
     0,    81,     8,    81,     0,    81,     9,    81,     0,    81,
    10,    81,     0,    81,    11,    81,     0,    14,    81,     0,
    13,    81,     0,     5,    81,     0,    44,    25,    81,    26,
     0,    45,    25,    81,    26,     0,    46,    25,    81,    26,
     0,    48,    25,    81,    26,     0,    47,    25,    81,    26,
     0,    49,    25,    81,    26,     0,    82,     0,    40,    25,
    68,    26,     0,    40,     0,    39,     0,    25,    82,    26,
     0,     0,     0,    37,    40,    84,    25,    63,    26,    85,
    27,    38,    57,    23,    70,    24,    27,     0,     0,     0,
    36,    40,    87,    25,    63,    26,    88,    27,    38,    57,
    23,    70,    24,    27,     0,     0,    89,    83,     0,    89,
    86,     0,    89,     1,     0,     0,    38,    57,    89,    23,
    91,    70,    24,     0,    90,    50,     0,    35,    40,    27,
    90,    50,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    54,    55,    60,    61,    62,    62,    63,    63,    66,    67,
    72,    73,    74,    77,    78,    84,    84,    89,    89,    94,
    97,    98,    99,   102,   104,   107,   108,   109,   112,   113,
   114,   117,   120,   122,   123,   124,   138,   138,   139,   144,
   145,   146,   147,   156,   160,   168,   171,   176,   181,   189,
   205,   208,   210,   211,   212,   213,   214,   215,   216,   217,
   218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
   228,   229,   230,   231,   232,   233,   258,   259,   268,   272,
   282,   289,   294,   304,   311,   320,   321,   322,   323,   326,
   328,   331,   333
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","OR","AND",
"NOT","AFFECTATION","INFERIEUR","SUPERIEUR","INFERIEUR_EGAL","SUPERIEUR_EGAL",
"EGALITE","DIFFERENCE","PLUS","MOINS","UNARY_PLUS","UNARY_MOINS","MULTIPLIER",
"DIVISER","MODULO","TYPE_BOOL","TYPE_ENTIER","TYPE_REAL","PBEGIN","PEND","PARENTHESE_OUVERTE",
"PARENTHESE_FERMEE","FIN_INSTRUCTION","VIRGULE","DOUBLE_POINT","IF","THEN","ELSE",
"GOTO","WHILE","PROGRAM","FUNCTION","PROCEDURE","VAR","CONSTANTE","IDENTIFICATEUR",
"WRITE","WRITELN","JUMP_EQ","COS","SIN","LN","EXP","SQRT","ABS","POINT","PUSHADDR",
"POPJUMP","PUSHARG","POPARG","RETURN","UNARY","start_declarations","declarations",
"@1","@2","@3","declarations_var","start_input_parameters","input_parameters",
"@4","@5","@6","start_getparameters","getparameters","start_instructions","then_else",
"@7","instructions","@8","@9","@10","writeln","write","write_expr","expressions_arith",
"expressions_logiq","terme","procedure","@11","@12","function","@13","@14","start_proc_fct",
"prog_bloc","@15","program", NULL
};
#endif

static const short yyr1[] = {     0,
    57,    57,    59,    58,    60,    58,    61,    58,    62,    62,
    63,    63,    63,    65,    64,    66,    64,    67,    64,    64,
    68,    68,    68,    69,    69,    70,    70,    70,    71,    71,
    72,    71,    73,    73,    74,    73,    75,    76,    73,    73,
    73,    73,    73,    73,    73,    77,    78,    79,    79,    80,
    80,    81,    81,    81,    81,    81,    81,    81,    81,    81,
    81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
    81,    81,    81,    81,    81,    81,    82,    82,    82,    84,
    85,    83,    87,    88,    86,    89,    89,    89,    89,    91,
    90,    92,    92
};

static const short yyr2[] = {     0,
     0,     2,     0,     4,     0,     4,     0,     4,     1,     3,
     0,     2,     2,     0,     3,     0,     3,     0,     3,     3,
     0,     2,     2,     1,     3,     0,     2,     2,     0,     2,
     0,     5,     1,     1,     0,     4,     0,     0,     5,     1,
     1,     3,     4,     1,     4,     1,     4,     1,     3,     3,
     1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     2,     2,     2,     4,     4,
     4,     4,     4,     4,     1,     4,     1,     1,     3,     0,
     0,    14,     0,     0,    14,     0,     2,     2,     2,     0,
     7,     2,     5
};

static const short yydefact[] = {     0,
     0,     1,     0,     0,    86,    92,     0,     7,     3,     5,
     2,     0,     0,     0,     0,     0,    89,    90,     0,     0,
    87,    88,    93,     9,     0,     0,     0,    26,    83,    80,
     8,     0,     4,     6,     0,     0,     0,    10,    28,    26,
    91,     0,    33,     0,    37,    78,    77,     0,    46,    44,
    27,    41,    40,    34,    51,    11,    11,     0,    77,     0,
     0,     0,     0,     0,    77,     0,     0,     0,     0,     0,
     0,    35,    75,     0,    21,     0,     0,     0,     0,     0,
    42,    79,    68,    67,    66,     0,    75,    21,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    29,    38,     0,
     0,    48,     0,    50,    13,    18,    14,    16,    84,    12,
    81,    52,     0,     0,     0,     0,     0,     0,     0,    59,
    60,    61,    62,    63,    64,    65,    58,    53,    54,    55,
    56,    57,     0,    36,     0,    23,    43,    22,    24,    47,
     0,    45,     0,     0,     0,     0,     0,     0,    76,    69,
    70,    71,    73,    72,    74,    30,    39,     0,    49,    19,
    15,    17,     0,    20,     0,    31,    25,     1,     1,     0,
     0,     0,    32,    26,    26,     0,     0,     0,     0,    85,
    82,     0,     0,     0
};

static const short yydefgoto[] = {     5,
    11,    15,    16,    14,    25,    79,   120,   154,   155,   153,
   110,   148,    35,   144,   180,    51,   108,    74,   145,    52,
    53,   111,    54,   149,    73,    21,    37,   158,    22,    36,
   156,    12,     3,    28,   192
};

static const short yypact[] = {   -25,
   -26,-32768,   -11,     6,    15,-32768,    37,-32768,-32768,-32768,
-32768,    11,    28,    53,    53,    53,-32768,-32768,    61,    63,
-32768,-32768,-32768,-32768,    24,    72,    78,-32768,-32768,-32768,
-32768,    67,-32768,-32768,    49,    84,    91,-32768,-32768,-32768,
-32768,   -24,-32768,   233,-32768,-32768,    95,    97,-32768,   100,
-32768,-32768,-32768,-32768,   117,-32768,-32768,    94,-32768,   101,
   233,   233,   233,   233,   104,   105,   112,   113,   114,   115,
   116,   252,-32768,   233,-32768,   233,   233,   233,    23,    76,
-32768,-32768,-32768,-32768,-32768,   300,   101,-32768,   233,   233,
   233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
   233,   233,   233,   233,   233,   233,   233,   111,   252,   188,
    66,   252,   320,   252,-32768,-32768,-32768,-32768,-32768,   120,
-32768,-32768,   204,   340,   360,   380,   400,   420,   440,   453,
   453,    20,    20,    20,-32768,-32768,-32768,   453,   453,    10,
    10,    10,   260,-32768,   260,-32768,-32768,   122,   252,-32768,
   233,-32768,    86,   103,   119,   129,    60,   134,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   131,-32768,   233,   252,-32768,
-32768,-32768,   106,   120,   124,-32768,   252,-32768,-32768,   260,
   -17,    64,-32768,-32768,-32768,   130,   157,   138,   139,-32768,
-32768,   167,   168,-32768
};

static const short yypgoto[] = {   -67,
-32768,-32768,-32768,-32768,    98,   118,    16,-32768,-32768,-32768,
    88,-32768,   -38,-32768,-32768,  -134,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   -36,   -35,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,   170,-32768,-32768
};


#define	YYLAST		472


static const short yytable[] = {    55,
    42,    58,     8,     9,    10,   184,    60,    72,   166,     1,
   167,    17,     2,     4,    46,    59,    97,    98,    99,   100,
   101,   102,    55,   115,    83,    84,    85,    86,    87,   100,
   101,   102,     7,    18,     8,     9,    10,   109,     6,   112,
   113,   114,   116,   117,   118,   183,    19,    20,   119,    39,
    31,    32,   124,   125,   126,   127,   128,   129,   130,   131,
   132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
   142,    40,    41,    42,     2,    43,   115,    23,    44,   116,
   117,   118,    45,     8,     9,    10,   185,    46,    47,    48,
    49,   150,    24,   151,    39,   116,   117,   118,    33,    32,
    29,   121,    30,    50,    34,    32,    38,    55,    56,    55,
   181,   182,    26,    27,   169,    57,    40,    81,    42,    75,
    43,    76,    78,    44,    77,   170,    82,    45,    88,    89,
    39,   177,    46,    47,    48,    49,    90,    91,    92,    93,
    94,   143,   171,   178,    55,   186,   187,   157,    50,   168,
    55,    55,    40,   188,    42,   173,    43,    39,   172,    44,
   175,   179,   176,    45,   190,   191,   193,   194,    46,    47,
    48,    49,   174,     0,    80,   123,    13,     0,     0,    40,
   189,    42,     0,    43,    50,     0,    44,     0,   146,     0,
    45,     0,    61,     0,     0,    46,    47,    48,    49,     0,
    62,    63,     0,     0,   146,     0,     0,     0,    61,     0,
     0,    50,    64,   147,     0,     0,    62,    63,     0,     0,
     0,     0,     0,     0,     0,     0,    46,    65,    64,   159,
     0,    66,    67,    68,    69,    70,    71,    61,     0,     0,
     0,     0,    46,    65,     0,    62,    63,    66,    67,    68,
    69,    70,    71,     0,    95,    96,     0,    64,    97,    98,
    99,   100,   101,   102,   103,   104,     0,     0,   105,   106,
   107,    46,    65,     0,     0,     0,    66,    67,    68,    69,
    70,    71,    40,     0,    42,     0,    43,     0,     0,    44,
     0,     0,     0,    45,     0,     0,     0,     0,    46,    47,
    48,    49,    95,    96,     0,     0,    97,    98,    99,   100,
   101,   102,   103,   104,    50,     0,   105,   106,   107,     0,
     0,     0,    95,    96,     0,   122,    97,    98,    99,   100,
   101,   102,   103,   104,     0,     0,   105,   106,   107,     0,
     0,     0,    95,    96,     0,   152,    97,    98,    99,   100,
   101,   102,   103,   104,     0,     0,   105,   106,   107,     0,
     0,     0,    95,    96,     0,   160,    97,    98,    99,   100,
   101,   102,   103,   104,     0,     0,   105,   106,   107,     0,
     0,     0,    95,    96,     0,   161,    97,    98,    99,   100,
   101,   102,   103,   104,     0,     0,   105,   106,   107,     0,
     0,     0,    95,    96,     0,   162,    97,    98,    99,   100,
   101,   102,   103,   104,     0,     0,   105,   106,   107,     0,
     0,     0,    95,    96,     0,   163,    97,    98,    99,   100,
   101,   102,   103,   104,     0,     0,   105,   106,   107,     0,
     0,     0,    95,    96,     0,   164,    97,    98,    99,   100,
   101,   102,   103,   104,     0,     0,   105,   106,   107,    97,
    98,    99,   100,   101,   102,   165,     0,     0,     0,   105,
   106,   107
};

static const short yycheck[] = {    35,
    25,    40,    20,    21,    22,    23,    42,    44,   143,    35,
   145,     1,    38,    40,    39,    40,     7,     8,     9,    10,
    11,    12,    58,     1,    61,    62,    63,    64,    64,    10,
    11,    12,    27,    23,    20,    21,    22,    74,    50,    76,
    77,    78,    20,    21,    22,   180,    36,    37,    26,     1,
    27,    28,    89,    90,    91,    92,    93,    94,    95,    96,
    97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
   107,    23,    24,    25,    38,    27,     1,    50,    30,    20,
    21,    22,    34,    20,    21,    22,    23,    39,    40,    41,
    42,    26,    40,    28,     1,    20,    21,    22,    27,    28,
    40,    26,    40,    55,    27,    28,    40,   143,    25,   145,
   178,   179,    15,    16,   151,    25,    23,    24,    25,    25,
    27,    25,     6,    30,    25,    40,    26,    34,    25,    25,
     1,   168,    39,    40,    41,    42,    25,    25,    25,    25,
    25,    31,    40,    38,   180,   184,   185,    28,    55,    28,
   186,   187,    23,    24,    25,    27,    27,     1,    40,    30,
    27,    38,    32,    34,    27,    27,     0,     0,    39,    40,
    41,    42,   157,    -1,    57,    88,     7,    -1,    -1,    23,
    24,    25,    -1,    27,    55,    -1,    30,    -1,     1,    -1,
    34,    -1,     5,    -1,    -1,    39,    40,    41,    42,    -1,
    13,    14,    -1,    -1,     1,    -1,    -1,    -1,     5,    -1,
    -1,    55,    25,    26,    -1,    -1,    13,    14,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    39,    40,    25,    26,
    -1,    44,    45,    46,    47,    48,    49,     5,    -1,    -1,
    -1,    -1,    39,    40,    -1,    13,    14,    44,    45,    46,
    47,    48,    49,    -1,     3,     4,    -1,    25,     7,     8,
     9,    10,    11,    12,    13,    14,    -1,    -1,    17,    18,
    19,    39,    40,    -1,    -1,    -1,    44,    45,    46,    47,
    48,    49,    23,    -1,    25,    -1,    27,    -1,    -1,    30,
    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    39,    40,
    41,    42,     3,     4,    -1,    -1,     7,     8,     9,    10,
    11,    12,    13,    14,    55,    -1,    17,    18,    19,    -1,
    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,    10,
    11,    12,    13,    14,    -1,    -1,    17,    18,    19,    -1,
    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,    10,
    11,    12,    13,    14,    -1,    -1,    17,    18,    19,    -1,
    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,    10,
    11,    12,    13,    14,    -1,    -1,    17,    18,    19,    -1,
    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,    10,
    11,    12,    13,    14,    -1,    -1,    17,    18,    19,    -1,
    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,    10,
    11,    12,    13,    14,    -1,    -1,    17,    18,    19,    -1,
    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,    10,
    11,    12,    13,    14,    -1,    -1,    17,    18,    19,    -1,
    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,    10,
    11,    12,    13,    14,    -1,    -1,    17,    18,    19,     7,
     8,     9,    10,    11,    12,    26,    -1,    -1,    -1,    17,
    18,    19
};
#define YYPURE 1

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 3:
#line 61 "fpc.y"
{ actoper_type=TYPE_ENTIER; ;
    break;}
case 5:
#line 62 "fpc.y"
{ actoper_type=TYPE_REAL; ;
    break;}
case 7:
#line 63 "fpc.y"
{ actoper_type=TYPE_BOOL; ;
    break;}
case 9:
#line 66 "fpc.y"
{ if( InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION) < 0 ) error(nbligne,"Duplicate Id ",yylval.nom); ;
    break;}
case 10:
#line 68 "fpc.y"
{ if( InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION) < 0 )  error(nbligne,"Duplicate Id ",yylval.nom); ;
    break;}
case 14:
#line 78 "fpc.y"
{ actoper_type=TYPE_ENTIER; ;
    break;}
case 15:
#line 78 "fpc.y"
{
                                                                      if( (tmp=InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION)) < 0 ) 
                                                                          error(nbligne,"Duplicate Id ",yylval.nom);  
                                                                       FctHead[NBFUNCTION].arg[FctHead[NBFUNCTION].nbarg++]=tmp;
             
                                                                     ;
    break;}
case 16:
#line 84 "fpc.y"
{ actoper_type=TYPE_REAL; ;
    break;}
case 17:
#line 84 "fpc.y"
{ 
                                                                   if( (tmp=InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION)) < 0 ) 
	                                                               error(nbligne,"Duplicate Id ",yylval.nom); 
  	                                                            FctHead[NBFUNCTION].arg[FctHead[NBFUNCTION].nbarg++]=tmp;
                                                                  ;
    break;}
case 18:
#line 89 "fpc.y"
{ actoper_type=TYPE_BOOL; ;
    break;}
case 19:
#line 89 "fpc.y"
{
                                                                  if( (tmp=InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION)) < 0 )
                                                                     error(nbligne,"Duplicate Id ",yylval.nom);
                                                                   FctHead[NBFUNCTION].arg[FctHead[NBFUNCTION].nbarg++]=tmp;
                                                                  ;
    break;}
case 24:
#line 103 "fpc.y"
{ BuildPseudo(PUSHARG,-1,-1,yyvsp[0].place); ;
    break;}
case 25:
#line 104 "fpc.y"
{ BuildPseudo(PUSHARG,-1,-1,yyvsp[0].place); ;
    break;}
case 30:
#line 113 "fpc.y"
{ tmp2=0; ;
    break;}
case 31:
#line 115 "fpc.y"
{ tmp2=1; BuildPseudo(GOTO,-1,-1,-1); Push(NBSYMBOL);
                    ;
    break;}
case 32:
#line 117 "fpc.y"
{ Push(NBSYMBOL); ;
    break;}
case 35:
#line 123 "fpc.y"
{ Push(NBSYMBOL); BuildPseudo(JUMP_EQ,yyvsp[0].place,-1,-1); ;
    break;}
case 36:
#line 124 "fpc.y"
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
                         ;
    break;}
case 37:
#line 138 "fpc.y"
{ Push(NBSYMBOL); ;
    break;}
case 38:
#line 138 "fpc.y"
{ Push(NBSYMBOL); BuildPseudo(JUMP_EQ,yyvsp[-1].place,-1,-1); ;
    break;}
case 39:
#line 139 "fpc.y"
{ int res=Pop();  
                              Symtab[res].Res=NBSYMBOL+1;
                              Symtab[res].Oper1 = Symtab[res-1].Res;
                              BuildPseudo(GOTO,-1,-1,Pop());
                            ;
    break;}
case 43:
#line 148 "fpc.y"
{
                tmp=FindFunction(FctHead,yyvsp[-3].nom);
                if( tmp < 0 )
                   error(nbligne,"Undeclared Function:",yyvsp[-3].nom);

                BuildPseudo(PUSHADDR,-1,-1,NBSYMBOL+2);
                BuildPseudo(GOTO,-1,-1,FctHead[tmp].addr);
               ;
    break;}
case 44:
#line 156 "fpc.y"
{ if( FctHead[NBFUNCTION-1].ret != 0 )
                           error(nbligne,"Missing Return Value",NULL);
                        BuildPseudo(POPJUMP,-1,-1,-1); 
                      ;
    break;}
case 45:
#line 161 "fpc.y"
{
                        if( FctHead[NBFUNCTION-1].ret == 0 )
                           error(nbligne,"Procedure Can't Use A Return Value",NULL);
                        BuildPseudo(PUSHARG,-1,-1,yyvsp[-1].place);
                       ;
    break;}
case 46:
#line 168 "fpc.y"
{ BuildPseudo(WRITELN,-1,-1,-1); ;
    break;}
case 48:
#line 176 "fpc.y"
{ 
                                  if( yyvsp[0].type==PROCEDURE )
			             error(nbligne,"Invalid Procedure Affectation",NULL);
                                  BuildPseudo(WRITE,yyvsp[0].place,-1,-1); 
                                ;
    break;}
case 49:
#line 182 "fpc.y"
{
                                 if( yyvsp[0].type == PROCEDURE ) 
                                    error(nbligne,"Invalide Procedure Affectation",NULL);
                                 BuildPseudo(WRITE,yyvsp[0].place,-1,-1);
                                ;
    break;}
case 50:
#line 190 "fpc.y"
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
                                                  ;
    break;}
case 51:
#line 205 "fpc.y"
{ yyval.type = yyvsp[0].type; yyval.place=yyvsp[0].place; ;
    break;}
case 52:
#line 209 "fpc.y"
{ yyval.place=yyvsp[-1].place; ;
    break;}
case 53:
#line 210 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); ;
    break;}
case 54:
#line 211 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); ;
    break;}
case 55:
#line 212 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); ;
    break;}
case 56:
#line 213 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); ;
    break;}
case 57:
#line 214 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); ;
    break;}
case 58:
#line 215 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); ;
    break;}
case 59:
#line 216 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); ;
    break;}
case 60:
#line 217 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); ;
    break;}
case 61:
#line 218 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); ;
    break;}
case 62:
#line 219 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); ;
    break;}
case 63:
#line 220 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); ;
    break;}
case 64:
#line 221 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); ;
    break;}
case 65:
#line 222 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); ;
    break;}
case 66:
#line 223 "fpc.y"
{ yyval.place=BuildUnaryOper(UNARY_MOINS,yyvsp[0].place); ;
    break;}
case 67:
#line 224 "fpc.y"
{ yyval.place=BuildUnaryOper(UNARY_PLUS,yyvsp[0].place); ;
    break;}
case 68:
#line 225 "fpc.y"
{ yyval.place=BuildUnaryOper(NOT,yyvsp[0].place); ;
    break;}
case 69:
#line 226 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); ;
    break;}
case 70:
#line 227 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); ;
    break;}
case 71:
#line 228 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); ;
    break;}
case 72:
#line 229 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); ;
    break;}
case 73:
#line 230 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); ;
    break;}
case 74:
#line 231 "fpc.y"
{ yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); ;
    break;}
case 75:
#line 232 "fpc.y"
{ yyval.type = yyvsp[0].type; yyval.place=yyvsp[0].place;;
    break;}
case 76:
#line 234 "fpc.y"
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
              ;
    break;}
case 77:
#line 258 "fpc.y"
{ yyval.type=IDENTIFICATEUR; if( (yyval.place=FindIdent(IdentHead,yylval.nom,NBFUNCTION)) < 0 ) error(nbligne,"Undeclared Id ",yylval.nom); ;
    break;}
case 78:
#line 259 "fpc.y"
{ yyval.type=yyvsp[0].type;  
                    tmp=InsertIdent(IdentHead,yylval.nom,yyvsp[0].type,yyvsp[0].type==CONSTANTE_ENTIERE?TYPE_ENTIER:TYPE_REAL,NBFUNCTION); 
                    if( tmp >-1 )                         
                       if(yyvsp[0].type == CONSTANTE_ENTIERE )
                          IdentHead[tmp].eval = yyvsp[0].eval;
                       else
                          IdentHead[tmp].fval = yyvsp[0].fval;
                    yyval.place=tmp;
                  ;
    break;}
case 79:
#line 268 "fpc.y"
{ yyval.type=yyvsp[-1].type; yyval.place=yyvsp[-1].place; ;
    break;}
case 80:
#line 274 "fpc.y"
{ 
                         tmp=FindFunction(FctHead,yylval.nom);
                         if( tmp>=0 ) error(nbligne,"Duplicate Procedure Name: ",yylval.nom);
                         tmp=InsertFunction(FctHead,yylval.nom,NBSYMBOL);
                         FctHead[NBFUNCTION].ret=0;
                         NBFUNCTION++;
                       ;
    break;}
case 81:
#line 284 "fpc.y"
{ /* Now Build The Argument Stack In INVERSE ORDER */
             for(tmp=FctHead[NBFUNCTION].nbarg-1; tmp>=0 ; tmp-- )
                 BuildPseudo(POPARG,-1,-1,FctHead[NBFUNCTION].arg[tmp]);
           ;
    break;}
case 82:
#line 291 "fpc.y"
{ BuildPseudo(POPJUMP,-1,-1,-1); ;
    break;}
case 83:
#line 296 "fpc.y"
{
                 tmp=FindFunction(FctHead,yylval.nom);
                 if( tmp>=0 )
                    error(nbligne,"Duplicate Procedure Name: ",yylval.nom);                         tmp=InsertFunction(FctHead,yylval.nom,NBSYMBOL);
                 FctHead[NBFUNCTION].ret=1;
                 NBFUNCTION++;
                ;
    break;}
case 84:
#line 306 "fpc.y"
{ /* Now Build The Argument Stack In INVERSE ORDER */
             for(tmp=FctHead[NBFUNCTION].nbarg-1; tmp>=0 ; tmp-- )
                 BuildPseudo(POPARG,-1,-1,FctHead[NBFUNCTION].arg[tmp]);
           ;
    break;}
case 85:
#line 314 "fpc.y"
{ /* Assuming No Fonction return: It's For Stack underflow */
                               BuildPseudo(POPJUMP,-1,-1,-1);
                             ;
    break;}
case 90:
#line 327 "fpc.y"
{ Symtab[0].Res=NBSYMBOL; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 341 "fpc.y"


#include"lex.yy.c"
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

