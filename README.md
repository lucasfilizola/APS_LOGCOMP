# APS_LOGCOMP
Repositório para APS de LogComp


APS – Lógica da Computação
Proposta de Linguagem Educacional

# 1 Definições Importantes:
Uma linguagem simples para alunos e professores automatizarem pequenas rotinas educacionais, como registrar notas, calcular médias, aplicar limites e emitir alertas. O compilador cuida de condições, laços e variáveis internas e gera o assembly .edasm da EduVM.

# 2 Exemplos de Comandos:
definir nota 80
somar 10 a nota
mostrar media
alarme se media maior 90
parar


# 3 Visão Geral
Foco no usuário: frases em português, uma instrução por linha, sem símbolos estranhos.
O compilador implementa internamente condicionais e loops simples.
A EduVM possui dois registradores (RA, RB), memória básica (PUSH, POP), sensores de ambiente (MEDIA, ACERTOS, TEMPO) e instruções como SET, ADD, CMP, PRINT, HALT.
A linguagem não expõe if/while; o compilador traduz frases diretas em estruturas de controle equivalentes.


# 4 ReferÊncia de Comandos:


Controle e cálculo

definir nota N


Define a variável nota. → SET nota N

somar N a nota
subtrair N de nota


Ajusta o valor da nota. → ADD / SUB

Condições e alarmes

alarme se media REL K


Dispara alerta se a condição for verdadeira.
REL ∈ {maior, menor, igual} → gera CMP MEDIA, K + salto.

Monitoramento

garantir media acima de X por Ts


Garante média mínima durante tempo T.
Internamente: laço com checagens de MEDIA.

Exibição

mostrar media | mostrar nota | mostrar acertos | mostrar tempo


Imprime valor na tela. → PRINT

Interação

perguntar "texto"


Mostra a mensagem e lê inteiro. → PROMPT, READINT

Temporização

esperar Ts


Pausa de T segundos. → laço DECJZ

Encerramento

parar | halt


Finaliza o programa. → HALT


# 5 Gramática (EBNF)
   PROGRAMA   = { LINHA } ;
LINHA      = [ COMANDO ], ( "\n" | ";" ) ;

COMANDO    = DEFINIR | AJUSTE | ALARME | GARANTIR | MOSTRAR | PERGUNTA | ESPERA | PARAR ;

DEFINIR    = "definir", "nota", INT ;
AJUSTE     = ( "somar", INT, "a", "nota" )
           | ( "subtrair", INT, "de", "nota" ) ;

ALARME     = "alarme", "se", "media", REL, INT ;
REL        = "maior" | "menor" | "igual" ;

GARANTIR   = "garantir", "media", "acima", "de", INT, "por", DUR ;

MOSTRAR    = "mostrar", ( "media" | "nota" | "acertos" | "tempo" ) ;

PERGUNTA   = "perguntar", STRING ;
ESPERA     = "esperar", DUR ;

PARAR      = "parar" | "halt" ;

INT        = DIGITO, { DIGITO } ;
DUR        = INT, "s" ;
STRING     = '"', { CAR }, '"' ;

DIGITO     = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
CAR        = ? qualquer caractere exceto aspas ? ;


# 6 Exemplos
Exemplos

Cálculo simples

definir nota 70
somar 10 a nota
mostrar nota
parar


Alerta

alarme se media maior 80
mostrar media
parar


Interação e espera

perguntar "Quantos exercícios você fez?"
esperar 10s
mostrar tempo
parar



