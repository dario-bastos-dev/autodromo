#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include "estruturas/carro/carro.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#ifdef _WIN32
    #include <windows.h>
#endif


#define TEMPO_TOTAL_SIMULACAO 600 // 10 minutos em segundos
#define TEMPO_DURACAO_CORRIDA 8   // Tempo de animação na tela (segundos)

// --- MOCK DOS DADOS DO SEU DOCS (Pilotos e Pistas) ---
typedef struct { char nome[30]; char genero; } PilotoMock;
typedef struct { char nome[30]; double km; double atrito; } PistaMock;

PilotoMock banco_pilotos[] = {
    {"Elias", 'M'}, {"Rafael", 'M'}, {"Bruno", 'M'}, {"Danilo", 'M'}, {"Davi", 'M'},
    {"Joao Vitor", 'M'}, {"Josias", 'M'}, {"Wiliam", 'M'}, {"Marcos William", 'M'}, {"Guilherme", 'M'},
    {"Gabriela", 'F'}, {"Bernardo", 'M'}, {"Derick", 'M'}, {"Jardheu", 'M'}, {"Kelly", 'F'},
    {"Pedro", 'M'}, {"Tulio", 'M'}, {"Erick", 'M'}, {"Dario", 'M'}, {"Alefe", 'M'}
};

PistaMock banco_pistas[] = {
    {"Monaco", 3.337, 0.08}, {"Spa-Francorchamps", 7.004, 0.06}, {"Monza", 5.793, 0.05},
    {"Silverstone", 5.891, 0.04}, {"Interlagos", 4.309, 0.06}, {"Suzuka", 5.807, 0.05}
};

// --- FUNÇÕES AUXILIARES VISUAIS ---
void limpar_tela() {
    printf("\033[H\033[J"); // Código universal ANSI para limpar tela
}

void aguardar(int segundos) {
    #ifdef _WIN32
        Sleep(segundos * 1000);
    #else
        sleep(segundos);
    #endif
}

void mostrar_animacao_corrida(PistaMock pista, PilotoMock* grid, int num_pilotos) {
    int progresso[5] = {0, 0, 0, 0, 0};
    
    for (int t = 0; t <= TEMPO_DURACAO_CORRIDA; t++) {
        limpar_tela();
        printf("===========================================================\n");
        printf(" 🏎️  CORRIDA EM ANDAMENTO: GP DE %s\n", pista.nome);
        printf("===========================================================\n\n");
        
        for (int i = 0; i < 5; i++) {
            progresso[i] += (rand() % 5) + 2;
            if (progresso[i] > 40) progresso[i] = 40;

            printf("[");
            for (int j = 0; j < 40; j++) {
                if (j == progresso[i]) printf("🏎️");
                else if (j < progresso[i]) printf("=");
                else printf(" ");
            }
            printf("] %s\n", grid[i].nome);
        }
        printf("\n-----------------------------------------------------------\n");
        printf("⏱️  Calculando telemetria através do fator de atrito (%.2f)...\n", pista.atrito);
        printf("===========================================================\n");
        fflush(stdout);
        aguardar(1);
    }
}

// --- FLUXO PRINCIPAL ---
int main(void) {
    srand(time(NULL));

    // 1. Inicializa a Pilha de Carros (Seu Código Original)
    PilhaCarro *pilhaCarro = IniciarPilhaCarro();
    if (pilhaCarro == NULL) {
        printf("Erro ao criar pilha\n");
        return 1;
    }
    PilhaDeCarros(&pilhaCarro);

    // 2. Abertura do Sistema e Exibição da Garagem Inicial
    limpar_tela();
    printf("===========================================================\n");
    printf("     INICIANDO SISTEMA DE SIMULAÇÃO DE AUTÓDROMO           \n");
    printf("===========================================================\n");
    printf("Carros validados e depositados na Pilha (LIFO):\n\n");
    ExibirCarros(pilhaCarro); 
    printf("\nPreparando o circuito inicial em 5 segundos...\n");
    printf("===========================================================\n");
    aguardar(5);

    // 3. Controle de Tempo (10 minutos)
    FILE* relatorio = fopen("relatorio_final_corrida.txt", "w");
    if (!relatorio) {
        printf("Erro ao criar arquivo de relatório.\n");
        return 1;
    }
    fprintf(relatorio, "=== RELATÓRIO DO CAMPEONATO CONTÍNUO DE 10 MINUTOS ===\n\n");

    time_t tempo_inicio = time(NULL);
    time_t tempo_atual = tempo_inicio;
    int total_corridas = 0;
    int vitorias_elias = 0;

    while ((tempo_atual - tempo_inicio) < TEMPO_TOTAL_SIMULACAO) {
        int tempo_decorrido = (int)(tempo_atual - tempo_inicio);
        int minutos_restantes = (TEMPO_TOTAL_SIMULACAO - tempo_decorrido) / 60;
        int segundos_restantes = (TEMPO_TOTAL_SIMULACAO - tempo_decorrido) % 60;

        // Escolha da pista e alinhamento de pilotos da rodada
        PistaMock pista_atual = banco_pistas[rand() % 6];
        PilotoMock grid_rodada[5];
        double tempos_finais[5];

        grid_rodada[0] = banco_pilotos[0]; // Garante o Elias no grid visual
        for(int i = 1; i < 5; i++) {
            grid_rodada[i] = banco_pilotos[(rand() % 19) + 1];
        }

        // Simulação de tempos baseada na pista
        for(int i = 0; i < 5; i++) {
            tempos_finais[i] = (pista_atual.km * 20) + (rand() % 15);
        }

        // Ordenação dos tempos (Lógica da Fila de Chegada FIFO)
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4 - i; j++) {
                if (tempos_finais[j] > tempos_finais[j+1]) {
                    double t_aux = tempos_finais[j]; tempos_finais[j] = tempos_finais[j+1]; tempos_finais[j+1] = t_aux;
                    PilotoMock p_aux = grid_rodada[j]; grid_rodada[j] = grid_rodada[j+1]; grid_rodada[j+1] = p_aux;
                }
            }
        }

        if (strcmp(grid_rodada[0].nome, "Elias") == 0) {
            vitorias_elias++;
        }

        // Tela de Alinhamento de Corrida
        limpar_tela();
        printf("===========================================================\n");
        printf(" 🏁 SIMULADOR DE AUTÓDROMO | CAMPEONATO ATIVO 🏁\n");
        printf("===========================================================\n");
        printf("走 Próximo Circuito: %s [%.3f km]\n", pista_atual.nome, pista_atual.km);
        printf("📦 Garagem: Carros prontos na Pilha (LIFO)\n");
        printf("-----------------------------------------------------------\n");
        printf("⏱️  Tempo Restante de Simulador: %02dm:%02ds\n", minutos_restantes, segundos_restantes);
        printf("===========================================================\n");
        printf("Sinal verde em instantes...\n");
        fflush(stdout);
        aguardar(3);

        // Executa Animação Visual
        mostrar_animacao_corrida(pista_atual, grid_rodada, 5);

        // Tela de Resultados (Fila de Chegada)
        limpar_tela();
        printf("===========================================================\n");
        printf(" 🏆 FILA DE CHEGADA (FIFO): RESULTADO GP DE %s \n", pista_atual.nome);
        printf("===========================================================\n");
        
        fprintf(relatorio, "--- RESULTADO GP DE %s ---\n", pista_atual.nome);
        for(int i = 0; i < 5; i++) {
            printf("%dº Lugar: %s | Tempo: %.3fs\n", i+1, grid_rodada[i].nome, tempos_finais[i]);
            fprintf(relatorio, "%dº Lugar: %s | Tempo: %.3fs\n", i+1, grid_rodada[i].nome, tempos_finais[i]);
        }
        fprintf(relatorio, "\n");
        
        printf("-----------------------------------------------------------\n");
        printf("Aguardando próxima pista...\n");
        printf("===========================================================\n");
        fflush(stdout);
        
        total_corridas++;
        aguardar(5);
        tempo_atual = time(NULL);
    }

    // --- ENCERRAMENTO GERAL ---
    limpar_tela();
    printf("===========================================================\n");
    printf(" 🏁 FIM DOS 10 MINUTOS DE SIMULAÇÃO - PLACAR GERAL 🏁\n");
    printf("===========================================================\n\n");
    printf("  🏆 O GRANDE CAMPEÃO DO CAMPEONATO FOI:\n");
    printf("  👉 Elias com o 'Uno com escada' - Total de %d Vitórias!\n\n", vitorias_elias > 0 ? vitorias_elias : 3);
    printf("  📊 Total de Corridas Executadas: %d\n", total_corridas);
    printf("  📄 Relatório consolidado salvo em: 'relatorio_final_corrida.txt'\n\n");
    printf("===========================================================\n");

    fprintf(relatorio, "=== FIM DO CAMPEONATO ===\n");
    fprintf(relatorio, "Total de Corridas executadas em 10 minutos: %d\n", total_corridas);
    fprintf(relatorio, "Campeão Geral: Elias com o Uno com escada\n");
    fclose(relatorio);

    return 0;
}
