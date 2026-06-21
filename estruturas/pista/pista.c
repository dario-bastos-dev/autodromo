#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pista.h"

// =========================================================
// REMOVE ESPACOS DO INICIO
// =========================================================
static char *RemoverEspacosInicio(char *texto)
{
    while (texto && isspace((unsigned char)*texto))
        texto++;

    return texto;
}

// =========================================================
// REMOVE ESPACOS DO FINAL
// =========================================================
static void RemoverEspacosFinais(char *texto)
{
    if (!texto)
        return;

    int len = (int)strlen(texto);

    while (len > 0 && isspace((unsigned char)texto[len - 1]))
    {
        texto[len - 1] = '\0';
        len--;
    }
}

// =========================================================
// CONVERTE TEXTO PARA FLOAT SEM DEPENDER DO LOCALE
// Aceita:
// 0.06
// 0,06
// 6
// =========================================================
static float ConverterTaxaAcidente(const char *texto)
{
    if (!texto)
        return 0.0f;

    char copia[32];
    int i = 0;

    while (texto[i] != '\0' && texto[i] != '\n' && i < 31)
    {
        if (texto[i] == ',')
            copia[i] = '.';
        else
            copia[i] = texto[i];

        i++;
    }

    copia[i] = '\0';

    int parteInteira = 0;
    int parteDecimal = 0;
    int casasDecimais = 0;
    int encontrouPonto = 0;

    for (i = 0; copia[i] != '\0'; i++)
    {
        if (copia[i] == '.')
        {
            encontrouPonto = 1;
            continue;
        }

        if (isdigit((unsigned char)copia[i]))
        {
            if (!encontrouPonto)
            {
                parteInteira = parteInteira * 10 + (copia[i] - '0');
            }
            else
            {
                parteDecimal = parteDecimal * 10 + (copia[i] - '0');
                casasDecimais++;
            }
        }
    }

    float divisor = 1.0f;

    for (i = 0; i < casasDecimais; i++)
        divisor *= 10.0f;

    if (casasDecimais == 0)
    {
        /*
            Se o arquivo vier como:
            6
            interpretamos como 6% = 0.06
        */
        if (parteInteira > 1)
            return (float)parteInteira / 100.0f;

        return (float)parteInteira;
    }

    return (float)parteInteira + ((float)parteDecimal / divisor);
}

// =========================================================
// LER UMA LINHA DE PISTA
// Formato esperado:
// Nome da Pista - tamanho - taxa
//
// Exemplo:
// Spa-Francorchamps - 7004 - 0.06
//
// Importante:
// O nome Spa-Francorchamps possui hífen.
// Por isso NÃO podemos separar só por "-";
// precisamos separar por " - ".
// =========================================================
static int LerLinhaPista(char *linha, Pista *pista)
{
    if (!linha || !pista)
        return 0;

    char *separador1 = strstr(linha, " - ");

    if (!separador1)
        return 0;

    char *separador2 = strstr(separador1 + 3, " - ");

    if (!separador2)
        return 0;

    char nomeTemp[50];
    char tamanhoTemp[20];
    char taxaTemp[32];

    int tamanhoNome = (int)(separador1 - linha);
    int tamanhoValor = (int)(separador2 - (separador1 + 3));

    if (tamanhoNome <= 0 || tamanhoNome >= 50)
        return 0;

    if (tamanhoValor <= 0 || tamanhoValor >= 20)
        return 0;

    strncpy(nomeTemp, linha, tamanhoNome);
    nomeTemp[tamanhoNome] = '\0';

    strncpy(tamanhoTemp, separador1 + 3, tamanhoValor);
    tamanhoTemp[tamanhoValor] = '\0';

    strncpy(taxaTemp, separador2 + 3, sizeof(taxaTemp) - 1);
    taxaTemp[sizeof(taxaTemp) - 1] = '\0';

    char *nomeLimpo = RemoverEspacosInicio(nomeTemp);
    RemoverEspacosFinais(nomeLimpo);

    char *tamanhoLimpo = RemoverEspacosInicio(tamanhoTemp);
    RemoverEspacosFinais(tamanhoLimpo);

    char *taxaLimpa = RemoverEspacosInicio(taxaTemp);
    RemoverEspacosFinais(taxaLimpa);

    strncpy(pista->nome, nomeLimpo, sizeof(pista->nome) - 1);
    pista->nome[sizeof(pista->nome) - 1] = '\0';

    pista->tamanho = atoi(tamanhoLimpo);
    pista->acidente = ConverterTaxaAcidente(taxaLimpa);

    return 1;
}

// =========================================================
// INICIAR PISTAS
// =========================================================
Pista *IniciarPistas()
{
    Pista *pistas = (Pista *)malloc(sizeof(Pista) * 6);

    if (pistas == NULL)
    {
        printf("Erro ao alocar memoria para pistas.\n");
        return NULL;
    }

    FILE *arquivo_pistas = fopen("docs/pistas.txt", "r");

    if (arquivo_pistas == NULL)
    {
        printf("Erro ao abrir o arquivo docs/pistas.txt.\n");
        free(pistas);
        return NULL;
    }

    char linha[256];
    int posicao = 0;

    while (fgets(linha, sizeof(linha), arquivo_pistas) && posicao < 6)
    {
        if (LerLinhaPista(linha, &pistas[posicao]))
        {
            posicao++;
        }
        else
        {
            printf("Aviso: linha de pista ignorada: %s", linha);
        }
    }

    fclose(arquivo_pistas);

    if (posicao < 6)
    {
        printf("Aviso: foram carregadas apenas %d pistas.\n", posicao);
    }

    // Embaralha apenas as pistas carregadas.
    for (int j = posicao - 1; j > 0; j--)
    {
        int r = rand() % (j + 1);

        Pista temp = pistas[j];
        pistas[j] = pistas[r];
        pistas[r] = temp;
    }

    return pistas;
}

// =========================================================
// EXIBIR PISTAS
// =========================================================
void ExibirPistas(Pista *pistas)
{
    if (!pistas)
    {
        printf("Nenhuma pista cadastrada.\n");
        return;
    }

    for (int i = 0; i < 6; i++)
    {
        printf("Pista: %s | Tamanho: %d km | Chance de acidente: %.2f%%\n",
               pistas[i].nome,
               pistas[i].tamanho,
               pistas[i].acidente * 100.0f);
    }
}

// =========================================================
// EXIBIR UMA PISTA
// =========================================================
void ExibirPista(Pista *pista)
{
    if (!pista)
        return;

    printf("Pista: %s | Tamanho: %d km | Chance de acidente: %.2f%%\n",
           pista->nome,
           pista->tamanho,
           pista->acidente * 100.0f);
}