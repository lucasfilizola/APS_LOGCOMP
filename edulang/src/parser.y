%{
#include <stdio.h>
#include <stdlib.h>

/* protótipos gerados pelo flex */
int yylex(void);
void yyerror(const char *s);

/* para o main poder localizar yyin */
extern FILE *yyin;
%}

/* ----- tipos do yylval ----- */
%union {
  int ival;
  char* sval;
}

/* ----- tokens sem valor ----- */
%token EOL ERROR
%token TK_DEF TK_NOTA TK_SOMAR TK_SUBTRAIR TK_A TK_DE
%token TK_ALARME TK_SE TK_MEDIA TK_MAIOR TK_MENOR TK_IGUAL
%token TK_GARANTIR TK_ACIMA TK_POR
%token TK_MOSTRAR TK_ACERTOS TK_TEMPO
%token TK_PERGUNTAR TK_ESPERAR
%token TK_PARAR TK_HALT

/* ----- tokens com valor ----- */
%token <ival> INT
%token <ival> DUR
%token <sval> STRING

/* ----- precedência (se necessário) ----- */
/* (não precisamos aqui) */

%start programa

%%

programa
  : /* vazio */ 
  | programa linha
  ;

linha
  : /* linha vazia */ EOL
  | comando EOL
  ;

comando
  : definir
  | ajuste
  | alarme
  | garantir
  | mostrar
  | perguntar
  | esperar
  | parar
  ;

definir
  : TK_DEF TK_NOTA INT
    { /* stub: printf("definir nota %d\n", $3); */ }
  ;

ajuste
  : TK_SOMAR INT TK_A TK_NOTA
    { /* printf("somar %d a nota\n", $2); */ }
  | TK_SUBTRAIR INT TK_DE TK_NOTA
    { /* printf("subtrair %d de nota\n", $2); */ }
  ;

alarme
  : TK_ALARME TK_SE TK_MEDIA rel INT
    { /* printf("alarme se media ... %d\n", $5); */ }
  ;

rel
  : TK_MAIOR
  | TK_MENOR
  | TK_IGUAL
  ;

garantir
  : TK_GARANTIR TK_MEDIA TK_ACIMA TK_DE INT TK_POR DUR
    { /* printf("garantir media >= %d por %ds\n", $5, $7); */ }
  ;

mostrar
  : TK_MOSTRAR TK_MEDIA
  | TK_MOSTRAR TK_NOTA
  | TK_MOSTRAR TK_ACERTOS
  | TK_MOSTRAR TK_TEMPO
  ;

perguntar
  : TK_PERGUNTAR STRING
    { free($2); }
  ;

esperar
  : TK_ESPERAR DUR
  ;

parar
  : TK_PARAR
  | TK_HALT
  ;

%%

void yyerror(const char *s) {
  fprintf(stderr, "Erro sintático: %s\n", s);
}
