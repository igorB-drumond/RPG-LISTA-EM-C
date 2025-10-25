/*
 * PROJETO: Sistema de Inventário de RPG
 * DISCIPLINA: Estrutura de Dados
 * OBJETIVO: Implementar e comparar o desempenho de Listas Estáticas (Vetor)
 * e Listas Encadeadas, além de algoritmos de busca e ordenação.
 * DATA: 22/10/2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para toupper

// --- 1. ESTRUTURAS DE DADOS ---

#define MAX_ITENS 100 // Tamanho máximo do inventário estático

/**
 * @brief Estrutura que modela um item do jogo.
 */
typedef struct {
    int id;
    char nome[50];
    char tipo[30];
    int raridade; // 1 a 5
} Item;

/**
 * @brief Estrutura para a Lista Estática (Vetor).
 * Contém o vetor de itens e um contador de tamanho.
 */
typedef struct {
    Item itens[MAX_ITENS];
    int tamanho;
    int ordenadoPorID; // 0 = Não, 1 = Sim (para busca binária)
} ListaVetor;

/**
 * @brief Estrutura do Nó para a Lista Encadeada.
 * Contém o item e um ponteiro para o próximo nó.
 */
typedef struct No {
    Item item;
    struct No* proximo;
} No;


// --- 2. FUNÇÕES AUXILIARES ---

/**
 * @brief Limpa o buffer de entrada (stdin)
 * Necessário após usar scanf para ler números ou chars.
 */
void limpar_buffer_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Pausa a execução e aguarda o usuário pressionar Enter.
 */
void pausar() {
    printf("\nPressione Enter para continuar...");
    limpar_buffer_stdin();
}

/**
 * @brief Limpa a tela do console (multi-plataforma).
 */
void limpar_tela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/**
 * @brief Solicita os dados de um novo item ao usuário.
 * @return O Item preenchido.
 */
Item criar_item() {
    Item novoItem;
    int idValido = 0;
    int raridadeValida = 0;

    // Validação do ID
    while (!idValido) {
        printf("Digite o ID unico do item: ");
        if (scanf("%d", &novoItem.id) == 1 && novoItem.id > 0) {
            idValido = 1;
        } else {
            printf("ID invalido. Deve ser um numero inteiro positivo.\n");
            limpar_buffer_stdin();
        }
    }
    limpar_buffer_stdin();

    printf("Digite o nome do item: ");
    // Lê strings com espaços
    fgets(novoItem.nome, 50, stdin); 
    novoItem.nome[strcspn(novoItem.nome, "\n")] = 0; // Remove o '\n'

    printf("Digite o tipo (Ex: Arma, Pocao, Armadura): ");
    fgets(novoItem.tipo, 30, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0;

    // Validação da Raridade
    while (!raridadeValida) {
        printf("Digite a raridade (1 a 5): ");
        if (scanf("%d", &novoItem.raridade) == 1 && novoItem.raridade >= 1 && novoItem.raridade <= 5) {
            raridadeValida = 1;
        } else {
            printf("Raridade invalida. Deve ser um numero de 1 a 5.\n");
            limpar_buffer_stdin();
        }
    }
    limpar_buffer_stdin();

    return novoItem;
}

/**
 * @brief Exibe os detalhes de um único item.
 * @param item O item a ser exibido.
 */
void exibir_item(Item item) {
    printf("----------------------------------\n");
    printf("ID:       %d\n", item.id);
    printf("Nome:     %s\n", item.nome);
    printf("Tipo:     %s\n", item.tipo);
    printf("Raridade: %d\n", item.raridade);
    printf("----------------------------------\n");
}

/**
 * @brief Função de troca para os algoritmos de ordenação.
 */
void trocar(Item* a, Item* b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}


// --- 3. OPERAÇÕES DA LISTA VETOR (ESTÁTICA) ---

/**
 * @brief Inicializa a lista vetor.
 */
void inicializar_vetor(ListaVetor* lista) {
    lista->tamanho = 0;
    lista->ordenadoPorID = 0;
    printf("Inventario (Vetor) inicializado.\n");
}

/**
 * @brief Insere um item no vetor.
 * @return 1 em caso de sucesso, 0 se o inventário estiver cheio.
 */
int inserir_vetor(ListaVetor* lista, Item item) {
    if (lista->tamanho >= MAX_ITENS) {
        printf("Erro: Inventario (Vetor) esta cheio!\n");
        return 0;
    }
    
    // Verifica se o ID já existe
    for(int i = 0; i < lista->tamanho; i++) {
        if(lista->itens[i].id == item.id) {
            printf("Erro: ID %d ja existe no inventario!\n", item.id);
            return 0;
        }
    }

    lista->itens[lista->tamanho] = item;
    lista->tamanho++;
    lista->ordenadoPorID = 0; // Inserção desordena a lista
    printf("Item '%s' (ID: %d) adicionado ao vetor.\n", item.nome, item.id);
    return 1;
}

/**
 * @brief Lista todos os itens do vetor.
 */
void listar_vetor(ListaVetor* lista) {
    if (lista->tamanho == 0) {
        printf("Inventario (Vetor) esta vazio.\n");
        return;
    }
    printf("\n=== INVENTARIO (VETOR) [%d/%d] ===\n", lista->tamanho, MAX_ITENS);
    for (int i = 0; i < lista->tamanho; i++) {
        exibir_item(lista->itens[i]);
    }
}

/**
 * @brief Remove um item do vetor pelo ID.
 * @return 1 em caso de sucesso, 0 se o item não for encontrado.
 */
int remover_vetor(ListaVetor* lista, int id) {
    int indiceEncontrado = -1;
    for (int i = 0; i < lista->tamanho; i++) {
        if (lista->itens[i].id == id) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Erro: Item com ID %d nao encontrado no vetor.\n", id);
        return 0;
    }
    
    printf("Item '%s' (ID: %d) removido.\n", lista->itens[indiceEncontrado].nome, id);

    // "Puxa" os elementos para cobrir o buraco
    for (int i = indiceEncontrado; i < lista->tamanho - 1; i++) {
        lista->itens[i] = lista->itens[i + 1];
    }

    lista->tamanho--;
    // Não podemos garantir a ordenação após a remoção,
    // embora tecnicamente ela se mantenha.
    // Para segurança, resetamos o flag.
    lista->ordenadoPorID = 0; 
    return 1;
}

// --- 4. OPERAÇÕES DA LISTA ENCADEADA ---

/**
 * @brief Inicializa a lista encadeada.
 */
void inicializar_lista(No** lista) {
    *lista = NULL;
    printf("Inventario (Lista Encadeada) inicializado.\n");
}

/**
 * @brief Insere um item na lista encadeada (no início).
 * @return 1 em caso de sucesso, 0 se falhar a alocação.
 */
int inserir_lista(No** lista, Item item) {
    // Verifica se o ID já existe
    No* temp = *lista;
    while(temp != NULL) {
        if(temp->item.id == item.id) {
            printf("Erro: ID %d ja existe no inventario!\n", item.id);
            return 0;
        }
        temp = temp->proximo;
    }

    // Aloca novo nó
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro: Falha ao alocar memoria para novo item!\n");
        return 0;
    }

    novoNo->item = item;
    novoNo->proximo = *lista; // O novo nó aponta para o antigo início
    *lista = novoNo;         // O início da lista agora é o novo nó

    printf("Item '%s' (ID: %d) adicionado a lista.\n", item.nome, item.id);
    return 1;
}

/**
 * @brief Lista todos os itens da lista encadeada.
 */
void listar_lista(No* lista) {
    if (lista == NULL) {
        printf("Inventario (Lista Encadeada) esta vazio.\n");
        return;
    }

    printf("\n=== INVENTARIO (LISTA ENCADEADA) ===\n");
    No* temp = lista;
    while (temp != NULL) {
        exibir_item(temp->item);
        temp = temp->proximo;
    }
}

/**
 * @brief Remove um item da lista encadeada pelo ID.
 * @return 1 em caso de sucesso, 0 se o item não for encontrado.
 */
int remover_lista(No** lista, int id) {
    No* temp = *lista;
    No* anterior = NULL;

    // Procura o nó a ser removido
    while (temp != NULL && temp->item.id != id) {
        anterior = temp;
        temp = temp->proximo;
    }

    if (temp == NULL) {
        printf("Erro: Item com ID %d nao encontrado na lista.\n", id);
        return 0;
    }
    
    printf("Item '%s' (ID: %d) removido.\n", temp->item.nome, id);

    // Caso 1: O nó a ser removido é o primeiro da lista
    if (anterior == NULL) {
        *lista = temp->proximo;
    }
    // Caso 2: O nó está no meio ou no fim
    else {
        anterior->proximo = temp->proximo;
    }

    free(temp); // Libera a memória do nó removido
    return 1;
}


// --- 5. ALGORITMOS DE BUSCA ---

/**
 * @brief Busca Linear no Vetor.
 * @param comparacoes Ponteiro para contar o número de comparações.
 * @return 1 se encontrado, 0 se não.
 */
int busca_linear_vetor(ListaVetor* lista, int id, int* comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < lista->tamanho; i++) {
        (*comparacoes)++; // Conta cada checagem de ID
        if (lista->itens[i].id == id) {
            printf("Busca Linear (Vetor): Item encontrado!\n");
            exibir_item(lista->itens[i]);
            return 1;
        }
    }
    printf("Busca Linear (Vetor): Item com ID %d nao encontrado.\n", id);
    return 0;
}

/**
 * @brief Busca Linear na Lista Encadeada.
 * @param comparacoes Ponteiro para contar o número de comparações.
 * @return 1 se encontrado, 0 se não.
 */
int busca_linear_lista(No* lista, int id, int* comparacoes) {
    *comparacoes = 0;
    No* temp = lista;
    while (temp != NULL) {
        (*comparacoes)++; // Conta cada checagem de ID
        if (temp->item.id == id) {
            printf("Busca Linear (Lista): Item encontrado!\n");
            exibir_item(temp->item);
            return 1;
        }
        temp = temp->proximo;
    }
    printf("Busca Linear (Lista): Item com ID %d nao encontrado.\n", id);
    return 0;
}

/**
 * @brief Busca Binária no Vetor.
 * @param comparacoes Ponteiro para contar o número de comparações.
 * @return 1 se encontrado, 0 se não.
 */
int busca_binaria_vetor(ListaVetor* lista, int id, int* comparacoes) {
    *comparacoes = 0;
    int inicio = 0;
    int fim = lista->tamanho - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2; // Evita overflow

        (*comparacoes)++; // Conta a primeira checagem (igualdade)
        if (lista->itens[meio].id == id) {
            printf("Busca Binaria (Vetor): Item encontrado!\n");
            exibir_item(lista->itens[meio]);
            return 1;
        }

        (*comparacoes)++; // Conta a segunda checagem (menor/maior)
        if (lista->itens[meio].id < id) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    printf("Busca Binaria (Vetor): Item com ID %d nao encontrado.\n", id);
    return 0;
}


// --- 6. ALGORITMOS DE ORDENAÇÃO (VETOR) ---
// Todos ordenam por RARIDADE (crescente)

/**
 * @brief Ordena o vetor por raridade usando Bubble Sort.
 */
void bubbleSort(ListaVetor* lista, long* comparacoes, long* trocas) {
    *comparacoes = 0;
    *trocas = 0;
    int n = lista->tamanho;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            (*comparacoes)++;
            if (lista->itens[j].raridade > lista->itens[j + 1].raridade) {
                trocar(&lista->itens[j], &lista->itens[j + 1]);
                (*trocas)++;
            }
        }
    }
    lista->ordenadoPorID = 0; // Lista foi ordenada por raridade
}

/**
 * @brief Ordena o vetor por raridade usando Selection Sort.
 */
void selectionSort(ListaVetor* lista, long* comparacoes, long* trocas) {
    *comparacoes = 0;
    *trocas = 0;
    int n = lista->tamanho;
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (lista->itens[j].raridade < lista->itens[min_idx].raridade) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            trocar(&lista->itens[i], &lista->itens[min_idx]);
            (*trocas)++;
        }
    }
    lista->ordenadoPorID = 0; // Lista foi ordenada por raridade
}

/**
 * @brief Ordena o vetor por raridade usando Insertion Sort.
 */
void insertionSort(ListaVetor* lista, long* comparacoes, long* trocas) {
    *comparacoes = 0;
    *trocas = 0;
    int n = lista->tamanho;
    for (int i = 1; i < n; i++) {
        Item chave = lista->itens[i];
        int j = i - 1;

        /* Move os elementos de lista->itens[0..i-1] que são
           maiores que a chave, para uma posição à frente */
        while (j >= 0) {
            (*comparacoes)++;
            if (lista->itens[j].raridade > chave.raridade) {
                lista->itens[j + 1] = lista->itens[j];
                (*trocas)++; // Conta como uma "troca" ou "movimentação"
                j = j - 1;
            } else {
                break; // Sai do while se a condição não for atendida
            }
        }
        lista->itens[j + 1] = chave;
        // (Opcional) Contar a inserção final como uma troca
        // if(j != i - 1) (*trocas)++; 
    }
    lista->ordenadoPorID = 0; // Lista foi ordenada por raridade
}

/**
 * @brief Ordena o vetor por ID (necessário para Busca Binária).
 * Usaremos o Insertion Sort para esta tarefa.
 */
void ordenarPorID(ListaVetor* lista, long* comparacoes, long* trocas) {
    *comparacoes = 0;
    *trocas = 0;
    int n = lista->tamanho;
    for (int i = 1; i < n; i++) {
        Item chave = lista->itens[i];
        int j = i - 1;
        while (j >= 0) {
            (*comparacoes)++;
            if (lista->itens[j].id > chave.id) {
                lista->itens[j + 1] = lista->itens[j];
                (*trocas)++;
                j = j - 1;
            } else {
                break;
            }
        }
        lista->itens[j + 1] = chave;
    }
    lista->ordenadoPorID = 1; // MARCA o vetor como ordenado por ID
}


// --- 7. MENUS E FUNÇÃO PRINCIPAL ---

void exibir_menu_principal(int tipoLista) {
    printf("\n======= SISTEMA DE INVENTARIO RPG =======\n");
    if (tipoLista == 1) {
        printf("     Usando: VETOR (Estatico)\n");
    } else {
        printf("     Usando: LISTA (Encadeada)\n");
    }
    printf("=========================================\n");
    printf("1. Inserir Item\n");
    printf("2. Listar Itens\n");
    printf("3. Remover Item (por ID)\n");
    printf("4. Buscar Item (Busca Linear por ID)\n");
    
    // Funções exclusivas do VETOR
    if (tipoLista == 1) {
        printf("5. Buscar Item (Busca Binaria por ID)\n");
        printf("6. Ordenar por Raridade (Bubble/Select/Insert)\n");
        printf("7. Preparar para Busca Binaria (Ordenar por ID)\n");
    } else {
        printf("5. (Indisponivel na Lista Encadeada)\n");
        printf("6. (Indisponivel na Lista Encadeada)\n");
        printf("7. (Indisponivel na Lista Encadeada)\n");
    }
    printf("0. Sair\n");
    printf("=========================================\n");
    printf("Escolha uma opcao: ");
}

void menu_ordenacao_raridade(ListaVetor* lista) {
    int escolha;
    long comparacoes = 0;
    long trocas = 0;

    printf("\n--- Escolha o Algoritmo de Ordenacao (por Raridade) ---\n");
    printf("1. Bubble Sort\n");
    printf("2. Selection Sort\n");
    printf("3. Insertion Sort\n");
    printf("0. Voltar\n");
    printf("Escolha: ");
    
    if (scanf("%d", &escolha) != 1) {
        escolha = -1; // Força opção inválida
    }
    limpar_buffer_stdin();

    switch (escolha) {
        case 1:
            bubbleSort(lista, &comparacoes, &trocas);
            printf("\nOrdenado com Bubble Sort!\n");
            printf("Relatorio de Analise:\n");
            printf("- Comparacoes: %ld\n", comparacoes);
            printf("- Trocas:      %ld\n", trocas);
            break;
        case 2:
            selectionSort(lista, &comparacoes, &trocas);
            printf("\nOrdenado com Selection Sort!\n");
            printf("Relatorio de Analise:\n");
            printf("- Comparacoes: %ld\n", comparacoes);
            printf("- Trocas:      %ld\n", trocas);
            break;
        case 3:
            insertionSort(lista, &comparacoes, &trocas);
            printf("\nOrdenado com Insertion Sort!\n");
            printf("Relatorio de Analise:\n");
            printf("- Comparacoes: %ld\n", comparacoes);
            printf("- Trocas:      %ld\n", trocas);
            break;
        case 0:
            printf("Voltando...\n");
            break;
        default:
            printf("Opcao invalida!\n");
            break;
    }
}

int main() {
    int tipoLista = 0; // 1 = Vetor, 2 = Lista
    int escolha_menu = -1;
    int id_busca_remove;
    int comparacoes_busca;
    long comp_ord, trocas_ord;

    // Estruturas de dados
    ListaVetor inventarioVetor;
    No* inventarioLista = NULL; // Importante inicializar com NULL

    // --- Escolha da Estrutura ---
    while (tipoLista != 1 && tipoLista != 2) {
        limpar_tela();
        printf("Bem-vindo ao Gerenciador de Inventario!\n");
        printf("Qual estrutura de dados deseja usar?\n");
        printf("1. Vetor (Lista Estatica)\n");
        printf("2. Lista Encadeada\n");
        printf("Escolha: ");
        
        if (scanf("%d", &tipoLista) != 1) {
            tipoLista = 0; // Força repetição
        }
        limpar_buffer_stdin();

        if (tipoLista == 1) {
            inicializar_vetor(&inventarioVetor);
        } else if (tipoLista == 2) {
            inicializar_lista(&inventarioLista);
        } else {
            printf("Opcao invalida! Tente novamente.\n");
            pausar();
        }
    }
    
    pausar();

    // --- Loop Principal do Menu ---
    do {
        limpar_tela();
        exibir_menu_principal(tipoLista);
        
        if (scanf("%d", &escolha_menu) != 1) {
            escolha_menu = -1; // Força opção inválida
        }
        limpar_buffer_stdin();

        switch (escolha_menu) {
            case 1: // Inserir
                printf("\n--- Inserir Novo Item ---\n");
                Item novoItem = criar_item();
                if (tipoLista == 1) {
                    inserir_vetor(&inventarioVetor, novoItem);
                } else {
                    inserir_lista(&inventarioLista, novoItem);
                }
                break;
            
            case 2: // Listar
                if (tipoLista == 1) {
                    listar_vetor(&inventarioVetor);
                } else {
                    listar_lista(inventarioLista);
                }
                break;

            case 3: // Remover
                printf("\n--- Remover Item ---\n");
                printf("Digite o ID do item a remover: ");
                scanf("%d", &id_busca_remove);
                limpar_buffer_stdin();
                if (tipoLista == 1) {
                    remover_vetor(&inventarioVetor, id_busca_remove);
                } else {
                    remover_lista(&inventarioLista, id_busca_remove);
                }
                break;

            case 4: // Busca Linear
                printf("\n--- Busca Linear ---\n");
                printf("Digite o ID do item a buscar: ");
                scanf("%d", &id_busca_remove);
                limpar_buffer_stdin();
                comparacoes_busca = 0;
                if (tipoLista == 1) {
                    busca_linear_vetor(&inventarioVetor, id_busca_remove, &comparacoes_busca);
                } else {
                    busca_linear_lista(inventarioLista, id_busca_remove, &comparacoes_busca);
                }
                printf("Relatorio de Analise: Busca Linear realizou %d comparacoes.\n", comparacoes_busca);
                break;

            case 5: // Busca Binária (Só Vetor)
                if (tipoLista == 1) {
                    printf("\n--- Busca Binaria ---\n");
                    if (inventarioVetor.ordenadoPorID == 0) {
                        printf("ATENCAO: A Busca Binaria exige que o vetor esteja ordenado por ID.\n");
                        printf("Use a Opcao 7 para ordenar primeiro.\n");
                    } else {
                        printf("Digite o ID do item a buscar: ");
                        scanf("%d", &id_busca_remove);
                        limpar_buffer_stdin();
                        comparacoes_busca = 0;
                        busca_binaria_vetor(&inventarioVetor, id_busca_remove, &comparacoes_busca);
                        printf("Relatorio de Analise: Busca Binaria realizou %d comparacoes.\n", comparacoes_busca);
                    }
                } else {
                    printf("Opcao invalida para Lista Encadeada.\n");
                }
                break;

            case 6: // Ordenar por Raridade (Só Vetor)
                if (tipoLista == 1) {
                    menu_ordenacao_raridade(&inventarioVetor);
                } else {
                    printf("Opcao invalida para Lista Encadeada.\n");
                }
                break;
            
            case 7: // Ordenar por ID (Só Vetor)
                if (tipoLista == 1) {
                    printf("\n--- Preparando para Busca Binaria (Ordenando por ID) ---\n");
                    comp_ord = 0;
                    trocas_ord = 0;
                    ordenarPorID(&inventarioVetor, &comp_ord, &trocas_ord);
                    printf("Vetor ordenado por ID (Insertion Sort).\n");
                    printf("Busca Binaria (Opcao 5) esta pronta para uso.\n");
                    printf("(Stats da ordenacao por ID: %ld comparacoes, %ld trocas)\n", comp_ord, trocas_ord);
                } else {
                    printf("Opcao invalida para Lista Encadeada.\n");
                }
                break;

            case 0: // Sair
                printf("Saindo do sistema...\n");
                break;
            
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }

        if(escolha_menu != 0) {
            pausar();
        }

    } while (escolha_menu != 0);

    // --- Limpeza de Memória (para Lista Encadeada) ---
    if (tipoLista == 2) {
        No* temp = inventarioLista;
        while (temp != NULL) {
            No* proximo = temp->proximo;
            free(temp);
            temp = proximo;
        }
        printf("Memoria da lista encadeada liberada.\n");
    }

    return 0;
}

/*
 * =============================================================================
 * RELATÓRIO DE ANÁLISE (Modelo para preenchimento)
 * =============================================================================
 *
 * Ao testar o programa com N itens, observei o seguinte:
 *
 * 1. ESTRUTURA DE DADOS (Vetor vs Lista):
 * - Inserção: A inserção no Vetor (O(1) no final) foi rápida,
 * enquanto a inserção na Lista (O(1) no início) também foi.
 * No entanto, o Vetor falhou ao atingir o limite de MAX_ITENS.
 * - Remoção: A remoção no Vetor (O(N)) exigiu deslocar todos os
 * elementos seguintes, o que é lento para listas grandes. A remoção
 * na Lista (O(N) para buscar, O(1) para remover) foi
 * comparável em tempo de busca, mas a operação de "ponteiros" foi
 * mais eficiente que o deslocamento de memória.
 *
 * 2. ALGORITMOS DE BUSCA (Linear vs Binária):
 * - Para N = [Número de itens]...
 * - Busca Linear: A busca pelo último item no vetor exigiu [N] comparações.
 * Na lista, o resultado foi similar, exigindo [N] comparações.
 * A complexidade O(N) foi claramente observada.
 * - Busca Binária (Vetor): Após ordenar por ID, a busca pelo mesmo
 * item exigiu apenas [Número, ex: 3 ou 4] comparações.
 * Isso demonstra a eficiência do O(log N) da busca binária.
 *
 * 3. ALGORITMOS DE ORDENAÇÃO (Por Raridade, N = [Número de itens]):
 * - Bubble Sort:
 * - Comparações: [Número de comps]
 * - Trocas:      [Número de trocas]
 * - Análise: Foi o mais lento e realizou o maior número de trocas
 * em um cenário desordenado.
 *
 * - Selection Sort:
 * - Comparações: [Número de comps]
 * - Trocas:      [Número de trocas]
 * - Análise: Realizou menos trocas que o Bubble Sort,
 * mas um número similar de comparações (ambos O(N^2)).
 *
 * - Insertion Sort:
 * - Comparações: [Número de comps]
 * - Trocas:      [Número de trocas/movimentações]
 * - Análise: Para uma lista quase ordenada, foi o mais rápido.
 * Em uma lista invertida, seu desempenho foi similar aos outros O(N^2).
 *
 * CONCLUSÃO:
 * A Busca Binária é drasticamente mais eficiente que a Linear, mas exige
 * o custo adicional de manter a lista ordenada. Listas Encadeadas são
 * flexíveis em tamanho, mas perdem a capacidade de usar a Busca Binária.
 * Entre os sorts O(N^2), o Insertion Sort é geralmente preferível para
 * listas pequenas ou quase ordenadas.
 *
 * =============================================================================
 */