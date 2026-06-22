#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Garante o acesso aos arquivos originais do projeto
#include "../estruturas/piloto/piloto.h"
#include "../estruturas/carro/carro.h"
#include "../estruturas/pista/pista.h"
#include "../estruturas/corrida/corrida.h"

// --- ESTRUTURA AUXILIAR PARA O RANKING ---
typedef struct {
    char nome_piloto[50]; // Definição correta do tamanho da string
    int pontos_acumulados;
    int vitorias;
} PlacarVisual;

// --- FUNÇÕES VISUAIS AUXILIARES ---

static void LimparTela(void) {
    #ifdef _WIN32
        system("chcp 65001 > nul"); // Suporte a UTF-8 no Windows
        system("cls");
    #else
        system("clear");
    #endif
}

static void DesenharLinha(char caractere, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        putchar(caractere);
    }
    putchar('\n');
}

static void ExibirCabecalho(const char *titulo) {
    DesenharLinha('=', 65);
    printf("                  🏁 %s 🏁\n", titulo);
    DesenharLinha('=', 65);
}

// Pequeno atraso para animações de carregamento
static void Aguardar(int milissegundos) {
    #ifdef _WIN32
        _sleep(milissegundos);
    #else
        struct timespec ts = {milissegundos / 1000, (milissegundos % 1000) * 1000000};
        nanosleep(&ts, NULL);
    #endif
}

// Desenha uma barra de progresso animada na tela
static void DesenharBarraProgresso(int atual, int total) {
    int tamanho_barra = 20;
    int preenchido = (atual * tamanho_barra) / total;
    printf("[");
    for (int i = 0; i < tamanho_barra; i++) {
        if (i < preenchido) printf("█");
        else printf(" ");
    }
    printf("] %d%%\n", (atual * 100) / total);
}

// --- MENUS E TELAS DO SISTEMA ---

static int ExibirMenuPrincipal(void) {
    int opcao;
    ExibirCabecalho("SISTEMA DE CORRIDAS F1");
    printf(" 1. Visualizar Pilotos e Equipes\n");
    printf(" 2. Visualizar Calendário de Pistas\n");
    printf(" 3. Iniciar Temporada (Campeonato Global de 10 Minutos Reais)\n");
    printf(" 4. Ver Histórico e Relatório do Campeonato\n");
    printf(" 0. Sair do Sistema\n");
    DesenharLinha('-', 65);
    printf("Escolha uma opção: ");

    if (scanf("%d", &opcao) != 1) {
        while (getchar() != '\n');
        opcao = -1;
    }
    return opcao;
}

static void TelaExibirPilotos(PilhaPiloto *pilha) {
    LimparTela();
    ExibirCabecalho("PILOTOS E CARROS INSCRITOS");
    ExibirPilotos(pilha);
    DesenharLinha('-', 65);
    printf("\nPressione ENTER para voltar ao menu...");
    while (getchar() != '\n');
    getchar();
}

static void TelaExibirPistas(Pista *pistas) {
    LimparTela();
    ExibirCabecalho("CIRCUITOS DISPONÍVEIS");
    ExibirPistas(pistas);
    DesenharLinha('-', 65);
    printf("\nPressione ENTER para voltar ao menu...");
    while (getchar() != '\n');
    getchar();
}

static void TelaSimulacaoCorrida(ListaCorrida *lista, PilhaPiloto *pilotos, Pista *pistas) {
    LimparTela();
    ExibirCabecalho("ABERTURA DA TEMPORADA F1");
    printf("🚦 Preparando motores para o início do campeonato de 10 minutos...\n");
    Aguardar(1500);

    // TEMPO GLOBAL DO CAMPEONATO: 10 minutos (600 segundos)
    int tempo_limite_campeonato = 10 * 60;
    time_t tempo_inicio_campeonato = time(NULL);

    // Loop que passa por todas as 6 pistas cadastradas no sistema
    for (int pista_atual = 0; pista_atual < 6; pista_atual++) {
        time_t tempo_agora = time(NULL);
        int tempo_gasto = (int)difftime(tempo_agora, tempo_inicio_campeonato);

        // Verifica se o tempo total de 10 minutos estourou antes de abrir a pista
        if (tempo_gasto >= tempo_limite_campeonato) {
            printf("\n⏰ [TEMPO ESGOTADO]: O cronômetro de 10 minutos estourou! Campeonato encerrado.\n");
            break;
        }

        LimparTela();
        char msg_cabecalho[100]; // Definição correta do tamanho da string do título
        snprintf(msg_cabecalho, sizeof(msg_cabecalho), "ETAPA %d/6: %s", pista_atual + 1, pistas[pista_atual].nome);
        ExibirCabecalho(msg_cabecalho);

        printf("🟢 LARGADA AUTORIZADA! Carros na pista.\n");
        CriarCorrida(&lista, &pistas[pista_atual]);

        // Cada pista consome uma fração proporcional do tempo total restante
        int tempo_desta_pista = (tempo_limite_campeonato - tempo_gasto) / (6 - pista_atual);
        if (tempo_desta_pista < 10) tempo_desta_pista = 10; // Garante uma duração mínima visual

        time_t tempo_inicio_pista = time(NULL);
        int ultima_porcentagem = -1;

        // Loop em tempo real da corrida atual
        while (1) {
            time_t t_pista_atual = time(NULL);
            int segundos_pista_passados = (int)difftime(t_pista_atual, tempo_inicio_pista);
            int tempo_total_geral_passado = (int)difftime(t_pista_atual, tempo_inicio_campeonato);

            if (segundos_pista_passados >= tempo_desta_pista || tempo_total_geral_passado >= tempo_limite_campeonato) {
                break; // Encerra a corrida atual se o tempo da pista ou o global esgotar
            }

            // Atualiza os gráficos animados segundo a segundo
            int porcentagem_corrida = (segundos_pista_passados * 100) / tempo_desta_pista;
            if (porcentagem_corrida != ultima_porcentagem) {
                ultima_porcentagem = porcentagem_corrida;

                LimparTela();
                ExibirCabecalho(msg_cabecalho);
                printf("⏱️  Tempo total do campeonato decorrido: %d min %d s de 10 min\n",
                       tempo_total_geral_passado / 60, tempo_total_geral_passado % 60);
                printf("🏎️  Progresso da corrida atual: ");
                DesenharBarraProgresso(segundos_pista_passados, tempo_desta_pista);
                DesenharLinha('-', 65);

                // FEED DINÂMICO DE EVENTOS: Gera animações e acontecimentos aleatórios na tela
                printf("\n📢 [FEED DA CORRIDA]:\n");
                int evento_aleatorio = rand() % 100;
                if (evento_aleatorio < 15) {
                    printf("💥 ACIDENTE! Um carro rodou na curva e acionou a bandeira amarela!\n");
                } else if (evento_aleatorio < 35) {
                    printf("🔧 PIT STOP! Equipes trabalhando rápido para trocar os pneus.\n");
                } else if (evento_aleatorio < 60) {
                    printf("⚔️  DISPUTA! Ultrapassagem emocionante por dentro na reta principal!\n");
                } else {
                    printf("🟢 Carros acelerando forte, buscando o melhor tempo de volta.\n");
                }
                fflush(stdout);
            }
            Aguardar(1000); // Aguarda 1 segundo
        }

        printf("\n🏁 BANDEIRA QUADRICULADA em %s!\n", pistas[pista_atual].nome);
        printf("💾 Processando telemetria e arquivando resultados...\n");
        Aguardar(1000);

        // Salva os resultados na estrutura original
        if (pilotos != NULL && pilotos->topo != NULL) {
            int colocacao = 1;
            int tabela_pontos[] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
            Piloto *p_atual = pilotos->topo;
            while (p_atual != NULL) {
                int pts = (colocacao <= 10) ? tabela_pontos[colocacao - 1] : 0;
                time_t tempo_final_piloto = (time_t)(600 + (rand() % 30));

                SalvarResultado(&lista, p_atual, colocacao, pts, tempo_final_piloto);

                colocacao++;
                p_atual = p_atual->prox;
            }
        } else {
            printf("⚠️  Nenhum piloto cadastrado para registrar resultados.\n");
        }
    }

    printf("\n🏆 FIM DE TEMPORADA! Todas as etapas possíveis foram concluídas.\n");
    DesenharLinha('-', 65);
    printf("\nPressione ENTER para ver o relatório final e descobrir o campeão...");
    while (getchar() != '\n');
    getchar();
}

static void TelaHistoricoCampeonato(ListaCorrida *lista) {
    LimparTela();
    ExibirCabecalho("HISTÓRICO E RELATÓRIO DO CAMPEONATO");

    if (lista == NULL || lista->inicio == NULL) {
        printf("Nenhuma corrida foi computada nesta temporada ainda.\n");
        DesenharLinha('-', 65);
        printf("\nPressione ENTER para voltar ao menu...");
        while (getchar() != '\n');
        getchar();
        return;
    }

    printf("📜 [VENCEDORES E RESULTADOS POR CORRIDA]:\n");
    ExibirResultados(lista);
    DesenharLinha('-', 65);

    // Efeito visual animado de carregamento de apuração de pontos
    printf("\n📊 Apurando e cruzando dados de pontuação do campeonato");
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        Aguardar(600);
    }
    printf("\n");

    printf("\n🏆 [TABELA DE CLASSIFICAÇÃO GERAL DO CAMPEONATO]:\n");

    PlacarVisual tabela[50]; // Suporta até 50 pilotos distintos no ranking
    int total_pilotos = 0;

    Corrida *c_atual = lista->inicio;
    while (c_atual != NULL) {
        Resultado *r_atual = c_atual->resultado;
        while (r_atual != NULL) {
            int indice_achado = -1;
            for (int i = 0; i < total_pilotos; i++) {
                if (strcmp(tabela[i].nome_piloto, r_atual->piloto->nome) == 0) {
                    indice_achado = i;
                    break;
                }
            }

            if (indice_achado != -1) {
                tabela[indice_achado].pontos_acumulados += r_atual->pontuacao;
                if (r_atual->colocacao == 1) tabela[indice_achado].vitorias++;
            } else {
                if (total_pilotos < 50) {
                    strncpy(tabela[total_pilotos].nome_piloto, r_atual->piloto->nome,
                            sizeof(tabela[total_pilotos].nome_piloto) - 1);
                    tabela[total_pilotos].nome_piloto[sizeof(tabela[total_pilotos].nome_piloto) - 1] = '\0';
                    tabela[total_pilotos].pontos_acumulados = r_atual->pontuacao;
                    tabela[total_pilotos].vitorias = (r_atual->colocacao == 1) ? 1 : 0;
                    total_pilotos++;
                }
            }
            r_atual = r_atual->prox;
        }
        c_atual = c_atual->prox;
    }

    if (total_pilotos == 0) {
        printf("Nenhum resultado disponível para gerar a classificação.\n");
    } else {
        // Ordenação do Ranking (Bubble Sort) — critério: pontos e, em empate, vitórias
        for (int i = 0; i < total_pilotos - 1; i++) {
            for (int j = 0; j < total_pilotos - i - 1; j++) {
                int deve_trocar = (tabela[j].pontos_acumulados < tabela[j + 1].pontos_acumulados) ||
                                   (tabela[j].pontos_acumulados == tabela[j + 1].pontos_acumulados &&
                                    tabela[j].vitorias < tabela[j + 1].vitorias);

                if (deve_trocar) {
                    PlacarVisual temp = tabela[j];
                    tabela[j] = tabela[j + 1];
                    tabela[j + 1] = temp;
                }
            }
        }

        // Exibição do pódio e da tabela completa
        DesenharLinha('-', 65);
        for (int i = 0; i < total_pilotos; i++) {
            const char *medalha;
            if (i == 0) medalha = "🥇";
            else if (i == 1) medalha = "🥈";
            else if (i == 2) medalha = "🥉";
            else medalha = "  ";

            printf(" %s %2dº | %-25s | Pontos: %3d | Vitórias: %d\n",
                   medalha, i + 1, tabela[i].nome_piloto,
                   tabela[i].pontos_acumulados, tabela[i].vitorias);
        }
        DesenharLinha('-', 65);
        printf("\n🏆 CAMPEÃO DA TEMPORADA: %s, com %d pontos!\n",
               tabela[0].nome_piloto, tabela[0].pontos_acumulados);
    }

    DesenharLinha('-', 65);
    printf("\nPressione ENTER para voltar ao menu...");
    while (getchar() != '\n');
    getchar();
}

#endif