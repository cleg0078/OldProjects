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

#ifndef __RELEASE 
extern void yy_scan_string(const char *);
#endif

#ifndef NODE_DEFINED
#define NODE_DEFINED
	typedef struct Node {
		enum { NUM, VAR, ADD, SUB, MUL, DIV, NODE_SIN, NODE_COS, NODE_TAN, NODE_EXP, NODE_LN, NODE_LOG, NODE_POW } type;
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
	

#line 101 "expr.tab.c"

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
  YYSYMBOL_LOG = 10,                       /* LOG  */
  YYSYMBOL_POW = 11,                       /* POW  */
  YYSYMBOL_12_ = 12,                       /* '+'  */
  YYSYMBOL_13_ = 13,                       /* '-'  */
  YYSYMBOL_14_ = 14,                       /* '^'  */
  YYSYMBOL_15_ = 15,                       /* '*'  */
  YYSYMBOL_16_ = 16,                       /* '/'  */
  YYSYMBOL_UMINUS = 17,                    /* UMINUS  */
  YYSYMBOL_18_ = 18,                       /* '('  */
  YYSYMBOL_19_ = 19,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 20,                  /* $accept  */
  YYSYMBOL_input = 21,                     /* input  */
  YYSYMBOL_func = 22,                      /* func  */
  YYSYMBOL_expr = 23                       /* expr  */
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
#define YYFINAL  20
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   97

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  20
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  4
/* YYNRULES -- Number of rules.  */
#define YYNRULES  17
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  44

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   267


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
      18,    19,    15,    12,     2,    13,     2,    16,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    14,     2,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11,    17
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int8 yyrline[] =
{
       0,    62,    62,    73,    74,    75,    76,    77,    78,    81,
      82,    83,    84,    85,    86,    87,    88,    89
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
  "SIN", "COS", "TAN", "EXP", "LN", "LOG", "POW", "'+'", "'-'", "'^'",
  "'*'", "'/'", "UMINUS", "'('", "')'", "$accept", "input", "func", "expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-18)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      14,   -18,   -18,   -17,   -16,   -15,    -8,    -7,     7,    14,
      29,   -18,    78,    14,    14,    14,    14,    14,    14,    15,
     -18,    14,    14,    14,    14,    14,    24,    33,    42,    51,
      60,    69,   -18,    78,    81,    81,    22,    22,   -18,   -18,
     -18,   -18,   -18,   -18
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     9,    10,     0,     0,     0,     0,     0,     0,     0,
       0,    11,     2,     0,     0,     0,     0,     0,     0,     0,
       1,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    17,    16,    12,    13,    14,    15,     3,     6,
       4,     5,     7,     8
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -18,   -18,   -18,    -9
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    10,    11,    12
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      19,    13,    14,    15,    26,    27,    28,    29,    30,    31,
      16,    17,    33,    34,    35,    36,    37,     1,     2,     3,
       4,     5,     6,     7,     8,    18,    21,    22,    23,    20,
      24,    25,     9,    21,    32,    21,    22,    23,     0,    24,
      25,     0,     0,    38,    21,    22,    23,     0,    24,    25,
       0,     0,    39,    21,    22,    23,     0,    24,    25,     0,
       0,    40,    21,    22,    23,     0,    24,    25,     0,     0,
      41,    21,    22,    23,     0,    24,    25,     0,     0,    42,
      21,    22,    23,     0,    24,    25,     0,     0,    43,    21,
      22,    23,    21,    24,    25,     0,    24,    25
};

static const yytype_int8 yycheck[] =
{
       9,    18,    18,    18,    13,    14,    15,    16,    17,    18,
      18,    18,    21,    22,    23,    24,    25,     3,     4,     5,
       6,     7,     8,     9,    10,    18,    11,    12,    13,     0,
      15,    16,    18,    11,    19,    11,    12,    13,    -1,    15,
      16,    -1,    -1,    19,    11,    12,    13,    -1,    15,    16,
      -1,    -1,    19,    11,    12,    13,    -1,    15,    16,    -1,
      -1,    19,    11,    12,    13,    -1,    15,    16,    -1,    -1,
      19,    11,    12,    13,    -1,    15,    16,    -1,    -1,    19,
      11,    12,    13,    -1,    15,    16,    -1,    -1,    19,    11,
      12,    13,    11,    15,    16,    -1,    15,    16
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    18,
      21,    22,    23,    18,    18,    18,    18,    18,    18,    23,
       0,    11,    12,    13,    15,    16,    23,    23,    23,    23,
      23,    23,    19,    23,    23,    23,    23,    23,    19,    19,
      19,    19,    19,    19
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    20,    21,    22,    22,    22,    22,    22,    22,    23,
      23,    23,    23,    23,    23,    23,    23,    23
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     4,     4,     4,     4,     4,     4,     1,
       1,     1,     3,     3,     3,     3,     3,     3
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
#line 62 "expr.y"
     {
	Node* d = differentiate((yyvsp[0].node));
	Node* s = simplify(d);
	print_expr(s); printf("\n");
	free_expr((yyvsp[0].node)); 
	free_expr(d); 
	free_expr(s);
}
#line 1139 "expr.tab.c"
    break;

  case 3: /* func: SIN '(' expr ')'  */
#line 73 "expr.y"
                      { (yyval.node) = make_node(NODE_SIN, (yyvsp[-1].node), NULL, 0); }
#line 1145 "expr.tab.c"
    break;

  case 4: /* func: TAN '(' expr ')'  */
#line 74 "expr.y"
                      { (yyval.node) = make_node(NODE_TAN, (yyvsp[-1].node), NULL, 0); }
#line 1151 "expr.tab.c"
    break;

  case 5: /* func: EXP '(' expr ')'  */
#line 75 "expr.y"
                      { (yyval.node) = make_node(NODE_EXP, (yyvsp[-1].node), NULL, 0); }
#line 1157 "expr.tab.c"
    break;

  case 6: /* func: COS '(' expr ')'  */
#line 76 "expr.y"
                      { (yyval.node) = make_node(NODE_COS, (yyvsp[-1].node), NULL, 0); }
#line 1163 "expr.tab.c"
    break;

  case 7: /* func: LN '(' expr ')'  */
#line 77 "expr.y"
                      { (yyval.node) = make_node(NODE_LN, (yyvsp[-1].node), NULL, 0);  }
#line 1169 "expr.tab.c"
    break;

  case 8: /* func: LOG '(' expr ')'  */
#line 78 "expr.y"
                      { (yyval.node) = make_node(NODE_LOG, (yyvsp[-1].node), NULL, 0);  }
#line 1175 "expr.tab.c"
    break;

  case 9: /* expr: NUMBER  */
#line 81 "expr.y"
                 { (yyval.node) = make_node(NUM, NULL, NULL, (yyvsp[0].num)); }
#line 1181 "expr.tab.c"
    break;

  case 10: /* expr: VARIABLE  */
#line 82 "expr.y"
                   { (yyval.node) = make_node(VAR, NULL, NULL, 0); }
#line 1187 "expr.tab.c"
    break;

  case 11: /* expr: func  */
#line 83 "expr.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 1193 "expr.tab.c"
    break;

  case 12: /* expr: expr '+' expr  */
#line 84 "expr.y"
                   { (yyval.node) = make_node(ADD, (yyvsp[-2].node), (yyvsp[0].node), 0); }
#line 1199 "expr.tab.c"
    break;

  case 13: /* expr: expr '-' expr  */
#line 85 "expr.y"
                   { (yyval.node) = make_node(SUB, (yyvsp[-2].node), (yyvsp[0].node), 0); }
#line 1205 "expr.tab.c"
    break;

  case 14: /* expr: expr '*' expr  */
#line 86 "expr.y"
                   { (yyval.node) = make_node(MUL, (yyvsp[-2].node), (yyvsp[0].node), 0); }
#line 1211 "expr.tab.c"
    break;

  case 15: /* expr: expr '/' expr  */
#line 87 "expr.y"
                   { (yyval.node) = make_node(DIV, (yyvsp[-2].node), (yyvsp[0].node), 0); }
#line 1217 "expr.tab.c"
    break;

  case 16: /* expr: expr POW expr  */
#line 88 "expr.y"
                   { (yyval.node) = make_node(NODE_POW, (yyvsp[-2].node), (yyvsp[0].node), 0); }
#line 1223 "expr.tab.c"
    break;

  case 17: /* expr: '(' expr ')'  */
#line 89 "expr.y"
                   { (yyval.node) = (yyvsp[-1].node); }
#line 1229 "expr.tab.c"
    break;


#line 1233 "expr.tab.c"

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

#line 92 "expr.y"


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
		case NODE_LOG:
			return make_node(DIV,
					differentiate(e->left),
					e->left,
					0);
		case NODE_POW:
			// d/dx(u^v) = v*u^(v-1)*u' + u^v*ln(u)*v' (general power rule)
			// For v = constant: d/dx(u^n) = n*u^(n-1)*u'
			// For u = x, v = n (constant): d/dx(x^n) = n*x^(n-1)
			if (e->right && e->right->type == NUM) {
				// d/dx(u^n) = n*u^(n-1)*u'
				int n = e->right->value;
				return make_node(MUL,
						make_node(NUM, NULL, NULL, n),
						make_node(MUL,
								make_node(NODE_POW, e->left, make_node(NUM, NULL, NULL, n-1), 0),
								differentiate(e->left),
								0),
						0);
			} else {
				// General case: d/dx(u^v) = u^v * (v*u^(v-1)*u' + ln(u)*v')
				return make_node(MUL,
						make_node(NODE_POW, e->left, e->right, 0),
						make_node(ADD,
								make_node(MUL,
										e->right,
										make_node(MUL,
												make_node(NODE_POW, e->left, make_node(NUM, NULL, NULL, 1), 0),
												differentiate(e->left),
												0),
										0),
								make_node(MUL,
										make_node(NODE_LN, e->left, NULL, 0),
										differentiate(e->right),
										0),
								0),
						0);
			}


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
						  differentiate(e->left),
						  make_node(NODE_SIN, e->left, NULL, 0),
						  0
						  ),
					  0
					  );
		default: return NULL;
	}
}

// Forward declarations
Node* distribute(Node* e);
Node* evaluate_constant(Node* e);
int nodes_equal(Node* a, Node* b);
void term_from_product(Node* e, int* coef, int* x_power, Node** trans);
typedef struct Term {
	int coef;
	int x_power;
	Node* trans;
	struct Term* next;
} Term;
Term* flatten_to_terms(Node* e);
Term* negate_terms(Term* t);
Term* concat_terms(Term* a, Term* b);
void free_terms(Term* t);
Term* collect_terms(Term* t);
Node* terms_to_expr(Term* t);
Node* deep_copy_node(Node* e);
Node* simplify_arithmetic(Node* e);

Node* simplify(Node* e) {
	if (!e) return NULL;

	// Deep copy to avoid destroying the input tree
	e = deep_copy_node(e);

	// First, distribute and evaluate constants
	e = distribute(e);
	e = evaluate_constant(e);

	// Flatten to terms, collect, rebuild
	Term* terms = flatten_to_terms(e);
	free_expr(e);

	Term* collected = collect_terms(terms);
	terms = NULL;
	free_terms(terms);

	Node* result = terms_to_expr(collected);
	free_terms(collected);

	return result;
}

// Simplify arithmetic expressions: 0*x=0, 1*x=x, 0+x=x
Node* simplify_arithmetic(Node* e) {
    if (!e) return NULL;
    
    e->left = simplify_arithmetic(e->left);
    e->right = simplify_arithmetic(e->right);
    
    // 0 * x = 0
    if (e->type == MUL && e->left && e->left->type == NUM && e->left->value == 0) {
        Node* res = make_node(NUM, NULL, NULL, 0);
        free(e);
        return res;
    }
    
    // x * 0 = 0
    if (e->type == MUL && e->right && e->right->type == NUM && e->right->value == 0) {
        Node* res = make_node(NUM, NULL, NULL, 0);
        free(e);
        return res;
    }
    
    // 1 * x = x
    if (e->type == MUL && e->left && e->left->type == NUM && e->left->value == 1) {
        Node* res = e->right;
        e->right = NULL;
        free(e);
        return res;
    }
    
    // x * 1 = x
    if (e->type == MUL && e->right && e->right->type == NUM && e->right->value == 1) {
        Node* res = e->left;
        e->left = NULL;
        free(e);
        return res;
    }
    
    // 0 + x = x
    if (e->type == ADD && e->left && e->left->type == NUM && e->left->value == 0) {
        Node* res = e->right;
        e->right = NULL;
        free(e);
        return res;
    }
    
    // x + 0 = x
    if (e->type == ADD && e->right && e->right->type == NUM && e->right->value == 0) {
        Node* res = e->left;
        e->left = NULL;
        free(e);
        return res;
    }
    
    // x - 0 = x
    if (e->type == SUB && e->right && e->right->type == NUM && e->right->value == 0) {
        Node* res = e->left;
        e->left = NULL;
        free(e);
        return res;
    }
    
    // 0 - x = -x
    if (e->type == SUB && e->left && e->left->type == NUM && e->left->value == 0) {
        Node* neg = make_node(MUL, make_node(NUM, NULL, NULL, -1), e->right, 0);
        e->right = NULL;
        free(e);
        return simplify_arithmetic(neg);
    }
    
    // x / 1 = x
    if (e->type == DIV && e->right && e->right->type == NUM && e->right->value == 1) {
        Node* res = e->left;
        e->left = NULL;
        free(e);
        return res;
    }
    
    return e;
}


Node* deep_copy_node(Node* e) {
	if (!e) return NULL;
	Node* n = make_node(e->type, NULL, NULL, e->value);
	n->left = deep_copy_node(e->left);
	n->right = deep_copy_node(e->right);
	return n;
}

// Distribute multiplication over addition: (a+b)*c => a*c + b*c
Node* distribute(Node* e) {
	if (!e) return NULL;
	e->left = distribute(e->left);
	e->right = distribute(e->right);

	if (e->type == MUL && e->left && e->right && e->left->type == ADD) {
		Node* a = e->left->left;
		Node* b = e->left->right;
		Node* c = e->right;
		Node* new_add = make_node(ADD, make_node(MUL, a, c, 0), make_node(MUL, b, c, 0), 0);
		free(e->left);
		free(e);
		return distribute(new_add);
	}

	if (e->type == MUL && e->left && e->right && e->right->type == ADD) {
		Node* a = e->left;
		Node* b = e->right->left;
		Node* c = e->right->right;
		Node* new_add = make_node(ADD, make_node(MUL, a, b, 0), make_node(MUL, a, c, 0), 0);
		free(e->right);
		free(e);
		return distribute(new_add);
	}

	return e;
}

// Evaluate constant subexpressions: sin(0)=0, cos(0)=1, exp(0)=1, ln(1)=0
Node* evaluate_constant(Node* e) {
	if (!e) return NULL;
	e->left = evaluate_constant(e->left);
	e->right = evaluate_constant(e->right);

	if (e->type == NODE_SIN && e->left && e->left->type == NUM && e->left->value == 0) {
		Node* res = make_node(NUM, NULL, NULL, 0);
		free(e);
		return res;
	}
	if (e->type == NODE_COS && e->left && e->left->type == NUM && e->left->value == 0) {
		Node* res = make_node(NUM, NULL, NULL, 1);
		free(e);
		return res;
	}
	if (e->type == NODE_EXP && e->left && e->left->type == NUM && e->left->value == 0) {
		Node* res = make_node(NUM, NULL, NULL, 1);
		free(e);
		return res;
	}
	if ((e->type == NODE_LN || e->type == NODE_LOG) && e->left && e->left->type == NUM && e->left->value == 1) {
		Node* res = make_node(NUM, NULL, NULL, 0);
		free(e);
		return res;
	}

	return e;
}

// Check if two nodes are structurally equal
int nodes_equal(Node* a, Node* b) {
	if (!a && !b) return 1;
	if (!a || !b) return 0;
	if (a->type != b->type) return 0;
	if (a->type == NUM) return a->value == b->value;
	if (a->type == VAR) return 1;
	if (nodes_equal(a->left, b->left) && nodes_equal(a->right, b->right)) return 1;
	return 0;
}

// Helper function to process a product chain without resetting
void term_from_product_inner(Node* e, int* coef, int* x_power, Node** trans) {
	while (e) {
		if (e->type == NUM) {
			*coef *= e->value;
			e = NULL;
		} else if (e->type == VAR) {
			(*x_power)++;
			e = NULL;
		} else if (e->type == MUL) {
			term_from_product_inner(e->left, coef, x_power, trans);
			e = e->right;
		} else {
			if (*trans) {
				*trans = make_node(MUL, *trans, e, 0);
			} else {
				*trans = e;
			}
			e = NULL;
		}
	}
}

// Extract coefficient, x_power, and transcendental part from a product chain
void term_from_product(Node* e, int* coef, int* x_power, Node** trans) {
	*coef = 1;
	*x_power = 0;
	*trans = NULL;
	term_from_product_inner(e, coef, x_power, trans);
}

// Convert AST to linked list of terms
Term* flatten_to_terms(Node* e) {
	if (!e) return NULL;

	if (e->type == ADD) {
		return concat_terms(flatten_to_terms(e->left), flatten_to_terms(e->right));
	}

	if (e->type == SUB) {
		Term* right = flatten_to_terms(e->right);
		right = negate_terms(right);
		return concat_terms(flatten_to_terms(e->left), right);
	}

	// Single term (product or atomic)
	int coef = 1, x_power = 0;
	Node* trans = NULL;
	term_from_product(e, &coef, &x_power, &trans);

	Term* t = malloc(sizeof(Term));
	t->coef = coef;
	t->x_power = x_power;
	t->trans = trans;
	t->next = NULL;
	return t;
}

Term* negate_terms(Term* t) {
	if (!t) return NULL;
	t->coef = -t->coef;
	return t;
}

Term* concat_terms(Term* a, Term* b) {
	if (!a) return b;
	if (!b) return a;
	Term* t = a;
	while (t->next) t = t->next;
	t->next = b;
	return a;
}

void free_terms(Term* t) {
	while (t) {
		Term* next = t->next;
		if (t->trans) free_expr(t->trans);
		free(t);
		t = next;
	}
}

// Collect terms with matching (x_power, trans)
Term* collect_terms(Term* t) {
	if (!t) return NULL;

	Term* result = NULL;
	while (t) {
		Term* next = t->next;
		t->next = NULL;

		Term* match = result;
		Term* prev = NULL;
		while (match) {
			if (match->x_power == t->x_power && nodes_equal(match->trans, t->trans)) {
				match->coef += t->coef;
				t->trans = NULL;  // Detach to avoid double-free
				t->next = NULL;  // Prevent double-free
				if (prev) prev->next = match;
				else result = match;
				// Remove term if coefficient is 0
				if (match->coef == 0) {
					Term* to_remove = match;
					if (prev) prev->next = match->next;
					else result = match->next;
					free_terms(to_remove);
				}
				break;
			}
			prev = match;
			match = match->next;
		}

		if (!match) {
			t->next = result;
			result = t;
		}

		t = next;
	}

	// Remove any remaining terms with coefficient 0
	Term** pp = &result;
	while (*pp) {
		Term* curr = *pp;
		if (curr->coef == 0) {
			*pp = curr->next;
			curr->next = NULL;
			free_terms(curr);
		} else {
			pp = &curr->next;
		}
	}

	return result;
}

// Rebuild AST from collected terms
Node* terms_to_expr(Term* t) {
	if (!t) return make_node(NUM, NULL, NULL, 0);

	Node* result = NULL;
	while (t) {
		Node* term_expr = NULL;
		Node* trans_copy = t->trans ? deep_copy_node(t->trans) : NULL;

		if (t->trans && t->x_power > 0) {
			Node* vars = NULL;
			for (int i = 0; i < t->x_power; i++) {
				Node* v = make_node(VAR, NULL, NULL, 0);
				vars = vars ? make_node(MUL, vars, v, 0) : v;
			}
			term_expr = make_node(MUL, vars, trans_copy, 0);
		} else if (t->x_power > 0) {
			Node* vars = NULL;
			for (int i = 0; i < t->x_power; i++) {
				Node* v = make_node(VAR, NULL, NULL, 0);
				vars = vars ? make_node(MUL, vars, v, 0) : v;
			}
			term_expr = vars;
		} else if (t->trans) {
			term_expr = trans_copy;
		} else {
			term_expr = make_node(NUM, NULL, NULL, t->coef);
		}

		if (t->coef != 1) {
			term_expr = make_node(MUL, make_node(NUM, NULL, NULL, t->coef), term_expr, 0);
		}

		result = result ? make_node(ADD, result, term_expr, 0) : term_expr;
		t = t->next;
	}

	return result;
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
		case NODE_LOG: printf("log("); print_expr(e->left); printf(")"); break;
		case NODE_POW: print_expr(e->left); printf("^"); print_expr(e->right); break;
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
		printf("solving %s=", test_cases[i] );
		yy_scan_string(test_cases[i]);
		yyparse();
	}
#else
	yyparse();
#endif
}
