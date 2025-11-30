%option noyywrap
%option yylineno

%{
#include "impressora.tab.h"
#include <string.h>
#include <stdlib.h>

static char* imp_strdup(const char* s) {
    if (!s) return NULL;
    size_t n = strlen(s);
    char* p = (char*)malloc(n+1);
    memcpy(p, s, n+1);
    return p;
}
%}

%x CMT

WS          [ \t\r]+
DIGIT       [0-9]
LETTER      [A-Za-z]
IDREST      [A-Za-z0-9_]
ID          {LETTER}{IDREST}*
NUMBER      {DIGIT}+
ESC         \\["\\nrt]
STRCHAR     ([^"\\]|{ESC})
STRING      \"{STRCHAR}*\"

%%

{WS}                        ;
"//".*                      ;
"/*"                        { BEGIN(CMT); }
<CMT>"*/"                   { BEGIN(INITIAL); }
<CMT>\n                     ;
<CMT>.                      ;

"int"                       { return T_INT; }
"string"                    { return T_STRING; }
"se"                        { return T_SE; }
"senao"                     { return T_SENAO; }
"enquanto"                  { return T_ENQUANTO; }
"imprimir"                  { return T_IMPRIMIR; }
"imprimir_texto"            { return T_IMPRIMIR_TEXTO; }
"imprimir_pagina"           { return T_IMPRIMIR_PAGINA; }

"definir_cor"               { return T_DEFINIR_COR; }
"definir_qualidade"         { return T_DEFINIR_QUALIDADE; }
"definir_papel"             { return T_DEFINIR_PAPEL; }

"verificar_tinta"           { return T_VERIFICAR_TINTA; }
"verificar_papel"           { return T_VERIFICAR_PAPEL; }
"aguardar_pronta"           { return T_AGUARDAR_PRONTA; }
"resetar"                   { return T_RESETAR; }

"BAIXA"                     { return T_BAIXA; }
"MEDIA"                     { return T_MEDIA; }
"ALTA"                      { return T_ALTA; }

"A4"                        { return T_A4; }
"CARTA"                     { return T_CARTA; }
"A3"                        { return T_A3; }

"verdadeiro"                { yylval.ival = 1; return T_NUMBER; }
"falso"                     { yylval.ival = 0; return T_NUMBER; }

"nivel_tinta"               { return T_NIVEL_TINTA; }
"qtd_papel"                 { return T_QTD_PAPEL; }
"esta_pronta"               { return T_ESTA_PRONTA; }
"paginas_impressas"         { return T_PAGINAS_IMPRESSAS; }
"modo_cor"                  { return T_MODO_COR; }
"tamanho_fila"              { return T_TAMANHO_FILA; }

"=="                        { return T_EQ; }
"!="                        { return T_NE; }
"<="                        { return T_LE; }
">="                        { return T_GE; }

{ID}                        { yylval.sval = imp_strdup(yytext); return T_IDENT; }
{NUMBER}                    { yylval.ival = atoi(yytext); return T_NUMBER; }
{STRING}                    {
                                size_t n = yyleng;
                                char* buf = (char*)malloc(n-1);
                                size_t j=0;
                                for (size_t i=1; i<n-1; ++i) {
                                    if (yytext[i] == '\\') {
                                        ++i;
                                        switch(yytext[i]) {
                                            case 'n': buf[j++] = '\n'; break;
                                            case 'r': buf[j++] = '\r'; break;
                                            case 't': buf[j++] = '\t'; break;
                                            case '\\': buf[j++]='\\'; break;
                                            case '"': buf[j++]='"'; break;
                                            default: buf[j++]=yytext[i]; break;
                                        }
                                    } else {
                                        buf[j++] = yytext[i];
                                    }
                                }
                                buf[j]='\0';
                                yylval.sval = buf;
                                return T_STRING_LIT;
                            }

";"                         { return ';'; }
"="                         { return '='; }
"("                         { return '('; }
")"                         { return ')'; }
"{"                         { return '{'; }
"}"                         { return '}'; }
","                         { return ','; }
"+"                         { return '+'; }
"-"                         { return '-'; }
"*"                         { return '*'; }
"/"                         { return '/'; }
"<"                         { return '<'; }
">"                         { return '>'; }

\n                          ;

.                           { fprintf(stderr,"[LEXER] Caractere inválido '%c' na linha %d\n", yytext[0], yylineno); return T_ERROR; }

%%
