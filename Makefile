# Compilador
CC = gcc

# Flags de compilação
FLAGS = -Wall -Wextra -g3 -std=c99

# Nome do executável
TARGET = build/output.exe

# Todos os arquivos .c (adicione/remova conforme necessário)
SOURCES = $(shell find . -name "*.c")

ADITIONAL = -lm

# Regra padrão
all: $(TARGET)

# Como compilar
$(TARGET): $(SOURCES)
	$(CC) $(FLAGS) $(SOURCES) -o $(TARGET) $(ADITIONAL)

# Limpar arquivos compilados
clean:
	rm -rf $(TARGET)

# Compilar e executar
run: all
	.\$(TARGET)