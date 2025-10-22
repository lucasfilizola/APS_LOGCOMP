#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
int yyparse(void);

int main(int argc, char** argv) {
  const char* out_path = NULL;

  if (argc < 2) {
    fprintf(stderr, "Uso: %s arquivo.edu [-o saida.edasm]\n", argv[0]);
    return 1;
  }
  for (int i=2; i<argc; ++i) {
    if (strcmp(argv[i], "-o")==0 && i+1 < argc) { out_path = argv[++i]; }
  }

  FILE* in = fopen(argv[1], "r");
  if (!in) { perror("erro abrindo arquivo"); return 1; }
  yyin = in;

  int rc = yyparse();
  fclose(in);

  if (rc == 0) {
    printf("Sintaxe OK\n");
    if (out_path) {
      FILE* out = fopen(out_path, "w");
      if (!out) { perror("erro criando saida"); return 1; }
      fprintf(out, "// EduLang — stub Tarefa #2\n");
      fprintf(out, "// Sintaxe verificada. Geração de código virá na Tarefa #3.\n");
      fclose(out);
    }
    return 0;
  } else {
    fprintf(stderr, "Falha na análise sintática\n");
    return 2;
  }
}
