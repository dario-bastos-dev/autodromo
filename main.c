#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "estruturas/carro/carro.h"
#include "estruturas/piloto/piloto.h"
#include "estruturas/pista/pista.h"
#include "estruturas/corrida/corrida.h"
#include "interface/interface.h" // Inclui o seu arquivo visual único

int main(void) {
    srand(time(NULL));

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
    
    // Suas funções originais de carga e associação de dados
    PilhaDeCarros(&pilhaCarro);
    PilhaDePilotos(&pilhaPiloto);
    AtribuirCarroAoPiloto(&pilhaPiloto, &pilhaCarro);

    // --- LOOP DE INTERAÇÃO DO USUÁRIO ---
    int opcao = -1;
    while (opcao != 0) {
        LimparTela(); // Limpa o terminal para desenhar o menu
        opcao = ExibirMenuPrincipal(); // Captura o número digitado
        
        switch (opcao) {
            case 1:
                // Redireciona para a tela que envelopa sua função ExibirPilotos
                TelaExibirPilotos(pilhaPiloto);
                break;
            case 2:
                // Redireciona para a tela que envelopa sua função ExibirPistas
                TelaExibirPistas(pistas);
                break;
            case 3:
                // Abre a janela de simulação que gerencia as corridas
                TelaSimulacaoCorrida(listaCorrida, pilhaPiloto, pistas);
                break;
            case 4:
                // Redireciona para a tela que roda o seu ExibirResultados
                TelaHistoricoCampeonato(listaCorrida);
                break;
            case 0:
                printf("\n🏎️ Finalizando o sistema... Até a próxima temporada!\n");
                break;
            default:
                printf("\n⚠️ Opção inválida! Pressione ENTER para tentar novamente...");
                getchar(); getchar(); // Limpa buffer e aguarda o usuário ler
                break;
        }
    }

    // Suas funções de encerramento e liberação de memória originais intactas
    LimparPilotos(&pilhaPiloto);
    free(pistas);
    LimparCorrida(&listaCorrida);

    return 0;
}
