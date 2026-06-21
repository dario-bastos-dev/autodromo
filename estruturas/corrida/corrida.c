#include <stdlib.h>
#include <stdio.h>
#include "corrida.h"
#include "../piloto/piloto.h"
#include "../pista/pista.h"

#define MAX_PILOTOS_GRID 50
#define TOTAL_VOLTAS 5

// =========================================================
// FUNCOES AUXILIARES INTERNAS
// =========================================================
static void TrocarPilotos(Piloto **a, Piloto **b)
{
    Piloto *tmp = *a;
    *a = *b;
    *b = tmp;
}

static int ExistePontuacaoCampeonato(Piloto **grid, int qtd)
{
    for (int i = 0; i < qtd; i++)
    {
        if (grid[i]->pontosCampeonato > 0)
            return 1;
    }

    return 0;
}

static void EmbaralharGrid(Piloto **grid, int qtd)
{
    for (int i = qtd - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        TrocarPilotos(&grid[i], &grid[j]);
    }
}

static void OrdenarGridPorCampeonato(Piloto **grid, int qtd)
{
    for (int i = 0; i < qtd - 1; i++)
    {
        for (int j = 0; j < qtd - i - 1; j++)
        {
            if (grid[j]->pontosCampeonato < grid[j + 1]->pontosCampeonato)
            {
                TrocarPilotos(&grid[j], &grid[j + 1]);
            }
        }
    }
}

static void PrepararGridLargada(Piloto **grid, int qtd)
{
    if (ExistePontuacaoCampeonato(grid, qtd))
        OrdenarGridPorCampeonato(grid, qtd);
    else
        EmbaralharGrid(grid, qtd);
}

static double ObterChanceAcidente(Pista *pista)
{
    if (!pista)
        return 0.0;

    double chance = (double)pista->acidente;

    if (chance > 1.0)
        chance = chance / 100.0;

    if (chance < 0.0)
        chance = 0.0;

    if (chance > 1.0)
        chance = 1.0;

    return chance;
}

static int SortearAcidenteDaCorrida(Pista *pista)
{
    double chance = ObterChanceAcidente(pista);

    int chanceEscala = (int)(chance * 100000.0);
    int sorteio = rand() % 100000;

    return sorteio < chanceEscala;
}

static void ExibirClassificacaoVolta(Piloto **grid, int qtd, int volta)
{
    printf("\n--- CLASSIFICACAO APOS A VOLTA %d ---\n", volta);

    for (int i = 0; i < qtd; i++)
    {
        Piloto *p = grid[i];

        if (p->abandonou)
        {
            printf("%do %s | ABANDONOU na volta %d | tempo acumulado: %.2f\n",
                   i + 1,
                   p->nome,
                   p->voltaAbandono,
                   p->tempoFinal);
        }
        else
        {
            printf("%do %s | tempo acumulado: %.2f\n",
                   i + 1,
                   p->nome,
                   p->tempoFinal);
        }
    }
}

// =========================================================
// LISTA DE CORRIDAS
// =========================================================
ListaCorrida *IniciarListaCorrida()
{
    ListaCorrida *lista = malloc(sizeof(ListaCorrida));

    if (!lista)
    {
        printf("Erro ao alocar ListaCorrida.\n");
        return NULL;
    }

    lista->inicio = NULL;
    return lista;
}

// =========================================================
// CRIAR CORRIDA NA LISTA
// =========================================================
void CriarCorrida(ListaCorrida **listaCorrida, Pista *pista)
{
    if (!listaCorrida || !(*listaCorrida) || !pista)
        return;

    Corrida *nova = malloc(sizeof(Corrida));

    if (!nova)
    {
        printf("Erro ao criar Corrida.\n");
        return;
    }

    nova->pista = pista;
    nova->resultado = NULL;
    nova->prox = NULL;

    if ((*listaCorrida)->inicio == NULL)
    {
        (*listaCorrida)->inicio = nova;
    }
    else
    {
        Corrida *aux = (*listaCorrida)->inicio;

        while (aux->prox != NULL)
            aux = aux->prox;

        aux->prox = nova;
    }
}

// =========================================================
// SALVAR RESULTADO
// =========================================================
void SalvarResultado(ListaCorrida **listaCorrida,
                     Piloto *piloto,
                     int colocacao,
                     int pontuacao,
                     double tempoFinal,
                     int abandonou,
                     int voltaAbandono)
{
    if (!listaCorrida || !(*listaCorrida) || (*listaCorrida)->inicio == NULL || !piloto)
        return;

    Resultado *res = malloc(sizeof(Resultado));

    if (!res)
    {
        printf("Erro ao salvar resultado.\n");
        return;
    }

    res->piloto = piloto;
    res->colocacao = colocacao;
    res->pontuacao = pontuacao;
    res->tempoFinal = tempoFinal;
    res->abandonou = abandonou;
    res->voltaAbandono = voltaAbandono;
    res->prox = NULL;

    Corrida *c = (*listaCorrida)->inicio;

    while (c->prox != NULL)
        c = c->prox;

    if (c->resultado == NULL)
    {
        c->resultado = res;
    }
    else
    {
        Resultado *aux = c->resultado;

        while (aux->prox != NULL)
            aux = aux->prox;

        aux->prox = res;
    }
}

// =========================================================
// ACIDENTE
// Mantida por compatibilidade com corrida.h.
// A simulacao principal usa acidente por corrida,
// nao por piloto/volta.
// =========================================================
int VerificarAcidente(Pista *pista, Piloto *p, int volta, double tempo)
{
    if (!pista || !p)
        return 0;

    if (SortearAcidenteDaCorrida(pista))
    {
        p->abandonou = 1;
        p->voltaAbandono = volta;
        p->tempoFinal = tempo;

        return 1;
    }

    return 0;
}

// =========================================================
// ORDENACAO FINAL E ORDENACAO POR VOLTA
// Regra:
// 1. Quem nao abandonou fica na frente.
// 2. Entre os que estao correndo, menor tempo fica na frente.
// 3. Quem abandonou fica no final.
// 4. Entre abandonos, quem abandonou mais tarde fica melhor.
// =========================================================
void OrdenarCorrida(Piloto **grid, int qtd)
{
    for (int i = 0; i < qtd - 1; i++)
    {
        for (int j = 0; j < qtd - i - 1; j++)
        {
            int deveTrocar = 0;

            if (grid[j]->abandonou && !grid[j + 1]->abandonou)
            {
                deveTrocar = 1;
            }
            else if (!grid[j]->abandonou && !grid[j + 1]->abandonou &&
                     grid[j]->tempoFinal > grid[j + 1]->tempoFinal)
            {
                deveTrocar = 1;
            }
            else if (grid[j]->abandonou && grid[j + 1]->abandonou &&
                     grid[j]->voltaAbandono < grid[j + 1]->voltaAbandono)
            {
                deveTrocar = 1;
            }

            if (deveTrocar)
                TrocarPilotos(&grid[j], &grid[j + 1]);
        }
    }
}

// =========================================================
// SIMULACAO DA CORRIDA
// =========================================================
void SimularCorrida(PilhaPiloto *pilhaPiloto,
                    Pista *pista,
                    ListaCorrida **listaCorrida)
{
    if (!pilhaPiloto || !pilhaPiloto->topo || !pista)
    {
        printf("Nao ha dados suficientes para simular a corrida.\n");
        return;
    }

    double chanceAcidente = ObterChanceAcidente(pista);

    printf("\n==============================\n");
    printf("CORRIDA EM %s\n", pista->nome);
    printf("==============================\n");
    printf("Chance de acidente da corrida: %.2f%%\n", chanceAcidente * 100.0);

    if (chanceAcidente <= 0.0)
    {
        printf("ATENCAO: a chance de acidente desta pista esta 0%%.\n");
        printf("Verifique o arquivo docs/pistas.txt e a leitura em pista.c.\n");
    }

    Piloto *grid[MAX_PILOTOS_GRID];
    int qtd = 0;

    Piloto *aux = pilhaPiloto->topo;

    // Reset dos estados de corrida e montagem do grid.
    while (aux != NULL && qtd < MAX_PILOTOS_GRID)
    {
        aux->abandonou = 0;
        aux->tempoFinal = 0;
        aux->voltaAbandono = -1;

        grid[qtd++] = aux;
        aux = aux->prox;
    }

    PrepararGridLargada(grid, qtd);

    // =====================================================
    // SORTEIO DE ACIDENTE DA CORRIDA
    // A taxa da pista e aplicada UMA VEZ por corrida.
    // Nao exibimos aviso antecipado para nao revelar o acidente.
    // =====================================================
    int corridaTeraAcidente = SortearAcidenteDaCorrida(pista);
    int voltaDoAcidente = -1;
    Piloto *pilotoDoAcidente = NULL;
    int totalAcidentes = 0;

    if (corridaTeraAcidente && qtd > 0)
    {
        voltaDoAcidente = (rand() % TOTAL_VOLTAS) + 1;
        int indicePiloto = rand() % qtd;
        pilotoDoAcidente = grid[indicePiloto];
    }

    printf("\n--- GRID DE LARGADA ---\n");

    for (int i = 0; i < qtd; i++)
    {
        if (grid[i]->carro)
        {
            printf("%do %s | Carro: %s %s | Potencia: %d | Pontos campeonato: %.0f\n",
                   i + 1,
                   grid[i]->nome,
                   grid[i]->carro->marca,
                   grid[i]->carro->modelo,
                   grid[i]->carro->potencia,
                   grid[i]->pontosCampeonato);
        }
        else
        {
            printf("%do %s | Sem carro | Pontos campeonato: %.0f\n",
                   i + 1,
                   grid[i]->nome,
                   grid[i]->pontosCampeonato);
        }
    }

    // =====================================================
    // SIMULACAO DAS VOLTAS
    // =====================================================
    for (int v = 1; v <= TOTAL_VOLTAS; v++)
    {
        printf("\n--- VOLTA %d ---\n", v);

        for (int i = 0; i < qtd; i++)
        {
            Piloto *p = grid[i];

            if (p->abandonou)
                continue;

            if (!p->carro || p->carro->potencia <= 0)
            {
                printf("%s nao possui carro valido e abandonou a corrida.\n", p->nome);
                p->abandonou = 1;
                p->voltaAbandono = v;
                continue;
            }

            double base = (double)pista->tamanho / p->carro->potencia;
            double variacao = (rand() % 100) / 500.0;
            double tempoVolta = base + variacao;
            double novoTempo = p->tempoFinal + tempoVolta;

            /*
                Acidente por corrida:
                So acontece se:
                - a corrida foi sorteada para ter acidente;
                - a volta atual e a volta sorteada;
                - o piloto atual e o piloto sorteado.
            */
            if (corridaTeraAcidente &&
                p == pilotoDoAcidente &&
                v == voltaDoAcidente)
            {
                p->abandonou = 1;
                p->voltaAbandono = v;
                p->tempoFinal = novoTempo;
                totalAcidentes = 1;

                printf("ACIDENTE: %s abandonou na volta %d.\n",
                       p->nome,
                       v);

                continue;
            }

            p->tempoFinal = novoTempo;
        }

        OrdenarCorrida(grid, qtd);
        ExibirClassificacaoVolta(grid, qtd, v);
    }

    // =====================================================
    // RESUMO DE ACIDENTES
    // =====================================================
    printf("\n--- RESUMO DE ACIDENTES (%s) ---\n", pista->nome);
    printf("Chance aplicada: %.2f%% por corrida\n", chanceAcidente * 100.0);
    printf("Acidentes nesta corrida: %d\n", totalAcidentes);

    // =====================================================
    // ORDENACAO FINAL
    // =====================================================
    OrdenarCorrida(grid, qtd);

    int pontos[10] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};

    printf("\n==============================\n");
    printf("CLASSIFICACAO FINAL (%s)\n", pista->nome);
    printf("==============================\n");

    for (int i = 0; i < qtd; i++)
    {
        Piloto *p = grid[i];
        int pts = 0;

        if (!p->abandonou && i < 10)
        {
            pts = pontos[i];
            p->pontosCampeonato += pts;
        }

        SalvarResultado(listaCorrida,
                        p,
                        i + 1,
                        pts,
                        p->tempoFinal,
                        p->abandonou,
                        p->voltaAbandono);

        if (p->abandonou)
        {
            printf("%do %s | ABANDONOU na volta %d | Pontos: %d | Total: %.0f\n",
                   i + 1,
                   p->nome,
                   p->voltaAbandono,
                   pts,
                   p->pontosCampeonato);
        }
        else
        {
            printf("%do %s | Tempo: %.2f | Pontos: %d | Total: %.0f\n",
                   i + 1,
                   p->nome,
                   p->tempoFinal,
                   pts,
                   p->pontosCampeonato);
        }
    }
}

// =========================================================
// EXIBIR RESULTADOS (HISTORICO COMPLETO)
// =========================================================
void ExibirResultados(ListaCorrida *lista)
{
    if (!lista || !lista->inicio)
    {
        printf("Sem corridas registradas.\n");
        return;
    }

    Corrida *c = lista->inicio;

    while (c != NULL)
    {
        printf("\n==============================\n");
        printf("HISTORICO DA CORRIDA (%s)\n", c->pista->nome);
        printf("==============================\n");

        Resultado *r = c->resultado;

        if (r == NULL)
        {
            printf("Nenhum resultado salvo para esta corrida.\n");
        }

        while (r != NULL)
        {
            if (r->abandonou)
            {
                printf("%do %s | ABANDONOU na volta %d | Pontos: %d | Tempo acumulado: %.2f\n",
                       r->colocacao,
                       r->piloto->nome,
                       r->voltaAbandono,
                       r->pontuacao,
                       r->tempoFinal);
            }
            else
            {
                printf("%do %s | Pontos: %d | Tempo: %.2f\n",
                       r->colocacao,
                       r->piloto->nome,
                       r->pontuacao,
                       r->tempoFinal);
            }

            r = r->prox;
        }

        c = c->prox;
    }
}

// =========================================================
// RANKING CAMPEONATO FINAL ORDENADO
// =========================================================
void ExibirRankingCampeonato(PilhaPiloto *pilhaPiloto)
{
    if (!pilhaPiloto || !pilhaPiloto->topo)
    {
        printf("Sem pilotos no campeonato.\n");
        return;
    }

    Piloto *ranking[MAX_PILOTOS_GRID];
    int qtd = 0;

    Piloto *p = pilhaPiloto->topo;

    while (p != NULL && qtd < MAX_PILOTOS_GRID)
    {
        ranking[qtd++] = p;
        p = p->prox;
    }

    OrdenarGridPorCampeonato(ranking, qtd);

    printf("\n==============================\n");
    printf("RANKING DO CAMPEONATO\n");
    printf("==============================\n");

    for (int i = 0; i < qtd; i++)
    {
        printf("%do %s | Pontos: %.0f\n",
               i + 1,
               ranking[i]->nome,
               ranking[i]->pontosCampeonato);
    }
}

// =========================================================
// LIMPEZA
// =========================================================
void LimparCorrida(ListaCorrida **lista)
{
    if (!lista || !(*lista))
        return;

    Corrida *c = (*lista)->inicio;

    while (c != NULL)
    {
        Resultado *r = c->resultado;

        while (r != NULL)
        {
            Resultado *tmpResultado = r;
            r = r->prox;
            free(tmpResultado);
        }

        Corrida *tmpCorrida = c;
        c = c->prox;
        free(tmpCorrida);
    }

    free(*lista);
    *lista = NULL;
}