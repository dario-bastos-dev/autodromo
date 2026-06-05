#ifndef PISTA

typedef struct Pista {
    char nome[21];
    int tamanho;
    float acidente;
    Pista *prox;
} Pista;

#endif