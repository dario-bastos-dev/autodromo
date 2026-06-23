#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "estruturas/carro/carro.h"
#include "estruturas/piloto/piloto.h"
#include "estruturas/pista/pista.h"
#include "estruturas/corrida/corrida.h"
#include "estruturas/fila_chegada/fila_chegada.h"
#include "interface/interface.h"

int main(void) {
    srand(time(NULL));

    ExibirBanner();

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

    // Exibir preparacao
    ExibirPreparacao(pilhaPiloto, pistas);

    // Sistema de pontuação
    int pontos_sistema[10] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
    
    // Contar pilotos
    int numPilotos = 0;
    Piloto *atual = pilhaPiloto->topo;
    while (atual != NULL) {
        numPilotos++;
        atual = atual->prox;
    }

    if (numPilotos > 0) {
        PilotoStats *arrayStats = (PilotoStats*) malloc(numPilotos * sizeof(PilotoStats));
        atual = pilhaPiloto->topo;
        for (int i = 0; i < numPilotos; i++) {
            arrayStats[i].piloto = atual;
            arrayStats[i].pontos_campeonato = 0;
            arrayStats[i].tempo_total_campeonato = 0;
            arrayStats[i].acidentes = 0;
            atual = atual->prox;
        }

        // Loop das 6 corridas (uma para cada pista)
        for (int corrida_idx = 0; corrida_idx < 6; corrida_idx++) {
            
            AnimarLargada(corrida_idx + 1, pistas[corrida_idx].nome);
            
            SimularEExibirCorrida(arrayStats, numPilotos, &pistas[corrida_idx], corrida_idx + 1);
            printf("\n🏁 CORRIDA FINALIZADA! 🏁\n");

            // Ordenar por tempo na corrida atual
            for (int i = 0; i < numPilotos - 1; i++) {
                for (int j = i + 1; j < numPilotos; j++) {
                    if (arrayStats[j].tempo < arrayStats[i].tempo) {
                        PilotoStats temp = arrayStats[i];
                        arrayStats[i] = arrayStats[j];
                        arrayStats[j] = temp;
                    }
                }
            }

            // Distribuir pontos e inserir na Fila de Chegada
            FilaChegada *filaChegada = IniciarFilaChegada();
            for (int i = 0; i < numPilotos; i++) {
                // Pontua apenas os 10 primeiros que conseguiram completar (tempo != 99999)
                if (i < 10 && arrayStats[i].tempo != 99999) {
                    arrayStats[i].pontos_campeonato += pontos_sistema[i];
                }
                InserirFilaChegada(filaChegada, arrayStats[i].piloto, arrayStats[i].tempo);
            }

            // Relatorio da corrida atual
            ExibirFilaChegadaEGravarRelatorio(filaChegada, pistas[corrida_idx].nome, corrida_idx + 1, pontos_sistema);
            ExibirLeaderboardTempo(filaChegada);
            LimparFilaChegada(&filaChegada);
            
            if (corrida_idx < 5) {
                int time = 5;
                while (time != 0) {
                    LimparTela();
                    ExibirLinha();
                    printf("Proxima corrida em %d segundos...\n", time);
                    ExibirLinha();
                    sleep(1);
                    time--;
                }
            }
        }

        // --- RELATORIO FINAL CAMPEONATO ---
        // Ordenar globalmente por pontos, menor número de acidentes e tempo global como desempate
        for (int i = 0; i < numPilotos - 1; i++) {
            for (int j = i + 1; j < numPilotos; j++) {
                if (arrayStats[j].pontos_campeonato > arrayStats[i].pontos_campeonato || 
                   (arrayStats[j].pontos_campeonato == arrayStats[i].pontos_campeonato && arrayStats[j].acidentes < arrayStats[i].acidentes) ||
                   (arrayStats[j].pontos_campeonato == arrayStats[i].pontos_campeonato && arrayStats[j].acidentes == arrayStats[i].acidentes && arrayStats[j].tempo_total_campeonato < arrayStats[i].tempo_total_campeonato)) {
                    PilotoStats temp = arrayStats[i];
                    arrayStats[i] = arrayStats[j];
                    arrayStats[j] = temp;
                }
            }
        }

        ExibirPodinoCampeonato(arrayStats, numPilotos);
        
        FILE *arq_camp = fopen("logs/relatorio_campeonato.txt", "w");
        if(arq_camp) fprintf(arq_camp, "=== RANKING FINAL DO CAMPEONATO ===\n\n");

        for (int i = 0; i < numPilotos; i++) {
            if(arq_camp) fprintf(arq_camp, "%dº Lugar: %s (Pontos Totais: %d, Tempo Global: %d seg, Acidentes: %d)\n", i+1, ((Piloto*)(arrayStats[i].piloto))->nome, arrayStats[i].pontos_campeonato, arrayStats[i].tempo_total_campeonato, arrayStats[i].acidentes);
        }
        
        if (arq_camp) {
            fclose(arq_camp);
        }

        free(arrayStats);
    }

    LimparPilotos(&pilhaPiloto);
    free(pistas);
    LimparCorrida(&listaCorrida);

    printf("\nCorrida finalizada.\n");

    return 0;
}