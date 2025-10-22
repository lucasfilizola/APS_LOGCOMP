%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"   /* cabeçalho gerado pelo bison */
#define YY_USER_ACTION  /* noop */
%}

/* flags */
%option noyywrap
%option nounput
%option noinput

/* padrões auxiliares */
DIG     [0-9]
INT     {DIG}+
SPACE   [ \t\r]+

%%
/* --- comentários até fim da linha --- */
#[^\n]*                          { /* ignora */ }

/* --- separadores --- */
{SPACE}                          { /* ignora */ }
\n                               { return EOL; }
";"                              { return EOL; }

/* --- palavras-chave (fixas) --- */
"definir"                        { return TK_DEF; }
"nota"                           { return TK_NOTA; }
"somar"                          { return TK_SOMAR; }
"subtrair"                       { return TK_SUBTRAIR; }
"a"                              { return TK_A; }
"de"                             { return TK_DE; }

"alarme"                         { return TK_ALARME; }
"se"                             { return TK_SE; }
"media"                          { return TK_MEDIA; }
"maior"                          { return TK_MAIOR; }
"menor"                          { return TK_MENOR; }
"igual"                          { return TK_IGUAL; }

"garantir"                       { return TK_GARANTIR; }
"acima"                          { return TK_ACIMA; }
"por"                            { return TK_POR; }

"mostrar"                        { return TK_MOSTRAR; }
"acertos"                        { return TK_ACERTOS; }
"tempo"                          { return TK_TEMPO; }

"perguntar"                      { return TK_PERGUNTAR; }
"esperar"                        { return TK_ESPERAR; }

"parar"                          { return TK_PARAR; }
"halt"                           { return TK_HALT; }

/* --- duração:  INT 's'  (ex.: 10s) --- */
{INT}"s"                         { yylval.ival = atoi(yytext); return DUR; }

/* --- inteiros --- */
{INT}                            { yylval.ival = atoi(yytext); return INT; }

/* --- strings entre aspas --- */
\"([^\"\n]|\\\")*\"              {
                                   /* remove aspas e copia conteúdo */
                                   size_t n = yyleng;
                                   char* buf = (char*)malloc(n - 1);
                                   if (!buf) { fprintf(stderr,"mem error\n"); exit(1); }
                                   memcpy(buf, yytext+1, n-2);
                                   buf[n-2] = '\0';
                                   yylval.sval = buf;
                                   return STRING;
                                 }

/* --- caractere inesperado --- */
.                                 {
                                   fprintf(stderr, "Erro léxico: '%s'\n", yytext);
                                   return ERROR;
                                 }
%%
