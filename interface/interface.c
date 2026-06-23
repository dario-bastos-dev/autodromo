#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "interface.h"

void LimparTela() {
    system("clear || cls");
}

void ExibirLinha() {
    printf("════════════════════════════════════════════════════════════════════════════════════\n");
}

void ExibirBanner() {
    LimparTela();
    ExibirLinha();
    printf("\n");
    printf("  ██████╗  █████╗ ███╗   ███╗██████╗ ███████╗ █████╗ ██╗     ██╗  ██╗ █████╗ ██╗  ██╗\n");
    printf("  ██╔════╝ ██╔══██╗████╗ ████║██╔══██╗██╔════╝██╔══██╗██║     ██║  ██║██╔══██╗██║  ██║\n");
    printf("  ██║  █╗ ██║  ██║██╔████╔██║██████╔╝█████╗  ███████║██║     ███████║███████║███████║\n");
    printf("  ██║   ██╗██║  ██║██║╚██╔╝██║██╔═══╝ ██╔══╝  ██╔══██║██║     ██╔══██║██╔══██║██╔══██║\n");
    printf("  ╚██████╔╝╚█████╔╝██║ ╚═╝ ██║██║     ███████╗██║  ██║███████╗██║  ██║██║  ██║██║  ██║\n");
    printf("   ╚═════╝  ╚════╝ ╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝\n");
    printf("\n");
    printf("                        SIMULACAO DE CORRIDA EM AUTODROMO\n");
    printf("                               Campeonato 2026\n");
    printf("\n");
    ExibirLinha();
    printf("\n");
    sleep(2);
}

void ExibirPreparacao(PilhaPiloto *pilhaPiloto, Pista *pistas) {
    LimparTela();
    ExibirLinha();
    printf("                    PREPARACAO DO CAMPEONATO\n");
    ExibirLinha();
    printf("\n📍 PISTAS DISPONÍVEIS:\n\n");
    
    for (int i = 0; i < 6; i++) {
        printf("   %d. %-20s | Tamanho: %d km | Risco de Acidentes: %.1f%%\n", 
               i+1, pistas[i].nome, pistas[i].tamanho, pistas[i].acidente * 100);
    }
    
    printf("\n👨‍🏫 PILOTOS CONFIRMADOS:\n\n");
    
    Piloto *piloto = pilhaPiloto->topo;
    int cont = 1;
    while (piloto != NULL) {
        printf("   %2d. %-20s | Carro: %-25s | Potência: %d\n", 
               cont, piloto->nome, 
               (piloto->carro ? piloto->carro->modelo : "N/A"),
               (piloto->carro ? piloto->carro->potencia : 0));
        piloto = piloto->prox;
        cont++;
    }
    int time = 3;
    while (time != 0) {
        LimparTela();
        printf("\n");
        ExibirLinha();
        printf("                  Campeonato iniciando em %d segundos...\n", time);
        ExibirLinha();
        printf("\n");
        sleep(1);
        time--;
    }
    
}

void AnimarLargada(int corrida_num, const char *nome_pista) {
    LimparTela();
    ExibirLinha();
    printf("                    CORRIDA %d - %s\n", corrida_num, nome_pista);
    ExibirLinha();
    printf("\n");
    
    const char *fases[] = {"🔴 ATENÇÃO!", "🟡 PREPARADO!", "🟢 LARGADA!"};
    
    for (int i = 0; i < 3; i++) {
        printf("\r                        %-30s", fases[i]);
        fflush(stdout);
        sleep(1);
    }
    
    printf("\n\n");
    printf("╔════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                    🏁 CORRIDA EM ANDAMENTO 🏁                                      ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    sleep(1);
}

void ExibirStatusCorrida(int volta_atual, int total_voltas, int tempo_decorrido) {
    int progresso = (volta_atual * 100) / total_voltas;
    int barra_size = (progresso * 40) / 100;
    
    printf("\r[");
    for (int i = 0; i < 40; i++) {
        if (i < barra_size) printf("█");
        else printf("░");
    }
    printf("] %d%% | Volta %d/%d | Tempo: %ds", progresso, volta_atual, total_voltas, tempo_decorrido);
    fflush(stdout);
}

void ExibirAcidente(const char *nome_piloto, int volta) {
    printf("\n⚠️  ACIDENTE! O piloto %s sofreu um acidente na volta %d e foi retirado da corrida!\n", nome_piloto, volta);
}

// Estrutura interna para simulação
typedef struct {
    PilotoStats *stats;
    float progresso;
    int dnf;
    int tempo_gasto;
    int volta_acidente;
    int completou;
    int tempo_alvo;
    float ruido_fase;
} PilotoSimulacao;

// Função auxiliar de comparação para ordenação do grid
static int CompararPilotosSimulacao(const void *a_ptr, const void *b_ptr) {
    const PilotoSimulacao *a = *(const PilotoSimulacao **)a_ptr;
    const PilotoSimulacao *b = *(const PilotoSimulacao **)b_ptr;
    
    // 1. Completou vs Não completou
    if (a->completou && !b->completou) {
        return -1; // a na frente
    }
    if (!a->completou && b->completou) {
        return 1; // b na frente
    }
    
    // 2. Ambos completaram (ordenar por tempo gasto menor)
    if (a->completou && b->completou) {
        return a->tempo_gasto - b->tempo_gasto;
    }
    
    // 3. Um bateu (dnf) e outro não
    if (a->dnf && !b->dnf) {
        return 1; // b na frente
    }
    if (!a->dnf && b->dnf) {
        return -1; // a na frente
    }
    
    // 4. Ambos bateram (ordenar por maior progresso na frente)
    if (a->dnf && b->dnf) {
        if (a->progresso > b->progresso) return -1;
        if (a->progresso < b->progresso) return 1;
        return 0;
    }
    
    // 5. Ambos estão correndo (ordenar por maior progresso)
    if (a->progresso > b->progresso) return -1;
    if (a->progresso < b->progresso) return 1;
    return 0;
}

static void DelayMs(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void SimularEExibirCorrida(PilotoStats *arrayStats, int numPilotos, Pista *pista, int corrida_num) {
    // Alocar a estrutura de simulação
    PilotoSimulacao *sim = (PilotoSimulacao *)malloc(sizeof(PilotoSimulacao) * numPilotos);
    if (!sim) return;

    // Inicializar cada piloto
    for (int i = 0; i < numPilotos; i++) {
        sim[i].stats = &arrayStats[i];
        sim[i].progresso = 0.0f;
        sim[i].dnf = 0;
        sim[i].tempo_gasto = 0;
        sim[i].completou = 0;
        sim[i].ruido_fase = (float)(rand() % 100) / 10.0f;
        
        int pot = arrayStats[i].piloto->carro ? arrayStats[i].piloto->carro->potencia : 100;
        float chance_acidente = pista->acidente;
        
        if (arrayStats[i].piloto->sexo[0] == 'F' || arrayStats[i].piloto->sexo[0] == 'f') {
            chance_acidente *= 2.0f;
        }
        
        int bateu = 0;
        int volta_ac = -1;
        for (int volta = 1; volta <= 10; volta++) {
            float sorteio = (float)rand() / (float)RAND_MAX;
            if (sorteio <= (chance_acidente / 10.0f)) {
                bateu = 1;
                volta_ac = volta;
                break;
            }
        }
        
        if (bateu) {
            sim[i].volta_acidente = volta_ac;
            sim[i].tempo_alvo = 99999;
        } else {
            sim[i].volta_acidente = -1;
            int variacao = rand() % 21;
            sim[i].tempo_alvo = 120 - ((pot * 20) / 1000) + variacao;
        }
    }

    // Histórico local de acidentes para exibição
    char historico_acidentes[15][128];
    int num_acidentes = 0;

    // Ponteiros para ordenação rápida
    PilotoSimulacao **ordenados = (PilotoSimulacao **)malloc(sizeof(PilotoSimulacao *) * numPilotos);
    for (int i = 0; i < numPilotos; i++) {
        ordenados[i] = &sim[i];
    }

    int tempo_decorrido = 0;
    int todos_finalizaram = 0;

    // Loop de simulação em tempo real
    while (!todos_finalizaram) {
        todos_finalizaram = 1;

        // Atualizar progresso de cada piloto
        for (int i = 0; i < numPilotos; i++) {
            // Se o piloto já bateu (DNF) ou completou
            if (sim[i].dnf || sim[i].completou) {
                continue;
            }

            todos_finalizaram = 0;

            // Caso piloto vá bater
            if (sim[i].volta_acidente != -1) {
                int pot = sim[i].stats->piloto->carro ? sim[i].stats->piloto->carro->potencia : 100;
                int tempo_normal_estimado = 120 - ((pot * 20) / 1000) + 10;
                float tempo_por_volta = (float)tempo_normal_estimado / 10.0f;
                float tempo_acidente = ((float)sim[i].volta_acidente - 0.5f) * tempo_por_volta;

                if ((float)tempo_decorrido < tempo_acidente) {
                    float base_progresso = ((float)tempo_decorrido / (float)tempo_normal_estimado) * 10.0f;
                    float flutuacao = sinf((float)tempo_decorrido * 0.25f + sim[i].ruido_fase) * 0.12f;
                    sim[i].progresso = base_progresso + flutuacao;
                    if (sim[i].progresso < 0.0f) sim[i].progresso = 0.0f;
                    
                    float progresso_limite = (float)sim[i].volta_acidente - 0.5f;
                    if (sim[i].progresso > progresso_limite - 0.05f) {
                        sim[i].progresso = progresso_limite - 0.05f;
                    }
                } else {
                    sim[i].dnf = 1;
                    sim[i].progresso = (float)sim[i].volta_acidente - 0.5f;
                    sim[i].tempo_gasto = 99999;
                    sim[i].stats->tempo = 99999;

                    if (num_acidentes < 15) {
                        snprintf(historico_acidentes[num_acidentes], 128, 
                                 "⚠️  \033[1;31mACIDENTE!\033[0m O piloto %-20s sofreu um acidente na volta %d!", 
                                 sim[i].stats->piloto->nome, sim[i].volta_acidente);
                        num_acidentes++;
                    }
                }
            }
            // Caso piloto vá completar
            else {
                if (tempo_decorrido < sim[i].tempo_alvo) {
                    float base_progresso = ((float)tempo_decorrido / (float)sim[i].tempo_alvo) * 10.0f;
                    float flutuacao = sinf((float)tempo_decorrido * 0.25f + sim[i].ruido_fase) * 0.12f;
                    sim[i].progresso = base_progresso + flutuacao;
                    if (sim[i].progresso < 0.0f) sim[i].progresso = 0.0f;
                    if (sim[i].progresso > 9.9f) sim[i].progresso = 9.9f;
                } else {
                    sim[i].completou = 1;
                    sim[i].progresso = 10.0f;
                    sim[i].tempo_gasto = sim[i].tempo_alvo;
                    sim[i].stats->tempo = sim[i].tempo_alvo;
                }
            }
        }

        if (todos_finalizaram) {
            break;
        }

        // Ordenar os pilotos por posição atual
        qsort(ordenados, numPilotos, sizeof(PilotoSimulacao *), CompararPilotosSimulacao);

        // Renderizar a tela
        LimparTela();
        ExibirLinha();
        printf("                🏎️  CORRIDA EM ANDAMENTO: %s (Etapa %d) 🏎️\n", pista->nome, corrida_num);
        ExibirLinha();
        printf(" Pos | %-22s | %-16s | %-15s | Volta  | Status\n", "Piloto", "Carro", "Progresso");
        printf("─────┼────────────────────────┼──────────────────┼─────────────────┼────────┼───────────\n");

        int limite_exibir = numPilotos < 10 ? numPilotos : 10;
        for (int k = 0; k < limite_exibir; k++) {
            PilotoSimulacao *p = ordenados[k];
            
            if (k == 0) {
                printf(" \033[1;33m1º\033[0m");
            } else if (k == 1) {
                printf(" \033[1;37m2º\033[0m");
            } else if (k == 2) {
                printf(" \033[1;34m3º\033[0m");
            } else {
                printf(" %2dº", k + 1);
            }

            printf(" | %-22s | %-16s | [", 
                   p->stats->piloto->nome, 
                   p->stats->piloto->carro ? p->stats->piloto->carro->modelo : "N/A");

            int blocos = (int)(p->progresso * 15.0f / 10.0f);
            if (blocos > 15) blocos = 15;
            if (blocos < 0) blocos = 0;
            for (int b = 0; b < 15; b++) {
                if (b < blocos) printf("█");
                else printf("░");
            }
            printf("] | L%2.0f/10 | ", p->progresso + 1.0f > 10.0f ? 10.0f : p->progresso + 1.0f);

            if (p->dnf) {
                printf("\033[1;31m💥 DNF\033[0m\n");
            } else if (p->completou) {
                printf("\033[1;32m🏁 FIM (%ds)\033[0m\n", p->tempo_gasto);
            } else {
                printf("\033[1;36m⏳ RUN\033[0m\n");
            }
        }

        if (num_acidentes > 0) {
            printf("\n⚠️  OCORRÊNCIAS DA PROVA:\n");
            for (int a = 0; a < num_acidentes; a++) {
                printf("  %s\n", historico_acidentes[a]);
            }
        }

        ExibirLinha();
        printf("Tempo de prova decorrido: %ds\n", tempo_decorrido);
        fflush(stdout);

        tempo_decorrido++;
        DelayMs(150);
    }

    // Copiar tempos de volta finais para o arrayStats global do campeonato e aplicar penalidades
    for (int i = 0; i < numPilotos; i++) {
        arrayStats[i].tempo = sim[i].tempo_gasto;
        if (sim[i].dnf) {
            arrayStats[i].tempo_total_campeonato += 150; // Penalidade realista em vez de 99999
            arrayStats[i].acidentes++;
        } else {
            arrayStats[i].tempo_total_campeonato += sim[i].tempo_gasto;
        }
    }

    free(ordenados);
    free(sim);
}

void ExibirLeaderboardTempo(FilaChegada *fila) {
    printf("\n\n");
    ExibirLinha();
    printf("                        LEADERBOARD DA CORRIDA\n");
    ExibirLinha();
    printf("\n");
    
    NodoFila *atual = fila->frente;
    int colocacao = 1;
    
    while (atual != NULL && colocacao <= 10) {
        if (atual->tempo_total == 99999) {
            printf("  %2d. %-25s | Status: DNF (ACIDENTE)\n", colocacao, atual->piloto->nome);
        } else {
            printf("  %2d. %-25s | Tempo: %3d seg\n", colocacao, atual->piloto->nome, atual->tempo_total);
        }
        atual = atual->prox;
        colocacao++;
    }
    
    printf("\n");
}

void ExibirPodinoCampeonato(void *arrayStats_ptr, int numPilotos) {
    LimparTela();
    ExibirLinha();
    printf("                   🏆 PODIO FINAL DO CAMPEONATO 🏆\n");
    ExibirLinha();
    printf("\n");
    
    // Cast para a estrutura correta
    typedef struct {
        void *piloto_ptr;
        int tempo;
        int pontos_campeonato;
        int tempo_total_campeonato;
        int acidentes;
    } PilotoStatsLocal;
    
    PilotoStatsLocal *arrayStats = (PilotoStatsLocal*)arrayStats_ptr;
    
    // Top 3
    if (numPilotos >= 1 && arrayStats[0].piloto_ptr != NULL) {
        Piloto *p1 = (Piloto*)(arrayStats[0].piloto_ptr);
        printf("┌─────────────────────────────────────────────────────────────────────────────────────┐\n");
        printf("│ 🥇 1º LUGAR: %-25s | Pontos: %-3d | Tempo: %d seg | Acidentes: %d  │\n", 
               p1->nome, arrayStats[0].pontos_campeonato, arrayStats[0].tempo_total_campeonato, arrayStats[0].acidentes);
        printf("└─────────────────────────────────────────────────────────────────────────────────────┘\n");
    }
    
    if (numPilotos >= 2 && arrayStats[1].piloto_ptr != NULL) {
        Piloto *p2 = (Piloto*)(arrayStats[1].piloto_ptr);
        printf("┌─────────────────────────────────────────────────────────────────────────────────────┐\n");
        printf("│ 🥈 2º LUGAR: %-25s | Pontos: %-3d | Tempo: %d seg | Acidentes: %d  │\n", 
               p2->nome, arrayStats[1].pontos_campeonato, arrayStats[1].tempo_total_campeonato, arrayStats[1].acidentes);
        printf("└─────────────────────────────────────────────────────────────────────────────────────┘\n");
    }
    
    if (numPilotos >= 3 && arrayStats[2].piloto_ptr != NULL) {
        Piloto *p3 = (Piloto*)(arrayStats[2].piloto_ptr);
        printf("┌─────────────────────────────────────────────────────────────────────────────────────┐\n");
        printf("│ 🥉 3º LUGAR: %-25s | Pontos: %-3d | Tempo: %d seg | Acidentes: %d  │\n", 
               p3->nome, arrayStats[2].pontos_campeonato, arrayStats[2].tempo_total_campeonato, arrayStats[2].acidentes);
        printf("└─────────────────────────────────────────────────────────────────────────────────────┘\n");
    }
    
    printf("\n");
    ExibirLinha();
    printf("                    CLASSIFICACAO FINAL (TOP 10)\n");
    ExibirLinha();
    printf("\n");
    
    for (int i = 0; i < (numPilotos < 10 ? numPilotos : 10); i++) {
        if (arrayStats[i].piloto_ptr != NULL) {
            Piloto *p = (Piloto*)(arrayStats[i].piloto_ptr);
            printf("  %2d. %-25s | Pontos: %-3d | Tempo Global: %d seg | Acidentes: %d\n", 
                   i+1, p->nome, arrayStats[i].pontos_campeonato, arrayStats[i].tempo_total_campeonato, arrayStats[i].acidentes);
        }
    }
    
    printf("\n");
    ExibirLinha();
    sleep(3);
}
