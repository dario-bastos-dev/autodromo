#include "pista.h"
#include "piloto.h"
#include <time.h>

#ifndef CORRIDA
#define CORRIDA

typedef struct  Resultado {
    Piloto *piloto;
    int colocacao;
    int pontuacao;
    time_t tempo;
    Resultado *prox;
} Resultado;


typedef struct Corrida {
    Pista *pista;
    Resultado *resultado;
    Corrida *prox;
} Carro;

#endif