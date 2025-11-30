; Impressora Assembly

PUSH 15
STORE 0
PRINTS "Configurando para apostila..."
PUSH 1
POP R0
SET_COLOR R0
PUSH 2
POP R0
SET_QUALITY R0
PUSH 0
POP R0
SET_PAPER R0
PRINTS "Imprimindo capa..."
LOAD 0
POP R0
PRINT_DOC "capa_apostila.pdf" R0
PRINTS "Imprimindo conteúdo..."
LOAD 0
POP R0
PRINT_DOC "conteudo_apostila.pdf" R0
CHECK_INK
CHECK_PAPER
PRINTS "Apostilas impressas com sucesso!"
HALT
