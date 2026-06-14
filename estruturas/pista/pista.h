#ifndef PISTA
#define PISTA

typedef struct Pista {
    char nome[20];
    int tamanho;
    float acidente;
} Pista;

Pista *IniciarPistas();
void ExibirPistas(Pista *pistas);
void ExibirPista(Pista *pista);

#endif