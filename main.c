#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "estruturas/carro/carro.h"
#include "estruturas/piloto/piloto.h"
#include "estruturas/pista/pista.h"
#include "estruturas/corrida/corrida.h"

int main(void) {
    srand(time(NULL));

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

    Pista *pistas = IniciarPistas();
    if (pistas == NULL) {
        printf("Erro ao organizar pistas\n");
        return 1;
    }
    
    ListaCorrida *listaCorrida = IniciarListaCorrida();
    if (listaCorrida == NULL) {
        printf("Erro ao iniciar as corridas\n");
        return 1;
    }
    
    
    PilhaDeCarros(&pilhaCarro);
    PilhaDePilotos(&pilhaPiloto);
    AtribuirCarroAoPiloto(&pilhaPiloto, &pilhaCarro);
    ExibirPilotos(pilhaPiloto);
    ExibirPistas(pistas);
    LimparPilotos(&pilhaPiloto);

    return 0;
}