#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "carro.h"

PilhaCarro *IniciarPilhaCarro() {
    PilhaCarro *pilhaCarros = (PilhaCarro *)malloc(sizeof(PilhaCarro));
    if (pilhaCarros == NULL) {
        printf("Erro ao criar o ponteiro.\n");
        return NULL;
    }

    pilhaCarros->topo = NULL;

    return pilhaCarros;

}

void PilhaDeCarros(PilhaCarro **pilhaCarro) {
    char linha[256];

    FILE *arquivo_carros = fopen("docs/carros.txt", "r");
    if (arquivo_carros == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo_carros)) {
        Carro *carro = (Carro *)malloc(sizeof(Carro));
        if(carro == NULL) continue;

        if (sscanf(linha, "%[^-] - %[^-] - %d", carro->modelo, carro->marca, &carro->potencia) == 3) {

            size_t len_modelo = strlen(carro->modelo);
            if (len_modelo > 0 && carro->modelo[len_modelo - 1] == ' ') {
                carro->modelo[len_modelo - 1] = '\0';
            }

            size_t len_marca = strlen(carro->marca);
            if (len_marca > 0 && carro->marca[len_marca - 1] == ' ') {
                carro->marca[len_marca - 1] = '\0';
            }
            
            carro->prox = (*pilhaCarro)->topo;
            (*pilhaCarro)->topo = carro;
        
        } else {
            free(carro);
        }
          
    }

    fclose(arquivo_carros);
    
}

void ExibirCarro(Carro *carro) {
    printf("    Modelo: %s | Marca: %s | Potencia: %d\n", carro->modelo, carro->marca, carro->potencia);
    
}