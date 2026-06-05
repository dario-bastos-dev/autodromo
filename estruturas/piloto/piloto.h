#include "carro.h"

#ifndef PILOTO

typedef struct Piloto {
    char nome[21];
    char sexo[2];
    Carro *carro;
    struct Piloto *prox;
} Piloto;

typedef struct PilhaPiloto {
    Piloto *topo;
} PilhaPiloto;

#endif