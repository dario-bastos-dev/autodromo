#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>

#include "estruturas/carro/carro.h"
#include "estruturas/piloto/piloto.h"
#include "estruturas/pista/pista.h"
#include "estruturas/corrida/corrida.h"

int main(void)
{
    setlocale(LC_ALL, "");
    srand((unsigned int)time(NULL));

    // =========================
    // ESTRUTURAS BASE
    // =========================
    PilhaCarro *pilhaCarro = IniciarPilhaCarro();
    if (!pilhaCarro) return 1;

    PilhaPiloto *pilhaPiloto = IniciarPilhaPiloto();
    if (!pilhaPiloto) return 1;

    Pista *pistas = IniciarPistas();
    if (!pistas) return 1;

    ListaCorrida *listaCorrida = IniciarListaCorrida();
    if (!listaCorrida) return 1;

    // =========================
    // CARREGAMENTO
    // =========================
    PilhaDeCarros(&pilhaCarro);
    PilhaDePilotos(&pilhaPiloto);
    AtribuirCarroAoPiloto(&pilhaPiloto, &pilhaCarro);

    // =========================
    // VISUALIZACAO INICIAL
    // =========================
    ExibirPilotos(pilhaPiloto);
    ExibirPistas(pistas);

    printf("\n=== INICIO DO CAMPEONATO ===\n");

    // =========================
    // 6 CORRIDAS
    // =========================
    for (int i = 0; i < 6; i++)
    {
        // Cria a corrida na lista para registrar o historico.
        CriarCorrida(&listaCorrida, &pistas[i]);

        // Simula a corrida, atualiza pontuacao e salva o resultado no historico.
        SimularCorrida(pilhaPiloto, &pistas[i], &listaCorrida);

        printf("\n");
    }

    // =========================
    // FIM DO CAMPEONATO
    // =========================
    printf("=== FIM DO CAMPEONATO ===\n");

    ExibirRankingCampeonato(pilhaPiloto);

    // =========================
    // HISTORICO COMPLETO
    // =========================
    printf("\n=== HISTORICO DAS CORRIDAS ===\n");
    ExibirResultados(listaCorrida);

    // =========================
    // LIMPEZA
    // =========================
    LimparPilotos(&pilhaPiloto);
    free(pistas);
    LimparCorrida(&listaCorrida);

    return 0;
}