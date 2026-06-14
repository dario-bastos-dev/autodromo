#include "../carro/carro.h"

#ifndef PILOTO
#define PILOTO

typedef struct Piloto {
    char nome[21];
    char sexo[10];
    Carro *carro;
    struct Piloto *prox;
} Piloto;

typedef struct PilhaPiloto {
    Piloto *topo;
} PilhaPiloto;

PilhaPiloto *IniciarPilhaPiloto();
void PilhaDePilotos(PilhaPiloto **pilhaPiloto);
void AtribuirCarroAoPiloto(PilhaPiloto **pilhaPiloto, PilhaCarro **pilhaCarro);
void ExibirPilotos(PilhaPiloto *pilhaPiloto);
void ExibirPiloto(Piloto *piloto);
void LimparPilotos(PilhaPiloto **pilhaPiloto);

#endif