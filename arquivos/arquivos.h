#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../estruturas/carro/carro.h"

/* ── Caminhos padrão ─────────────────────────────────── */
#define ARQUIVO_CARROS  "dados/carros.csv"
#define ARQUIVO_LOG     "logs/corrida.log"

/* ── Status possíveis de um carro na corrida ─────────── */
#define STATUS_CORRENDO  "CORRENDO"
#define STATUS_ACIDENTE  "ACIDENTE"
#define STATUS_LARGOU    "LARGOU"
#define STATUS_VENCEDOR  "VENCEDOR"

/* ── Struct de estado de corrida (uso interno deste módulo) ── */
typedef struct EstadoCarro {
    char marca[21];
    char modelo[21];
    int potencia;
    int posicao;
    int voltas;
    char status[16];
} EstadoCarro;

/* ─────────────────────────────────────────────────────── */
/*  FUNÇÕES DE ARQUIVO                                     */
/* ─────────────────────────────────────────────────────── */

/**
 * Carrega carros de um CSV e os empilha na pilha fornecida.
 * Formato do CSV: marca,modelo,potencia
 * Retorna o número de carros carregados, ou -1 em erro.
 */
int CarregarCarros(const char *caminho, PilhaCarro **pilha);

/**
 * Salva todos os carros da pilha em um CSV.
 * Retorna 0 em sucesso, -1 em erro.
 */
int SalvarCarros(const char *caminho, PilhaCarro *pilha);

/**
 * Salva o estado atual da corrida (posição, voltas, status).
 * Retorna 0 em sucesso, -1 em erro.
 */
int SalvarEstadoCorrida(const char *caminho, EstadoCarro *estado, int total);

/**
 * Carrega o estado de uma corrida salva anteriormente.
 * Preenche o vetor `estado` e retorna o número de entradas lidas, ou -1.
 */
int CarregarEstadoCorrida(const char *caminho, EstadoCarro *estado, int maxCarros);

/* ─────────────────────────────────────────────────────── */
/*  FUNÇÕES DE LOG                                         */
/* ─────────────────────────────────────────────────────── */

/**
 * Abre (ou cria) o arquivo de log. Deve ser chamado antes de LogEvento.
 * Retorna o FILE* aberto, ou NULL em erro.
 */
FILE *AbrirLog(const char *caminho);

/**
 * Fecha o arquivo de log aberto por AbrirLog.
 */
void FecharLog(FILE *log);

/**
 * Registra um evento livre com timestamp automático.
 * Exemplo: LogEvento(log, "INICIO", "Corrida iniciada com 5 carros");
 */
void LogEvento(FILE *log, const char *tipo, const char *descricao);

/**
 * Registra a posição de todos os carros ao fim de uma volta.
 */
void LogPosicoes(FILE *log, int volta, EstadoCarro *estado, int total);

/**
 * Registra um evento de ultrapassagem entre dois carros.
 */
void LogUltrapassagem(FILE *log, int volta,
                      const char *quem_ultrapassa,
                      const char *quem_foi_ultrapassado);

/**
 * Registra um acidente.
 */
void LogAcidente(FILE *log, int volta, const char *modelo_carro);

/**
 * Registra o fim da corrida com o pódio.
 */
void LogFimCorrida(FILE *log, EstadoCarro *estado, int total);

/* ─────────────────────────────────────────────────────── */
/*  UTILITÁRIOS                                            */
/* ─────────────────────────────────────────────────────── */

/**
 * Garante que um diretório existe; cria se necessário (POSIX).
 * Retorna 0 em sucesso, -1 em erro.
 */
int CriarDiretorioSeNecessario(const char *caminho);

/**
 * Preenche um vetor de EstadoCarro a partir da pilha,
 * atribuindo posições iniciais por ordem de potência.
 * Retorna o total de carros preenchidos.
 */
int PilhaParaEstado(PilhaCarro *pilha, EstadoCarro *estado, int maxCarros);

#endif /* ARQUIVOS_H */
