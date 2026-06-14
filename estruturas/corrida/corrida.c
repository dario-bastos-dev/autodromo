#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "corrida.h"
#include "../piloto/piloto.h"
#include "../pista/pista.h"

ListaCorrida *IniciarListaCorrida() {
    ListaCorrida *listaCorrida = (ListaCorrida *)malloc(sizeof(ListaCorrida));
    if (listaCorrida == NULL) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }

    listaCorrida->inicio = NULL;

    return listaCorrida;
}

void CriarCorrida(ListaCorrida **listaCorrida, Pista *pista) {
    Corrida *corrida = (Corrida *)malloc(sizeof(Corrida));
    if (corrida == NULL) {
        printf("Erro ao iniciar corrida.\n");
        return;
    }

    corrida->pista = pista;
    corrida->resultado = NULL;
    corrida->prox = NULL;
    
    if((*listaCorrida)->inicio == NULL) (*listaCorrida)->inicio = corrida;
    else (*listaCorrida)->inicio->prox = corrida;
}

void SalvarResultado(
    ListaCorrida **listaCorrida,
    Piloto *piloto,
    int colocacao,
    int pontuacao,
    time_t tempo) {
        Resultado *resultado = (Resultado *)malloc(sizeof(Resultado));
        if (resultado == NULL) {
            printf("Erro ao criar resultado.\n");
            return;
        }

        resultado->piloto = piloto;
        resultado->colocacao = colocacao;
        resultado->pontuacao = pontuacao;
        resultado->tempo = tempo;
        resultado->prox = NULL;

        if ((*listaCorrida)->inicio->resultado == NULL) (*listaCorrida)->inicio->resultado = resultado;
        else (*listaCorrida)->inicio->resultado->prox = resultado;   
}

void ExibirResultado(Resultado *resultado) {
    do {
       printf("Piloto: %s | Colocacao: %d | Pontuacao: %d | Tempo: %d", resultado->piloto->nome, resultado->colocacao, resultado->pontuacao, resultado->tempo);
       resultado = resultado->prox;
    } while (resultado != NULL);
    
}

void ExibirResultados(ListaCorrida *listaCorrida) {
    if (listaCorrida->inicio == NULL) {
        printf("Lista de corrida vazia!\n");
        return;
    }

    Corrida *corrida = listaCorrida->inicio;
    Resultado *resultado = corrida->resultado;

    printf("Resultado das corridas:\n\n");

    do {
        ExibirPista(corrida->pista);
        ExibirResultado(resultado);
        corrida = corrida->prox;
        resultado = resultado->prox;
    } while (corrida != NULL && resultado != NULL);
     
}