#ifndef FILA_CHEGADA_H
#define FILA_CHEGADA_H

#include "../piloto/piloto.h"

typedef struct NodoFila {
    Piloto *piloto;
    int tempo_total;
    struct NodoFila *prox;
} NodoFila;

typedef struct FilaChegada {
    NodoFila *frente;
    NodoFila *tras;
} FilaChegada;

FilaChegada* IniciarFilaChegada();
void InserirFilaChegada(FilaChegada *fila, Piloto *piloto, int tempo);
NodoFila* RemoverFilaChegada(FilaChegada *fila);
void LimparFilaChegada(FilaChegada **fila);
void ExibirFilaChegadaEGravarRelatorio(FilaChegada *fila, const char *nome_pista, int corrida_num, int pontos_ganhos[]);

#endif