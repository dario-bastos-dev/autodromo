#include <stdlib.h>
#include <stdio.h>
#include "estruturas/carro/carro.h"
#include "estruturas/piloto/piloto.h"

int main(void) {
    PilhaCarro *pilhaCarro = IniciarPilhaCarro();
    if (pilhaCarro == NULL) {
    printf("Erro ao criar pilha de carro\n");
    return 1;
    }

    PilhaPiloto *pilhaPiloto = IniciarPilhaPiloto();
    if (pilhaPiloto == NULL) {
    printf("Erro ao criar pilha de piloto\n");
    return 1;
    }
    
    PilhaDeCarros(&pilhaCarro);
    PilhaDePilotos(&pilhaPiloto);
    AtribuirCarroAoPiloto(&pilhaPiloto, &pilhaCarro);
    ExibirPilotos(pilhaPiloto);
    LimparPilotos(&pilhaPiloto);

    return 0;
}