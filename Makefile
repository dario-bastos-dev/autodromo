# Compilador
CC = gcc

# Flags de compilação
FLAGS = -Wall -Wextra -g3 -std=c99

# Nome do executável
TARGET = build/output.exe

# Lista manual e segura dos arquivos .c do seu projeto estruturado
SOURCES = main.c \
          estruturas/carro/carro.c

ADITIONAL = -lm

# Regra padrão
all: $(TARGET)

# Como compilar
$(TARGET): $(SOURCES)
	$(CC) $(FLAGS) $(SOURCES) -o $(TARGET) $(ADITIONAL)

# Limpar arquivos compilados (Compatível com Windows)
clean:
	@if exist build\output.exe del /q build\output.exe

# Compilar e executar (Compatível com Windows)
run: all
	@$(TARGET)
