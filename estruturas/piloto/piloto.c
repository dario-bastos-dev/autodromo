#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "piloto.h"
#include "../carro/carro.h"

#define MAX_PILOTOS 100
#define MAX_CARROS 100

PilhaPiloto *IniciarPilhaPiloto()
{
    PilhaPiloto *pilhaPiloto = (PilhaPiloto *)malloc(sizeof(PilhaPiloto));

    if (pilhaPiloto == NULL)
    {
        printf("Erro ao criar o ponteiro.\n");
        return NULL;
    }

    pilhaPiloto->topo = NULL;
    return pilhaPiloto;
}

void PilhaDePilotos(PilhaPiloto **pilhaPiloto)
{
    char linha[256];

    FILE *arquivo_pilotos = fopen("docs/pilotos.txt", "r");

    if (arquivo_pilotos == NULL)
    {
        printf("Erro ao abrir o arquivo de pilotos.\n");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo_pilotos))
    {
        Piloto *piloto = (Piloto *)malloc(sizeof(Piloto));

        if (piloto == NULL)
            continue;

        piloto->abandonou = 0;
        piloto->voltaAbandono = 0;
        piloto->tempoFinal = 0;
        piloto->pontosCampeonato = 0;
        piloto->melhor_posicao = 0;

        piloto->carro = NULL;
        piloto->prox = NULL;

        char sexoTemp;

        if (sscanf(linha, " %49[^-] - %c", piloto->nome, &sexoTemp) == 2)
        {
            piloto->sexo[0] = sexoTemp;
            piloto->sexo[1] = '\0';

            size_t len = strlen(piloto->nome);

            if (len > 0 && piloto->nome[len - 1] == ' ')
                piloto->nome[len - 1] = '\0';

            piloto->prox = (*pilhaPiloto)->topo;
            (*pilhaPiloto)->topo = piloto;
        }
        else
        {
            free(piloto);
        }
    }

    fclose(arquivo_pilotos);
}

// =========================================================
// ATRIBUI CARROS ALEATORIOS AOS PILOTOS
// Cada campeonato tera uma distribuicao diferente.
// Nenhum piloto fica preso fixamente a um carro.
// =========================================================
void AtribuirCarroAoPiloto(PilhaPiloto **pilhaPiloto, PilhaCarro **pilhaCarro)
{
    if (!pilhaPiloto || !(*pilhaPiloto) || !pilhaCarro || !(*pilhaCarro))
        return;

    Piloto *pilotos[MAX_PILOTOS];
    Carro *carros[MAX_CARROS];

    int qtdPilotos = 0;
    int qtdCarros = 0;

    Piloto *pilotoAtual = (*pilhaPiloto)->topo;

    while (pilotoAtual != NULL && qtdPilotos < MAX_PILOTOS)
    {
        pilotos[qtdPilotos++] = pilotoAtual;
        pilotoAtual = pilotoAtual->prox;
    }

    Carro *carroAtual = (*pilhaCarro)->topo;

    while (carroAtual != NULL && qtdCarros < MAX_CARROS)
    {
        carros[qtdCarros++] = carroAtual;
        carroAtual = carroAtual->prox;
    }

    if (qtdPilotos == 0 || qtdCarros == 0)
        return;

    // Embaralha os carros.
    for (int i = qtdCarros - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        Carro *tmp = carros[i];
        carros[i] = carros[j];
        carros[j] = tmp;
    }

    // Atribui os carros embaralhados aos pilotos.
    // Se houver mais pilotos do que carros, os pilotos restantes ficam sem carro.
    for (int i = 0; i < qtdPilotos; i++)
    {
        if (i < qtdCarros)
            pilotos[i]->carro = carros[i];
        else
            pilotos[i]->carro = NULL;
    }
}

void ExibirPilotos(PilhaPiloto *pilhaPiloto)
{
    if (!pilhaPiloto || !pilhaPiloto->topo)
    {
        printf("Nenhum piloto cadastrado.\n");
        return;
    }

    Piloto *piloto = pilhaPiloto->topo;

    while (piloto != NULL)
    {
        printf("Nome: %s | Sexo: %s\n", piloto->nome, piloto->sexo);

        if (piloto->carro)
            ExibirCarro(piloto->carro);
        else
            printf("    Sem carro atribuido.\n");

        piloto = piloto->prox;
    }
}

void ExibirPiloto(Piloto *piloto)
{
    if (!piloto)
        return;

    printf("Nome: %s | Sexo: %s\n", piloto->nome, piloto->sexo);

    if (piloto->carro)
        ExibirCarro(piloto->carro);
    else
        printf("    Sem carro atribuido.\n");
}

void LimparPilotos(PilhaPiloto **pilhaPiloto)
{
    if (!pilhaPiloto || !(*pilhaPiloto))
        return;

    Piloto *piloto = (*pilhaPiloto)->topo;

    while (piloto != NULL)
    {
        Piloto *tmp = piloto;
        piloto = piloto->prox;
        free(tmp);
    }

    free(*pilhaPiloto);
    *pilhaPiloto = NULL;
}