/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
infringing on user name space.  This should be done even for local
variables, as they might otherwise be expanded by user macros.
There are some unavoidable exceptions within include files to
define necessary library symbols; they are noted "INFRINGES ON
USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 33 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:339  */

#include "api.h"
#include "pbrt.h"
#include "paramset.h"
#include <stdarg.h>

#ifdef PBRT_IS_MSVC
#pragma warning(disable:4065)
#pragma warning(disable:4996)
#pragma warning(disable:4018)
#endif // PBRT_IS_MSVC

void yyerror(const char *str) {
	pbrt::Error("Parsing error: %s", str);
	exit(1);
}
extern int yylex();

namespace pbrt {

	extern void include_push(char *filename);
	int line_num = 0;
	std::string current_file;

#define YYMAXDEPTH 100000000

	struct ParamArray {
		ParamArray() {
			isString = false;
			element_size = allocated = nelems = 0;
			array = nullptr;
		}
		bool isString;
		int element_size;
		int allocated;
		int nelems;
		void *array;
	};



	struct ParamListItem {
		ParamListItem(const char *t, ParamArray *array) {
			arg = array->array;
			name = t;
			size = array->nelems;
			isString = array->isString;
			array->allocated = 0;
			array->nelems = 0;
			array->array = nullptr;
		}
		const char *name;
		void *arg;
		int size;
		bool isString;
	};



	static std::vector<ParamListItem> cur_paramlist;

	static ParamArray *cur_array = nullptr;

	static void AddArrayElement(void *elem) {
		if (cur_array->nelems >= cur_array->allocated) {
			cur_array->allocated = 2 * cur_array->allocated + 1;
			cur_array->array = realloc(cur_array->array,
				cur_array->allocated*cur_array->element_size);
		}
		char *next = ((char *)cur_array->array) + cur_array->nelems * cur_array->element_size;
		CHECK(cur_array->element_size == 4 || cur_array->element_size == 8);
		if (cur_array->element_size == 4)
			*((uint32_t *)next) = *((uint32_t *)elem);
		else
			*((uint64_t *)next) = *((uint64_t *)elem);
		cur_array->nelems++;
	}



	static void ArrayFree(ParamArray *ra) {
		if (ra->isString && ra->array)
			for (int i = 0; i < ra->nelems; ++i) free(((char **)ra->array)[i]);
		free(ra->array);
		delete ra;
	}



	static void FreeArgs() {
		for (size_t i = 0; i < cur_paramlist.size(); ++i)
			free((char *)cur_paramlist[i].arg);
		cur_paramlist.erase(cur_paramlist.begin(), cur_paramlist.end());
	}



	static bool VerifyArrayLength(ParamArray *arr, int required,
		const char *command) {
		if (arr->nelems != required) {
			Error("\"%s\" requires a %d element array! (%d found)",
				command, required, arr->nelems);
			return false;
		}
		return true;
	}


	enum {
		PARAM_TYPE_INT, PARAM_TYPE_BOOL, PARAM_TYPE_FLOAT,
		PARAM_TYPE_POINT2, PARAM_TYPE_VECTOR2, PARAM_TYPE_POINT3,
		PARAM_TYPE_VECTOR3, PARAM_TYPE_NORMAL, PARAM_TYPE_RGB, PARAM_TYPE_XYZ,
		PARAM_TYPE_BLACKBODY, PARAM_TYPE_SPECTRUM,
		PARAM_TYPE_STRING, PARAM_TYPE_TEXTURE
	};
	static const char *paramTypeToName(int type);
	static void InitParamSet(ParamSet &ps, SpectrumType);
	static bool lookupType(const char *name, int *type, std::string &sname);
#define YYPRINT(file, type, value)  { \
	if ((type) == ID || (type) == STRING) \
		fprintf ((file), " %s", (value).string); \
	else if ((type) == NUM) \
		fprintf ((file), " %f", (value).num); \
}

}  // namespace pbrt


#line 193 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

   /* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

   /* In a future release of Bison, this section will be replaced
   by #include "pbrtparse.h".  */
#ifndef YY_YY_C_DATA_VICTORIA_CGRA408_RENDERING_FINALPROJECT_CGRA408_FINALPROJECT_BUILD_PBRTPARSE_H_INCLUDED
# define YY_YY_C_DATA_VICTORIA_CGRA408_RENDERING_FINALPROJECT_CGRA408_FINALPROJECT_BUILD_PBRTPARSE_H_INCLUDED
   /* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
enum yytokentype
{
	STRING = 258,
	ID = 259,
	NUM = 260,
	LBRACK = 261,
	RBRACK = 262,
	ACCELERATOR = 263,
	ACTIVETRANSFORM = 264,
	ALL = 265,
	AREALIGHTSOURCE = 266,
	ATTRIBUTEBEGIN = 267,
	ATTRIBUTEEND = 268,
	CAMERA = 269,
	CONCATTRANSFORM = 270,
	COORDINATESYSTEM = 271,
	COORDSYSTRANSFORM = 272,
	ENDTIME = 273,
	FILM = 274,
	IDENTITY = 275,
	INCLUDE = 276,
	LIGHTSOURCE = 277,
	LOOKAT = 278,
	MAKENAMEDMATERIAL = 279,
	MAKENAMEDMEDIUM = 280,
	MATERIAL = 281,
	MEDIUMINTERFACE = 282,
	NAMEDMATERIAL = 283,
	OBJECTBEGIN = 284,
	OBJECTEND = 285,
	OBJECTINSTANCE = 286,
	PIXELFILTER = 287,
	REVERSEORIENTATION = 288,
	ROTATE = 289,
	SAMPLER = 290,
	SCALE = 291,
	SHAPE = 292,
	STARTTIME = 293,
	INTEGRATOR = 294,
	TEXTURE = 295,
	TRANSFORMBEGIN = 296,
	TRANSFORMEND = 297,
	TRANSFORMTIMES = 298,
	TRANSFORM = 299,
	TRANSLATE = 300,
	WORLDBEGIN = 301,
	WORLDEND = 302,
	SCENEPHOTO = 303,
	SYNTHSCENEBEGIN = 304,
	SYNTHSCENEEND = 305,
	ANIMATIONFILE = 306,
	HIGH_PRECEDENCE = 307
};
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 160 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:355  */

	char string[1024];
	double num;
	pbrt::ParamArray *ribarray;

#line 292 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse(void);

#endif /* !YY_YY_C_DATA_VICTORIA_CGRA408_RENDERING_FINALPROJECT_CGRA408_FINALPROJECT_BUILD_PBRTPARSE_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 309 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
	  && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
	 || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
	 && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
	_Pragma ("GCC diagnostic push") \
	_Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
	_Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
void *malloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free(void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
	 && (! defined __cplusplus \
		 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

				   /* A type that is properly aligned for any stack member.  */
union yyalloc
{
	yytype_int16 yyss_alloc;
	YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
N elements.  */
# define YYSTACK_BYTES(N) \
	 ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
		YYSIZE_T yynewbytes;                                            \
		YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
		Stack = &yyptr->Stack_alloc;                                    \
		yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
		yyptr += yynewbytes / sizeof (*yyptr);                          \
	  }                                                                 \
	while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
	  __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
	  do                                        \
		{                                       \
		  YYSIZE_T yyi;                         \
		  for (yyi = 0; yyi < (Count); yyi++)   \
			(Dst)[yyi] = (Src)[yyi];            \
		}                                       \
	  while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  80
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   122

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  70
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  139

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   307

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
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
	45,    46,    47,    48,    49,    50,    51,    52
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
	0,   186,   186,   192,   200,   208,   216,   222,   229,   236,
	244,   250,   255,   261,   269,   276,   284,   290,   295,   301,
	309,   315,   328,   334,   339,   347,   352,   358,   367,   373,
	379,   385,   394,   400,   406,   415,   427,   433,   439,   448,
	454,   460,   469,   475,   484,   493,   502,   508,   514,   520,
	526,   532,   538,   547,   553,   559,   568,   574,   583,   592,
	601,   607,   613,   619,   631,   637,   643,   648,   656,   661,
	666
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
	"$end", "error", "$undefined", "STRING", "ID", "NUM", "LBRACK",
	"RBRACK", "ACCELERATOR", "ACTIVETRANSFORM", "ALL", "AREALIGHTSOURCE",
	"ATTRIBUTEBEGIN", "ATTRIBUTEEND", "CAMERA", "CONCATTRANSFORM",
	"COORDINATESYSTEM", "COORDSYSTRANSFORM", "ENDTIME", "FILM", "IDENTITY",
	"INCLUDE", "LIGHTSOURCE", "LOOKAT", "MAKENAMEDMATERIAL",
	"MAKENAMEDMEDIUM", "MATERIAL", "MEDIUMINTERFACE", "NAMEDMATERIAL",
	"OBJECTBEGIN", "OBJECTEND", "OBJECTINSTANCE", "PIXELFILTER",
	"REVERSEORIENTATION", "ROTATE", "SAMPLER", "SCALE", "SHAPE", "STARTTIME",
	"INTEGRATOR", "TEXTURE", "TRANSFORMBEGIN", "TRANSFORMEND",
	"TRANSFORMTIMES", "TRANSFORM", "TRANSLATE", "WORLDBEGIN", "WORLDEND",
	"SCENEPHOTO", "SYNTHSCENEBEGIN", "SYNTHSCENEEND", "ANIMATIONFILE",
	"HIGH_PRECEDENCE", "$accept", "start", "array_init", "string_array_init",
	"num_array_init", "array", "string_array", "single_element_string_array",
	"string_list", "string_list_entry", "num_array",
	"single_element_num_array", "num_list", "num_list_entry", "paramlist",
	"paramlist_init", "paramlist_contents", "paramlist_entry",
	"pbrt_stmt_list", "pbrt_stmt", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
(internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
	0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
	265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
	275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
	285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
	295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
	305,   306,   307
};
# endif

#define YYPACT_NINF -124

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-124)))

#define YYTABLE_NINF -6

#define yytable_value_is_error(Yytable_value) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
STATE-NUM.  */
static const yytype_int8 yypact[] =
{
	65,     3,    -5,     9,  -124,  -124,    11,  -124,    13,    15,
	19,  -124,    20,    22,    24,    29,    31,    32,    33,    34,
	35,  -124,    36,    37,  -124,    39,    38,    40,    43,    44,
	45,  -124,  -124,    46,  -124,    47,  -124,  -124,  -124,  -124,
	-124,  -124,    27,    65,  -124,  -124,  -124,  -124,  -124,  -124,
	-124,    48,  -124,  -124,  -124,  -124,  -124,  -124,  -124,    50,
	-124,  -124,  -124,    53,  -124,  -124,  -124,  -124,    52,  -124,
	54,  -124,  -124,    55,    56,  -124,    57,  -124,    60,  -124,
	-124,  -124,  -124,  -124,  -124,  -124,    59,  -124,  -124,  -124,
	61,  -124,  -124,  -124,  -124,  -124,    62,  -124,    63,  -124,
	-124,    66,  -124,    70,  -124,  -124,    60,    42,  -124,  -124,
	78,    98,  -124,  -124,  -124,     5,  -124,  -124,  -124,  -124,
	-124,  -124,  -124,   112,  -124,  -124,   113,    67,  -124,   114,
	58,  -124,  -124,   115,  -124,  -124,   116,   117,  -124
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
Performed when YYTABLE does not specify something else to do.  Zero
means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
	0,     0,     0,     0,    32,    33,     0,     3,     0,     0,
	0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
	0,    50,     0,     0,    53,     0,     0,     0,     0,     0,
	0,    60,    61,     0,     3,     0,    65,    66,    21,    68,
	69,    21,     0,     2,    26,    21,    28,    29,    30,    21,
	21,     5,    35,    15,    36,    37,    21,    40,    21,     0,
	21,    21,    21,    46,    48,    49,    51,    21,     0,    21,
	0,    21,    21,     0,     0,    63,     0,    67,    23,    70,
	1,    25,    27,    31,    34,     5,     0,    16,    38,    41,
	0,    43,    44,    45,    47,    52,     0,    55,     0,    57,
	58,     0,    62,     0,     3,    20,    23,     5,    18,    19,
	0,     0,    56,    21,    64,     4,    24,     6,     9,     7,
	22,    14,    17,     0,    54,    59,     4,     0,    10,     0,
	4,    12,    13,     0,     8,    11,     0,     0,    42
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
	-124,  -124,   -62,  -124,  -124,  -124,  -124,  -124,  -124,  -123,
	-33,  -124,  -124,   -83,   -41,  -124,   -56,  -124,  -124,    10
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
	-1,    42,    51,   127,    86,   116,   117,   118,   130,   128,
	52,    53,   107,    87,    77,    78,   105,   106,    43,    44
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
positive, shift that token.  If negative, reduce the rule whose
number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
	79,    75,   108,   131,    82,    46,    45,   135,    83,    84,
	-5,   126,    49,    47,    50,    88,    54,    89,    55,    91,
	92,    93,    56,    57,   122,    58,    95,    80,    97,    59,
	99,   100,    60,    48,    61,    62,    63,    64,    65,    66,
	67,    69,   115,   108,    68,    70,    71,    72,    73,   121,
	120,    74,    76,    81,    85,    90,    94,    96,   101,    98,
	0,   102,   103,   104,   109,   134,   110,   111,   112,   113,
	132,   119,   125,     1,     2,   114,     3,     4,     5,     6,
	7,     8,     9,   123,    10,    11,    12,    13,    14,    15,
	16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
	26,    27,    28,   124,    29,    30,    31,    32,    33,    34,
	35,    36,    37,    38,    39,    40,    41,   129,    -5,   133,
	136,   137,   138
};

static const yytype_int16 yycheck[] =
{
	41,    34,    85,   126,    45,    10,     3,   130,    49,    50,
	5,     6,     3,    18,     3,    56,     3,    58,     3,    60,
	61,    62,     3,     3,   107,     3,    67,     0,    69,     5,
	71,    72,     3,    38,     3,     3,     3,     3,     3,     3,
	3,     3,   104,   126,     5,     5,     3,     3,     3,     7,
	106,     5,     5,    43,     6,     5,     3,     5,     3,     5,
	-1,     5,     5,     3,     5,     7,     5,     5,     5,     3,
	3,   104,   113,     8,     9,     5,    11,    12,    13,    14,
	15,    16,    17,     5,    19,    20,    21,    22,    23,    24,
	25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
	35,    36,    37,     5,    39,    40,    41,    42,    43,    44,
	45,    46,    47,    48,    49,    50,    51,     5,     5,     5,
	5,     5,     5
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
	0,     8,     9,    11,    12,    13,    14,    15,    16,    17,
	19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
	29,    30,    31,    32,    33,    34,    35,    36,    37,    39,
	40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
	50,    51,    54,    71,    72,     3,    10,    18,    38,     3,
	3,    55,    63,    64,     3,     3,     3,     3,     3,     5,
	3,     3,     3,     3,     3,     3,     3,     3,     5,     3,
	5,     3,     3,     3,     5,    63,     5,    67,    68,    67,
	0,    72,    67,    67,    67,     6,    57,    66,    67,    67,
	5,    67,    67,    67,     3,    67,     5,    67,     5,    67,
	67,     3,     5,     5,     3,    69,    70,    65,    66,     5,
	5,     5,     5,     3,     5,    55,    58,    59,    60,    63,
	69,     7,    66,     5,     5,    67,     6,    56,    62,     5,
	61,    62,     3,     5,     7,    62,     5,     5,     5
};

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
	0,    53,    54,    55,    56,    57,    58,    58,    59,    59,
	60,    61,    61,    62,    63,    63,    64,    65,    65,    66,
	67,    68,    69,    69,    70,    71,    71,    72,    72,    72,
	72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
	72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
	72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
	72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
	72
};

/* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
	0,     2,     1,     0,     0,     0,     1,     1,     4,     1,
	2,     2,     1,     2,     4,     1,     2,     2,     1,     2,
	2,     0,     2,     0,     2,     2,     1,     3,     2,     2,
	2,     3,     1,     1,     3,     2,     2,     2,     3,     1,
	2,     3,    10,     3,     3,     3,     2,     3,     2,     2,
	1,     2,     3,     1,     5,     3,     4,     3,     3,     5,
	1,     1,     3,     2,     4,     1,     1,     2,     1,     1,
	2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
	{                                                                     \
	  YYFPRINTF (stderr, "%s ", Title);                                   \
	  yy_symbol_print (stderr,                                            \
				  Type, Value); \
	  YYFPRINTF (stderr, "\n");                                           \
	}                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print(FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
	FILE *yyo = yyoutput;
	YYUSE(yyo);
	if (!yyvaluep)
		return;
# ifdef YYPRINT
	if (yytype < YYNTOKENS)
		YYPRINT(yyoutput, yytoknum[yytype], *yyvaluep);
# endif
	YYUSE(yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print(FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
	YYFPRINTF(yyoutput, "%s %s (",
		yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

	yy_symbol_value_print(yyoutput, yytype, yyvaluep);
	YYFPRINTF(yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print(yytype_int16 *yybottom, yytype_int16 *yytop)
{
	YYFPRINTF(stderr, "Stack now");
	for (; yybottom <= yytop; yybottom++)
	{
		int yybot = *yybottom;
		YYFPRINTF(stderr, " %d", yybot);
	}
	YYFPRINTF(stderr, "\n");
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
yy_reduce_print(yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
	unsigned long int yylno = yyrline[yyrule];
	int yynrhs = yyr2[yyrule];
	int yyi;
	YYFPRINTF(stderr, "Reducing stack by rule %d (line %lu):\n",
		yyrule - 1, yylno);
	/* The symbols being reduced.  */
	for (yyi = 0; yyi < yynrhs; yyi++)
	{
		YYFPRINTF(stderr, "   $%d = ", yyi + 1);
		yy_symbol_print(stderr,
			yystos[yyssp[yyi + 1 - yynrhs]],
			&(yyvsp[(yyi + 1) - (yynrhs)])
		);
		YYFPRINTF(stderr, "\n");
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen(const char *yystr)
{
	YYSIZE_T yylen;
	for (yylen = 0; yystr[yylen]; yylen++)
		continue;
	return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
YYDEST.  */
static char *
yystpcpy(char *yydest, const char *yysrc)
{
	char *yyd = yydest;
	const char *yys = yysrc;

	while ((*yyd++ = *yys++) != '\0')
		continue;

	return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
quotes and backslashes, so that it's suitable for yyerror.  The
heuristic is that double-quoting is unnecessary unless the string
contains an apostrophe, a comma, or backslash (other than
backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
null, do not copy; instead, return the length of what the result
would have been.  */
static YYSIZE_T
yytnamerr(char *yyres, const char *yystr)
{
	if (*yystr == '"')
	{
		YYSIZE_T yyn = 0;
		char const *yyp = yystr;

		for (;;)
			switch (*++yyp)
			{
			case '\'':
			case ',':
				goto do_not_strip_quotes;

			case '\\':
				if (*++yyp != '\\')
					goto do_not_strip_quotes;
				/* Fall through.  */
			default:
				if (yyres)
					yyres[yyn] = *yyp;
				yyn++;
				break;

			case '"':
				if (yyres)
					yyres[yyn] = '\0';
				return yyn;
			}
	do_not_strip_quotes:;
	}

	if (!yyres)
		return yystrlen(yystr);

	return yystpcpy(yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
about the unexpected token YYTOKEN for the state stack whose top is
YYSSP.

Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
not large enough to hold the message.  In that case, also set
*YYMSG_ALLOC to the required number of bytes.  Return 2 if the
required number of bytes is too large to store.  */
static int
yysyntax_error(YYSIZE_T *yymsg_alloc, char **yymsg,
	yytype_int16 *yyssp, int yytoken)
{
	YYSIZE_T yysize0 = yytnamerr(YY_NULLPTR, yytname[yytoken]);
	YYSIZE_T yysize = yysize0;
	enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
	/* Internationalized format string. */
	const char *yyformat = YY_NULLPTR;
	/* Arguments of yyformat. */
	char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	/* Number of reported tokens (one for the "unexpected", one per
	"expected"). */
	int yycount = 0;

	/* There are many possibilities here to consider:
	- If this state is a consistent state with a default action, then
	the only way this function was invoked is if the default action
	is an error action.  In that case, don't check for expected
	tokens because there are none.
	- The only way there can be no lookahead present (in yychar) is if
	this state is a consistent state with a default action.  Thus,
	detecting the absence of a lookahead is sufficient to determine
	that there is no unexpected or expected token to report.  In that
	case, just report a simple "syntax error".
	- Don't assume there isn't a lookahead just because this state is a
	consistent state with a default action.  There might have been a
	previous inconsistent state, consistent state with a non-default
	action, or user semantic action that manipulated yychar.
	- Of course, the expected token list depends on states to have
	correct lookahead information, and it depends on the parser not
	to perform extra reductions after fetching a lookahead from the
	scanner and before detecting a syntax error.  Thus, state merging
	(from LALR or IELR) and default reductions corrupt the expected
	token list.  However, the list is correct for canonical LR with
	one exception: it will still contain any token that will not be
	accepted due to an error action in a later state.
	*/
	if (yytoken != YYEMPTY)
	{
		int yyn = yypact[*yyssp];
		yyarg[yycount++] = yytname[yytoken];
		if (!yypact_value_is_default(yyn))
		{
			/* Start YYX at -YYN if negative to avoid negative indexes in
			YYCHECK.  In other words, skip the first -YYN actions for
			this state because they are default actions.  */
			int yyxbegin = yyn < 0 ? -yyn : 0;
			/* Stay within bounds of both yycheck and yytname.  */
			int yychecklim = YYLAST - yyn + 1;
			int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
			int yyx;

			for (yyx = yyxbegin; yyx < yyxend; ++yyx)
				if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
					&& !yytable_value_is_error(yytable[yyx + yyn]))
				{
					if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
					{
						yycount = 1;
						yysize = yysize0;
						break;
					}
					yyarg[yycount++] = yytname[yyx];
					{
						YYSIZE_T yysize1 = yysize + yytnamerr(YY_NULLPTR, yytname[yyx]);
						if (!(yysize <= yysize1
							&& yysize1 <= YYSTACK_ALLOC_MAXIMUM))
							return 2;
						yysize = yysize1;
					}
				}
		}
	}

	switch (yycount)
	{
# define YYCASE_(N, S)                      \
	  case N:                               \
		yyformat = S;                       \
	  break
		YYCASE_(0, YY_("syntax error"));
		YYCASE_(1, YY_("syntax error, unexpected %s"));
		YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
		YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
		YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
		YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
	}

	{
		YYSIZE_T yysize1 = yysize + yystrlen(yyformat);
		if (!(yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
			return 2;
		yysize = yysize1;
	}

	if (*yymsg_alloc < yysize)
	{
		*yymsg_alloc = 2 * yysize;
		if (!(yysize <= *yymsg_alloc
			&& *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
			*yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
		return 1;
	}

	/* Avoid sprintf, as that infringes on the user's name space.
	Don't have undefined behavior even if the translation
	produced a string with the wrong number of "%s"s.  */
	{
		char *yyp = *yymsg;
		int yyi = 0;
		while ((*yyp = *yyformat) != '\0')
			if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
			{
				yyp += yytnamerr(yyp, yyarg[yyi++]);
				yyformat += 2;
			}
			else
			{
				yyp++;
				yyformat++;
			}
	}
	return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct(const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
	YYUSE(yyvaluep);
	if (!yymsg)
		yymsg = "Deleting";
	YY_SYMBOL_PRINT(yymsg, yytype, yyvaluep, yylocationp);

	YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
		YYUSE(yytype);
	YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse(void)
{
	int yystate;
	/* Number of tokens to shift before error messages enabled.  */
	int yyerrstatus;

	/* The stacks and their tools:
	'yyss': related to states.
	'yyvs': related to semantic values.

	Refer to the stacks through separate pointers, to allow yyoverflow
	to reallocate them elsewhere.  */

	/* The state stack.  */
	yytype_int16 yyssa[YYINITDEPTH];
	yytype_int16 *yyss;
	yytype_int16 *yyssp;

	/* The semantic value stack.  */
	YYSTYPE yyvsa[YYINITDEPTH];
	YYSTYPE *yyvs;
	YYSTYPE *yyvsp;

	YYSIZE_T yystacksize;

	int yyn;
	int yyresult;
	/* Lookahead token as an internal (translated) token number.  */
	int yytoken = 0;
	/* The variables used to return semantic value and location from the
	action routines.  */
	YYSTYPE yyval;

#if YYERROR_VERBOSE
	/* Buffer for error messages, and its allocated size.  */
	char yymsgbuf[128];
	char *yymsg = yymsgbuf;
	YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

	/* The number of symbols on the RHS of the reduced rule.
	Keep to zero when no symbol should be popped.  */
	int yylen = 0;

	yyssp = yyss = yyssa;
	yyvsp = yyvs = yyvsa;
	yystacksize = YYINITDEPTH;

	YYDPRINTF((stderr, "Starting parse\n"));

	yystate = 0;
	yyerrstatus = 0;
	yynerrs = 0;
	yychar = YYEMPTY; /* Cause a token to be read.  */
	goto yysetstate;

	/*------------------------------------------------------------.
	| yynewstate -- Push a new state, which is found in yystate.  |
	`------------------------------------------------------------*/
yynewstate:
	/* In all cases, when you get here, the value and location stacks
	have just been pushed.  So pushing a state here evens the stacks.  */
	yyssp++;

yysetstate:
	*yyssp = yystate;

	if (yyss + yystacksize - 1 <= yyssp)
	{
		/* Get the current used size of the three stacks, in elements.  */
		YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
		{
			/* Give user a chance to reallocate the stack.  Use copies of
			these so that the &'s don't force the real ones into
			memory.  */
			YYSTYPE *yyvs1 = yyvs;
			yytype_int16 *yyss1 = yyss;

			/* Each stack pointer address is followed by the size of the
			data in use in that stack, in bytes.  This used to be a
			conditional around just the two extra args, but that might
			be undefined if yyoverflow is a macro.  */
			yyoverflow(YY_("memory exhausted"),
				&yyss1, yysize * sizeof(*yyssp),
				&yyvs1, yysize * sizeof(*yyvsp),
				&yystacksize);

			yyss = yyss1;
			yyvs = yyvs1;
		}
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
		goto yyexhaustedlab;
# else
		/* Extend the stack our own way.  */
		if (YYMAXDEPTH <= yystacksize)
			goto yyexhaustedlab;
		yystacksize *= 2;
		if (YYMAXDEPTH < yystacksize)
			yystacksize = YYMAXDEPTH;

		{
			yytype_int16 *yyss1 = yyss;
			union yyalloc *yyptr =
				(union yyalloc *) YYSTACK_ALLOC(YYSTACK_BYTES(yystacksize));
			if (!yyptr)
				goto yyexhaustedlab;
			YYSTACK_RELOCATE(yyss_alloc, yyss);
			YYSTACK_RELOCATE(yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
			if (yyss1 != yyssa)
				YYSTACK_FREE(yyss1);
		}
# endif
#endif /* no yyoverflow */

		yyssp = yyss + yysize - 1;
		yyvsp = yyvs + yysize - 1;

		YYDPRINTF((stderr, "Stack size increased to %lu\n",
			(unsigned long int) yystacksize));

		if (yyss + yystacksize - 1 <= yyssp)
			YYABORT;
	}

	YYDPRINTF((stderr, "Entering state %d\n", yystate));

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
	if (yypact_value_is_default(yyn))
		goto yydefault;

	/* Not known => get a lookahead token if don't already have one.  */

	/* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
	if (yychar == YYEMPTY)
	{
		YYDPRINTF((stderr, "Reading a token: "));
		yychar = yylex();
	}

	if (yychar <= YYEOF)
	{
		yychar = yytoken = YYEOF;
		YYDPRINTF((stderr, "Now at end of input.\n"));
	}
	else
	{
		yytoken = YYTRANSLATE(yychar);
		YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
	}

	/* If the proper action on seeing token YYTOKEN is to reduce or to
	detect an error, take that action.  */
	yyn += yytoken;
	if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
		goto yydefault;
	yyn = yytable[yyn];
	if (yyn <= 0)
	{
		if (yytable_value_is_error(yyn))
			goto yyerrlab;
		yyn = -yyn;
		goto yyreduce;
	}

	/* Count tokens shifted since error; after three, turn off error
	status.  */
	if (yyerrstatus)
		yyerrstatus--;

	/* Shift the lookahead token.  */
	YY_SYMBOL_PRINT("Shifting", yytoken, &yylval, &yylloc);

	/* Discard the shifted token.  */
	yychar = YYEMPTY;

	yystate = yyn;
	YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
		*++yyvsp = yylval;
	YY_IGNORE_MAYBE_UNINITIALIZED_END

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
	| yyreduce -- Do a reduction.  |
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
	yyval = yyvsp[1 - yylen];


	YY_REDUCE_PRINT(yyn);
	switch (yyn)
	{
	case 2:
#line 187 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
	}
#line 1486 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 3:
#line 193 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		CHECK(pbrt::cur_array == nullptr);
		pbrt::cur_array = new pbrt::ParamArray;
	}
#line 1495 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 4:
#line 201 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::cur_array->element_size = sizeof(const char *);
		pbrt::cur_array->isString = true;
	}
#line 1504 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 5:
#line 209 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::cur_array->element_size = sizeof(double);
		pbrt::cur_array->isString = false;
	}
#line 1513 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 6:
#line 217 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		(yyval.ribarray) = (yyvsp[0].ribarray);
	}
#line 1521 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 7:
#line 223 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		(yyval.ribarray) = (yyvsp[0].ribarray);
	}
#line 1529 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 8:
#line 230 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		(yyval.ribarray) = pbrt::cur_array;
		pbrt::cur_array = nullptr;
	}
#line 1538 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 9:
#line 237 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		(yyval.ribarray) = pbrt::cur_array;
		pbrt::cur_array = nullptr;
	}
#line 1547 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 10:
#line 245 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
	}
#line 1554 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 11:
#line 251 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
	}
#line 1561 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 12:
#line 256 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
	}
#line 1568 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 13:
#line 262 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		char *to_add = strdup((yyvsp[0].string));
		pbrt::AddArrayElement(&to_add);
	}
#line 1577 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 14:
#line 270 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		(yyval.ribarray) = pbrt::cur_array;
		pbrt::cur_array = nullptr;
	}
#line 1586 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 15:
#line 277 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		(yyval.ribarray) = pbrt::cur_array;
		pbrt::cur_array = nullptr;
	}
#line 1595 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 16:
#line 285 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
	}
#line 1602 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 17:
#line 291 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
	}
#line 1609 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 18:
#line 296 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
	}
#line 1616 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 19:
#line 302 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		double to_add = (yyvsp[0].num);
		pbrt::AddArrayElement(&to_add);
	}
#line 1625 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 20:
#line 310 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
	}
#line 1632 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 21:
#line 316 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		for (size_t i = 0; i < pbrt::cur_paramlist.size(); ++i) {
			if (pbrt::cur_paramlist[i].isString) {
				for (size_t j = 0; j < pbrt::cur_paramlist[i].size; ++j)
					free(((char **)pbrt::cur_paramlist[i].arg)[j]);
			}
		}
		pbrt::cur_paramlist.erase(pbrt::cur_paramlist.begin(), pbrt::cur_paramlist.end());
	}
#line 1646 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 22:
#line 329 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
	}
#line 1653 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 23:
#line 334 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
	}
#line 1660 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 24:
#line 340 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::cur_paramlist.push_back(pbrt::ParamListItem((yyvsp[-1].string), (yyvsp[0].ribarray)));
		pbrt::ArrayFree((yyvsp[0].ribarray));
	}
#line 1669 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 25:
#line 348 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
	}
#line 1676 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 26:
#line 353 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
	}
#line 1683 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 27:
#line 359 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Reflectance);
		pbrt::pbrtAccelerator((yyvsp[-1].string), params);
		pbrt::FreeArgs();
	}
#line 1694 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 28:
#line 368 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtActiveTransformAll();
	}
#line 1702 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 29:
#line 374 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtActiveTransformEndTime();
	}
#line 1710 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 30:
#line 380 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtActiveTransformStartTime();
	}
#line 1718 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 31:
#line 386 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Illuminant);
		pbrt::pbrtAreaLightSource((yyvsp[-1].string), params);
		pbrt::FreeArgs();
	}
#line 1729 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 32:
#line 395 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtAttributeBegin();
	}
#line 1737 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 33:
#line 401 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtAttributeEnd();
	}
#line 1745 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 34:
#line 407 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Reflectance);
		pbrt::pbrtCamera((yyvsp[-1].string), params);
		pbrt::FreeArgs();
	}
#line 1756 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 35:
#line 416 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		if (pbrt::VerifyArrayLength((yyvsp[0].ribarray), 16, "ConcatTransform")) {
			pbrt::Float m[16];
			double *dm = (double *)(yyvsp[0].ribarray)->array;
			std::copy(dm, dm + 16, m);
			pbrt::pbrtConcatTransform(m);
		}
		pbrt::ArrayFree((yyvsp[0].ribarray));
	}
#line 1770 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 36:
#line 428 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtCoordinateSystem((yyvsp[0].string));
	}
#line 1778 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 37:
#line 434 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtCoordSysTransform((yyvsp[0].string));
	}
#line 1786 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 38:
#line 440 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Reflectance);
		pbrt::pbrtFilm((yyvsp[-1].string), params);
		pbrt::FreeArgs();
	}
#line 1797 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 39:
#line 449 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtIdentity();
	}
#line 1805 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 40:
#line 455 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::include_push((yyvsp[0].string));
	}
#line 1813 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 41:
#line 461 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Illuminant);
		pbrt::pbrtLightSource((yyvsp[-1].string), params);
		pbrt::FreeArgs();
	}
#line 1824 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 42:
#line 470 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtLookAt((yyvsp[-8].num), (yyvsp[-7].num), (yyvsp[-6].num), (yyvsp[-5].num), (yyvsp[-4].num), (yyvsp[-3].num), (yyvsp[-2].num), (yyvsp[-1].num), (yyvsp[0].num));
	}
#line 1832 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 43:
#line 476 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Reflectance);
		pbrt::pbrtMakeNamedMaterial((yyvsp[-1].string), params);
		pbrt::FreeArgs();
	}
#line 1843 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 44:
#line 485 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Reflectance);
		pbrt::pbrtMakeNamedMedium((yyvsp[-1].string), params);
		pbrt::FreeArgs();
	}
#line 1854 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 45:
#line 494 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Reflectance);
		pbrt::pbrtMaterial((yyvsp[-1].string), params);
		pbrt::FreeArgs();
	}
#line 1865 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 46:
#line 503 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtMediumInterface((yyvsp[0].string), (yyvsp[0].string));
	}
#line 1873 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 47:
#line 509 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtMediumInterface((yyvsp[-1].string), (yyvsp[0].string));
	}
#line 1881 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 48:
#line 515 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtNamedMaterial((yyvsp[0].string));
	}
#line 1889 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 49:
#line 521 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtObjectBegin((yyvsp[0].string));
	}
#line 1897 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 50:
#line 527 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtObjectEnd();
	}
#line 1905 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 51:
#line 533 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtObjectInstance((yyvsp[0].string));
	}
#line 1913 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 52:
#line 539 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Reflectance);
		pbrt::pbrtPixelFilter((yyvsp[-1].string), params);
		pbrt::FreeArgs();
	}
#line 1924 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 53:
#line 548 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtReverseOrientation();
	}
#line 1932 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 54:
#line 554 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtRotate((yyvsp[-3].num), (yyvsp[-2].num), (yyvsp[-1].num), (yyvsp[0].num));
	}
#line 1940 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 55:
#line 560 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Reflectance);
		pbrt::pbrtSampler((yyvsp[-1].string), params);
		pbrt::FreeArgs();
	}
#line 1951 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 56:
#line 569 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtScale((yyvsp[-2].num), (yyvsp[-1].num), (yyvsp[0].num));
	}
#line 1959 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 57:
#line 575 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Reflectance);
		pbrt::pbrtShape((yyvsp[-1].string), params);
		pbrt::FreeArgs();
	}
#line 1970 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 58:
#line 584 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Reflectance);
		pbrt::pbrtIntegrator((yyvsp[-1].string), params);
		pbrt::FreeArgs();
	}
#line 1981 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 59:
#line 593 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Reflectance);
		pbrt::pbrtTexture((yyvsp[-3].string), (yyvsp[-2].string), (yyvsp[-1].string), params);
		pbrt::FreeArgs();
	}
#line 1992 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 60:
#line 602 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtTransformBegin();
	}
#line 2000 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 61:
#line 608 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtTransformEnd();
	}
#line 2008 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 62:
#line 614 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtTransformTimes((yyvsp[-1].num), (yyvsp[0].num));
	}
#line 2016 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 63:
#line 620 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		if (pbrt::VerifyArrayLength((yyvsp[0].ribarray), 16, "Transform")) {
			pbrt::Float m[16];
			double *dm = (double *)(yyvsp[0].ribarray)->array;
			std::copy(dm, dm + 16, m);
			pbrt::pbrtTransform(m);
		}
		pbrt::ArrayFree((yyvsp[0].ribarray));
	}
#line 2030 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 64:
#line 632 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtTranslate((yyvsp[-2].num), (yyvsp[-1].num), (yyvsp[0].num));
	}
#line 2038 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 65:
#line 638 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtWorldBegin();
	}
#line 2046 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 66:
#line 644 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::pbrtWorldEnd();
	}
#line 2054 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 67:
#line 649 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Reflectance);
		pbrt::cgraScenePhoto(params);
		pbrt::FreeArgs();
	}
#line 2065 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 68:
#line 657 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::cgraSynthSceneBegin();
	}
#line 2073 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 69:
#line 662 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::cgraSynthSceneEnd();
	}
#line 2081 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;

	case 70:
#line 667 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1646  */
	{
		pbrt::ParamSet params;
		pbrt::InitParamSet(params, pbrt::SpectrumType::Reflectance);
		pbrt::cgraAnimation(params);
		pbrt::FreeArgs();
	}
#line 2092 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
	break;


#line 2096 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/build/pbrtparse.c" /* yacc.c:1646  */
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
	YY_SYMBOL_PRINT("-> $$ =", yyr1[yyn], &yyval, &yyloc);

	YYPOPSTACK(yylen);
	yylen = 0;
	YY_STACK_PRINT(yyss, yyssp);

	*++yyvsp = yyval;

	/* Now 'shift' the result of the reduction.  Determine what state
	that goes to, based on the state we popped back to and the rule
	number reduced by.  */

	yyn = yyr1[yyn];

	yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
	if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
		yystate = yytable[yystate];
	else
		yystate = yydefgoto[yyn - YYNTOKENS];

	goto yynewstate;


	/*--------------------------------------.
	| yyerrlab -- here on detecting error.  |
	`--------------------------------------*/
yyerrlab:
	/* Make sure we have latest lookahead translation.  See comments at
	user semantic actions for why this is necessary.  */
	yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE(yychar);

	/* If not already recovering from an error, report this error.  */
	if (!yyerrstatus)
	{
		++yynerrs;
#if ! YYERROR_VERBOSE
		yyerror(YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
										yyssp, yytoken)
		{
			char const *yymsgp = YY_("syntax error");
			int yysyntax_error_status;
			yysyntax_error_status = YYSYNTAX_ERROR;
			if (yysyntax_error_status == 0)
				yymsgp = yymsg;
			else if (yysyntax_error_status == 1)
			{
				if (yymsg != yymsgbuf)
					YYSTACK_FREE(yymsg);
				yymsg = (char *)YYSTACK_ALLOC(yymsg_alloc);
				if (!yymsg)
				{
					yymsg = yymsgbuf;
					yymsg_alloc = sizeof yymsgbuf;
					yysyntax_error_status = 2;
				}
				else
				{
					yysyntax_error_status = YYSYNTAX_ERROR;
					yymsgp = yymsg;
				}
			}
			yyerror(yymsgp);
			if (yysyntax_error_status == 2)
				goto yyexhaustedlab;
		}
# undef YYSYNTAX_ERROR
#endif
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
			yydestruct("Error: discarding",
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

	/* Pacify compilers like GCC when the user code never invokes
	YYERROR and the label yyerrorlab therefore never appears in user
	code.  */
	if (/*CONSTCOND*/ 0)
		goto yyerrorlab;

	/* Do not reclaim the symbols of the rule whose action triggered
	this YYERROR.  */
	YYPOPSTACK(yylen);
	yylen = 0;
	YY_STACK_PRINT(yyss, yyssp);
	yystate = *yyssp;
	goto yyerrlab1;


	/*-------------------------------------------------------------.
	| yyerrlab1 -- common code for both syntax error and YYERROR.  |
	`-------------------------------------------------------------*/
yyerrlab1:
	yyerrstatus = 3;      /* Each real token shifted decrements this.  */

	for (;;)
	{
		yyn = yypact[yystate];
		if (!yypact_value_is_default(yyn))
		{
			yyn += YYTERROR;
			if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
			{
				yyn = yytable[yyn];
				if (0 < yyn)
					break;
			}
		}

		/* Pop the current state because it cannot handle the error token.  */
		if (yyssp == yyss)
			YYABORT;


		yydestruct("Error: popping",
			yystos[yystate], yyvsp);
		YYPOPSTACK(1);
		yystate = *yyssp;
		YY_STACK_PRINT(yyss, yyssp);
	}

	YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
		*++yyvsp = yylval;
	YY_IGNORE_MAYBE_UNINITIALIZED_END


		/* Shift the error token.  */
		YY_SYMBOL_PRINT("Shifting", yystos[yyn], yyvsp, yylsp);

	yystate = yyn;
	goto yynewstate;


	/*-------------------------------------.
	| yyacceptlab -- YYACCEPT comes here.  |
	`-------------------------------------*/
yyacceptlab:
	yyresult = 0;
	goto yyreturn;

	/*-----------------------------------.
	| yyabortlab -- YYABORT comes here.  |
	`-----------------------------------*/
yyabortlab:
	yyresult = 1;
	goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
	/*-------------------------------------------------.
	| yyexhaustedlab -- memory exhaustion comes here.  |
	`-------------------------------------------------*/
	yyexhaustedlab :
				   yyerror(YY_("memory exhausted"));
				   yyresult = 2;
				   /* Fall through.  */
#endif

			   yyreturn:
				   if (yychar != YYEMPTY)
				   {
					   /* Make sure we have latest lookahead translation.  See comments at
					   user semantic actions for why this is necessary.  */
					   yytoken = YYTRANSLATE(yychar);
					   yydestruct("Cleanup: discarding lookahead",
						   yytoken, &yylval);
				   }
				   /* Do not reclaim the symbols of the rule whose action triggered
				   this YYABORT or YYACCEPT.  */
				   YYPOPSTACK(yylen);
				   YY_STACK_PRINT(yyss, yyssp);
				   while (yyssp != yyss)
				   {
					   yydestruct("Cleanup: popping",
						   yystos[*yyssp], yyvsp);
					   YYPOPSTACK(1);
				   }
#ifndef yyoverflow
				   if (yyss != yyssa)
					   YYSTACK_FREE(yyss);
#endif
#if YYERROR_VERBOSE
				   if (yymsg != yymsgbuf)
					   YYSTACK_FREE(yymsg);
#endif
				   return yyresult;
}
#line 674 "C:/Data/Victoria/CGRA408_Rendering/FinalProject/CGRA408_FinalProject/pbrt-v3/src/core/pbrtparse.y" /* yacc.c:1906  */


namespace pbrt {

	static const char *paramTypeToName(int type) {
		switch (type) {
		case PARAM_TYPE_INT: return "int";
		case PARAM_TYPE_BOOL: return "bool";
		case PARAM_TYPE_FLOAT: return "float";
		case PARAM_TYPE_POINT2: return "point2";
		case PARAM_TYPE_VECTOR2: return "vector2";
		case PARAM_TYPE_POINT3: return "point3";
		case PARAM_TYPE_VECTOR3: return "vector3";
		case PARAM_TYPE_NORMAL: return "normal";
		case PARAM_TYPE_RGB: return "rgb/color";
		case PARAM_TYPE_XYZ: return "xyz";
		case PARAM_TYPE_BLACKBODY: return "blackbody";
		case PARAM_TYPE_SPECTRUM: return "spectrum";
		case PARAM_TYPE_STRING: return "string";
		case PARAM_TYPE_TEXTURE: return "texture";
		default: LOG(FATAL) << "Error in paramTypeToName"; return nullptr;
		}
	}


	static void InitParamSet(ParamSet &ps, SpectrumType type) {
		ps.Clear();
		for (size_t i = 0; i < cur_paramlist.size(); ++i) {
			int type;
			std::string name;
			if (lookupType(cur_paramlist[i].name, &type, name)) {
				if (type == PARAM_TYPE_TEXTURE || type == PARAM_TYPE_STRING ||
					type == PARAM_TYPE_BOOL) {
					if (!cur_paramlist[i].isString) {
						Error("Expected string parameter value for parameter "
							"\"%s\" with type \"%s\". Ignoring.",
							name.c_str(), paramTypeToName(type));
						continue;
					}
				}
				else if (type != PARAM_TYPE_SPECTRUM) { /* spectrum can be either... */
					if (cur_paramlist[i].isString) {
						Error("Expected numeric parameter value for parameter "
							"\"%s\" with type \"%s\".  Ignoring.",
							name.c_str(), paramTypeToName(type));
						continue;
					}
				}
				void *data = cur_paramlist[i].arg;
				int nItems = cur_paramlist[i].size;
				if (type == PARAM_TYPE_INT) {
					// parser doesn't handle ints, so convert from doubles here....
					int nAlloc = nItems;
					std::unique_ptr<int[]> idata(new int[nAlloc]);
					double *fdata = (double *)cur_paramlist[i].arg;
					for (int j = 0; j < nAlloc; ++j)
						idata[j] = int(fdata[j]);
					ps.AddInt(name, std::move(idata), nItems);
				}
				else if (type == PARAM_TYPE_BOOL) {
					// strings -> bools
					int nAlloc = cur_paramlist[i].size;
					std::unique_ptr<bool[]> bdata(new bool[nAlloc]);
					for (int j = 0; j < nAlloc; ++j) {
						std::string s(((const char **)data)[j]);
						if (s == "true") bdata[j] = true;
						else if (s == "false") bdata[j] = false;
						else {
							Warning("Value \"%s\" unknown for Boolean parameter \"%s\"."
								"Using \"false\".", s.c_str(), cur_paramlist[i].name);
							bdata[j] = false;
						}
					}
					ps.AddBool(name, std::move(bdata), nItems);
				}
				else if (type == PARAM_TYPE_FLOAT) {
					std::unique_ptr<Float[]> floats(new Float[nItems]);
					for (int i = 0; i < nItems; ++i)
						floats[i] = ((double *)data)[i];
					ps.AddFloat(name, std::move(floats), nItems);
				}
				else if (type == PARAM_TYPE_POINT2) {
					if ((nItems % 2) != 0)
						Warning("Excess values given with point2 parameter \"%s\". "
							"Ignoring last one of them.", cur_paramlist[i].name);
					std::unique_ptr<Point2f[]> pts(new Point2f[nItems / 2]);
					for (int i = 0; i < nItems / 2; ++i) {
						pts[i].x = ((double *)data)[2 * i];
						pts[i].y = ((double *)data)[2 * i + 1];
					}
					ps.AddPoint2f(name, std::move(pts), nItems / 2);
				}
				else if (type == PARAM_TYPE_VECTOR2) {
					if ((nItems % 2) != 0)
						Warning("Excess values given with vector2 parameter \"%s\". "
							"Ignoring last one of them.", cur_paramlist[i].name);
					std::unique_ptr<Vector2f[]> vecs(new Vector2f[nItems / 2]);
					for (int i = 0; i < nItems / 2; ++i) {
						vecs[i].x = ((double *)data)[2 * i];
						vecs[i].y = ((double *)data)[2 * i + 1];
					}
					ps.AddVector2f(name, std::move(vecs), nItems / 2);
				}
				else if (type == PARAM_TYPE_POINT3) {
					if ((nItems % 3) != 0)
						Warning("Excess values given with point3 parameter \"%s\". "
							"Ignoring last %d of them.", cur_paramlist[i].name,
							nItems % 3);
					std::unique_ptr<Point3f[]> pts(new Point3f[nItems / 3]);
					for (int i = 0; i < nItems / 3; ++i) {
						pts[i].x = ((double *)data)[3 * i];
						pts[i].y = ((double *)data)[3 * i + 1];
						pts[i].z = ((double *)data)[3 * i + 2];
					}
					ps.AddPoint3f(name, std::move(pts), nItems / 3);
				}
				else if (type == PARAM_TYPE_VECTOR3) {
					if ((nItems % 3) != 0)
						Warning("Excess values given with vector3 parameter \"%s\". "
							"Ignoring last %d of them.", cur_paramlist[i].name,
							nItems % 3);
					std::unique_ptr<Vector3f[]> vecs(new Vector3f[nItems / 3]);
					for (int j = 0; j < nItems / 3; ++j) {
						vecs[j].x = ((double *)data)[3 * j];
						vecs[j].y = ((double *)data)[3 * j + 1];
						vecs[j].z = ((double *)data)[3 * j + 2];
					}
					ps.AddVector3f(name, std::move(vecs), nItems / 3);
				}
				else if (type == PARAM_TYPE_NORMAL) {
					if ((nItems % 3) != 0)
						Warning("Excess values given with \"normal\" parameter \"%s\". "
							"Ignoring last %d of them.", cur_paramlist[i].name,
							nItems % 3);
					std::unique_ptr<Normal3f[]> normals(new Normal3f[nItems / 3]);
					for (int j = 0; j < nItems / 3; ++j) {
						normals[j].x = ((double *)data)[3 * j];
						normals[j].y = ((double *)data)[3 * j + 1];
						normals[j].z = ((double *)data)[3 * j + 2];
					}
					ps.AddNormal3f(name, std::move(normals), nItems / 3);
				}
				else if (type == PARAM_TYPE_RGB) {
					if ((nItems % 3) != 0) {
						Warning("Excess RGB values given with parameter \"%s\". "
							"Ignoring last %d of them", cur_paramlist[i].name,
							nItems % 3);
						nItems -= nItems % 3;
					}
					std::unique_ptr<Float[]> floats(new Float[nItems]);
					for (int j = 0; j < nItems; ++j)
						floats[j] = ((double *)data)[j];
					ps.AddRGBSpectrum(name, std::move(floats), nItems);
				}
				else if (type == PARAM_TYPE_XYZ) {
					if ((nItems % 3) != 0) {
						Warning("Excess XYZ values given with parameter \"%s\". "
							"Ignoring last %d of them", cur_paramlist[i].name,
							nItems % 3);
						nItems -= nItems % 3;
					}
					std::unique_ptr<Float[]> floats(new Float[nItems]);
					for (int j = 0; j < nItems; ++j)
						floats[j] = ((double *)data)[j];
					ps.AddXYZSpectrum(name, std::move(floats), nItems);
				}
				else if (type == PARAM_TYPE_BLACKBODY) {
					if ((nItems % 2) != 0) {
						Warning("Excess value given with blackbody parameter \"%s\". "
							"Ignoring extra one.", cur_paramlist[i].name);
						nItems -= nItems % 2;
					}
					std::unique_ptr<Float[]> floats(new Float[nItems]);
					for (int j = 0; j < nItems; ++j)
						floats[j] = ((double *)data)[j];
					ps.AddBlackbodySpectrum(name, std::move(floats), nItems);
				}
				else if (type == PARAM_TYPE_SPECTRUM) {
					if (cur_paramlist[i].isString) {
						ps.AddSampledSpectrumFiles(name, (const char **)data, nItems);
					}
					else {
						if ((nItems % 2) != 0) {
							Warning("Non-even number of values given with sampled spectrum "
								"parameter \"%s\". Ignoring extra.",
								cur_paramlist[i].name);
							nItems -= nItems % 2;
						}
						std::unique_ptr<Float[]> floats(new Float[nItems]);
						for (int j = 0; j < nItems; ++j)
							floats[j] = ((double *)data)[j];
						ps.AddSampledSpectrum(name, std::move(floats), nItems);
					}
				}
				else if (type == PARAM_TYPE_STRING) {
					std::unique_ptr<std::string[]> strings(new std::string[nItems]);
					for (int j = 0; j < nItems; ++j)
						strings[j] = std::string(((const char **)data)[j]);
					ps.AddString(name, std::move(strings), nItems);
				}
				else if (type == PARAM_TYPE_TEXTURE) {
					if (nItems == 1) {
						std::string val(*((const char **)data));
						ps.AddTexture(name, val);
					}
					else
						Error("Only one string allowed for \"texture\" parameter \"%s\"",
							name.c_str());
				}
			}
			else
				Warning("Type of parameter \"%s\" is unknown", cur_paramlist[i].name);
		}
	}


	static bool lookupType(const char *name, int *type, std::string &sname) {
		CHECK_NOTNULL(name);
		*type = 0;
		const char *strp = name;
		while (*strp && isspace(*strp))
			++strp;
		if (!*strp) {
			Error("Parameter \"%s\" doesn't have a type declaration?!", name);
			return false;
		}
#define TRY_DECODING_TYPE(name, mask) \
		if (strncmp(name, strp, strlen(name)) == 0) { \
			*type = mask; strp += strlen(name); \
		}
		TRY_DECODING_TYPE("float", PARAM_TYPE_FLOAT)
		else TRY_DECODING_TYPE("integer", PARAM_TYPE_INT)
		else TRY_DECODING_TYPE("bool", PARAM_TYPE_BOOL)
	else TRY_DECODING_TYPE("point2", PARAM_TYPE_POINT2)
	else TRY_DECODING_TYPE("vector2", PARAM_TYPE_VECTOR2)
	else TRY_DECODING_TYPE("point3", PARAM_TYPE_POINT3)
	else TRY_DECODING_TYPE("vector3", PARAM_TYPE_VECTOR3)
	else TRY_DECODING_TYPE("point", PARAM_TYPE_POINT3)
	else TRY_DECODING_TYPE("vector", PARAM_TYPE_VECTOR3)
	else TRY_DECODING_TYPE("normal", PARAM_TYPE_NORMAL)
	else TRY_DECODING_TYPE("string", PARAM_TYPE_STRING)
	else TRY_DECODING_TYPE("texture", PARAM_TYPE_TEXTURE)
	else TRY_DECODING_TYPE("color", PARAM_TYPE_RGB)
	else TRY_DECODING_TYPE("rgb", PARAM_TYPE_RGB)
	else TRY_DECODING_TYPE("xyz", PARAM_TYPE_XYZ)
	else TRY_DECODING_TYPE("blackbody", PARAM_TYPE_BLACKBODY)
	else TRY_DECODING_TYPE("spectrum", PARAM_TYPE_SPECTRUM)
	else {
	Error("Unable to decode type for name \"%s\"", name);
	return false;
	}
	while (*strp && isspace(*strp))
		++strp;
	sname = std::string(strp);
	return true;
	}

}  // namespace pbrt
