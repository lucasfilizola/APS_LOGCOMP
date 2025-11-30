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
#line 1 "impressora.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* AST */
typedef enum {
  NK_PROGRAM, NK_LIST, NK_DECL_INT, NK_DECL_STRING, NK_ASSIGN,
  NK_IF, NK_WHILE, NK_BLOCK, NK_PRINT, NK_PRINT_TEXT,
  NK_PRINT_DOC, NK_PRINT_PAGE,
  NK_SET_COLOR, NK_SET_QUALITY, NK_SET_PAPER,
  NK_CHECK_INK, NK_CHECK_PAPER, NK_WAIT_READY, NK_RESET,
  NK_IDENT, NK_NUMBER, NK_STRING,
  NK_INK_LEVEL, NK_PAPER_COUNT, NK_IS_READY, NK_PAGES_PRINTED, NK_IS_COLOR, NK_QUEUE_SIZE,
  NK_LOW, NK_MEDIUM, NK_HIGH,
  NK_A4, NK_LETTER, NK_A3,
  NK_EQ, NK_NE, NK_LT, NK_LE, NK_GT, NK_GE,
  NK_ADD, NK_SUB, NK_MUL, NK_DIV, NK_NEG
} NodeKind;

typedef struct Node Node;
typedef struct NodeList NodeList;

struct Node {
  NodeKind kind;
  int line;
  char* sval;
  int ival;
  NodeList* kids;
};

struct NodeList {
  Node* node;
  NodeList* next;
};

static Node* mk(NodeKind k, int line, const char* s, int v, NodeList* kids) {
  Node* n = (Node*)calloc(1,sizeof(Node));
  n->kind = k; n->line=line;
  if (s) n->sval = strdup(s);
  n->ival = v;
  n->kids = kids;
  return n;
}

static NodeList* cons(Node* n, NodeList* xs) {
  NodeList* c=(NodeList*)malloc(sizeof(NodeList));
  c->node=n; c->next=xs;
  return c;
}

static NodeList* rev(NodeList* xs){
  NodeList* r=NULL;
  while(xs){
    NodeList* t=xs->next;
    xs->next=r;
    r=xs;
    xs=t;
  }
  return r;
}

static void free_ast(Node* n){
  if(!n) return;
  for(NodeList* k=n->kids;k;){
    NodeList* t=k->next;
    free_ast(k->node);
    free(k);
    k=t;
  }
  free(n->sval);
  free(n);
}

/* --- Geração de Código --- */
static FILE* out = NULL;
static int label_counter = 0;
static int var_counter = 0;

typedef struct VarEntry {
  char* name;
  int addr;
  struct VarEntry* next;
} VarEntry;

static VarEntry* symtab = NULL;

static int get_var_addr(const char* name) {
  for (VarEntry* e = symtab; e; e = e->next) {
    if (strcmp(e->name, name) == 0) return e->addr;
  }
  VarEntry* e = (VarEntry*)malloc(sizeof(VarEntry));
  e->name = strdup(name);
  e->addr = var_counter++;
  e->next = symtab;
  symtab = e;
  return e->addr;
}

static int new_label() { return label_counter++; }

static void gen_expr(Node* n);
static void gen_stmt(Node* n);

static void gen_expr(Node* n) {
  if (!n) return;
  NodeList* kids = n->kids;

  switch(n->kind) {
    case NK_NUMBER:
      fprintf(out, "PUSH %d\n", n->ival);
      break;

    case NK_IDENT: {
      int addr = get_var_addr(n->sval);
      fprintf(out, "LOAD %d\n", addr);
      break;
    }

    case NK_STRING:
      break;

    case NK_ADD:
      gen_expr(kids->node);
      gen_expr(kids->next->node);
      fprintf(out, "ADD\n");
      break;

    case NK_SUB:
      gen_expr(kids->node);
      gen_expr(kids->next->node);
      fprintf(out, "SUB\n");
      break;

    case NK_MUL:
      gen_expr(kids->node);
      gen_expr(kids->next->node);
      fprintf(out, "MUL\n");
      break;

    case NK_DIV:
      gen_expr(kids->node);
      gen_expr(kids->next->node);
      fprintf(out, "DIV\n");
      break;

    case NK_NEG:
      gen_expr(kids->node);
      fprintf(out, "NEG\n");
      break;

    case NK_EQ:
      gen_expr(kids->node);
      gen_expr(kids->next->node);
      fprintf(out, "EQ\n");
      break;

    case NK_NE:
      gen_expr(kids->node);
      gen_expr(kids->next->node);
      fprintf(out, "NE\n");
      break;

    case NK_LT:
      gen_expr(kids->node);
      gen_expr(kids->next->node);
      fprintf(out, "LT\n");
      break;

    case NK_LE:
      gen_expr(kids->node);
      gen_expr(kids->next->node);
      fprintf(out, "LE\n");
      break;

    case NK_GT:
      gen_expr(kids->node);
      gen_expr(kids->next->node);
      fprintf(out, "GT\n");
      break;

    case NK_GE:
      gen_expr(kids->node);
      gen_expr(kids->next->node);
      fprintf(out, "GE\n");
      break;

    case NK_INK_LEVEL:
      fprintf(out, "GET_INK_LEVEL\n");
      break;

    case NK_PAPER_COUNT:
      fprintf(out, "GET_PAPER_COUNT\n");
      break;

    case NK_IS_READY:
      fprintf(out, "GET_IS_READY\n");
      break;

    case NK_PAGES_PRINTED:
      fprintf(out, "GET_PAGES_PRINTED\n");
      break;

    case NK_IS_COLOR:
      fprintf(out, "GET_IS_COLOR\n");
      break;

    case NK_QUEUE_SIZE:
      fprintf(out, "GET_QUEUE_SIZE\n");
      break;

    case NK_LOW:
      fprintf(out, "PUSH 0\n");
      break;

    case NK_MEDIUM:
      fprintf(out, "PUSH 1\n");
      break;

    case NK_HIGH:
      fprintf(out, "PUSH 2\n");
      break;

    case NK_A4:
      fprintf(out, "PUSH 0\n");
      break;

    case NK_LETTER:
      fprintf(out, "PUSH 1\n");
      break;

    case NK_A3:
      fprintf(out, "PUSH 2\n");
      break;

    default:
      break;
  }
}

static void gen_stmt(Node* n) {
  if (!n) return;
  NodeList* kids = n->kids;

  switch(n->kind) {
    case NK_PROGRAM:
      for (NodeList* k = kids; k; k = k->next) {
        gen_stmt(k->node);
      }
      fprintf(out, "HALT\n");
      break;

    case NK_DECL_INT:
      if (kids) {
        gen_expr(kids->node);
        fprintf(out, "STORE %d\n", get_var_addr(n->sval));
      } else {
        fprintf(out, "PUSH 0\n");
        fprintf(out, "STORE %d\n", get_var_addr(n->sval));
      }
      break;

    case NK_DECL_STRING:
      break;

    case NK_ASSIGN:
      if (kids && kids->next) {
        Node* lhs = kids->node;
        Node* rhs = kids->next->node;
        if (rhs->kind == NK_STRING) {
          /* strings ignoradas por enquanto */
        } else {
          gen_expr(rhs);
          fprintf(out, "STORE %d\n", get_var_addr(lhs->sval));
        }
      }
      break;

    case NK_IF: {
      int else_label = new_label();
      int end_label = new_label();

      gen_expr(kids->node);

      if (kids->next && kids->next->next) {
        fprintf(out, "JUMPZ L%d\n", else_label);
        gen_stmt(kids->next->node);
        fprintf(out, "GOTO L%d\n", end_label);
        fprintf(out, "L%d:\n", else_label);
        gen_stmt(kids->next->next->node);
        fprintf(out, "L%d:\n", end_label);
      } else {
        fprintf(out, "JUMPZ L%d\n", end_label);
        gen_stmt(kids->next->node);
        fprintf(out, "L%d:\n", end_label);
      }
      break;
    }

    case NK_WHILE: {
      int loop_label = new_label();
      int end_label = new_label();

      fprintf(out, "L%d:\n", loop_label);
      gen_expr(kids->node);
      fprintf(out, "JUMPZ L%d\n", end_label);
      gen_stmt(kids->next->node);
      fprintf(out, "GOTO L%d\n", loop_label);
      fprintf(out, "L%d:\n", end_label);
      break;
    }

    case NK_BLOCK:
      for (NodeList* k = kids; k; k = k->next) {
        gen_stmt(k->node);
      }
      break;

    case NK_PRINT:
      if (n->sval) {
        fprintf(out, "PRINTS \"%s\"\n", n->sval);
      } else if (kids) {
        gen_expr(kids->node);
        fprintf(out, "PRINT\n");
      }
      break;

    case NK_PRINT_TEXT:
      fprintf(out, "PRINTS \"%s\"\n", n->sval);
      break;

    case NK_PRINT_DOC:
      if (kids && kids->next) {
        gen_expr(kids->next->node);  /* quantidade */
        fprintf(out, "POP R0\n");
        fprintf(out, "PRINT_DOC \"%s\" R0\n", kids->node->sval);
      }
      break;

    case NK_PRINT_PAGE:
      fprintf(out, "PRINT_PAGE \"%s\"\n", n->sval);
      break;

    case NK_SET_COLOR:
      gen_expr(kids->node);
      fprintf(out, "POP R0\n");
      fprintf(out, "SET_COLOR R0\n");
      break;

    case NK_SET_QUALITY:
      gen_expr(kids->node);
      fprintf(out, "POP R0\n");
      fprintf(out, "SET_QUALITY R0\n");
      break;

    case NK_SET_PAPER:
      gen_expr(kids->node);
      fprintf(out, "POP R0\n");
      fprintf(out, "SET_PAPER R0\n");
      break;

    case NK_CHECK_INK:
      fprintf(out, "CHECK_INK\n");
      break;

    case NK_CHECK_PAPER:
      fprintf(out, "CHECK_PAPER\n");
      break;

    case NK_WAIT_READY:
      fprintf(out, "WAIT_READY\n");
      break;

    case NK_RESET:
      fprintf(out, "RESET\n");
      break;

    case NK_LIST:
      break;

    default:
      break;
  }
}

/* lexer hooks */
int yylex(void);
extern int yylineno;
void yyerror(const char* s){
  fprintf(stderr,"[PARSER] Erro na linha %d: %s\n", yylineno, s);
}
Node* g_root = NULL;

#line 465 "impressora.tab.c"

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

#include "impressora.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_INT = 3,                      /* T_INT  */
  YYSYMBOL_T_STRING = 4,                   /* T_STRING  */
  YYSYMBOL_T_SE = 5,                       /* T_SE  */
  YYSYMBOL_T_SENAO = 6,                    /* T_SENAO  */
  YYSYMBOL_T_ENQUANTO = 7,                 /* T_ENQUANTO  */
  YYSYMBOL_T_IMPRIMIR = 8,                 /* T_IMPRIMIR  */
  YYSYMBOL_T_IMPRIMIR_TEXTO = 9,           /* T_IMPRIMIR_TEXTO  */
  YYSYMBOL_T_IMPRIMIR_PAGINA = 10,         /* T_IMPRIMIR_PAGINA  */
  YYSYMBOL_T_DEFINIR_COR = 11,             /* T_DEFINIR_COR  */
  YYSYMBOL_T_DEFINIR_QUALIDADE = 12,       /* T_DEFINIR_QUALIDADE  */
  YYSYMBOL_T_DEFINIR_PAPEL = 13,           /* T_DEFINIR_PAPEL  */
  YYSYMBOL_T_VERIFICAR_TINTA = 14,         /* T_VERIFICAR_TINTA  */
  YYSYMBOL_T_VERIFICAR_PAPEL = 15,         /* T_VERIFICAR_PAPEL  */
  YYSYMBOL_T_AGUARDAR_PRONTA = 16,         /* T_AGUARDAR_PRONTA  */
  YYSYMBOL_T_RESETAR = 17,                 /* T_RESETAR  */
  YYSYMBOL_T_BAIXA = 18,                   /* T_BAIXA  */
  YYSYMBOL_T_MEDIA = 19,                   /* T_MEDIA  */
  YYSYMBOL_T_ALTA = 20,                    /* T_ALTA  */
  YYSYMBOL_T_A4 = 21,                      /* T_A4  */
  YYSYMBOL_T_CARTA = 22,                   /* T_CARTA  */
  YYSYMBOL_T_A3 = 23,                      /* T_A3  */
  YYSYMBOL_T_NIVEL_TINTA = 24,             /* T_NIVEL_TINTA  */
  YYSYMBOL_T_QTD_PAPEL = 25,               /* T_QTD_PAPEL  */
  YYSYMBOL_T_ESTA_PRONTA = 26,             /* T_ESTA_PRONTA  */
  YYSYMBOL_T_PAGINAS_IMPRESSAS = 27,       /* T_PAGINAS_IMPRESSAS  */
  YYSYMBOL_T_MODO_COR = 28,                /* T_MODO_COR  */
  YYSYMBOL_T_TAMANHO_FILA = 29,            /* T_TAMANHO_FILA  */
  YYSYMBOL_T_EQ = 30,                      /* T_EQ  */
  YYSYMBOL_T_NE = 31,                      /* T_NE  */
  YYSYMBOL_T_LE = 32,                      /* T_LE  */
  YYSYMBOL_T_GE = 33,                      /* T_GE  */
  YYSYMBOL_T_ERROR = 34,                   /* T_ERROR  */
  YYSYMBOL_T_IDENT = 35,                   /* T_IDENT  */
  YYSYMBOL_T_STRING_LIT = 36,              /* T_STRING_LIT  */
  YYSYMBOL_T_NUMBER = 37,                  /* T_NUMBER  */
  YYSYMBOL_LOWER_THAN_ELSE = 38,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_39_ = 39,                       /* '<'  */
  YYSYMBOL_40_ = 40,                       /* '>'  */
  YYSYMBOL_41_ = 41,                       /* '+'  */
  YYSYMBOL_42_ = 42,                       /* '-'  */
  YYSYMBOL_43_ = 43,                       /* '*'  */
  YYSYMBOL_44_ = 44,                       /* '/'  */
  YYSYMBOL_UMINUS = 45,                    /* UMINUS  */
  YYSYMBOL_46_ = 46,                       /* ';'  */
  YYSYMBOL_47_ = 47,                       /* '='  */
  YYSYMBOL_48_ = 48,                       /* '('  */
  YYSYMBOL_49_ = 49,                       /* ')'  */
  YYSYMBOL_50_ = 50,                       /* '{'  */
  YYSYMBOL_51_ = 51,                       /* '}'  */
  YYSYMBOL_52_ = 52,                       /* ','  */
  YYSYMBOL_YYACCEPT = 53,                  /* $accept  */
  YYSYMBOL_program = 54,                   /* program  */
  YYSYMBOL_stmtlist = 55,                  /* stmtlist  */
  YYSYMBOL_decl = 56,                      /* decl  */
  YYSYMBOL_optInitNum = 57,                /* optInitNum  */
  YYSYMBOL_optInitStr = 58,                /* optInitStr  */
  YYSYMBOL_stmt = 59,                      /* stmt  */
  YYSYMBOL_assign = 60,                    /* assign  */
  YYSYMBOL_ifStmt = 61,                    /* ifStmt  */
  YYSYMBOL_whileStmt = 62,                 /* whileStmt  */
  YYSYMBOL_block = 63,                     /* block  */
  YYSYMBOL_printStmt = 64,                 /* printStmt  */
  YYSYMBOL_printerStmt = 65,               /* printerStmt  */
  YYSYMBOL_printDoc = 66,                  /* printDoc  */
  YYSYMBOL_printPage = 67,                 /* printPage  */
  YYSYMBOL_setColor = 68,                  /* setColor  */
  YYSYMBOL_setQuality = 69,                /* setQuality  */
  YYSYMBOL_setPaper = 70,                  /* setPaper  */
  YYSYMBOL_checkInk = 71,                  /* checkInk  */
  YYSYMBOL_checkPaper = 72,                /* checkPaper  */
  YYSYMBOL_waitReady = 73,                 /* waitReady  */
  YYSYMBOL_reset = 74,                     /* reset  */
  YYSYMBOL_quality = 75,                   /* quality  */
  YYSYMBOL_paperType = 76,                 /* paperType  */
  YYSYMBOL_expr = 77,                      /* expr  */
  YYSYMBOL_equality = 78,                  /* equality  */
  YYSYMBOL_relational = 79,                /* relational  */
  YYSYMBOL_additive = 80,                  /* additive  */
  YYSYMBOL_term = 81,                      /* term  */
  YYSYMBOL_factor = 82,                    /* factor  */
  YYSYMBOL_primary = 83                    /* primary  */
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
#define YYFINAL  54
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   197

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  79
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  166

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   294


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
      48,    49,    43,    41,    52,    42,     2,    44,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    46,
      39,    47,    40,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    50,     2,    51,     2,     2,     2,     2,
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
      35,    36,    37,    38,    45
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   440,   440,   441,   445,   446,   450,   452,   457,   458,
     462,   463,   467,   468,   469,   470,   471,   472,   473,   474,
     478,   482,   490,   492,   497,   502,   504,   509,   511,   513,
     518,   519,   520,   521,   522,   523,   524,   525,   526,   530,
     537,   542,   547,   552,   557,   562,   567,   572,   577,   578,
     579,   583,   584,   585,   590,   594,   595,   596,   600,   601,
     602,   603,   604,   608,   609,   610,   614,   615,   616,   620,
     621,   625,   626,   627,   628,   629,   630,   631,   632,   633
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
  "\"end of file\"", "error", "\"invalid token\"", "T_INT", "T_STRING",
  "T_SE", "T_SENAO", "T_ENQUANTO", "T_IMPRIMIR", "T_IMPRIMIR_TEXTO",
  "T_IMPRIMIR_PAGINA", "T_DEFINIR_COR", "T_DEFINIR_QUALIDADE",
  "T_DEFINIR_PAPEL", "T_VERIFICAR_TINTA", "T_VERIFICAR_PAPEL",
  "T_AGUARDAR_PRONTA", "T_RESETAR", "T_BAIXA", "T_MEDIA", "T_ALTA", "T_A4",
  "T_CARTA", "T_A3", "T_NIVEL_TINTA", "T_QTD_PAPEL", "T_ESTA_PRONTA",
  "T_PAGINAS_IMPRESSAS", "T_MODO_COR", "T_TAMANHO_FILA", "T_EQ", "T_NE",
  "T_LE", "T_GE", "T_ERROR", "T_IDENT", "T_STRING_LIT", "T_NUMBER",
  "LOWER_THAN_ELSE", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "UMINUS",
  "';'", "'='", "'('", "')'", "'{'", "'}'", "','", "$accept", "program",
  "stmtlist", "decl", "optInitNum", "optInitStr", "stmt", "assign",
  "ifStmt", "whileStmt", "block", "printStmt", "printerStmt", "printDoc",
  "printPage", "setColor", "setQuality", "setPaper", "checkInk",
  "checkPaper", "waitReady", "reset", "quality", "paperType", "expr",
  "equality", "relational", "additive", "term", "factor", "primary", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-107)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     110,   -31,    -5,    -3,     4,    19,    36,    45,    46,    47,
      52,    53,    54,    57,    60,    59,  -107,    12,   109,   110,
    -107,  -107,    70,  -107,  -107,  -107,  -107,  -107,  -107,  -107,
    -107,  -107,  -107,  -107,  -107,  -107,  -107,    87,    88,    11,
      11,    62,   100,   101,    11,    31,    58,    89,    93,    95,
      98,   104,  -107,    61,  -107,  -107,  -107,    11,   102,   113,
     105,   106,   107,   111,   114,   115,   116,  -107,  -107,    11,
      11,   108,     2,   -26,     1,    13,  -107,  -107,   112,    -8,
     117,   118,   119,   120,  -107,  -107,  -107,   121,  -107,  -107,
    -107,   122,   126,   127,   128,   129,  -107,  -107,  -107,  -107,
    -107,  -107,  -107,   130,   131,   132,   133,   134,   135,  -107,
     136,   110,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,   110,   140,    11,   141,   142,   143,   144,   145,
     146,  -107,  -107,  -107,  -107,  -107,  -107,  -107,  -107,  -107,
    -107,  -107,   147,   -26,   -26,     1,     1,     1,     1,    13,
      13,  -107,  -107,  -107,  -107,   148,  -107,  -107,  -107,  -107,
    -107,  -107,   110,   149,  -107,  -107
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    19,     0,     0,     3,
      12,     4,     0,    14,    15,    16,    17,    18,    30,    31,
      32,    33,    34,    35,    36,    37,    38,     8,    10,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     1,     5,    13,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    72,    71,     0,
       0,     0,    54,    55,    58,    63,    66,    69,     0,     0,
       0,     0,     0,     0,    48,    49,    50,     0,    51,    52,
      53,     0,     0,     0,     0,     0,    21,    20,    26,     9,
       6,    11,     7,     0,     0,     0,     0,     0,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    44,    45,    46,    47,    74,    75,    76,    77,    78,
      79,    73,    22,    56,    57,    61,    62,    59,    60,    64,
      65,    67,    68,    24,    28,     0,    27,    29,    40,    41,
      42,    43,     0,     0,    23,    39
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -107,  -107,   159,  -107,  -107,  -107,   -19,  -107,  -107,  -107,
    -107,  -107,  -107,  -107,  -107,  -107,  -107,  -107,  -107,  -107,
    -107,  -107,  -107,  -107,   -39,  -107,   -52,  -106,   -36,   -66,
    -107
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    18,    19,    20,    58,    60,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    87,    91,    71,    72,    73,    74,    75,    76,
      77
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      55,    78,    80,   109,    37,    83,   114,   115,   145,   146,
     147,   148,    97,   116,   117,     1,     2,     3,    99,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      38,   110,   112,   113,    55,    61,    62,    63,    64,    65,
      66,   123,   118,   119,   124,    39,    67,    15,    68,    84,
      85,    86,    40,    69,   151,   152,   120,   121,    16,    70,
     143,   144,    17,    52,     1,     2,     3,    41,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    88,
      89,    90,   149,   150,    42,   155,    61,    62,    63,    64,
      65,    66,   142,    43,    44,    45,    15,    67,    79,    68,
      46,    47,    48,   153,    69,    49,    51,    16,    50,    54,
      70,    17,    98,     1,     2,     3,    56,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    61,    62,
      63,    64,    65,    66,    57,    59,    81,    82,    92,    67,
      96,    68,    93,   164,    94,    15,    69,    95,   100,   101,
       0,   102,    70,   162,   103,   104,    16,   111,     0,   105,
      17,   122,   106,   107,   108,     0,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    53,     0,     0,   135,
     136,   137,   138,   139,   140,   141,   154,   156,   157,   158,
     159,   160,   161,     0,     0,   165,     0,   163
};

static const yytype_int16 yycheck[] =
{
      19,    40,    41,    69,    35,    44,    32,    33,   114,   115,
     116,   117,    51,    39,    40,     3,     4,     5,    57,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      35,    70,    30,    31,    53,    24,    25,    26,    27,    28,
      29,    49,    41,    42,    52,    48,    35,    35,    37,    18,
      19,    20,    48,    42,   120,   121,    43,    44,    46,    48,
     112,   113,    50,    51,     3,     4,     5,    48,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    21,
      22,    23,   118,   119,    48,   124,    24,    25,    26,    27,
      28,    29,   111,    48,    48,    48,    35,    35,    36,    37,
      48,    48,    48,   122,    42,    48,    47,    46,    48,     0,
      48,    50,    51,     3,     4,     5,    46,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    24,    25,
      26,    27,    28,    29,    47,    47,    36,    36,    49,    35,
      36,    37,    49,   162,    49,    35,    42,    49,    46,    36,
      -1,    46,    48,     6,    48,    48,    46,    49,    -1,    48,
      50,    49,    48,    48,    48,    -1,    49,    49,    49,    49,
      49,    49,    46,    46,    46,    46,    17,    -1,    -1,    49,
      49,    49,    49,    49,    49,    49,    46,    46,    46,    46,
      46,    46,    46,    -1,    -1,    46,    -1,    49
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    35,    46,    50,    54,    55,
      56,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    35,    35,    48,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    47,    51,    55,     0,    59,    46,    47,    57,    47,
      58,    24,    25,    26,    27,    28,    29,    35,    37,    42,
      48,    77,    78,    79,    80,    81,    82,    83,    77,    36,
      77,    36,    36,    77,    18,    19,    20,    75,    21,    22,
      23,    76,    49,    49,    49,    49,    36,    77,    51,    77,
      46,    36,    46,    48,    48,    48,    48,    48,    48,    82,
      77,    49,    30,    31,    32,    33,    39,    40,    41,    42,
      43,    44,    49,    49,    52,    49,    49,    49,    49,    49,
      49,    46,    46,    46,    46,    49,    49,    49,    49,    49,
      49,    49,    59,    79,    79,    80,    80,    80,    80,    81,
      81,    82,    82,    59,    46,    77,    46,    46,    46,    46,
      46,    46,     6,    49,    59,    46
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    53,    54,    54,    55,    55,    56,    56,    57,    57,
      58,    58,    59,    59,    59,    59,    59,    59,    59,    59,
      60,    60,    61,    61,    62,    63,    63,    64,    64,    64,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    75,
      75,    76,    76,    76,    77,    78,    78,    78,    79,    79,
      79,    79,    79,    80,    80,    80,    81,    81,    81,    82,
      82,    83,    83,    83,    83,    83,    83,    83,    83,    83
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     4,     4,     0,     2,
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       3,     3,     5,     7,     5,     2,     3,     5,     5,     5,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     7,
       5,     5,     5,     5,     4,     4,     4,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     1,
       2,     1,     1,     3,     3,     3,     3,     3,     3,     3
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
  case 2: /* program: %empty  */
#line 440 "impressora.y"
                                { (yyval.node) = mk(NK_PROGRAM, yylineno, NULL, 0, NULL); g_root = (yyval.node); }
#line 1651 "impressora.tab.c"
    break;

  case 3: /* program: stmtlist  */
#line 441 "impressora.y"
                                { (yyval.node) = mk(NK_PROGRAM, yylineno, NULL, 0, rev((yyvsp[0].list)));   g_root = (yyval.node); }
#line 1657 "impressora.tab.c"
    break;

  case 4: /* stmtlist: stmt  */
#line 445 "impressora.y"
                                { (yyval.list) = cons((yyvsp[0].node), NULL); }
#line 1663 "impressora.tab.c"
    break;

  case 5: /* stmtlist: stmtlist stmt  */
#line 446 "impressora.y"
                                { (yyval.list) = cons((yyvsp[0].node), (yyvsp[-1].list)); }
#line 1669 "impressora.tab.c"
    break;

  case 6: /* decl: T_INT T_IDENT optInitNum ';'  */
#line 451 "impressora.y"
      { (yyval.node) = mk(NK_DECL_INT, yylineno, (yyvsp[-2].sval), 0, (yyvsp[-1].node) ? cons((yyvsp[-1].node),NULL):NULL); free((yyvsp[-2].sval)); }
#line 1675 "impressora.tab.c"
    break;

  case 7: /* decl: T_STRING T_IDENT optInitStr ';'  */
#line 453 "impressora.y"
      { (yyval.node) = mk(NK_DECL_STRING, yylineno, (yyvsp[-2].sval), 0, (yyvsp[-1].node) ? cons((yyvsp[-1].node),NULL):NULL); free((yyvsp[-2].sval)); }
#line 1681 "impressora.tab.c"
    break;

  case 8: /* optInitNum: %empty  */
#line 457 "impressora.y"
                                { (yyval.node) = NULL; }
#line 1687 "impressora.tab.c"
    break;

  case 9: /* optInitNum: '=' expr  */
#line 458 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1693 "impressora.tab.c"
    break;

  case 10: /* optInitStr: %empty  */
#line 462 "impressora.y"
                                { (yyval.node) = NULL; }
#line 1699 "impressora.tab.c"
    break;

  case 11: /* optInitStr: '=' T_STRING_LIT  */
#line 463 "impressora.y"
                                { (yyval.node) = mk(NK_STRING, yylineno, (yyvsp[0].sval), 0, NULL); free((yyvsp[0].sval)); }
#line 1705 "impressora.tab.c"
    break;

  case 12: /* stmt: decl  */
#line 467 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1711 "impressora.tab.c"
    break;

  case 13: /* stmt: assign ';'  */
#line 468 "impressora.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 1717 "impressora.tab.c"
    break;

  case 14: /* stmt: ifStmt  */
#line 469 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1723 "impressora.tab.c"
    break;

  case 15: /* stmt: whileStmt  */
#line 470 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1729 "impressora.tab.c"
    break;

  case 16: /* stmt: block  */
#line 471 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1735 "impressora.tab.c"
    break;

  case 17: /* stmt: printStmt  */
#line 472 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1741 "impressora.tab.c"
    break;

  case 18: /* stmt: printerStmt  */
#line 473 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1747 "impressora.tab.c"
    break;

  case 19: /* stmt: ';'  */
#line 474 "impressora.y"
                                { (yyval.node) = mk(NK_LIST, yylineno, NULL, 0, NULL); }
#line 1753 "impressora.tab.c"
    break;

  case 20: /* assign: T_IDENT '=' expr  */
#line 479 "impressora.y"
      { Node* id=mk(NK_IDENT,yylineno,(yyvsp[-2].sval),0,NULL);
        (yyval.node)=mk(NK_ASSIGN,yylineno,NULL,0,rev(cons((yyvsp[0].node),cons(id,NULL))));
        free((yyvsp[-2].sval)); }
#line 1761 "impressora.tab.c"
    break;

  case 21: /* assign: T_IDENT '=' T_STRING_LIT  */
#line 483 "impressora.y"
      { Node* id=mk(NK_IDENT,yylineno,(yyvsp[-2].sval),0,NULL);
        Node* s=mk(NK_STRING,yylineno,(yyvsp[0].sval),0,NULL);
        (yyval.node)=mk(NK_ASSIGN,yylineno,NULL,0,rev(cons(s,cons(id,NULL))));
        free((yyvsp[-2].sval)); free((yyvsp[0].sval)); }
#line 1770 "impressora.tab.c"
    break;

  case 22: /* ifStmt: T_SE '(' expr ')' stmt  */
#line 491 "impressora.y"
      { (yyval.node) = mk(NK_IF, yylineno, NULL, 0, rev(cons((yyvsp[0].node), cons((yyvsp[-2].node), NULL)))); }
#line 1776 "impressora.tab.c"
    break;

  case 23: /* ifStmt: T_SE '(' expr ')' stmt T_SENAO stmt  */
#line 493 "impressora.y"
      { (yyval.node) = mk(NK_IF, yylineno, NULL, 0, rev(cons((yyvsp[0].node), cons((yyvsp[-2].node), cons((yyvsp[-4].node), NULL))))); }
#line 1782 "impressora.tab.c"
    break;

  case 24: /* whileStmt: T_ENQUANTO '(' expr ')' stmt  */
#line 498 "impressora.y"
      { (yyval.node) = mk(NK_WHILE, yylineno, NULL, 0, rev(cons((yyvsp[0].node), cons((yyvsp[-2].node), NULL)))); }
#line 1788 "impressora.tab.c"
    break;

  case 25: /* block: '{' '}'  */
#line 503 "impressora.y"
      { (yyval.node) = mk(NK_BLOCK, yylineno, NULL, 0, NULL); }
#line 1794 "impressora.tab.c"
    break;

  case 26: /* block: '{' stmtlist '}'  */
#line 505 "impressora.y"
      { (yyval.node) = mk(NK_BLOCK, yylineno, NULL, 0, (yyvsp[-1].list)); }
#line 1800 "impressora.tab.c"
    break;

  case 27: /* printStmt: T_IMPRIMIR '(' expr ')' ';'  */
#line 510 "impressora.y"
      { (yyval.node) = mk(NK_PRINT, yylineno, NULL, 0, cons((yyvsp[-2].node),NULL)); }
#line 1806 "impressora.tab.c"
    break;

  case 28: /* printStmt: T_IMPRIMIR '(' T_STRING_LIT ')' ';'  */
#line 512 "impressora.y"
      { (yyval.node) = mk(NK_PRINT, yylineno, (yyvsp[-2].sval), 0, NULL); free((yyvsp[-2].sval)); }
#line 1812 "impressora.tab.c"
    break;

  case 29: /* printStmt: T_IMPRIMIR_TEXTO '(' T_STRING_LIT ')' ';'  */
#line 514 "impressora.y"
      { (yyval.node) = mk(NK_PRINT_TEXT, yylineno, (yyvsp[-2].sval), 0, NULL); free((yyvsp[-2].sval)); }
#line 1818 "impressora.tab.c"
    break;

  case 30: /* printerStmt: printDoc  */
#line 518 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1824 "impressora.tab.c"
    break;

  case 31: /* printerStmt: printPage  */
#line 519 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1830 "impressora.tab.c"
    break;

  case 32: /* printerStmt: setColor  */
#line 520 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1836 "impressora.tab.c"
    break;

  case 33: /* printerStmt: setQuality  */
#line 521 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1842 "impressora.tab.c"
    break;

  case 34: /* printerStmt: setPaper  */
#line 522 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1848 "impressora.tab.c"
    break;

  case 35: /* printerStmt: checkInk  */
#line 523 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1854 "impressora.tab.c"
    break;

  case 36: /* printerStmt: checkPaper  */
#line 524 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1860 "impressora.tab.c"
    break;

  case 37: /* printerStmt: waitReady  */
#line 525 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1866 "impressora.tab.c"
    break;

  case 38: /* printerStmt: reset  */
#line 526 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1872 "impressora.tab.c"
    break;

  case 39: /* printDoc: T_IMPRIMIR '(' T_STRING_LIT ',' expr ')' ';'  */
#line 531 "impressora.y"
      { Node* file = mk(NK_STRING, yylineno, (yyvsp[-4].sval), 0, NULL);
        (yyval.node) = mk(NK_PRINT_DOC, yylineno, NULL, 0, rev(cons((yyvsp[-2].node), cons(file, NULL))));
        free((yyvsp[-4].sval)); }
#line 1880 "impressora.tab.c"
    break;

  case 40: /* printPage: T_IMPRIMIR_PAGINA '(' T_STRING_LIT ')' ';'  */
#line 538 "impressora.y"
      { (yyval.node) = mk(NK_PRINT_PAGE, yylineno, (yyvsp[-2].sval), 0, NULL); free((yyvsp[-2].sval)); }
#line 1886 "impressora.tab.c"
    break;

  case 41: /* setColor: T_DEFINIR_COR '(' expr ')' ';'  */
#line 543 "impressora.y"
      { (yyval.node) = mk(NK_SET_COLOR, yylineno, NULL, 0, cons((yyvsp[-2].node), NULL)); }
#line 1892 "impressora.tab.c"
    break;

  case 42: /* setQuality: T_DEFINIR_QUALIDADE '(' quality ')' ';'  */
#line 548 "impressora.y"
      { (yyval.node) = mk(NK_SET_QUALITY, yylineno, NULL, 0, cons((yyvsp[-2].node), NULL)); }
#line 1898 "impressora.tab.c"
    break;

  case 43: /* setPaper: T_DEFINIR_PAPEL '(' paperType ')' ';'  */
#line 553 "impressora.y"
      { (yyval.node) = mk(NK_SET_PAPER, yylineno, NULL, 0, cons((yyvsp[-2].node), NULL)); }
#line 1904 "impressora.tab.c"
    break;

  case 44: /* checkInk: T_VERIFICAR_TINTA '(' ')' ';'  */
#line 558 "impressora.y"
      { (yyval.node) = mk(NK_CHECK_INK, yylineno, NULL, 0, NULL); }
#line 1910 "impressora.tab.c"
    break;

  case 45: /* checkPaper: T_VERIFICAR_PAPEL '(' ')' ';'  */
#line 563 "impressora.y"
      { (yyval.node) = mk(NK_CHECK_PAPER, yylineno, NULL, 0, NULL); }
#line 1916 "impressora.tab.c"
    break;

  case 46: /* waitReady: T_AGUARDAR_PRONTA '(' ')' ';'  */
#line 568 "impressora.y"
      { (yyval.node) = mk(NK_WAIT_READY, yylineno, NULL, 0, NULL); }
#line 1922 "impressora.tab.c"
    break;

  case 47: /* reset: T_RESETAR '(' ')' ';'  */
#line 573 "impressora.y"
      { (yyval.node) = mk(NK_RESET, yylineno, NULL, 0, NULL); }
#line 1928 "impressora.tab.c"
    break;

  case 48: /* quality: T_BAIXA  */
#line 577 "impressora.y"
                                { (yyval.node) = mk(NK_LOW, yylineno, NULL, 0, NULL); }
#line 1934 "impressora.tab.c"
    break;

  case 49: /* quality: T_MEDIA  */
#line 578 "impressora.y"
                                { (yyval.node) = mk(NK_MEDIUM, yylineno, NULL, 1, NULL); }
#line 1940 "impressora.tab.c"
    break;

  case 50: /* quality: T_ALTA  */
#line 579 "impressora.y"
                                { (yyval.node) = mk(NK_HIGH, yylineno, NULL, 2, NULL); }
#line 1946 "impressora.tab.c"
    break;

  case 51: /* paperType: T_A4  */
#line 583 "impressora.y"
                                { (yyval.node) = mk(NK_A4, yylineno, NULL, 0, NULL); }
#line 1952 "impressora.tab.c"
    break;

  case 52: /* paperType: T_CARTA  */
#line 584 "impressora.y"
                                { (yyval.node) = mk(NK_LETTER, yylineno, NULL, 1, NULL); }
#line 1958 "impressora.tab.c"
    break;

  case 53: /* paperType: T_A3  */
#line 585 "impressora.y"
                                { (yyval.node) = mk(NK_A3, yylineno, NULL, 2, NULL); }
#line 1964 "impressora.tab.c"
    break;

  case 54: /* expr: equality  */
#line 590 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1970 "impressora.tab.c"
    break;

  case 55: /* equality: relational  */
#line 594 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1976 "impressora.tab.c"
    break;

  case 56: /* equality: equality T_EQ relational  */
#line 595 "impressora.y"
                                { (yyval.node) = mk(NK_EQ, yylineno, NULL, 0, rev(cons((yyvsp[0].node), cons((yyvsp[-2].node),NULL)))); }
#line 1982 "impressora.tab.c"
    break;

  case 57: /* equality: equality T_NE relational  */
#line 596 "impressora.y"
                                { (yyval.node) = mk(NK_NE, yylineno, NULL, 0, rev(cons((yyvsp[0].node), cons((yyvsp[-2].node),NULL)))); }
#line 1988 "impressora.tab.c"
    break;

  case 58: /* relational: additive  */
#line 600 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 1994 "impressora.tab.c"
    break;

  case 59: /* relational: relational '<' additive  */
#line 601 "impressora.y"
                                { (yyval.node) = mk(NK_LT, yylineno, NULL, 0, rev(cons((yyvsp[0].node), cons((yyvsp[-2].node),NULL)))); }
#line 2000 "impressora.tab.c"
    break;

  case 60: /* relational: relational '>' additive  */
#line 602 "impressora.y"
                                { (yyval.node) = mk(NK_GT, yylineno, NULL, 0, rev(cons((yyvsp[0].node), cons((yyvsp[-2].node),NULL)))); }
#line 2006 "impressora.tab.c"
    break;

  case 61: /* relational: relational T_LE additive  */
#line 603 "impressora.y"
                                { (yyval.node) = mk(NK_LE, yylineno, NULL, 0, rev(cons((yyvsp[0].node), cons((yyvsp[-2].node),NULL)))); }
#line 2012 "impressora.tab.c"
    break;

  case 62: /* relational: relational T_GE additive  */
#line 604 "impressora.y"
                                { (yyval.node) = mk(NK_GE, yylineno, NULL, 0, rev(cons((yyvsp[0].node), cons((yyvsp[-2].node),NULL)))); }
#line 2018 "impressora.tab.c"
    break;

  case 63: /* additive: term  */
#line 608 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 2024 "impressora.tab.c"
    break;

  case 64: /* additive: additive '+' term  */
#line 609 "impressora.y"
                                { (yyval.node) = mk(NK_ADD, yylineno, NULL, 0, rev(cons((yyvsp[0].node), cons((yyvsp[-2].node),NULL)))); }
#line 2030 "impressora.tab.c"
    break;

  case 65: /* additive: additive '-' term  */
#line 610 "impressora.y"
                                { (yyval.node) = mk(NK_SUB, yylineno, NULL, 0, rev(cons((yyvsp[0].node), cons((yyvsp[-2].node),NULL)))); }
#line 2036 "impressora.tab.c"
    break;

  case 66: /* term: factor  */
#line 614 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 2042 "impressora.tab.c"
    break;

  case 67: /* term: term '*' factor  */
#line 615 "impressora.y"
                                { (yyval.node) = mk(NK_MUL, yylineno, NULL, 0, rev(cons((yyvsp[0].node), cons((yyvsp[-2].node),NULL)))); }
#line 2048 "impressora.tab.c"
    break;

  case 68: /* term: term '/' factor  */
#line 616 "impressora.y"
                                { (yyval.node) = mk(NK_DIV, yylineno, NULL, 0, rev(cons((yyvsp[0].node), cons((yyvsp[-2].node),NULL)))); }
#line 2054 "impressora.tab.c"
    break;

  case 69: /* factor: primary  */
#line 620 "impressora.y"
                                { (yyval.node) = (yyvsp[0].node); }
#line 2060 "impressora.tab.c"
    break;

  case 70: /* factor: '-' factor  */
#line 621 "impressora.y"
                                { (yyval.node) = mk(NK_NEG, yylineno, NULL, 0, cons((yyvsp[0].node),NULL)); }
#line 2066 "impressora.tab.c"
    break;

  case 71: /* primary: T_NUMBER  */
#line 625 "impressora.y"
                                { (yyval.node) = mk(NK_NUMBER, yylineno, NULL, (yyvsp[0].ival), NULL); }
#line 2072 "impressora.tab.c"
    break;

  case 72: /* primary: T_IDENT  */
#line 626 "impressora.y"
                                { (yyval.node) = mk(NK_IDENT,  yylineno, (yyvsp[0].sval), 0, NULL); free((yyvsp[0].sval)); }
#line 2078 "impressora.tab.c"
    break;

  case 73: /* primary: '(' expr ')'  */
#line 627 "impressora.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 2084 "impressora.tab.c"
    break;

  case 74: /* primary: T_NIVEL_TINTA '(' ')'  */
#line 628 "impressora.y"
                                { (yyval.node) = mk(NK_INK_LEVEL, yylineno, NULL, 0, NULL); }
#line 2090 "impressora.tab.c"
    break;

  case 75: /* primary: T_QTD_PAPEL '(' ')'  */
#line 629 "impressora.y"
                                { (yyval.node) = mk(NK_PAPER_COUNT, yylineno, NULL, 0, NULL); }
#line 2096 "impressora.tab.c"
    break;

  case 76: /* primary: T_ESTA_PRONTA '(' ')'  */
#line 630 "impressora.y"
                                { (yyval.node) = mk(NK_IS_READY, yylineno, NULL, 0, NULL); }
#line 2102 "impressora.tab.c"
    break;

  case 77: /* primary: T_PAGINAS_IMPRESSAS '(' ')'  */
#line 631 "impressora.y"
                                  { (yyval.node) = mk(NK_PAGES_PRINTED, yylineno, NULL, 0, NULL); }
#line 2108 "impressora.tab.c"
    break;

  case 78: /* primary: T_MODO_COR '(' ')'  */
#line 632 "impressora.y"
                                { (yyval.node) = mk(NK_IS_COLOR, yylineno, NULL, 0, NULL); }
#line 2114 "impressora.tab.c"
    break;

  case 79: /* primary: T_TAMANHO_FILA '(' ')'  */
#line 633 "impressora.y"
                                { (yyval.node) = mk(NK_QUEUE_SIZE, yylineno, NULL, 0, NULL); }
#line 2120 "impressora.tab.c"
    break;


#line 2124 "impressora.tab.c"

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

#line 636 "impressora.y"


int main(int argc, char** argv){
  int ret = yyparse();
  if(ret==0){
    puts("[OK] Parsing concluído.");

    const char* outfile = "output.asm";
    if (argc > 1) {
      outfile = argv[1];
    }

    out = fopen(outfile, "w");
    if (!out) {
      fprintf(stderr, "[ERRO] Não foi possível abrir arquivo: %s\n", outfile);
      return 1;
    }

    fprintf(out, "; Impressora Assembly\n\n");
    gen_stmt(g_root);
    fclose(out);

    printf("[OK] Código assembly gerado: %s\n", outfile);
  }
  free_ast(g_root);
  return ret;
}
