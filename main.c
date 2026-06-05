#include <stdlib.h>
#include <stdio.h>
#include "estruturas/carro/carro.h"

int main(void) {
    PilhaCarro *pilhaCarro = IniciarPilhaCarro();
    if (pilhaCarro == NULL) {
    printf("Erro ao criar pilha\n");
    return 1;
    }
    
    PilhaDeCarros(&pilhaCarro);
    ExibirCarros(pilhaCarro);

    return 0;
}