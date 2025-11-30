# Compilador
CC = gcc
CFLAGS = -Wall -g

# Nomes
TARGET = impressora
LEX_SRC = impressora.l
YACC_SRC = impressora.y

# Arquivos gerados
LEX_OUT = lex.yy.c
YACC_OUT = impressora.tab.c
YACC_HEADER = impressora.tab.h

# Regra principal
$(TARGET): $(YACC_OUT) $(LEX_OUT)
	@echo "Compilando $(TARGET)..."
	$(CC) $(CFLAGS) -o $(TARGET) $(YACC_OUT) $(LEX_OUT) -lfl
	@echo "Build completo!"

# Gerar parser
$(YACC_OUT) $(YACC_HEADER): $(YACC_SRC)
	@echo "Gerando parser com Bison..."
	bison -d -v $(YACC_SRC)

# Gerar lexer
$(LEX_OUT): $(LEX_SRC) $(YACC_HEADER)
	@echo "Gerando lexer com Flex..."
	flex $(LEX_SRC)

# Limpeza
clean:
	@echo "Limpando arquivos gerados..."
	rm -f $(TARGET) $(LEX_OUT) $(YACC_OUT) $(YACC_HEADER) impressora.output *.asm
	@echo "Limpeza concluída!"

# Rebuild completo
rebuild: clean $(TARGET)

.PHONY: clean rebuild
