; Impressora Assembly

PUSH 30
STORE 0
PRINTS "Verificando recursos..."
GET_INK_LEVEL
PUSH 20
LT
JUMPZ L1
CHECK_INK
PRINTS "AVISO: Tinta baixa!"
L1:
GET_PAPER_COUNT
LOAD 0
LT
JUMPZ L2
PRINTS "ERRO: Papel insuficiente!"
GOTO L3
L2:
GET_PAGES_PRINTED
PRINT
PRINTS "Total de páginas:"
PRINTS "Impressão concluída!"
LOAD 0
POP R0
PRINT_DOC "prova_matematica.pdf" R0
PRINTS "Imprimindo provas..."
PUSH 1
POP R0
SET_QUALITY R0
PUSH 0
POP R0
SET_COLOR R0
L3:
HALT
