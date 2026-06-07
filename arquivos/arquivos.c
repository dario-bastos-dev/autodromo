#include "arquivos.h"
#include <sys/stat.h>
#include <errno.h>

static void timestamp_agora(char *buf, size_t tamanho) {
    time_t agora = time(NULL);
    struct tm *t  = localtime(&agora);
    strftime(buf, tamanho, "%Y-%m-%d %H:%M:%S", t);
}

static void empilhar_carro(PilhaCarro **pilha, const char *marca,
                           const char *modelo, int potencia) {
    Carro *novo = (Carro *)malloc(sizeof(Carro));
    if (!novo) return;

    strncpy(novo->marca,  marca,  20); novo->marca[20]  = '\0';
    strncpy(novo->modelo, modelo, 20); novo->modelo[20] = '\0';
    novo->potencia = potencia;
    novo->prox     = (*pilha)->topo;
    (*pilha)->topo = novo;
}

/* ═══════════════════════════════════════════════════════
   FUNÇÕES DE ARQUIVO
   ═══════════════════════════════════════════════════════ */

int CarregarCarros(const char *caminho, PilhaCarro **pilha) {
    if (!caminho || !pilha || !(*pilha)) return -1;

    FILE *f = fopen(caminho, "r");
    if (!f) {
        fprintf(stderr, "[ERRO] Nao foi possivel abrir '%s': %s\n",
                caminho, strerror(errno));
        return -1;
    }

    char linha[128];
    int  count = 0;

    /* Pula cabeçalho, se existir */
    if (fgets(linha, sizeof(linha), f)) {
        /* Se a primeira linha não é numérica no 3º campo, é cabeçalho */
        char tmp_marca[21], tmp_modelo[21];
        int  tmp_pot;
        if (sscanf(linha, "%20[^,],%20[^,],%d",
                   tmp_marca, tmp_modelo, &tmp_pot) == 3) {
            /* Era dado, não cabeçalho — empilha */
            empilhar_carro(pilha, tmp_marca, tmp_modelo, tmp_pot);
            count++;
        }
    }

    while (fgets(linha, sizeof(linha), f)) {
        /* Remove \n no final */
        linha[strcspn(linha, "\n")] = '\0';
        if (linha[0] == '\0') continue; /* linha em branco */

        char marca[21], modelo[21];
        int  potencia;

        if (sscanf(linha, "%20[^,],%20[^,],%d",
                   marca, modelo, &potencia) == 3) {
            empilhar_carro(pilha, marca, modelo, potencia);
            count++;
        } else {
            fprintf(stderr, "[AVISO] Linha ignorada (formato invalido): %s\n",
                    linha);
        }
    }

    fclose(f);
    return count;
}

int SalvarCarros(const char *caminho, PilhaCarro *pilha) {
    if (!caminho || !pilha) return -1;

    FILE *f = fopen(caminho, "w");
    if (!f) {
        fprintf(stderr, "[ERRO] Nao foi possivel salvar em '%s': %s\n",
                caminho, strerror(errno));
        return -1;
    }

    /* Cabeçalho */
    fprintf(f, "marca,modelo,potencia\n");

    Carro *atual = pilha->topo;
    while (atual) {
        fprintf(f, "%s,%s,%d\n", atual->marca, atual->modelo, atual->potencia);
        atual = atual->prox;
    }

    fclose(f);
    return 0;
}

int SalvarEstadoCorrida(const char *caminho, EstadoCarro *estado, int total) {
    if (!caminho || !estado || total <= 0) return -1;

    FILE *f = fopen(caminho, "w");
    if (!f) {
        fprintf(stderr, "[ERRO] Nao foi possivel salvar estado em '%s': %s\n",
                caminho, strerror(errno));
        return -1;
    }

    fprintf(f, "posicao,marca,modelo,potencia,voltas,status\n");
    for (int i = 0; i < total; i++) {
        fprintf(f, "%d,%s,%s,%d,%d,%s\n",
                estado[i].posicao,
                estado[i].marca,
                estado[i].modelo,
                estado[i].potencia,
                estado[i].voltas,
                estado[i].status);
    }

    fclose(f);
    return 0;
}

int CarregarEstadoCorrida(const char *caminho, EstadoCarro *estado, int maxCarros) {
    if (!caminho || !estado || maxCarros <= 0) return -1;

    FILE *f = fopen(caminho, "r");
    if (!f) {
        fprintf(stderr, "[ERRO] Nao foi possivel abrir estado '%s': %s\n",
                caminho, strerror(errno));
        return -1;
    }

    char linha[256];
    int  count = 0;

    fgets(linha, sizeof(linha), f); /* pula cabeçalho */

    while (fgets(linha, sizeof(linha), f) && count < maxCarros) {
        linha[strcspn(linha, "\n")] = '\0';
        if (linha[0] == '\0') continue;

        EstadoCarro *e = &estado[count];
        if (sscanf(linha, "%d,%20[^,],%20[^,],%d,%d,%15s",
                   &e->posicao, e->marca, e->modelo,
                   &e->potencia, &e->voltas, e->status) == 6) {
            count++;
        }
    }

    fclose(f);
    return count;
}

/* ═══════════════════════════════════════════════════════
   FUNÇÕES DE LOG
   ═══════════════════════════════════════════════════════ */

FILE *AbrirLog(const char *caminho) {
    if (!caminho) return NULL;

    FILE *f = fopen(caminho, "a"); /* append: não apaga logs anteriores */
    if (!f) {
        fprintf(stderr, "[ERRO] Nao foi possivel abrir log '%s': %s\n",
                caminho, strerror(errno));
        return NULL;
    }

    /* Separador de sessão */
    char ts[32];
    timestamp_agora(ts, sizeof(ts));
    fprintf(f, "\n══════════════════════════════════════════\n");
    fprintf(f, "  SESSAO INICIADA: %s\n", ts);
    fprintf(f, "══════════════════════════════════════════\n");
    fflush(f);

    return f;
}

void FecharLog(FILE *log) {
    if (!log) return;

    char ts[32];
    timestamp_agora(ts, sizeof(ts));
    fprintf(log, "──────────────────────────────────────────\n");
    fprintf(log, "  SESSAO ENCERRADA: %s\n", ts);
    fprintf(log, "══════════════════════════════════════════\n\n");
    fclose(log);
}

void LogEvento(FILE *log, const char *tipo, const char *descricao) {
    if (!log || !tipo || !descricao) return;

    char ts[32];
    timestamp_agora(ts, sizeof(ts));
    fprintf(log, "[%s] %-14s | %s\n", ts, tipo, descricao);
    fflush(log);
}

void LogPosicoes(FILE *log, int volta, EstadoCarro *estado, int total) {
    if (!log || !estado || total <= 0) return;

    char ts[32];
    timestamp_agora(ts, sizeof(ts));

    fprintf(log, "[%s] POSICOES       | === Volta %d ===\n", ts, volta);
    for (int i = 0; i < total; i++) {
        fprintf(log,
                "[%s] POSICOES       |   %dº  %-20s %-20s  %3d cv  voltas:%d  [%s]\n",
                ts,
                estado[i].posicao,
                estado[i].marca,
                estado[i].modelo,
                estado[i].potencia,
                estado[i].voltas,
                estado[i].status);
    }
    fflush(log);
}

void LogUltrapassagem(FILE *log, int volta,
                      const char *quem_ultrapassa,
                      const char *quem_foi_ultrapassado) {
    if (!log || !quem_ultrapassa || !quem_foi_ultrapassado) return;

    char ts[32];
    char desc[128];
    timestamp_agora(ts, sizeof(ts));

    snprintf(desc, sizeof(desc),
             "Volta %d | %s ultrapassou %s",
             volta, quem_ultrapassa, quem_foi_ultrapassado);

    fprintf(log, "[%s] %-14s | %s\n", ts, "ULTRAPASSAGEM", desc);
    fflush(log);
}

void LogAcidente(FILE *log, int volta, const char *modelo_carro) {
    if (!log || !modelo_carro) return;

    char ts[32];
    char desc[128];
    timestamp_agora(ts, sizeof(ts));

    snprintf(desc, sizeof(desc),
             "Volta %d | %s sofreu um acidente e abandonou a corrida",
             volta, modelo_carro);

    fprintf(log, "[%s] %-14s | %s\n", ts, "ACIDENTE", desc);
    fflush(log);
}

void LogFimCorrida(FILE *log, EstadoCarro *estado, int total) {
    if (!log || !estado || total <= 0) return;

    char ts[32];
    timestamp_agora(ts, sizeof(ts));

    fprintf(log, "[%s] %-14s | *** CORRIDA ENCERRADA ***\n", ts, "FIM");
    fprintf(log, "[%s] %-14s | === PODIO ===\n", ts, "FIM");

    int podio = 0;
    const char *medalhas[] = {"🥇 1º", "🥈 2º", "🥉 3º"};

    for (int i = 0; i < total && podio < 3; i++) {
        if (strcmp(estado[i].status, STATUS_ACIDENTE) != 0 &&
            strcmp(estado[i].status, STATUS_LARGOU)   != 0) {
            fprintf(log,
                    "[%s] %-14s |   %s  %s %s (%d cv)\n",
                    ts, "FIM",
                    medalhas[podio],
                    estado[i].marca,
                    estado[i].modelo,
                    estado[i].potencia);
            podio++;
        }
    }
    fflush(log);
}

/* ═══════════════════════════════════════════════════════
   UTILITÁRIOS
   ═══════════════════════════════════════════════════════ */

int CriarDiretorioSeNecessario(const char *caminho) {
    struct stat st;
    if (stat(caminho, &st) == 0) return 0; /* já existe */

#ifdef _WIN32
    if (mkdir(caminho) != 0 && errno != EEXIST) {
#else
    if (mkdir(caminho, 0755) != 0 && errno != EEXIST) {
#endif
        fprintf(stderr, "[ERRO] Nao foi possivel criar diretorio '%s': %s\n",
                caminho, strerror(errno));
        return -1;
    }
    return 0;
}

/* Comparador para qsort — ordena por potência decrescente */
static int cmp_potencia(const void *a, const void *b) {
    const EstadoCarro *ca = (const EstadoCarro *)a;
    const EstadoCarro *cb = (const EstadoCarro *)b;
    return cb->potencia - ca->potencia; /* maior potência = posição melhor */
}

int PilhaParaEstado(PilhaCarro *pilha, EstadoCarro *estado, int maxCarros) {
    if (!pilha || !estado || maxCarros <= 0) return 0;

    int count = 0;
    Carro *atual = pilha->topo;

    while (atual && count < maxCarros) {
        strncpy(estado[count].marca,  atual->marca,  20);
        strncpy(estado[count].modelo, atual->modelo, 20);
        estado[count].marca[20]  = '\0';
        estado[count].modelo[20] = '\0';
        estado[count].potencia   = atual->potencia;
        estado[count].voltas     = 0;
        strncpy(estado[count].status, STATUS_CORRENDO, 15);
        estado[count].status[15] = '\0';
        count++;
        atual = atual->prox;
    }

    /* Ordena e atribui posições iniciais por potência */
    qsort(estado, count, sizeof(EstadoCarro), cmp_potencia);
    for (int i = 0; i < count; i++) {
        estado[i].posicao = i + 1;
    }

    return count;
}
