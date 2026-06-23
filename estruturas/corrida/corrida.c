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
    if (resultado == NULL) return;
    while (resultado != NULL) {
       printf("Piloto: %s | Colocacao: %d | Pontuacao: %d | Tempo: %ld", resultado->piloto->nome, resultado->colocacao, resultado->pontuacao, (long)resultado->tempo);
       resultado = resultado->prox;
    }
}

void ExibirResultados(ListaCorrida *listaCorrida) {
    if (listaCorrida->inicio == NULL) {
        printf("Lista de corrida vazia!\n");
        return;
    }

    Corrida *corrida = listaCorrida->inicio;
    Resultado *resultado = corrida->resultado;

    printf("Resultado das corridas:\n\n");

    while (corrida != NULL && resultado != NULL) {
        ExibirPista(corrida->pista);
        ExibirResultado(resultado);
        corrida = corrida->prox;
        resultado = resultado->prox;
    }
}

void LimparCorrida(ListaCorrida **listaCorrida) {
    if (listaCorrida == NULL || *listaCorrida == NULL) return;
    
    while ((*listaCorrida)->inicio != NULL) {
        Corrida *corrida = (*listaCorrida)->inicio;
        Resultado *resultado = corrida->resultado;
        (*listaCorrida)->inicio = (*listaCorrida)->inicio->prox;
        free(resultado);
        free(corrida);
    }
    
}