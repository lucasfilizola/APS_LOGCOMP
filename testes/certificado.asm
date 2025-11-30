; Impressora Assembly

PUSH 50
STORE 0
PRINTS "Iniciando impressão de certificados..."
PUSH 1
POP R0
SET_COLOR R0
PUSH 2
POP R0
SET_QUALITY R0
PUSH 0
POP R0
SET_PAPER R0
GET_PAPER_COUNT
LOAD 0
GE
JUMPZ L0
PRINTS "Contador resetado."
RESET
PRINTS "Certificados impressos!"
LOAD 0
POP R0
PRINT_DOC "certificados.pdf" R0
GOTO L1
L0:
PRINTS "ERRO: Papel insuficiente para certificados!"
L1:
CHECK_INK
CHECK_PAPER
HALT
