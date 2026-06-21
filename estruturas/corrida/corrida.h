#ifndef CORRIDA_H
#define CORRIDA_H

#include "../piloto/piloto.h"
#include "../pista/pista.h"

// =========================================================
// RESULTADO DE UMA CORRIDA
// =========================================================
typedef struct Resultado
{
    Piloto *piloto;
    int colocacao;
    int pontuacao;
    double tempoFinal;
    int abandonou;
    int voltaAbandono;

    struct Resultado *prox;
} Resultado;

// =========================================================
// CORRIDA (UMA POR PISTA)
// =========================================================
typedef struct Corrida
{
    Pista *pista;
    Resultado *resultado;

    struct Corrida *prox;
} Corrida;

// =========================================================
// LISTA DE CORRIDAS (CAMPEONATO)
// =========================================================
typedef struct ListaCorrida
{
    Corrida *inicio;
} ListaCorrida;

// =========================================================
// PRINCIPAL
// =========================================================
ListaCorrida *IniciarListaCorrida();

void CriarCorrida(ListaCorrida **listaCorrida, Pista *pista);

void SimularCorrida(PilhaPiloto *pilhaPiloto,
                    Pista *pista,
                    ListaCorrida **listaCorrida);

// =========================================================
// RESULTADOS
// =========================================================
void SalvarResultado(ListaCorrida **listaCorrida,
                     Piloto *piloto,
                     int colocacao,
                     int pontuacao,
                     double tempoFinal,
                     int abandonou,
                     int voltaAbandono);

void ExibirResultados(ListaCorrida *listaCorrida);

// =========================================================
// LOGICA DA CORRIDA
// =========================================================
int VerificarAcidente(Pista *pista, Piloto *piloto, int voltaAtual, double tempoAtual);

void OrdenarCorrida(Piloto **grid, int qtd);

// =========================================================
// CAMPEONATO
// =========================================================
void ExibirRankingCampeonato(PilhaPiloto *pilhaPiloto);

// =========================================================
// LIMPEZA
// =========================================================
void LimparCorrida(ListaCorrida **listaCorrida);

#endif