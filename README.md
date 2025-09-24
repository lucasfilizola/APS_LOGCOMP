# APS_LOGCOMP
Repositório para APS de LogComp


APS – Lógica da Computação
Proposta de Linguagem Educacional
1. Introdução

Essa é a primeira parte da APS de Lógica da Computação. A ideia aqui é propor uma linguagem de programação simples, pensando em algo voltado para o ambiente educacional.
Nessa entrega (24/09/2025), o foco é só montar a gramática da linguagem em EBNF.

2. Motivação

Escolhi o tema de educação porque faz parte do meu dia a dia e acho que combina bem com a proposta da disciplina. A ideia é criar uma linguagem fácil de entender, que possa ser usada em situações ligadas a escola, como resolver contas, testar expressões matemáticas e até simular pequenas regras de sala de aula.

3. Objetivos

A linguagem (que chamei de EduLang) vai ter variáveis, operações matemáticas básicas, condicionais, laços de repetição e comandos de saída (como imprimir valores). A ideia é que seja simples de usar, mas que já dê para praticar conceitos importantes de programação.

4. Estrutura da Linguagem em EBNF

A gramática inicial da EduLang ficou assim:

<programa>   = { <comando> } ;
<comando>    = <atribuicao> | <impressao> | <condicional> | <repeticao> | ";" ;
<atribuicao> = <identificador> "=" <expressao> ";" ;
<impressao>  = ("print" | "log") "(" <expressao> ")" ";" ;
<condicional> = "if" "(" <expressao> ")" "{" { <comando> } "}" 
                [ "else" "{" { <comando> } "}" ] ;
<repeticao>  = "while" "(" <expressao> ")" "{" { <comando> } "}" ;
<expressao>  = <termo> { ("+" | "-") <termo> } ;
<termo>      = <fator> { ("*" | "/") <fator> } ;
<fator>      = <numero> | <identificador> | "(" <expressao> ")" | ("+" | "-") <fator> ;
<identificador> = <letra> { <letra> | <digito> | "_" } ;
<numero>     = <digito> { <digito> } ;
<letra>      = "a" | ... | "z" | "A" | ... | "Z" ;
<digito>     = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;

5. Considerações finais

Com essa gramática em EBNF a gente já tem a base da linguagem. A partir daqui, nas próximas entregas, a ideia é implementar a parte léxica e sintática (com ferramentas tipo Flex e Bison) e depois chegar até a execução em uma VM.