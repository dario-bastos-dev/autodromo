#include "../piloto/piloto.h"
#include "../pista/pista.h"
#include <time.h>

#ifndef CORRIDA
#define CORRIDA

typedef struct  Resultado {
    Piloto *piloto;
    int colocacao;
    int pontuacao;
    time_t tempo;
    struct Resultado *prox;
} Resultado;


typedef struct Corrida {
    Pista *pista;
    Resultado *resultado;
    struct Corrida *prox;
} Corrida;

typedef struct ListaCorrida {
    Corrida *inicio;
} ListaCorrida;

ListaCorrida *IniciarListaCorrida();
void CriarCorrida(ListaCorrida **listaCorrida, Pista *pista);
void SalvarResultado(
    ListaCorrida **listaCorrida,
    Piloto *piloto,
    int colocacao,
    int pontuacao,
    time_t tempo);
void ExibirResultados(ListaCorrida *listaCorrida);

#endif