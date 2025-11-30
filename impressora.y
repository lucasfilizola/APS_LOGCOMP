%{
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
%}

/* types */
%union {
  int ival;
  char* sval;
  struct Node* node;
  struct NodeList* list;
}

/* tokens sem valores */
%token T_INT T_STRING T_SE T_SENAO T_ENQUANTO T_IMPRIMIR T_IMPRIMIR_TEXTO T_IMPRIMIR_PAGINA
%token T_DEFINIR_COR T_DEFINIR_QUALIDADE T_DEFINIR_PAPEL
%token T_VERIFICAR_TINTA T_VERIFICAR_PAPEL T_AGUARDAR_PRONTA T_RESETAR
%token T_BAIXA T_MEDIA T_ALTA
%token T_A4 T_CARTA T_A3
%token T_NIVEL_TINTA T_QTD_PAPEL T_ESTA_PRONTA T_PAGINAS_IMPRESSAS T_MODO_COR T_TAMANHO_FILA
%token T_EQ T_NE T_LE T_GE
%token T_ERROR

/* tokens com valores */
%token <sval> T_IDENT
%token <sval> T_STRING_LIT
%token <ival> T_NUMBER

/* precedência */
%nonassoc LOWER_THAN_ELSE
%nonassoc T_SENAO

%left T_EQ T_NE
%left '<' '>' T_LE T_GE
%left '+' '-'
%left '*' '/'
%right UMINUS

/* tipos dos não-terminais */
%type <node> program decl optInitNum optInitStr
%type <node> stmt assign ifStmt whileStmt block printStmt printerStmt
%type <node> printDoc printPage setColor setQuality setPaper
%type <node> checkInk checkPaper waitReady reset
%type <node> expr equality relational additive term factor primary
%type <node> quality paperType
%type <list> stmtlist

%%

program
    : %empty                    { $$ = mk(NK_PROGRAM, yylineno, NULL, 0, NULL); g_root = $$; }
    | stmtlist                  { $$ = mk(NK_PROGRAM, yylineno, NULL, 0, rev($1));   g_root = $$; }
    ;

stmtlist
    : stmt                      { $$ = cons($1, NULL); }
    | stmtlist stmt             { $$ = cons($2, $1); }
    ;

decl
    : T_INT T_IDENT optInitNum ';'
      { $$ = mk(NK_DECL_INT, yylineno, $2, 0, $3 ? cons($3,NULL):NULL); free($2); }
    | T_STRING T_IDENT optInitStr ';'
      { $$ = mk(NK_DECL_STRING, yylineno, $2, 0, $3 ? cons($3,NULL):NULL); free($2); }
    ;

optInitNum
    : %empty                    { $$ = NULL; }
    | '=' expr                  { $$ = $2; }
    ;

optInitStr
    : %empty                    { $$ = NULL; }
    | '=' T_STRING_LIT          { $$ = mk(NK_STRING, yylineno, $2, 0, NULL); free($2); }
    ;

stmt
    : decl                      { $$ = $1; }
    | assign ';'                { $$ = $1; }
    | ifStmt                    { $$ = $1; }
    | whileStmt                 { $$ = $1; }
    | block                     { $$ = $1; }
    | printStmt                 { $$ = $1; }
    | printerStmt               { $$ = $1; }
    | ';'                       { $$ = mk(NK_LIST, yylineno, NULL, 0, NULL); }
    ;

assign
    : T_IDENT '=' expr
      { Node* id=mk(NK_IDENT,yylineno,$1,0,NULL);
        $$=mk(NK_ASSIGN,yylineno,NULL,0,rev(cons($3,cons(id,NULL))));
        free($1); }
    | T_IDENT '=' T_STRING_LIT
      { Node* id=mk(NK_IDENT,yylineno,$1,0,NULL);
        Node* s=mk(NK_STRING,yylineno,$3,0,NULL);
        $$=mk(NK_ASSIGN,yylineno,NULL,0,rev(cons(s,cons(id,NULL))));
        free($1); free($3); }
    ;

ifStmt
    : T_SE '(' expr ')' stmt %prec LOWER_THAN_ELSE
      { $$ = mk(NK_IF, yylineno, NULL, 0, rev(cons($5, cons($3, NULL)))); }
    | T_SE '(' expr ')' stmt T_SENAO stmt
      { $$ = mk(NK_IF, yylineno, NULL, 0, rev(cons($7, cons($5, cons($3, NULL))))); }
    ;

whileStmt
    : T_ENQUANTO '(' expr ')' stmt
      { $$ = mk(NK_WHILE, yylineno, NULL, 0, rev(cons($5, cons($3, NULL)))); }
    ;

block
    : '{' '}'
      { $$ = mk(NK_BLOCK, yylineno, NULL, 0, NULL); }
    | '{' stmtlist '}'
      { $$ = mk(NK_BLOCK, yylineno, NULL, 0, $2); }
    ;

printStmt
    : T_IMPRIMIR '(' expr ')' ';'
      { $$ = mk(NK_PRINT, yylineno, NULL, 0, cons($3,NULL)); }
    | T_IMPRIMIR '(' T_STRING_LIT ')' ';'
      { $$ = mk(NK_PRINT, yylineno, $3, 0, NULL); free($3); }
    | T_IMPRIMIR_TEXTO '(' T_STRING_LIT ')' ';'
      { $$ = mk(NK_PRINT_TEXT, yylineno, $3, 0, NULL); free($3); }
    ;

printerStmt
    : printDoc                  { $$ = $1; }
    | printPage                 { $$ = $1; }
    | setColor                  { $$ = $1; }
    | setQuality                { $$ = $1; }
    | setPaper                  { $$ = $1; }
    | checkInk                  { $$ = $1; }
    | checkPaper                { $$ = $1; }
    | waitReady                 { $$ = $1; }
    | reset                     { $$ = $1; }
    ;

printDoc
    : T_IMPRIMIR '(' T_STRING_LIT ',' expr ')' ';'
      { Node* file = mk(NK_STRING, yylineno, $3, 0, NULL);
        $$ = mk(NK_PRINT_DOC, yylineno, NULL, 0, rev(cons($5, cons(file, NULL))));
        free($3); }
    ;

printPage
    : T_IMPRIMIR_PAGINA '(' T_STRING_LIT ')' ';'
      { $$ = mk(NK_PRINT_PAGE, yylineno, $3, 0, NULL); free($3); }
    ;

setColor
    : T_DEFINIR_COR '(' expr ')' ';'
      { $$ = mk(NK_SET_COLOR, yylineno, NULL, 0, cons($3, NULL)); }
    ;

setQuality
    : T_DEFINIR_QUALIDADE '(' quality ')' ';'
      { $$ = mk(NK_SET_QUALITY, yylineno, NULL, 0, cons($3, NULL)); }
    ;

setPaper
    : T_DEFINIR_PAPEL '(' paperType ')' ';'
      { $$ = mk(NK_SET_PAPER, yylineno, NULL, 0, cons($3, NULL)); }
    ;

checkInk
    : T_VERIFICAR_TINTA '(' ')' ';'
      { $$ = mk(NK_CHECK_INK, yylineno, NULL, 0, NULL); }
    ;

checkPaper
    : T_VERIFICAR_PAPEL '(' ')' ';'
      { $$ = mk(NK_CHECK_PAPER, yylineno, NULL, 0, NULL); }
    ;

waitReady
    : T_AGUARDAR_PRONTA '(' ')' ';'
      { $$ = mk(NK_WAIT_READY, yylineno, NULL, 0, NULL); }
    ;

reset
    : T_RESETAR '(' ')' ';'
      { $$ = mk(NK_RESET, yylineno, NULL, 0, NULL); }
    ;

quality
    : T_BAIXA                   { $$ = mk(NK_LOW, yylineno, NULL, 0, NULL); }
    | T_MEDIA                   { $$ = mk(NK_MEDIUM, yylineno, NULL, 1, NULL); }
    | T_ALTA                    { $$ = mk(NK_HIGH, yylineno, NULL, 2, NULL); }
    ;

paperType
    : T_A4                      { $$ = mk(NK_A4, yylineno, NULL, 0, NULL); }
    | T_CARTA                   { $$ = mk(NK_LETTER, yylineno, NULL, 1, NULL); }
    | T_A3                      { $$ = mk(NK_A3, yylineno, NULL, 2, NULL); }
    ;

/* expressões */
expr
    : equality                  { $$ = $1; }
    ;

equality
    : relational                { $$ = $1; }
    | equality T_EQ relational  { $$ = mk(NK_EQ, yylineno, NULL, 0, rev(cons($3, cons($1,NULL)))); }
    | equality T_NE relational  { $$ = mk(NK_NE, yylineno, NULL, 0, rev(cons($3, cons($1,NULL)))); }
    ;

relational
    : additive                  { $$ = $1; }
    | relational '<'  additive  { $$ = mk(NK_LT, yylineno, NULL, 0, rev(cons($3, cons($1,NULL)))); }
    | relational '>'  additive  { $$ = mk(NK_GT, yylineno, NULL, 0, rev(cons($3, cons($1,NULL)))); }
    | relational T_LE additive  { $$ = mk(NK_LE, yylineno, NULL, 0, rev(cons($3, cons($1,NULL)))); }
    | relational T_GE additive  { $$ = mk(NK_GE, yylineno, NULL, 0, rev(cons($3, cons($1,NULL)))); }
    ;

additive
    : term                      { $$ = $1; }
    | additive '+' term         { $$ = mk(NK_ADD, yylineno, NULL, 0, rev(cons($3, cons($1,NULL)))); }
    | additive '-' term         { $$ = mk(NK_SUB, yylineno, NULL, 0, rev(cons($3, cons($1,NULL)))); }
    ;

term
    : factor                    { $$ = $1; }
    | term '*' factor           { $$ = mk(NK_MUL, yylineno, NULL, 0, rev(cons($3, cons($1,NULL)))); }
    | term '/' factor           { $$ = mk(NK_DIV, yylineno, NULL, 0, rev(cons($3, cons($1,NULL)))); }
    ;

factor
    : primary                   { $$ = $1; }
    | '-' factor %prec UMINUS   { $$ = mk(NK_NEG, yylineno, NULL, 0, cons($2,NULL)); }
    ;

primary
    : T_NUMBER                  { $$ = mk(NK_NUMBER, yylineno, NULL, $1, NULL); }
    | T_IDENT                   { $$ = mk(NK_IDENT,  yylineno, $1, 0, NULL); free($1); }
    | '(' expr ')'              { $$ = $2; }
    | T_NIVEL_TINTA '(' ')'     { $$ = mk(NK_INK_LEVEL, yylineno, NULL, 0, NULL); }
    | T_QTD_PAPEL '(' ')'       { $$ = mk(NK_PAPER_COUNT, yylineno, NULL, 0, NULL); }
    | T_ESTA_PRONTA '(' ')'     { $$ = mk(NK_IS_READY, yylineno, NULL, 0, NULL); }
    | T_PAGINAS_IMPRESSAS '(' ')' { $$ = mk(NK_PAGES_PRINTED, yylineno, NULL, 0, NULL); }
    | T_MODO_COR '(' ')'        { $$ = mk(NK_IS_COLOR, yylineno, NULL, 0, NULL); }
    | T_TAMANHO_FILA '(' ')'    { $$ = mk(NK_QUEUE_SIZE, yylineno, NULL, 0, NULL); }
    ;

%%

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
