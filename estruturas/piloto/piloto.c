#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "piloto.h"
#include "../carro/carro.h"

PilhaPiloto *IniciarPilhaPiloto() {
    PilhaPiloto *pilhaPiloto = (PilhaPiloto *)malloc(sizeof(PilhaPiloto));
    if (pilhaPiloto == NULL) {
        printf("Erro ao criar o ponteiro.\n");
        return NULL;
    }

    pilhaPiloto->topo = NULL;

    return pilhaPiloto;
}

void PilhaDePilotos(PilhaPiloto **pilhaPiloto) {
    char linha[256];

    FILE *arquivo_pilotos = fopen("docs/pilotos.txt", "r");
    if (arquivo_pilotos == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo_pilotos)) {
        Piloto *piloto = (Piloto *)malloc(sizeof(Piloto));
        if(piloto == NULL) continue;

        if (sscanf(linha, "%20[^-] - %10s", piloto->nome, piloto->sexo) == 2) {

            size_t len_nome = strlen(piloto->nome);
            if (len_nome > 0 && piloto->nome[len_nome - 1] == ' ') {
                piloto->nome[len_nome - 1] = '\0';
            }

            // Remove \n, \r or spaces from sexo
            size_t len_sexo = strlen(piloto->sexo);
            while (len_sexo > 0 && (piloto->sexo[len_sexo - 1] == ' ' || piloto->sexo[len_sexo - 1] == '\n' || piloto->sexo[len_sexo - 1] == '\r')) {
                piloto->sexo[len_sexo - 1] = '\0';
                len_sexo--;
            }
            
            piloto->prox = (*pilhaPiloto)->topo;
            (*pilhaPiloto)->topo = piloto;
        
        } else {
            free(piloto);
        }
          
    }

    fclose(arquivo_pilotos);   
}

void AtribuirCarroAoPiloto(PilhaPiloto **pilhaPiloto, PilhaCarro **pilhaCarro) {
    Piloto *piloto = (*pilhaPiloto)->topo;
    Carro *carro = (*pilhaCarro)->topo;

    do {
        piloto->carro = carro;
        piloto = piloto->prox;
        carro = carro->prox;   
    } while (piloto != NULL && carro != NULL);

    free((*pilhaCarro));
}

void ExibirPilotos(PilhaPiloto *pilhaPiloto) {
    if (pilhaPiloto->topo == NULL) {
        printf("Pilha de piloto vazia!\n");
    }
    
    Piloto *piloto = pilhaPiloto->topo;
    
    do {
        printf("Nome: %s | Sexo: %s | Carro:\n", piloto->nome, piloto->sexo);
        ExibirCarro(piloto->carro);
        printf("\n");
        piloto = piloto->prox;
        
    } while (piloto->prox != NULL);
    
}

void LimparPilotos(PilhaPiloto **pilhaPiloto) {
    
    do {
        Piloto *piloto = (*pilhaPiloto)->topo;
        (*pilhaPiloto)->topo = piloto->prox;
        free(piloto->carro);
        free(piloto);
    } while ((*pilhaPiloto)->topo != NULL);

    free((*pilhaPiloto));
    
}