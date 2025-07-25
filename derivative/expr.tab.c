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
#line 12 "expr.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NODE_DEFINED
#define NODE_DEFINED
typedef struct Node {
    enum { NUM, VAR, ADD, SUB, MUL, DIV, NODE_SIN, NODE_COS, NODE_TAN, NODE_EXP, NODE_LN } type;
    struct Node *left, *right;
    int value;
} Node;
#endif

Node* make_node(int type, Node* left, Node* right, int value);
Node* differentiate(Node* expr);
Node* simplify(Node* expr);
char* print_to_string(Node* e);
void print_expr(Node* expr);
void free_expr(Node* expr);

int yylex(void);
void yyerror(const char *s);

#line 96 "expr.tab.c"

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

#include "expr.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER = 3,                     /* NUMBER  */
  YYSYMBOL_VARIABLE = 4,                   /* VARIABLE  */
  YYSYMBOL_SIN = 5,                        /* SIN  */
  YYSYMBOL_COS = 6,                        /* COS  */
  YYSYMBOL_TAN = 7,                        /* TAN  */
  YYSYMBOL_EXP = 8,                        /* EXP  */
  YYSYMBOL_LN = 9,                         /* LN  */
  YYSYMBOL_10_ = 10,                       /* '+'  */
  YYSYMBOL_11_ = 11,                       /* '-'  */
  YYSYMBOL_12_ = 12,                       /* '*'  */
  YYSYMBOL_13_ = 13,                       /* '/'  */
  YYSYMBOL_UMINUS = 14,                    /* UMINUS  */
  YYSYMBOL_15_ = 15,                       /* '('  */
  YYSYMBOL_16_ = 16,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 17,                  /* $accept  */
  YYSYMBOL_input = 18,                     /* input  */
  YYSYMBOL_func = 19,                      /* func  */
  YYSYMBOL_expr = 20                       /* expr  */
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
typedef yytype_int8 yy_state_t;

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
#define YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   68

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  17
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  4
/* YYNRULES -- Number of rules.  */
#define YYNRULES  15
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  38

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   265


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
      15,    16,    12,    10,     2,    11,     2,    13,     2,     2,
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
       5,     6,     7,     8,     9,    14
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int8 yyrline[] =
{
       0,    57,    57,    68,    69,    70,    71,    72,    75,    76,
      77,    78,    79,    80,    81,    82
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
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER", "VARIABLE",
  "SIN", "COS", "TAN", "EXP", "LN", "'+'", "'-'", "'*'", "'/'", "UMINUS",
  "'('", "')'", "$accept", "input", "func", "expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-13)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      12,   -13,   -13,   -12,    -6,    -5,     7,    19,    12,    28,
     -13,    55,    12,    12,    12,    12,    12,    13,   -13,    12,
      12,    12,    12,    20,    27,    34,    41,    48,   -13,   -11,
     -11,   -13,   -13,   -13,   -13,   -13,   -13,   -13
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     8,     9,     0,     0,     0,     0,     0,     0,     0,
      10,     2,     0,     0,     0,     0,     0,     0,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    15,    11,
      12,    13,    14,     3,     6,     4,     5,     7
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -13,   -13,   -13,    -8
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     9,    10,    11
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      17,    21,    22,    12,    23,    24,    25,    26,    27,    13,
      14,    29,    30,    31,    32,     1,     2,     3,     4,     5,
       6,     7,    15,    19,    20,    21,    22,     8,    18,    28,
      19,    20,    21,    22,    16,     0,    33,    19,    20,    21,
      22,     0,     0,    34,    19,    20,    21,    22,     0,     0,
      35,    19,    20,    21,    22,     0,     0,    36,    19,    20,
      21,    22,     0,     0,    37,    19,    20,    21,    22
};

static const yytype_int8 yycheck[] =
{
       8,    12,    13,    15,    12,    13,    14,    15,    16,    15,
      15,    19,    20,    21,    22,     3,     4,     5,     6,     7,
       8,     9,    15,    10,    11,    12,    13,    15,     0,    16,
      10,    11,    12,    13,    15,    -1,    16,    10,    11,    12,
      13,    -1,    -1,    16,    10,    11,    12,    13,    -1,    -1,
      16,    10,    11,    12,    13,    -1,    -1,    16,    10,    11,
      12,    13,    -1,    -1,    16,    10,    11,    12,    13
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    15,    18,
      19,    20,    15,    15,    15,    15,    15,    20,     0,    10,
      11,    12,    13,    20,    20,    20,    20,    20,    16,    20,
      20,    20,    20,    16,    16,    16,    16,    16
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    17,    18,    19,    19,    19,    19,    19,    20,    20,
      20,    20,    20,    20,    20,    20
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     4,     4,     4,     4,     4,     1,     1,
       1,     3,     3,     3,     3,     3
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
  case 2: /* input: expr  */
#line 57 "expr.y"
          {
	Node* d = differentiate((yyvsp[0].node));
	Node* s = simplify(d);
	print_expr(s); printf("\n");
	free_expr((yyvsp[0].node)); 
	free_expr(d); 
	free_expr(s);
    }
#line 1122 "expr.tab.c"
    break;

  case 3: /* func: SIN '(' expr ')'  */
#line 68 "expr.y"
                          { (yyval.node) = make_node(NODE_SIN, (yyvsp[-1].node), NULL, 0); }
#line 1128 "expr.tab.c"
    break;

  case 4: /* func: TAN '(' expr ')'  */
#line 69 "expr.y"
                          { (yyval.node) = make_node(NODE_TAN, (yyvsp[-1].node), NULL, 0); }
#line 1134 "expr.tab.c"
    break;

  case 5: /* func: EXP '(' expr ')'  */
#line 70 "expr.y"
                          { (yyval.node) = make_node(NODE_EXP, (yyvsp[-1].node), NULL, 0); }
#line 1140 "expr.tab.c"
    break;

  case 6: /* func: COS '(' expr ')'  */
#line 71 "expr.y"
                          { (yyval.node) = make_node(NODE_COS, (yyvsp[-1].node), NULL, 0); }
#line 1146 "expr.tab.c"
    break;

  case 7: /* func: LN '(' expr ')'  */
#line 72 "expr.y"
                         { (yyval.node) = make_node(NODE_LN, (yyvsp[-1].node), NULL, 0); }
#line 1152 "expr.tab.c"
    break;

  case 8: /* expr: NUMBER  */
#line 75 "expr.y"
                     { (yyval.node) = make_node(NUM, NULL, NULL, (yyvsp[0].num)); }
#line 1158 "expr.tab.c"
    break;

  case 9: /* expr: VARIABLE  */
#line 76 "expr.y"
                       { (yyval.node) = make_node(VAR, NULL, NULL, 0); }
#line 1164 "expr.tab.c"
    break;

  case 10: /* expr: func  */
#line 77 "expr.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1170 "expr.tab.c"
    break;

  case 11: /* expr: expr '+' expr  */
#line 78 "expr.y"
                       { (yyval.node) = make_node(ADD, (yyvsp[-2].node), (yyvsp[0].node), 0); }
#line 1176 "expr.tab.c"
    break;

  case 12: /* expr: expr '-' expr  */
#line 79 "expr.y"
                       { (yyval.node) = make_node(SUB, (yyvsp[-2].node), (yyvsp[0].node), 0); }
#line 1182 "expr.tab.c"
    break;

  case 13: /* expr: expr '*' expr  */
#line 80 "expr.y"
                       { (yyval.node) = make_node(MUL, (yyvsp[-2].node), (yyvsp[0].node), 0); }
#line 1188 "expr.tab.c"
    break;

  case 14: /* expr: expr '/' expr  */
#line 81 "expr.y"
                       { (yyval.node) = make_node(DIV, (yyvsp[-2].node), (yyvsp[0].node), 0); }
#line 1194 "expr.tab.c"
    break;

  case 15: /* expr: '(' expr ')'  */
#line 82 "expr.y"
                       { (yyval.node) = (yyvsp[-1].node); }
#line 1200 "expr.tab.c"
    break;


#line 1204 "expr.tab.c"

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

#line 85 "expr.y"


Node* make_node(int type, Node* left, Node* right, int value) {
    Node* n = malloc(sizeof(Node));
    n->type = type;
    n->left = left;
    n->right = right;
    n->value = value;
    return n;
}

Node* differentiate(Node* e) {
    switch (e->type) {

case NODE_TAN:
		return make_node(MUL,
			 make_node(DIV,
				   make_node(NUM, NULL, NULL, 1),
				   make_node(MUL,
					     make_node(NODE_COS, e->left, NULL, 0),
					     make_node(NODE_COS, e->left, NULL, 0),
					     0),
				   0),
			 differentiate(e->left),
			 0);

case NODE_EXP:
		return make_node(MUL,
			 make_node(NODE_EXP, e->left, NULL, 0),
			 differentiate(e->left),
			 0);

case NODE_LN:
                return make_node(DIV,
                         differentiate(e->left),
			 e->left,
                         0);

case NUM: return make_node(NUM, NULL, NULL, 0);
	case VAR: return make_node(NUM, NULL, NULL, 1);
	case ADD: return make_node(ADD, differentiate(e->left) , differentiate(e->right) , 0);
	case SUB: return make_node(SUB, differentiate(e->left), differentiate(e->right), 0);
	case MUL:
	    return make_node(ADD,
		make_node(MUL, differentiate(e->left), e->right, 0),
		make_node(MUL, e->left, differentiate(e->right), 0),
		0
	    );
	case DIV:
	    return make_node(DIV,
		make_node(SUB,
		    make_node(MUL, differentiate(e->left), e->right, 0),
		    make_node(MUL, e->left, differentiate(e->right), 0),
		    0
		),
		make_node(MUL, e->right, e->right, 0),
		0
	    );
	case NODE_SIN:
	    return make_node(MUL,
		differentiate(e->left),
		make_node(NODE_COS, e->left, NULL, 0),
		0
	    );
	case NODE_COS:
	    return make_node(MUL,
		make_node(NUM, NULL, NULL, -1),
		make_node(MUL,
		    simplify(differentiate(e->left)),
		    make_node(NODE_SIN, e->left, NULL, 0),
		    0
		),
		0
	    );
	default: return NULL;
    }
}


Node* simplify(Node* e) {
    if (!e) return NULL;

    if( !e->left )
	    e->left = simplify(e->left);

    if( !e->right )
	    e->right = simplify(e->right);

// Combine (x * x) + (x * (2 * x)) => 3 * x * x
    if (e->type == ADD &&
	!e->left && !e->right &&
	e->left->type == MUL && e->right->type == MUL) {

	Node *a1 = e->left->left, *a2 = e->left->right;
	Node *b1 = e->right->left, *b2 = e->right->right;

if (a1 && a2 && b1 && b2 &&
	    a1->type == VAR && a2->type == VAR &&
	    b1->type == VAR &&
	    b2->type == MUL && b2->left && b2->right &&
	    b2->left->type == NUM && b2->right->type == VAR) {

int coef = b2->left->value + 1;
	    Node* count = make_node(NUM, NULL, NULL, coef);
	    Node* var1 = make_node(VAR, NULL, NULL, 0);
	    Node* var2 = make_node(VAR, NULL, NULL, 0);
	    Node* inner = make_node(MUL, var1, var2, 0);
	    Node* res = make_node(MUL, count, inner, 0);
	    free_expr(e);
	    return simplify(res);
	}
    }




// Advanced combine: (a * x * x * x) + (b * x * x * x) => (a + b) * x * x * x
    if (e->type == ADD &&
	e->left && e->left->type == MUL &&
	e->right && e->right->type == MUL) {

Node *l1 = e->left, *r1 = e->right;

// Flatten helper
	Node* flatten_mul(Node* n, int* coef, Node** term) {
	    if (!n) return NULL;
	    if (n->type == NUM) {
		*coef *= n->value;
		return NULL;
	    } else if (n->type == VAR) {
		*term = make_node(VAR, NULL, NULL, 0);
		return NULL;
	    } else if (n->type == MUL) {
		flatten_mul(n->left, coef, term);
		flatten_mul(n->right, coef, term);
	    } else {
		*term = simplify(n);
	    }
	    return NULL;
	}

int coefL = 1, coefR = 1;
	Node *tL = NULL, *tR = NULL;
	flatten_mul(l1, &coefL, &tL);
	flatten_mul(r1, &coefR, &tR);

if (tL && tR && tL->type == MUL && tR->type == MUL &&
	    strcmp(print_to_string(tL), print_to_string(tR)) == 0) {
	    int total = coefL + coefR;
	    Node* const_node = make_node(NUM, NULL, NULL, total);
	    Node* new_mul = make_node(MUL, const_node, tL, 0);
	    free_expr(tR);
	    free_expr(e);
	    return simplify(new_mul);
	}
    }


// Identity simplifications

// Fold nested constant multiplications: 4 * (3 * e) => 12 * e
    if (e->type == MUL &&
	e->left && e->right &&
	e->left->type == NUM &&
	e->right->type == MUL &&
	e->right->left && e->right->left->type == NUM) {

int folded = e->left->value * e->right->left->value;
	Node* folded_const = make_node(NUM, NULL, NULL, folded);
	Node* new_mul = make_node(MUL, folded_const, e->right->right, 0);
	free(e->right->left);
	free(e->right);
	free(e->left);
	free(e);
	return simplify(new_mul);
    }

// 1 * e => e
    if (e->type == MUL) {
	if (e->left && e->left->type == NUM && e->left->value == 1) {
	    Node* res = e->right;
	    free(e->left);
	    free(e);
	    return simplify(res);
	}
	if (e->right && e->right->type == NUM && e->right->value == 1) {
	    Node* res = e->left;
	    free(e->right);
	    free(e);
	    return simplify(res);
	}
    }

// 0 * e => 0
    if (e->type == MUL) {
	if ((e->left && e->left->type == NUM && e->left->value == 0) ||
	    (e->right && e->right->type == NUM && e->right->value == 0)) {
	    free_expr(e);
	    return make_node(NUM, NULL, NULL, 0);
	}
    }

// e + 0 or 0 + e => e
    if (e->type == ADD) {
	if (e->left && e->left->type == NUM && e->left->value == 0) {
	    Node* res = e->right;
	    free(e->left);
	    free(e);
	    return simplify(res);
	}
	if (e->right && e->right->type == NUM && e->right->value == 0) {
	    Node* res = e->left;
	    free(e->right);
	    free(e);
	    return simplify(res);
	}
    }

// e - 0 => e
    if (e->type == SUB && e->right && e->right->type == NUM && e->right->value == 0) {
	Node* res = e->left;
	free(e->right);
	free(e);
	return simplify(res);
    }

// e / 1 => e
    if (e->type == DIV && e->right && e->right->type == NUM && e->right->value == 1) {
	Node* res = e->left;
	free(e->right);
	free(e);
	return simplify(res);
    }


// Simplify x + x => 2 * x
    if (e->type == ADD &&
	e->left && e->right &&
	e->left->type == VAR &&
	e->right->type == VAR) {
	Node* coeff = make_node(NUM, NULL, NULL, 2);
	Node* var = make_node(VAR, NULL, NULL, 0);
	Node* res = make_node(MUL, coeff, var, 0);
	free_expr(e);
	return simplify(res);
    }

// Combine (x * x) + (2 * x * x) => 3 * x * x
    if (e->type == ADD &&
	e->left && e->right &&
	e->left->type == MUL && e->right->type == MUL) {

Node *a1 = e->left->left, *a2 = e->left->right;
	Node *b1 = e->right->left, *b2 = e->right->right;

// Handle pattern: (x * x) + (NUM * (x * x))
	if (a1 && a2 && b1 && b2 &&
	    a1->type == VAR && a2->type == VAR &&
	    b1->type == NUM &&
	    b2->type == MUL && b2->left && b2->right &&
	    b2->left->type == VAR && b2->right->type == VAR) {

int coef = b1->value + 1;
	    Node* count = make_node(NUM, NULL, NULL, coef);
	    Node* var1 = make_node(VAR, NULL, NULL, 0);
	    Node* var2 = make_node(VAR, NULL, NULL, 0);
	    Node* inner = make_node(MUL, var1, var2, 0);
	    Node* res = make_node(MUL, count, inner, 0);
	    free_expr(e);
	    return simplify(res);
	}

// Match: (x * x) + (x * (x + x)) => 3 * x * x
	if (a1 && a2 && b1 && b2 &&
	    a1->type == VAR && a2->type == VAR &&
	    b1->type == VAR &&
	    b2->type == ADD &&
	    b2->left && b2->right &&
	    b2->left->type == VAR && b2->right->type == VAR) {

Node* count = make_node(NUM, NULL, NULL, 3);
	    Node* var1 = make_node(VAR, NULL, NULL, 0);
	    Node* var2 = make_node(VAR, NULL, NULL, 0);
	    Node* inner = make_node(MUL, var1, var2, 0);
	    Node* res = make_node(MUL, count, inner, 0);
	    free_expr(e);
	    return simplify(res);
	}
    }
    // Combine (a * x * x) + (b * x * x) => (a + b) * x * x
    if (e->type == ADD &&
	e->left && e->right &&
	e->left->type == MUL && e->right->type == MUL) {

Node *a1 = e->left->left, *a2 = e->left->right;
	Node *b1 = e->right->left, *b2 = e->right->right;

// Normalize if NUM is on the right
	if (a1->type == NUM) { Node* tmp = a1; a1 = a2; a2 = tmp; }
	if (b1->type == NUM) { Node* tmp = b1; b1 = b2; b2 = tmp; }

if (a2->type == NUM && b2->type == NUM &&
	    a1->type == MUL && b1->type == MUL &&
	    a1->left->type == VAR && a1->right->type == VAR &&
	    b1->left->type == VAR && b1->right->type == VAR) {

int coef = a2->value + b2->value;
	    Node* folded = make_node(NUM, NULL, NULL, coef);
	    Node* var1 = make_node(VAR, NULL, NULL, 0);
	    Node* var2 = make_node(VAR, NULL, NULL, 0);
	    Node* inner = make_node(MUL, var1, var2, 0);
	    Node* res = make_node(MUL, folded, inner, 0);
	    free_expr(e);
	    return simplify(res);
	}
    }

return e;
}


void print_expr(Node* e) {
    if (!e) return;
    switch (e->type) {
	case NUM: printf("%d", e->value); break;
	case VAR: printf("x"); break;
	case ADD: printf("("); print_expr(e->left); printf(" + "); print_expr(e->right); printf(")"); break;
	case SUB: printf("("); print_expr(e->left); printf(" - "); print_expr(e->right); printf(")"); break;
	case MUL: printf("("); print_expr(e->left); printf(" * "); print_expr(e->right); printf(")"); break;
	case DIV: printf("("); print_expr(e->left); printf(" / "); print_expr(e->right); printf(")"); break;
	case NODE_SIN: printf("sin("); print_expr(e->left); printf(")"); break;
	case NODE_COS: printf("cos("); print_expr(e->left); printf(")"); break;
	case NODE_TAN: printf("tan("); print_expr(e->left); printf(")"); break;
	case NODE_LN: printf("ln("); print_expr(e->left); printf(")"); break;
    }
}

void free_expr(Node* e) {
    if (!e) return;

if( !e->left ) 
	    free_expr(e->left);
    if ( !e->right )
	    free_expr(e->right);
/*
    free(e);
*/
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

char* print_to_string_rec(Node* e, char* buf) {
    if (!e) return buf;

switch (e->type) {
	case NUM:
	    buf += sprintf(buf, "%d", e->value);
	    break;
	case VAR:
	    buf += sprintf(buf, "x");
	    break;
	case ADD:
	    *buf++ = '(';
	    buf = print_to_string_rec(e->left, buf);
	    buf += sprintf(buf, "+");
	    buf = print_to_string_rec(e->right, buf);
	    *buf++ = ')';
	    break;
	case SUB:
	    *buf++ = '(';
	    buf = print_to_string_rec(e->left, buf);
	    buf += sprintf(buf, "-");
	    buf = print_to_string_rec(e->right, buf);
	    *buf++ = ')';
	    break;
	case MUL:
	    *buf++ = '(';
	    buf = print_to_string_rec(e->left, buf);
	    buf += sprintf(buf, "*");
	    buf = print_to_string_rec(e->right, buf);
	    *buf++ = ')';
	    break;
	case DIV:
	    *buf++ = '(';
	    buf = print_to_string_rec(e->left, buf);
	    buf += sprintf(buf, "/");
	    buf = print_to_string_rec(e->right, buf);
	    *buf++ = ')';
	    break;
	case NODE_SIN:
	    buf += sprintf(buf, "sin(");
	    buf = print_to_string_rec(e->left, buf);
	    *buf++ = ')';
	    break;
	case NODE_COS:
	    buf += sprintf(buf, "cos(");
	    buf = print_to_string_rec(e->left, buf);
	    *buf++ = ')';
	    break;
	case NODE_EXP:
	    buf += sprintf(buf, "exp(");
	    buf = print_to_string_rec(e->left, buf);
	    *buf++ = ')';
	    break;
	case NODE_TAN:
	    buf += sprintf(buf, "tan(");
	    buf = print_to_string_rec(e->left, buf);
	    *buf++ = ')';
	    break;
    }
    *buf = '\0';
    return buf;
}

char* print_to_string(Node* e) {
    static char buffer[1024];
    buffer[0] = '\0';
    print_to_string_rec(e, buffer);
    return buffer;
}


void main(void) {

#ifndef __RELEASE 
const char* test_cases[] = {
	"sin(x)", "cos(x)", "tan(x)", 
	"(4*x*x*x)+(8*x*x*x)*x",
	"tan(exp(x))",
	"tan(exp(x+exp(x)))",
	"cos(2*x)",
	"ln( cos(x))"
    };

size_t num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

printf(" running %zu tests\n", num_cases);

for (size_t i = 0; i < num_cases; ++i) {
	yy_scan_string(test_cases[i]);
	yyparse();
    }
#else
 yyparse();
#endif
}
