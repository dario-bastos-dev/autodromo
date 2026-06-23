#include <stdio.h>
#include <stdlib.h>
#include "fila_chegada.h"

FilaChegada* IniciarFilaChegada() {
    FilaChegada *fila = (FilaChegada*) malloc(sizeof(FilaChegada));
    if (fila != NULL) {
        fila->frente = NULL;
        fila->tras = NULL;
    }
    return fila;
}

void InserirFilaChegada(FilaChegada *fila, Piloto *piloto, int tempo) {
    if (fila == NULL) return;

    NodoFila *novo = (NodoFila*) malloc(sizeof(NodoFila));
    if (novo == NULL) return;

    novo->piloto = piloto;
    novo->tempo_total = tempo;
    novo->prox = NULL;

    if (fila->tras == NULL) {
        fila->frente = novo;
    } else {
        fila->tras->prox = novo;
    }
    fila->tras = novo;
}

NodoFila* RemoverFilaChegada(FilaChegada *fila) {
    if (fila == NULL || fila->frente == NULL) return NULL;

    NodoFila *removido = fila->frente;
    fila->frente = fila->frente->prox;

    if (fila->frente == NULL) {
        fila->tras = NULL;
    }

    return removido;
}

void LimparFilaChegada(FilaChegada **fila) {
    if (fila == NULL || *fila == NULL) return;

    NodoFila *atual = (*fila)->frente;
    while (atual != NULL) {
        NodoFila *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    free(*fila);
    *fila = NULL;
}

void ExibirFilaChegadaEGravarRelatorio(FilaChegada *fila, const char *nome_pista, int corrida_num, int pontos_ganhos[]) {
    if (fila == NULL || fila->frente == NULL) {
        printf("Fila de chegada vazia!\n");
        return;
    }

    char nome_arquivo[64];
    sprintf(nome_arquivo, "logs/relatorio_corrida_%d.txt", corrida_num);

    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de relatorio!\n");
    }

    printf("\n=== RESULTADO DA CORRIDA %d: %s ===\n", corrida_num, nome_pista);
    if(arquivo) fprintf(arquivo, "=== RESULTADO DA CORRIDA %d: %s ===\n\n", corrida_num, nome_pista);

    NodoFila *atual = fila->frente;
    int colocacao = 1;

    while (atual != NULL) {
        int pontos = (colocacao <= 10 && atual->tempo_total != 99999) ? pontos_ganhos[colocacao - 1] : 0;

        if (atual->tempo_total == 99999) {
            printf("%do Lugar: Piloto: %s | Status: ACIDENTE (DNF) | Pontos: +0\n", colocacao, atual->piloto->nome);
            
            if(arquivo) {
                fprintf(arquivo, "%do Lugar: %s\n", colocacao, atual->piloto->nome);
                fprintf(arquivo, "   Carro: %s %s (Potencia: %d)\n", 
                        atual->piloto->carro->marca, atual->piloto->carro->modelo, atual->piloto->carro->potencia);
                fprintf(arquivo, "   Status: ACIDENTE (Incompletou a prova)\n");
                fprintf(arquivo, "   Pontos Ganhos: 0\n\n");
            }
        } else {
            printf("%do Lugar: Piloto: %s | Tempo: %d seg | Pontos: +%d\n", colocacao, atual->piloto->nome, atual->tempo_total, pontos);
            
            if(arquivo) {
                fprintf(arquivo, "%do Lugar: %s\n", colocacao, atual->piloto->nome);
                fprintf(arquivo, "   Carro: %s %s (Potencia: %d)\n", 
                        atual->piloto->carro->marca, atual->piloto->carro->modelo, atual->piloto->carro->potencia);
                fprintf(arquivo, "   Tempo Total: %d segundos\n", atual->tempo_total);
                fprintf(arquivo, "   Pontos Ganhos: %d\n\n", pontos);
            }
        }
        
        atual = atual->prox;
        colocacao++;
    }

    if (arquivo) {
        fclose(arquivo);
        printf("-> Relatorio detalhado salvo em '%s'.\n", nome_arquivo);
    }
}