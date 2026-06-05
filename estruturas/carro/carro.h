#ifndef CARRO

typedef struct Carro {
    char marca[21];
    char modelo[21];
    int potencia;
    struct Carro *prox;
} Carro;

typedef struct PilhaCarro {
    Carro *topo;
} PilhaCarro;

PilhaCarro *IniciarPilhaCarro();
void PilhaDeCarros(PilhaCarro **pilhaCarro);
void ExibirCarros(PilhaCarro *pilhaCarros);

#endif