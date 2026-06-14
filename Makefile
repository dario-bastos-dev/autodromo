# Variáveis por sistema operacional
ifeq ($(OS),Windows_NT)
    # Comandos para Windows
    RM = del /Q /S
    MKDIR = mkdir
    TARGET = build\output.exe
else
    # Comandos para Linux/Mac
    RM = rm -rf
    MKDIR = mkdir -p
    TARGET = ./build/output.exe
endif

# Compilador
CC = gcc

# Flags de compilação
FLAGS = -Wall -Wextra -g3 -std=c99

# Busca todos os arquivos .c (recursivo)
SOURCES = $(shell find . -name "*.c")

ADITIONAL = -lm

# Regra padrão
all: setup $(TARGET)

# Cria a pasta build se ela não existir
setup:
	$(MKDIR) build

# Como compilar: Depende dos fontes (.c)
$(TARGET): $(SOURCES)
	$(CC) $(FLAGS) $(SOURCES) -o $(TARGET) $(ADITIONAL)

# Limpar arquivos compilados
clean:
	$(RM) build

# Compilar e executar: AGORA DEPENDE DE "all"
run: all
	$(TARGET)