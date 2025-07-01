/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
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


#line 104 "fpc.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "fpc.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_OR = 3,                         /* OR  */
  YYSYMBOL_AND = 4,                        /* AND  */
  YYSYMBOL_NOT = 5,                        /* NOT  */
  YYSYMBOL_AFFECTATION = 6,                /* AFFECTATION  */
  YYSYMBOL_INFERIEUR = 7,                  /* INFERIEUR  */
  YYSYMBOL_SUPERIEUR = 8,                  /* SUPERIEUR  */
  YYSYMBOL_INFERIEUR_EGAL = 9,             /* INFERIEUR_EGAL  */
  YYSYMBOL_SUPERIEUR_EGAL = 10,            /* SUPERIEUR_EGAL  */
  YYSYMBOL_EGALITE = 11,                   /* EGALITE  */
  YYSYMBOL_DIFFERENCE = 12,                /* DIFFERENCE  */
  YYSYMBOL_PLUS = 13,                      /* PLUS  */
  YYSYMBOL_MOINS = 14,                     /* MOINS  */
  YYSYMBOL_UNARY_PLUS = 15,                /* UNARY_PLUS  */
  YYSYMBOL_UNARY_MOINS = 16,               /* UNARY_MOINS  */
  YYSYMBOL_MULTIPLIER = 17,                /* MULTIPLIER  */
  YYSYMBOL_DIVISER = 18,                   /* DIVISER  */
  YYSYMBOL_MODULO = 19,                    /* MODULO  */
  YYSYMBOL_TYPE_BOOL = 20,                 /* TYPE_BOOL  */
  YYSYMBOL_TYPE_ENTIER = 21,               /* TYPE_ENTIER  */
  YYSYMBOL_TYPE_REAL = 22,                 /* TYPE_REAL  */
  YYSYMBOL_PBEGIN = 23,                    /* PBEGIN  */
  YYSYMBOL_PEND = 24,                      /* PEND  */
  YYSYMBOL_PARENTHESE_OUVERTE = 25,        /* PARENTHESE_OUVERTE  */
  YYSYMBOL_PARENTHESE_FERMEE = 26,         /* PARENTHESE_FERMEE  */
  YYSYMBOL_FIN_INSTRUCTION = 27,           /* FIN_INSTRUCTION  */
  YYSYMBOL_VIRGULE = 28,                   /* VIRGULE  */
  YYSYMBOL_DOUBLE_POINT = 29,              /* DOUBLE_POINT  */
  YYSYMBOL_IF = 30,                        /* IF  */
  YYSYMBOL_THEN = 31,                      /* THEN  */
  YYSYMBOL_ELSE = 32,                      /* ELSE  */
  YYSYMBOL_GOTO = 33,                      /* GOTO  */
  YYSYMBOL_WHILE = 34,                     /* WHILE  */
  YYSYMBOL_PROGRAM = 35,                   /* PROGRAM  */
  YYSYMBOL_FUNCTION = 36,                  /* FUNCTION  */
  YYSYMBOL_PROCEDURE = 37,                 /* PROCEDURE  */
  YYSYMBOL_VAR = 38,                       /* VAR  */
  YYSYMBOL_CONSTANTE = 39,                 /* CONSTANTE  */
  YYSYMBOL_IDENTIFICATEUR = 40,            /* IDENTIFICATEUR  */
  YYSYMBOL_WRITE = 41,                     /* WRITE  */
  YYSYMBOL_WRITELN = 42,                   /* WRITELN  */
  YYSYMBOL_JUMP_EQ = 43,                   /* JUMP_EQ  */
  YYSYMBOL_COS = 44,                       /* COS  */
  YYSYMBOL_SIN = 45,                       /* SIN  */
  YYSYMBOL_LN = 46,                        /* LN  */
  YYSYMBOL_EXP = 47,                       /* EXP  */
  YYSYMBOL_SQRT = 48,                      /* SQRT  */
  YYSYMBOL_ABS = 49,                       /* ABS  */
  YYSYMBOL_POINT = 50,                     /* POINT  */
  YYSYMBOL_PUSHADDR = 51,                  /* PUSHADDR  */
  YYSYMBOL_POPJUMP = 52,                   /* POPJUMP  */
  YYSYMBOL_PUSHARG = 53,                   /* PUSHARG  */
  YYSYMBOL_POPARG = 54,                    /* POPARG  */
  YYSYMBOL_RETURN = 55,                    /* RETURN  */
  YYSYMBOL_UNARY = 56,                     /* UNARY  */
  YYSYMBOL_YYACCEPT = 57,                  /* $accept  */
  YYSYMBOL_start_declarations = 58,        /* start_declarations  */
  YYSYMBOL_declarations = 59,              /* declarations  */
  YYSYMBOL_60_1 = 60,                      /* $@1  */
  YYSYMBOL_61_2 = 61,                      /* $@2  */
  YYSYMBOL_62_3 = 62,                      /* $@3  */
  YYSYMBOL_declarations_var = 63,          /* declarations_var  */
  YYSYMBOL_start_input_parameters = 64,    /* start_input_parameters  */
  YYSYMBOL_input_parameters = 65,          /* input_parameters  */
  YYSYMBOL_66_4 = 66,                      /* $@4  */
  YYSYMBOL_67_5 = 67,                      /* $@5  */
  YYSYMBOL_68_6 = 68,                      /* $@6  */
  YYSYMBOL_start_getparameters = 69,       /* start_getparameters  */
  YYSYMBOL_getparameters = 70,             /* getparameters  */
  YYSYMBOL_start_instructions = 71,        /* start_instructions  */
  YYSYMBOL_then_else = 72,                 /* then_else  */
  YYSYMBOL_73_7 = 73,                      /* $@7  */
  YYSYMBOL_instructions = 74,              /* instructions  */
  YYSYMBOL_75_8 = 75,                      /* $@8  */
  YYSYMBOL_76_9 = 76,                      /* @9  */
  YYSYMBOL_77_10 = 77,                     /* $@10  */
  YYSYMBOL_writeln = 78,                   /* writeln  */
  YYSYMBOL_write = 79,                     /* write  */
  YYSYMBOL_write_expr = 80,                /* write_expr  */
  YYSYMBOL_expressions_arith = 81,         /* expressions_arith  */
  YYSYMBOL_expressions_logiq = 82,         /* expressions_logiq  */
  YYSYMBOL_terme = 83,                     /* terme  */
  YYSYMBOL_procedure = 84,                 /* procedure  */
  YYSYMBOL_85_11 = 85,                     /* $@11  */
  YYSYMBOL_86_12 = 86,                     /* $@12  */
  YYSYMBOL_function = 87,                  /* function  */
  YYSYMBOL_88_13 = 88,                     /* $@13  */
  YYSYMBOL_89_14 = 89,                     /* $@14  */
  YYSYMBOL_start_proc_fct = 90,            /* start_proc_fct  */
  YYSYMBOL_prog_bloc = 91,                 /* prog_bloc  */
  YYSYMBOL_92_15 = 92,                     /* $@15  */
  YYSYMBOL_program = 93                    /* program  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   472

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  37
/* YYNRULES -- Number of rules.  */
#define YYNRULES  94
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  194

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   311


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    53,    53,    54,    60,    60,    61,    61,    62,    62,
      65,    66,    71,    72,    73,    77,    77,    83,    83,    88,
      88,    93,    96,    97,    98,   102,   103,   106,   107,   108,
     111,   112,   114,   113,   120,   121,   122,   122,   137,   137,
     137,   143,   144,   145,   146,   155,   159,   167,   170,   175,
     180,   189,   204,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   257,   258,
     267,   273,   283,   272,   295,   305,   294,   319,   320,   321,
     322,   326,   326,   331,   333
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "OR", "AND", "NOT",
  "AFFECTATION", "INFERIEUR", "SUPERIEUR", "INFERIEUR_EGAL",
  "SUPERIEUR_EGAL", "EGALITE", "DIFFERENCE", "PLUS", "MOINS", "UNARY_PLUS",
  "UNARY_MOINS", "MULTIPLIER", "DIVISER", "MODULO", "TYPE_BOOL",
  "TYPE_ENTIER", "TYPE_REAL", "PBEGIN", "PEND", "PARENTHESE_OUVERTE",
  "PARENTHESE_FERMEE", "FIN_INSTRUCTION", "VIRGULE", "DOUBLE_POINT", "IF",
  "THEN", "ELSE", "GOTO", "WHILE", "PROGRAM", "FUNCTION", "PROCEDURE",
  "VAR", "CONSTANTE", "IDENTIFICATEUR", "WRITE", "WRITELN", "JUMP_EQ",
  "COS", "SIN", "LN", "EXP", "SQRT", "ABS", "POINT", "PUSHADDR", "POPJUMP",
  "PUSHARG", "POPARG", "RETURN", "UNARY", "$accept", "start_declarations",
  "declarations", "$@1", "$@2", "$@3", "declarations_var",
  "start_input_parameters", "input_parameters", "$@4", "$@5", "$@6",
  "start_getparameters", "getparameters", "start_instructions",
  "then_else", "$@7", "instructions", "$@8", "@9", "$@10", "writeln",
  "write", "write_expr", "expressions_arith", "expressions_logiq", "terme",
  "procedure", "$@11", "$@12", "function", "$@13", "$@14",
  "start_proc_fct", "prog_bloc", "$@15", "program", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-137)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -25,   -26,  -137,   -11,    33,    48,    15,  -137,  -137,    40,
    -137,  -137,  -137,  -137,    11,    43,    61,    61,    61,  -137,
    -137,    63,    67,  -137,  -137,  -137,  -137,    24,    72,    78,
    -137,  -137,  -137,  -137,    69,  -137,  -137,    49,    91,    95,
    -137,  -137,  -137,  -137,   -24,  -137,   233,  -137,  -137,    97,
      98,  -137,   100,  -137,  -137,  -137,  -137,   120,  -137,  -137,
      94,  -137,   101,   233,   233,   233,   233,   104,   105,   112,
     113,   114,   115,   116,   252,  -137,   233,  -137,   233,   233,
     233,    23,    76,  -137,  -137,  -137,  -137,  -137,   300,   101,
    -137,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     111,   252,   188,    66,   252,   320,   252,  -137,  -137,  -137,
    -137,  -137,   122,  -137,  -137,   204,   340,   360,   380,   400,
     420,   440,   453,   453,    20,    20,    20,  -137,  -137,  -137,
     453,   453,    10,    10,    10,   260,  -137,   260,  -137,  -137,
     128,   252,  -137,   233,  -137,   103,   108,   119,   117,    60,
     134,  -137,  -137,  -137,  -137,  -137,  -137,  -137,   131,  -137,
     233,   252,  -137,  -137,  -137,   124,   122,   127,  -137,   252,
    -137,  -137,   260,   -17,    64,  -137,  -137,  -137,   130,   157,
     139,   140,  -137,  -137
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     2,     0,     0,     0,    87,    93,     1,     0,
       8,     4,     6,     3,     0,     0,     0,     0,     0,    90,
      91,     0,     0,    88,    89,    94,    10,     0,     0,     0,
      27,    84,    81,     9,     0,     5,     7,     0,     0,     0,
      11,    29,    27,    92,     0,    34,     0,    38,    79,    78,
       0,    47,    45,    28,    42,    41,    35,    52,    12,    12,
       0,    78,     0,     0,     0,     0,     0,    78,     0,     0,
       0,     0,     0,     0,    36,    76,     0,    22,     0,     0,
       0,     0,     0,    43,    80,    69,    68,    67,     0,    76,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      30,    39,     0,     0,    49,     0,    51,    14,    19,    15,
      17,    85,    13,    82,    53,     0,     0,     0,     0,     0,
       0,     0,    60,    61,    62,    63,    64,    65,    66,    59,
      54,    55,    56,    57,    58,     0,    37,     0,    24,    44,
      23,    25,    48,     0,    46,     0,     0,     0,     0,     0,
       0,    77,    70,    71,    72,    74,    73,    75,    31,    40,
       0,    50,    20,    16,    18,     0,    21,     0,    32,    26,
       2,     2,     0,     0,     0,    33,    27,    27,     0,     0,
       0,     0,    86,    83
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -137,   -69,  -137,  -137,  -137,  -137,    96,   109,    14,  -137,
    -137,  -137,    84,  -137,   -40,  -137,  -137,  -136,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,   -38,   -37,  -137,  -137,  -137,
    -137,  -137,  -137,  -137,   166,  -137,  -137
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     6,    13,    17,    18,    16,    27,    81,   122,   156,
     157,   155,   112,   150,    37,   146,   182,    53,   110,    76,
     147,    54,    55,   113,    56,   151,    75,    23,    39,   160,
      24,    38,   158,    14,     3,    30,     4
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      57,    44,    60,    10,    11,    12,   186,    62,    74,   168,
       1,   169,    19,     2,     5,    48,    61,    99,   100,   101,
     102,   103,   104,    57,   117,    85,    86,    87,    88,    89,
     102,   103,   104,     8,    20,    10,    11,    12,   111,     7,
     114,   115,   116,   118,   119,   120,   185,    21,    22,   121,
      41,    33,    34,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,    42,    43,    44,     9,    45,   117,     2,    46,
     118,   119,   120,    47,    10,    11,    12,   187,    48,    49,
      50,    51,   152,    25,   153,    41,   118,   119,   120,    35,
      34,    26,   123,    31,    52,    36,    34,    32,    57,    40,
      57,   183,   184,    28,    29,   171,    58,    42,    83,    44,
      59,    45,    77,    78,    46,    79,    80,    84,    47,    90,
      91,    41,   179,    48,    49,    50,    51,    92,    93,    94,
      95,    96,   145,   172,   175,    57,   188,   189,   173,    52,
     159,    57,    57,    42,   190,    44,   170,    45,    41,   174,
      46,   177,   180,   178,    47,   181,   192,   193,    82,    48,
      49,    50,    51,   176,   125,    15,     0,     0,     0,     0,
      42,   191,    44,     0,    45,    52,     0,    46,     0,   148,
       0,    47,     0,    63,     0,     0,    48,    49,    50,    51,
       0,    64,    65,     0,     0,   148,     0,     0,     0,    63,
       0,     0,    52,    66,   149,     0,     0,    64,    65,     0,
       0,     0,     0,     0,     0,     0,     0,    48,    67,    66,
     161,     0,    68,    69,    70,    71,    72,    73,    63,     0,
       0,     0,     0,    48,    67,     0,    64,    65,    68,    69,
      70,    71,    72,    73,     0,    97,    98,     0,    66,    99,
     100,   101,   102,   103,   104,   105,   106,     0,     0,   107,
     108,   109,    48,    67,     0,     0,     0,    68,    69,    70,
      71,    72,    73,    42,     0,    44,     0,    45,     0,     0,
      46,     0,     0,     0,    47,     0,     0,     0,     0,    48,
      49,    50,    51,    97,    98,     0,     0,    99,   100,   101,
     102,   103,   104,   105,   106,    52,     0,   107,   108,   109,
       0,     0,     0,    97,    98,     0,   124,    99,   100,   101,
     102,   103,   104,   105,   106,     0,     0,   107,   108,   109,
       0,     0,     0,    97,    98,     0,   154,    99,   100,   101,
     102,   103,   104,   105,   106,     0,     0,   107,   108,   109,
       0,     0,     0,    97,    98,     0,   162,    99,   100,   101,
     102,   103,   104,   105,   106,     0,     0,   107,   108,   109,
       0,     0,     0,    97,    98,     0,   163,    99,   100,   101,
     102,   103,   104,   105,   106,     0,     0,   107,   108,   109,
       0,     0,     0,    97,    98,     0,   164,    99,   100,   101,
     102,   103,   104,   105,   106,     0,     0,   107,   108,   109,
       0,     0,     0,    97,    98,     0,   165,    99,   100,   101,
     102,   103,   104,   105,   106,     0,     0,   107,   108,   109,
       0,     0,     0,    97,    98,     0,   166,    99,   100,   101,
     102,   103,   104,   105,   106,     0,     0,   107,   108,   109,
      99,   100,   101,   102,   103,   104,   167,     0,     0,     0,
     107,   108,   109
};

static const yytype_int16 yycheck[] =
{
      37,    25,    42,    20,    21,    22,    23,    44,    46,   145,
      35,   147,     1,    38,    40,    39,    40,     7,     8,     9,
      10,    11,    12,    60,     1,    63,    64,    65,    66,    66,
      10,    11,    12,     0,    23,    20,    21,    22,    76,    50,
      78,    79,    80,    20,    21,    22,   182,    36,    37,    26,
       1,    27,    28,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,    23,    24,    25,    27,    27,     1,    38,    30,
      20,    21,    22,    34,    20,    21,    22,    23,    39,    40,
      41,    42,    26,    50,    28,     1,    20,    21,    22,    27,
      28,    40,    26,    40,    55,    27,    28,    40,   145,    40,
     147,   180,   181,    17,    18,   153,    25,    23,    24,    25,
      25,    27,    25,    25,    30,    25,     6,    26,    34,    25,
      25,     1,   170,    39,    40,    41,    42,    25,    25,    25,
      25,    25,    31,    40,    27,   182,   186,   187,    40,    55,
      28,   188,   189,    23,    24,    25,    28,    27,     1,    40,
      30,    27,    38,    32,    34,    38,    27,    27,    59,    39,
      40,    41,    42,   159,    90,     9,    -1,    -1,    -1,    -1,
      23,    24,    25,    -1,    27,    55,    -1,    30,    -1,     1,
      -1,    34,    -1,     5,    -1,    -1,    39,    40,    41,    42,
      -1,    13,    14,    -1,    -1,     1,    -1,    -1,    -1,     5,
      -1,    -1,    55,    25,    26,    -1,    -1,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    40,    25,
      26,    -1,    44,    45,    46,    47,    48,    49,     5,    -1,
      -1,    -1,    -1,    39,    40,    -1,    13,    14,    44,    45,
      46,    47,    48,    49,    -1,     3,     4,    -1,    25,     7,
       8,     9,    10,    11,    12,    13,    14,    -1,    -1,    17,
      18,    19,    39,    40,    -1,    -1,    -1,    44,    45,    46,
      47,    48,    49,    23,    -1,    25,    -1,    27,    -1,    -1,
      30,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    39,
      40,    41,    42,     3,     4,    -1,    -1,     7,     8,     9,
      10,    11,    12,    13,    14,    55,    -1,    17,    18,    19,
      -1,    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,
      10,    11,    12,    13,    14,    -1,    -1,    17,    18,    19,
      -1,    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,
      10,    11,    12,    13,    14,    -1,    -1,    17,    18,    19,
      -1,    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,
      10,    11,    12,    13,    14,    -1,    -1,    17,    18,    19,
      -1,    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,
      10,    11,    12,    13,    14,    -1,    -1,    17,    18,    19,
      -1,    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,
      10,    11,    12,    13,    14,    -1,    -1,    17,    18,    19,
      -1,    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,
      10,    11,    12,    13,    14,    -1,    -1,    17,    18,    19,
      -1,    -1,    -1,     3,     4,    -1,    26,     7,     8,     9,
      10,    11,    12,    13,    14,    -1,    -1,    17,    18,    19,
       7,     8,     9,    10,    11,    12,    26,    -1,    -1,    -1,
      17,    18,    19
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    35,    38,    91,    93,    40,    58,    50,     0,    27,
      20,    21,    22,    59,    90,    91,    62,    60,    61,     1,
      23,    36,    37,    84,    87,    50,    40,    63,    63,    63,
      92,    40,    40,    27,    28,    27,    27,    71,    88,    85,
      40,     1,    23,    24,    25,    27,    30,    34,    39,    40,
      41,    42,    55,    74,    78,    79,    81,    83,    25,    25,
      71,    40,    83,     5,    13,    14,    25,    40,    44,    45,
      46,    47,    48,    49,    82,    83,    76,    25,    25,    25,
       6,    64,    64,    24,    26,    82,    82,    82,    82,    83,
      25,    25,    25,    25,    25,    25,    25,     3,     4,     7,
       8,     9,    10,    11,    12,    13,    14,    17,    18,    19,
      75,    82,    69,    80,    82,    82,    82,     1,    20,    21,
      22,    26,    65,    26,    26,    69,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    31,    72,    77,     1,    26,
      70,    82,    26,    28,    26,    68,    66,    67,    89,    28,
      86,    26,    26,    26,    26,    26,    26,    26,    74,    74,
      28,    82,    40,    40,    40,    27,    65,    27,    32,    82,
      38,    38,    73,    58,    58,    74,    23,    23,    71,    71,
      24,    24,    27,    27
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    58,    60,    59,    61,    59,    62,    59,
      63,    63,    64,    64,    64,    66,    65,    67,    65,    68,
      65,    65,    69,    69,    69,    70,    70,    71,    71,    71,
      72,    72,    73,    72,    74,    74,    75,    74,    76,    77,
      74,    74,    74,    74,    74,    74,    74,    78,    79,    80,
      80,    81,    81,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    83,    83,
      83,    85,    86,    84,    88,    89,    87,    90,    90,    90,
      90,    92,    91,    93,    93
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     0,     4,     0,     4,     0,     4,
       1,     3,     0,     2,     2,     0,     3,     0,     3,     0,
       3,     3,     0,     2,     2,     1,     3,     0,     2,     2,
       0,     2,     0,     5,     1,     1,     0,     4,     0,     0,
       5,     1,     1,     3,     4,     1,     4,     1,     4,     1,
       3,     3,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       4,     4,     4,     4,     4,     4,     1,     4,     1,     1,
       3,     0,     0,    14,     0,     0,    14,     0,     2,     2,
       2,     0,     7,     2,     5
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* $@1: %empty  */
#line 60 "fpc.y"
                          { actoper_type=TYPE_ENTIER; }
#line 1375 "fpc.tab.c"
    break;

  case 6: /* $@2: %empty  */
#line 61 "fpc.y"
                        { actoper_type=TYPE_REAL; }
#line 1381 "fpc.tab.c"
    break;

  case 8: /* $@3: %empty  */
#line 62 "fpc.y"
                        { actoper_type=TYPE_BOOL; }
#line 1387 "fpc.tab.c"
    break;

  case 10: /* declarations_var: IDENTIFICATEUR  */
#line 65 "fpc.y"
                                 { if( InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION) < 0 ) error(nbligne,"Duplicate Id ",yylval.nom); }
#line 1393 "fpc.tab.c"
    break;

  case 11: /* declarations_var: declarations_var VIRGULE IDENTIFICATEUR  */
#line 67 "fpc.y"
                                 { if( InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION) < 0 )  error(nbligne,"Duplicate Id ",yylval.nom); }
#line 1399 "fpc.tab.c"
    break;

  case 15: /* $@4: %empty  */
#line 77 "fpc.y"
                        { actoper_type=TYPE_ENTIER; }
#line 1405 "fpc.tab.c"
    break;

  case 16: /* input_parameters: TYPE_ENTIER $@4 IDENTIFICATEUR  */
#line 77 "fpc.y"
                                                                     {
                                                                      if( (tmp=InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION)) < 0 ) 
                                                                          error(nbligne,"Duplicate Id ",yylval.nom);  
                                                                       FctHead[NBFUNCTION].arg[FctHead[NBFUNCTION].nbarg++]=tmp;
             
                                                                     }
#line 1416 "fpc.tab.c"
    break;

  case 17: /* $@5: %empty  */
#line 83 "fpc.y"
                       { actoper_type=TYPE_REAL; }
#line 1422 "fpc.tab.c"
    break;

  case 18: /* input_parameters: TYPE_REAL $@5 IDENTIFICATEUR  */
#line 83 "fpc.y"
                                                                  { 
                                                                   if( (tmp=InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION)) < 0 ) 
	                                                               error(nbligne,"Duplicate Id ",yylval.nom); 
  	                                                            FctHead[NBFUNCTION].arg[FctHead[NBFUNCTION].nbarg++]=tmp;
                                                                  }
#line 1432 "fpc.tab.c"
    break;

  case 19: /* $@6: %empty  */
#line 88 "fpc.y"
                       { actoper_type=TYPE_BOOL; }
#line 1438 "fpc.tab.c"
    break;

  case 20: /* input_parameters: TYPE_BOOL $@6 IDENTIFICATEUR  */
#line 88 "fpc.y"
                                                                  {
                                                                  if( (tmp=InsertIdent(IdentHead,yylval.nom,IDENTIFICATEUR,actoper_type,NBFUNCTION)) < 0 )
                                                                     error(nbligne,"Duplicate Id ",yylval.nom);
                                                                   FctHead[NBFUNCTION].arg[FctHead[NBFUNCTION].nbarg++]=tmp;
                                                                  }
#line 1448 "fpc.tab.c"
    break;

  case 25: /* getparameters: expressions_logiq  */
#line 102 "fpc.y"
                           { BuildPseudo(PUSHARG,-1,-1,yyvsp[0].place); }
#line 1454 "fpc.tab.c"
    break;

  case 26: /* getparameters: getparameters VIRGULE expressions_logiq  */
#line 103 "fpc.y"
                                                 { BuildPseudo(PUSHARG,-1,-1,yyvsp[0].place); }
#line 1460 "fpc.tab.c"
    break;

  case 31: /* then_else: THEN instructions  */
#line 112 "fpc.y"
                                 { tmp2=0; }
#line 1466 "fpc.tab.c"
    break;

  case 32: /* $@7: %empty  */
#line 114 "fpc.y"
                    { tmp2=1; BuildPseudo(GOTO,-1,-1,-1); Push(NBSYMBOL);
                    }
#line 1473 "fpc.tab.c"
    break;

  case 33: /* then_else: THEN instructions ELSE $@7 instructions  */
#line 116 "fpc.y"
                            { Push(NBSYMBOL); }
#line 1479 "fpc.tab.c"
    break;

  case 36: /* $@8: %empty  */
#line 122 "fpc.y"
                                     { Push(NBSYMBOL); BuildPseudo(JUMP_EQ,yyvsp[0].place,-1,-1); }
#line 1485 "fpc.tab.c"
    break;

  case 37: /* instructions: IF expressions_logiq $@8 then_else  */
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
#line 1504 "fpc.tab.c"
    break;

  case 38: /* @9: %empty  */
#line 137 "fpc.y"
                     { Push(NBSYMBOL); }
#line 1510 "fpc.tab.c"
    break;

  case 39: /* $@10: %empty  */
#line 137 "fpc.y"
                                                           { Push(NBSYMBOL); BuildPseudo(JUMP_EQ,yyvsp[-1].place,-1,-1); }
#line 1516 "fpc.tab.c"
    break;

  case 40: /* instructions: WHILE @9 expressions_logiq $@10 instructions  */
#line 138 "fpc.y"
                            { int res=Pop();  
                              Symtab[res].Res=NBSYMBOL+1;
                              Symtab[res].Oper1 = Symtab[res-1].Res;
                              BuildPseudo(GOTO,-1,-1,Pop());
                            }
#line 1526 "fpc.tab.c"
    break;

  case 44: /* instructions: IDENTIFICATEUR PARENTHESE_OUVERTE start_getparameters PARENTHESE_FERMEE  */
#line 147 "fpc.y"
               {
                tmp=FindFunction(FctHead,yyvsp[-3].nom);
                if( tmp < 0 )
                   error(nbligne,"Undeclared Function:",yyvsp[-3].nom);

                BuildPseudo(PUSHADDR,-1,-1,NBSYMBOL+2);
                BuildPseudo(GOTO,-1,-1,FctHead[tmp].addr);
               }
#line 1539 "fpc.tab.c"
    break;

  case 45: /* instructions: RETURN  */
#line 155 "fpc.y"
                      { if( FctHead[NBFUNCTION-1].ret != 0 )
                           error(nbligne,"Missing Return Value",NULL);
                        BuildPseudo(POPJUMP,-1,-1,-1); 
                      }
#line 1548 "fpc.tab.c"
    break;

  case 46: /* instructions: RETURN PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE  */
#line 160 "fpc.y"
                      {
                        if( FctHead[NBFUNCTION-1].ret == 0 )
                           error(nbligne,"Procedure Can't Use A Return Value",NULL);
                        BuildPseudo(PUSHARG,-1,-1,yyvsp[-1].place);
                       }
#line 1558 "fpc.tab.c"
    break;

  case 47: /* writeln: WRITELN  */
#line 167 "fpc.y"
                    { BuildPseudo(WRITELN,-1,-1,-1); }
#line 1564 "fpc.tab.c"
    break;

  case 49: /* write_expr: expressions_logiq  */
#line 175 "fpc.y"
                                { 
                                  if( yyvsp[0].type==PROCEDURE )
			             error(nbligne,"Invalid Procedure Affectation",NULL);
                                  BuildPseudo(WRITE,yyvsp[0].place,-1,-1); 
                                }
#line 1574 "fpc.tab.c"
    break;

  case 50: /* write_expr: write_expr VIRGULE expressions_logiq  */
#line 181 "fpc.y"
                                {
                                 if( yyvsp[0].type == PROCEDURE ) 
                                    error(nbligne,"Invalide Procedure Affectation",NULL);
                                 BuildPseudo(WRITE,yyvsp[0].place,-1,-1);
                                }
#line 1584 "fpc.tab.c"
    break;

  case 51: /* expressions_arith: terme AFFECTATION expressions_logiq  */
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
#line 1604 "fpc.tab.c"
    break;

  case 52: /* expressions_arith: terme  */
#line 204 "fpc.y"
                    { yyval.type = yyvsp[0].type; yyval.place=yyvsp[0].place; }
#line 1610 "fpc.tab.c"
    break;

  case 53: /* expressions_logiq: PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE  */
#line 208 "fpc.y"
                                                                     { yyval.place=yyvsp[-1].place; }
#line 1616 "fpc.tab.c"
    break;

  case 54: /* expressions_logiq: expressions_logiq PLUS expressions_logiq  */
#line 209 "fpc.y"
                                                       { yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
#line 1622 "fpc.tab.c"
    break;

  case 55: /* expressions_logiq: expressions_logiq MOINS expressions_logiq  */
#line 210 "fpc.y"
                                                        { yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
#line 1628 "fpc.tab.c"
    break;

  case 56: /* expressions_logiq: expressions_logiq MULTIPLIER expressions_logiq  */
#line 211 "fpc.y"
                                                             { yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
#line 1634 "fpc.tab.c"
    break;

  case 57: /* expressions_logiq: expressions_logiq DIVISER expressions_logiq  */
#line 212 "fpc.y"
                                                          { yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
#line 1640 "fpc.tab.c"
    break;

  case 58: /* expressions_logiq: expressions_logiq MODULO expressions_logiq  */
#line 213 "fpc.y"
                                                         { yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
#line 1646 "fpc.tab.c"
    break;

  case 59: /* expressions_logiq: expressions_logiq DIFFERENCE expressions_logiq  */
#line 214 "fpc.y"
                                                             { yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
#line 1652 "fpc.tab.c"
    break;

  case 60: /* expressions_logiq: expressions_logiq OR expressions_logiq  */
#line 215 "fpc.y"
                                                     { yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
#line 1658 "fpc.tab.c"
    break;

  case 61: /* expressions_logiq: expressions_logiq AND expressions_logiq  */
#line 216 "fpc.y"
                                                      { yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
#line 1664 "fpc.tab.c"
    break;

  case 62: /* expressions_logiq: expressions_logiq INFERIEUR expressions_logiq  */
#line 217 "fpc.y"
                                                            { yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
#line 1670 "fpc.tab.c"
    break;

  case 63: /* expressions_logiq: expressions_logiq SUPERIEUR expressions_logiq  */
#line 218 "fpc.y"
                                                            { yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
#line 1676 "fpc.tab.c"
    break;

  case 64: /* expressions_logiq: expressions_logiq INFERIEUR_EGAL expressions_logiq  */
#line 219 "fpc.y"
                                                                 { yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
#line 1682 "fpc.tab.c"
    break;

  case 65: /* expressions_logiq: expressions_logiq SUPERIEUR_EGAL expressions_logiq  */
#line 220 "fpc.y"
                                                                 { yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
#line 1688 "fpc.tab.c"
    break;

  case 66: /* expressions_logiq: expressions_logiq EGALITE expressions_logiq  */
#line 221 "fpc.y"
                                                          { yyval.place=BuildOper(yyvsp[-1].type,yyvsp[-2].place,yyvsp[0].place); }
#line 1694 "fpc.tab.c"
    break;

  case 67: /* expressions_logiq: MOINS expressions_logiq  */
#line 222 "fpc.y"
                                                  { yyval.place=BuildUnaryOper(UNARY_MOINS,yyvsp[0].place); }
#line 1700 "fpc.tab.c"
    break;

  case 68: /* expressions_logiq: PLUS expressions_logiq  */
#line 223 "fpc.y"
                                                 { yyval.place=BuildUnaryOper(UNARY_PLUS,yyvsp[0].place); }
#line 1706 "fpc.tab.c"
    break;

  case 69: /* expressions_logiq: NOT expressions_logiq  */
#line 224 "fpc.y"
                                                { yyval.place=BuildUnaryOper(NOT,yyvsp[0].place); }
#line 1712 "fpc.tab.c"
    break;

  case 70: /* expressions_logiq: COS PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE  */
#line 225 "fpc.y"
                                                                         { yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); }
#line 1718 "fpc.tab.c"
    break;

  case 71: /* expressions_logiq: SIN PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE  */
#line 226 "fpc.y"
                                                                         { yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); }
#line 1724 "fpc.tab.c"
    break;

  case 72: /* expressions_logiq: LN PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE  */
#line 227 "fpc.y"
                                                                        { yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); }
#line 1730 "fpc.tab.c"
    break;

  case 73: /* expressions_logiq: SQRT PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE  */
#line 228 "fpc.y"
                                                                          { yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); }
#line 1736 "fpc.tab.c"
    break;

  case 74: /* expressions_logiq: EXP PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE  */
#line 229 "fpc.y"
                                                                         { yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); }
#line 1742 "fpc.tab.c"
    break;

  case 75: /* expressions_logiq: ABS PARENTHESE_OUVERTE expressions_logiq PARENTHESE_FERMEE  */
#line 230 "fpc.y"
                                                                         { yyval.place=BuildOper(yyvsp[-3].type,yyvsp[-1].place,-1); }
#line 1748 "fpc.tab.c"
    break;

  case 76: /* expressions_logiq: terme  */
#line 231 "fpc.y"
                     { yyval.type = yyvsp[0].type; yyval.place=yyvsp[0].place;}
#line 1754 "fpc.tab.c"
    break;

  case 77: /* expressions_logiq: IDENTIFICATEUR PARENTHESE_OUVERTE start_getparameters PARENTHESE_FERMEE  */
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
#line 1781 "fpc.tab.c"
    break;

  case 78: /* terme: IDENTIFICATEUR  */
#line 257 "fpc.y"
                      { yyval.type=IDENTIFICATEUR; if( (yyval.place=FindIdent(IdentHead,yylval.nom,NBFUNCTION)) < 0 ) error(nbligne,"Undeclared Id ",yylval.nom); }
#line 1787 "fpc.tab.c"
    break;

  case 79: /* terme: CONSTANTE  */
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
#line 1801 "fpc.tab.c"
    break;

  case 80: /* terme: PARENTHESE_OUVERTE terme PARENTHESE_FERMEE  */
#line 267 "fpc.y"
                                                   { yyval.type=yyvsp[-1].type; yyval.place=yyvsp[-1].place; }
#line 1807 "fpc.tab.c"
    break;

  case 81: /* $@11: %empty  */
#line 273 "fpc.y"
                       { 
                         tmp=FindFunction(FctHead,yylval.nom);
                         if( tmp>=0 ) error(nbligne,"Duplicate Procedure Name: ",yylval.nom);
                         tmp=InsertFunction(FctHead,yylval.nom,NBSYMBOL);
                         FctHead[NBFUNCTION].ret=0;
                         NBFUNCTION++;
                       }
#line 1819 "fpc.tab.c"
    break;

  case 82: /* $@12: %empty  */
#line 283 "fpc.y"
           { /* Now Build The Argument Stack In INVERSE ORDER */
             for(tmp=FctHead[NBFUNCTION].nbarg-1; tmp>=0 ; tmp-- )
                 BuildPseudo(POPARG,-1,-1,FctHead[NBFUNCTION].arg[tmp]);
           }
#line 1828 "fpc.tab.c"
    break;

  case 83: /* procedure: PROCEDURE IDENTIFICATEUR $@11 PARENTHESE_OUVERTE start_input_parameters PARENTHESE_FERMEE $@12 FIN_INSTRUCTION VAR start_declarations PBEGIN start_instructions PEND FIN_INSTRUCTION  */
#line 290 "fpc.y"
                             { BuildPseudo(POPJUMP,-1,-1,-1); }
#line 1834 "fpc.tab.c"
    break;

  case 84: /* $@13: %empty  */
#line 295 "fpc.y"
                {
                 tmp=FindFunction(FctHead,yylval.nom);
                 if( tmp>=0 )
                    error(nbligne,"Duplicate Procedure Name: ",yylval.nom);                         tmp=InsertFunction(FctHead,yylval.nom,NBSYMBOL);
                 FctHead[NBFUNCTION].ret=1;
                 NBFUNCTION++;
                }
#line 1846 "fpc.tab.c"
    break;

  case 85: /* $@14: %empty  */
#line 305 "fpc.y"
           { /* Now Build The Argument Stack In INVERSE ORDER */
             for(tmp=FctHead[NBFUNCTION].nbarg-1; tmp>=0 ; tmp-- )
                 BuildPseudo(POPARG,-1,-1,FctHead[NBFUNCTION].arg[tmp]);
           }
#line 1855 "fpc.tab.c"
    break;

  case 86: /* function: FUNCTION IDENTIFICATEUR $@13 PARENTHESE_OUVERTE start_input_parameters PARENTHESE_FERMEE $@14 FIN_INSTRUCTION VAR start_declarations PBEGIN start_instructions PEND FIN_INSTRUCTION  */
#line 313 "fpc.y"
                        { /* Assuming No Fonction return: It's For Stack underflow */
                               BuildPseudo(POPJUMP,-1,-1,-1);
                             }
#line 1863 "fpc.tab.c"
    break;

  case 91: /* $@15: %empty  */
#line 326 "fpc.y"
                                                     { Symtab[0].Res=NBSYMBOL; }
#line 1869 "fpc.tab.c"
    break;


#line 1873 "fpc.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 340 "fpc.y"


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

