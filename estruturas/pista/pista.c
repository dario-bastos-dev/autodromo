#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pista.h"

Pista *IniciarPistas() {
    Pista *pistas = (Pista *)malloc(sizeof(Pista) * 6);
    if (pistas == NULL) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }

    char linha[256];

    FILE *arquivo_pistas = fopen("docs/pistas.txt", "r");
    if (arquivo_pistas == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    int posicao = 0;
    while (fgets(linha, sizeof(linha), arquivo_pistas)) {
        
        if (sscanf(linha, "%19s - %d - %f", pistas[posicao].nome, &pistas[posicao].tamanho, &pistas[posicao].acidente) == 3) {

            size_t len_nome = strlen(pistas[posicao].nome);
            if (len_nome > 0 && pistas[posicao].nome[len_nome - 1] == ' ') {
                pistas[posicao].nome[len_nome - 1] = '\0';
            }
        
        }
        posicao++;
          
    }

    fclose(arquivo_pistas);

    for (int j = 5; j > 0; j--) {
        int r = rand() % (j + 1);
        
        Pista temp = pistas[j];
        pistas[j] = pistas[r];
        pistas[r] = temp;
    }

    return pistas;
    
}

void ExibirPistas(Pista *pistas) {
    for (int i = 0; i < 6; i++) {
        printf("Pista: %s | Tamanho: %d km | Chance de acidente: %.0f%\n", pistas[i].nome, pistas[i].tamanho, pistas[i].acidente * 100);
    }
}

void ExibirPista(Pista *pista) {
    printf("Pista: %s | Tamanho: %d km | Chance de acidente: %.0f%\n", pista->nome, pista->tamanho, pista->acidente * 100);
}