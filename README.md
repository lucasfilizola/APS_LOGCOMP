# Impressora
Linguagem para Controle de Impressoras

## Visão Geral

Impressora é uma linguagem para controlar operações básicas de impressoras. Pensada para cenários do dia a dia como impressão de provas, apostilas e documentos.

## Exemplo Rápido

```impressora
// Professora imprimindo prova
int alunos = 30;

se (nivel_tinta() < 20) {
    imprimir_texto("Tinta baixa!");
}

definir_cor(falso);
definir_qualidade(MEDIA);
imprimir("prova.pdf", alunos);

imprimir_texto("Concluído!");
```

## Comandos

### Configuração

```impressora
definir_cor(verdadeiro);     // verdadeiro = colorido, falso = P&B
definir_qualidade(ALTA);     // BAIXA, MEDIA, ALTA
definir_papel(A4);           // A4, CARTA, A3
```

### Impressão

```impressora
imprimir("arquivo.pdf", 10);      // Imprime arquivo N vezes
imprimir_pagina("Título");        // Imprime uma página
imprimir_texto("Mensagem");       // Exibe mensagem
```

### Verificação

```impressora
verificar_tinta();       // Verifica tinta
verificar_papel();       // Verifica papel
aguardar_pronta();       // Aguarda impressora
resetar();               // Reseta contador
```

### Sensores

```impressora
nivel_tinta()            // Nível de tinta (0-100)
qtd_papel()              // Quantidade de folhas
esta_pronta()            // 1 se pronta, 0 caso contrário
paginas_impressas()      // Total impresso
modo_cor()               // 1 se colorido, 0 se P&B
tamanho_fila()           // Tamanho da fila
```

## EBNF

```ebnf
program         = { decl | stmt } ;

decl            = "int" ident [ "=" expr ] ";"
                | "string" ident [ "=" string ] ";" ;

stmt            = assign ";"
                | ifStmt
                | whileStmt
                | block
                | printStmt
                | printerStmt
                | ";" ;

assign          = ident "=" (expr | string) ;
ifStmt          = "se" "(" expr ")" stmt [ "senao" stmt ] ;
whileStmt       = "enquanto" "(" expr ")" stmt ;
block           = "{" { stmt } "}" ;

printStmt       = "imprimir" "(" (expr | string) ")" ";"
                | "imprimir_texto" "(" string ")" ";" ;

printerStmt     = printDoc
                | printPage
                | setColor
                | setQuality
                | setPaper
                | checkInk
                | checkPaper
                | waitReady
                | reset ;

printDoc        = "imprimir" "(" string "," expr ")" ";" ;
printPage       = "imprimir_pagina" "(" string ")" ";" ;
setColor        = "definir_cor" "(" expr ")" ";" ;
setQuality      = "definir_qualidade" "(" quality ")" ";" ;
setPaper        = "definir_papel" "(" paperType ")" ";" ;
checkInk        = "verificar_tinta" "(" ")" ";" ;
checkPaper      = "verificar_papel" "(" ")" ";" ;
waitReady       = "aguardar_pronta" "(" ")" ";" ;
reset           = "resetar" "(" ")" ";" ;

quality         = "BAIXA" | "MEDIA" | "ALTA" ;
paperType       = "A4" | "CARTA" | "A3" ;

primary         = number
                | ident
                | "(" expr ")"
                | "nivel_tinta" "(" ")"
                | "qtd_papel" "(" ")"
                | "esta_pronta" "(" ")"
                | "paginas_impressas" "(" ")"
                | "modo_cor" "(" ")"
                | "tamanho_fila" "(" ")" ;

expr            = equality ;
equality        = relational { ("==" | "!=") relational } ;
relational      = additive  { ("<" | "<=" | ">" | ">=") additive } ;
additive        = term      { ("+" | "-" ) term } ;
term            = factor    { ("*" | "/" ) factor } ;
factor          = [ "-" ] primary ;

ident           = letter { letter | digit | "_" } ;
number          = digit { digit } ;
string          = "\"" { any_char_except_quote } "\"" ;
letter          = "A"…"Z" | "a"…"z" ;
digit           = "0"…"9" ;

comment_line    = "//" { any_char_exc_newline } ;
comment_block   = "/*" { any_char } "*/" ;
```

## Exemplos

### Impressão Simples
```impressora
definir_cor(falso);
definir_qualidade(MEDIA);
imprimir("apostila.pdf", 1);
```

### Verificação de Recursos
```impressora
int copias = 50;

se (qtd_papel() < copias) {
    imprimir_texto("Papel insuficiente!");
} senao {
    se (nivel_tinta() < 15) {
        imprimir_texto("Tinta baixa!");
    } senao {
        imprimir("trabalho.pdf", copias);
        imprimir_texto("Sucesso!");
    }
}
```

### Impressão em Lote
```impressora
definir_cor(verdadeiro);
definir_qualidade(ALTA);

imprimir("capa.pdf", 1);
imprimir("conteudo.pdf", 1);
imprimir("anexos.pdf", 1);

imprimir_texto("Total:");
imprimir(paginas_impressas());
```

### Loop com Verificação
```impressora
int turma = 1;

enquanto (turma <= 3) {
    se (nivel_tinta() < 20) {
        imprimir_texto("Reabastecer tinta!");
        verificar_tinta();
    }

    imprimir("prova_turma.pdf", 35);
    turma = turma + 1;
}
```

## Requisitos

- Flex
- Bison
- GCC
- Make
- Python 3

### Instalação
```bash
sudo apt-get install flex bison gcc make python3
```

## Compilação e Uso

```bash
make                                  # Compila o compilador
./impressora saida.asm < prog.imp     # Gera assembly
python3 impressoravm.py saida.asm     # Executa na VM
```

## Testes Prontos

Na pasta `testes/` você encontra programas prontos:

```bash
# Impressão de provas
./impressora testes/prova.asm < testes/prova.imp
python3 impressoravm.py testes/prova.asm

# Impressão de apostilas coloridas
./impressora testes/apostila.asm < testes/apostila.imp
python3 impressoravm.py testes/apostila.asm

# Impressão em lote para múltiplas turmas
./impressora testes/lote.asm < testes/lote.imp
python3 impressoravm.py testes/lote.asm

# Impressão de certificados
./impressora testes/certificado.asm < testes/certificado.imp
python3 impressoravm.py testes/certificado.asm
```
