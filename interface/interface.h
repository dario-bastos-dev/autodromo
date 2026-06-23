#ifndef INTERFACE_H
#define INTERFACE_H

#include "../estruturas/piloto/piloto.h"
#include "../estruturas/pista/pista.h"
#include "../estruturas/fila_chegada/fila_chegada.h"

/* ── Funcoes de Interface ─────────────────────────────── */

/**
 * Exibe o banner de boas-vindas do campeonato
 */
void ExibirBanner();

/**
 * Exibe a tela de preparacao com pilotos e carros
 */
void ExibirPreparacao(PilhaPiloto *pilhaPiloto, Pista *pistas);

/**
 * Exibe animacao de largada da corrida
 */
void AnimarLargada(int corrida_num, const char *nome_pista);

/**
 * Exibe status dinamico da corrida durante a execucao
 */
void ExibirStatusCorrida(int volta_atual, int total_voltas, int tempo_decorrido);

/**
 * Exibe pilotos que sofreram acidentes
 */
void ExibirAcidente(const char *nome_piloto, int volta);

/**
 * Exibe leaderboard em tempo real
 */
void ExibirLeaderboardTempo(FilaChegada *fila);

/**
 * Exibe o podio final do campeonato
 */
void ExibirPodinoCampeonato(void *arrayStats, int numPilotos);

/**
 * Simula e exibe a corrida em tempo real com ultrapassagens
 */
void SimularEExibirCorrida(PilotoStats *arrayStats, int numPilotos, Pista *pista, int corrida_num);

/**
 * Limpa a tela do terminal
 */
void LimparTela();

/**
 * Exibe linha decorativa
 */
void ExibirLinha();

#endif
