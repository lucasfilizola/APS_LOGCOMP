/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_IMPRESSORA_TAB_H_INCLUDED
# define YY_YY_IMPRESSORA_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    T_INT = 258,                   /* T_INT  */
    T_STRING = 259,                /* T_STRING  */
    T_SE = 260,                    /* T_SE  */
    T_SENAO = 261,                 /* T_SENAO  */
    T_ENQUANTO = 262,              /* T_ENQUANTO  */
    T_IMPRIMIR = 263,              /* T_IMPRIMIR  */
    T_IMPRIMIR_TEXTO = 264,        /* T_IMPRIMIR_TEXTO  */
    T_IMPRIMIR_PAGINA = 265,       /* T_IMPRIMIR_PAGINA  */
    T_DEFINIR_COR = 266,           /* T_DEFINIR_COR  */
    T_DEFINIR_QUALIDADE = 267,     /* T_DEFINIR_QUALIDADE  */
    T_DEFINIR_PAPEL = 268,         /* T_DEFINIR_PAPEL  */
    T_VERIFICAR_TINTA = 269,       /* T_VERIFICAR_TINTA  */
    T_VERIFICAR_PAPEL = 270,       /* T_VERIFICAR_PAPEL  */
    T_AGUARDAR_PRONTA = 271,       /* T_AGUARDAR_PRONTA  */
    T_RESETAR = 272,               /* T_RESETAR  */
    T_BAIXA = 273,                 /* T_BAIXA  */
    T_MEDIA = 274,                 /* T_MEDIA  */
    T_ALTA = 275,                  /* T_ALTA  */
    T_A4 = 276,                    /* T_A4  */
    T_CARTA = 277,                 /* T_CARTA  */
    T_A3 = 278,                    /* T_A3  */
    T_NIVEL_TINTA = 279,           /* T_NIVEL_TINTA  */
    T_QTD_PAPEL = 280,             /* T_QTD_PAPEL  */
    T_ESTA_PRONTA = 281,           /* T_ESTA_PRONTA  */
    T_PAGINAS_IMPRESSAS = 282,     /* T_PAGINAS_IMPRESSAS  */
    T_MODO_COR = 283,              /* T_MODO_COR  */
    T_TAMANHO_FILA = 284,          /* T_TAMANHO_FILA  */
    T_EQ = 285,                    /* T_EQ  */
    T_NE = 286,                    /* T_NE  */
    T_LE = 287,                    /* T_LE  */
    T_GE = 288,                    /* T_GE  */
    T_ERROR = 289,                 /* T_ERROR  */
    T_IDENT = 290,                 /* T_IDENT  */
    T_STRING_LIT = 291,            /* T_STRING_LIT  */
    T_NUMBER = 292,                /* T_NUMBER  */
    LOWER_THAN_ELSE = 293,         /* LOWER_THAN_ELSE  */
    UMINUS = 294                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 396 "impressora.y"

  int ival;
  char* sval;
  struct Node* node;
  struct NodeList* list;

#line 110 "impressora.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_IMPRESSORA_TAB_H_INCLUDED  */
