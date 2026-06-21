#ifndef PILOTO_H
#define PILOTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../carro/carro.h"

typedef struct Piloto {
    char nome[50];
    char sexo[10];

    Carro *carro;

    float pontosCampeonato;
    int melhor_posicao;

    // =========================
    // 🚨 SISTEMA DE CORRIDA
    // =========================
    int abandonou;
    double tempoFinal;
    int voltaAbandono;

    struct Piloto *prox;
} Piloto;

// 🔥 IMPORTANTE: forward declaration
typedef struct PilhaPiloto {
    Piloto *topo;
} PilhaPiloto;

typedef struct PilhaCarro PilhaCarro;

// =========================
// FUNÇÕES
// =========================

PilhaPiloto *IniciarPilhaPiloto();
void PilhaDePilotos(PilhaPiloto **pilhaPiloto);
void AtribuirCarroAoPiloto(PilhaPiloto **pilhaPiloto, PilhaCarro **pilhaCarro);
void ExibirPilotos(PilhaPiloto *pilhaPiloto);
void ExibirPiloto(Piloto *piloto);
void LimparPilotos(PilhaPiloto **pilhaPiloto);
void ExibirClassificacaoCampeonato(PilhaPiloto *pilhaPiloto);
int QuantidadePilotos(PilhaPiloto *pilhaPiloto);

#endif