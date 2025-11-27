/* mochila.c
   Desafio: Sistema de mochila (capacidade 10)
   Funcionalidades: adicionar, remover, listar, busca sequencial,
   ordenação (insertion sort com contador de comparações),
   busca binária por nome (checa se está ordenado por nome).
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_ITENS 10
#define MAX_NOME 50
#define MAX_TIPO 30

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int prioridade; // 1 a 5
} Item;

typedef enum {
    CRITERIO_NOME = 1,
    CRITERIO_TIPO = 2,
    CRITERIO_PRIORIDADE = 3
} SortCriteria;

typedef enum {
    SEM_ORDENACAO,
    ORDENADO_POR_NOME,
    ORDENADO_POR_TIPO,
    ORDENADO_POR_PRIORIDADE
} SortOrder;

/* Protótipos */
void adicionarItem(Item mochila[], int *qtd);
void removerItem(Item mochila[], int *qtd);
void listarItens(Item mochila[], int qtd);
int buscaSequencial(Item mochila[], int qtd, const char nome[]);
int insertionSort(Item mochila[], int qtd, SortCriteria criterio);
int buscaBinariaPorNome(Item mochila[], int qtd, const char nome[]);
void exibirItemDetalhado(const Item *it);
void limparBufferStdin(void);

int main() {
    Item mochila[MAX_ITENS];
    int qtd = 0;
    SortOrder estadoOrdenacao = SEM_ORDENACAO;
    int opcao;

    do {
        printf("\n=== Mochila da Ilha (Capacidade: %d itens) ===\n", MAX_ITENS);
        printf("1. Adicionar item\n");
        printf("2. Remover item por nome\n");
        printf("3. Listar itens\n");
        printf("4. Buscar (sequencial) por nome\n");
        printf("5. Ordenar mochila (Insertion Sort)\n");
        printf("6. Buscar (binária) por nome (requer ordenação por nome)\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Reinicie o programa.\n");
            return 1;
        }
        limparBufferStdin();

        switch(opcao) {
            case 1:
                adicionarItem(mochila, &qtd);
                estadoOrdenacao = SEM_ORDENACAO;
                break;
            case 2:
                removerItem(mochila, &qtd);
                estadoOrdenacao = SEM_ORDENACAO;
                break;
            case 3:
                listarItens(mochila, qtd);
                break;
            case 4: {
                char nomeBusca[MAX_NOME];
                printf("Digite o nome do item para busca sequencial: ");
                fgets(nomeBusca, sizeof(nomeBusca), stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

                int idx = buscaSequencial(mochila, qtd, nomeBusca);
                if (idx >= 0) {
                    printf("\nItem encontrado (posicao %d):\n", idx);
                    exibirItemDetalhado(&mochila[idx]);
                } else {
                    printf("\nItem '%s' nao encontrado (busca sequencial).\n", nomeBusca);
                }
                break;
            }
            case 5: {
                int criterio, comparacoes;
                printf("\nEscolha criterio de ordenacao:\n");
                printf("1 - Nome\n2 - Tipo\n3 - Prioridade\n");
                printf("Opcao: ");
                if (scanf("%d", &criterio) != 1) {
                    printf("Entrada invalida.\n");
                    limparBufferStdin();
                    break;
                }
                limparBufferStdin();

                if (criterio < 1 || criterio > 3) {
                    printf("Criterio invalido.\n");
                    break;
                }

                comparacoes = insertionSort(mochila, qtd, (SortCriteria)criterio);
                if (criterio == CRITERIO_NOME) estadoOrdenacao = ORDENADO_POR_NOME;
                else if (criterio == CRITERIO_TIPO) estadoOrdenacao = ORDENADO_POR_TIPO;
                else if (criterio == CRITERIO_PRIORIDADE) estadoOrdenacao = ORDENADO_POR_PRIORIDADE;

                printf("Ordenacao concluida. Comparacoes realizadas: %d\n", comparacoes);
                break;
            }
            case 6: {
                if (estadoOrdenacao != ORDENADO_POR_NOME) {
                    printf("\nERRO: A busca binaria por nome exige que a mochila esteja ordenada por NOME.\n");
                    printf("Ordene primeiro por NOME (opcao 5 -> criterio 1).\n");
                    break;
                }
                char nomeBusca[MAX_NOME];
                printf("Digite o nome do item para busca binaria: ");
                fgets(nomeBusca, sizeof(nomeBusca), stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

                int idx = buscaBinariaPorNome(mochila, qtd, nomeBusca);
                if (idx >= 0) {
                    printf("\nItem encontrado (posicao %d):\n", idx);
                    exibirItemDetalhado(&mochila[idx]);
                } else {
                    printf("\nItem '%s' nao encontrado (busca binaria).\n", nomeBusca);
                }
                break;
            }
            case 0:
                printf("Saindo... boa sorte na ilha!\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while(opcao != 0);

    return 0;
}

/* Limpa o buffer do stdin (consome ate newline) */
void limparBufferStdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Adiciona item */
void adicionarItem(Item mochila[], int *qtd) {
    if (*qtd >= MAX_ITENS) {
        printf("Mochila cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    Item novo;
    printf("Digite o nome do item: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Digite o tipo do item: ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Digite a quantidade: ");
    if (scanf("%d", &novo.quantidade) != 1) {
        printf("Quantidade invalida.\n");
        limparBufferStdin();
        return;
    }
    limparBufferStdin();

    printf("Digite a prioridade (1 a 5): ");
    if (scanf("%d", &novo.prioridade) != 1) {
        printf("Prioridade invalida.\n");
        limparBufferStdin();
        return;
    }
    limparBufferStdin();

    if (novo.prioridade < 1) novo.prioridade = 1;
    if (novo.prioridade > 5) novo.prioridade = 5;
    if (novo.quantidade < 0) novo.quantidade = 0;

    mochila[*qtd] = novo;
    (*qtd)++;
    printf("Item '%s' adicionado com sucesso.\n", novo.nome);
}

/* Remove item por nome (usa busca sequencial) */
void removerItem(Item mochila[], int *qtd) {
    if (*qtd == 0) {
        printf("Mochila vazia. Nada a remover.\n");
        return;
    }

    char nomeRemover[MAX_NOME];
    printf("Digite o nome do item a remover: ");
    fgets(nomeRemover, sizeof(nomeRemover), stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = '\0';

    int idx = buscaSequencial(mochila, *qtd, nomeRemover);
    if (idx < 0) {
        printf("Item '%s' nao encontrado. Nada removido.\n", nomeRemover);
        return;
    }

    // shift left to overwrite
    for (int i = idx; i < (*qtd) - 1; i++) {
        mochila[i] = mochila[i+1];
    }
    (*qtd)--;
    printf("Item '%s' removido com sucesso (posicao %d).\n", nomeRemover, idx);
}

/* Listar itens em formato de tabela */
void listarItens(Item mochila[], int qtd) {
    if (qtd == 0) {
        printf("Mochila vazia.\n");
        return;
    }
    printf("\n%-3s | %-30s | %-15s | %-10s | %-9s\n", "ID", "NOME", "TIPO", "QUANT.", "PRIORIDADE");
    printf("-----+--------------------------------+-----------------+-----------+-----------\n");
    for (int i = 0; i < qtd; i++) {
        printf("%-3d | %-30s | %-15s | %-9d | %-9d\n",
               i, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
}

/* Busca sequencial por nome - retorna indice ou -1 */
int buscaSequencial(Item mochila[], int qtd, const char nome[]) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

/* Exibe item detalhado */
void exibirItemDetalhado(const Item *it) {
    printf("Nome      : %s\n", it->nome);
    printf("Tipo      : %s\n", it->tipo);
    printf("Quantidade: %d\n", it->quantidade);
    printf("Prioridade: %d\n", it->prioridade);
}

/* Insertion Sort com contador de comparacoes
   Retorna numero de comparacoes realizadas. */
int insertionSort(Item mochila[], int qtd, SortCriteria criterio) {
    int comparacoes = 0;

    for (int i = 1; i < qtd; i++) {
        Item chave = mochila[i];
        int j = i - 1;

        if (criterio == CRITERIO_NOME) {
            // compara por nome (strcmp). Enquanto chave.nome < mochila[j].nome, shift.
            while (j >= 0) {
                comparacoes++;
                if (strcmp(chave.nome, mochila[j].nome) < 0) {
                    mochila[j+1] = mochila[j];
                    j--;
                } else {
                    break;
                }
            }
            mochila[j+1] = chave;
        } else if (criterio == CRITERIO_TIPO) {
            // compara por tipo (strcmp)
            while (j >= 0) {
                comparacoes++;
                if (strcmp(chave.tipo, mochila[j].tipo) < 0) {
                    mochila[j+1] = mochila[j];
                    j--;
                } else {
                    break;
                }
            }
            mochila[j+1] = chave;
        } else if (criterio == CRITERIO_PRIORIDADE) {
            // ordena por prioridade crescente (1 menor prioridade?).
            // Vamos ordenar prioridade DECRESCENTE (maior prioridade primeiro) - se preferir mudar, ajuste.
            while (j >= 0) {
                comparacoes++;
                if (chave.prioridade > mochila[j].prioridade) { // maior prioridade vem antes
                    mochila[j+1] = mochila[j];
                    j--;
                } else {
                    break;
                }
            }
            mochila[j+1] = chave;
        }
    }

    return comparacoes;
}

/* Busca binaria por nome - assume array ordenado por nome ascendente
   Retorna indice ou -1 */
int buscaBinariaPorNome(Item mochila[], int qtd, const char nome[]) {
    int lo = 0;
    int hi = qtd - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int cmp = strcmp(nome, mochila[mid].nome);
        if (cmp == 0) return mid;
        else if (cmp < 0) hi = mid - 1;
        else lo = mid + 1;
    }
    return -1;
}
