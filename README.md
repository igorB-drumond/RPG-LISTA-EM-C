Sistema de Inventário RPG — Estrutura de Dados
Implementação em linguagem C desenvolvida para a disciplina de Estrutura de Dados, com foco no estudo prático, análise empírica e comparação de desempenho entre Listas Estáticas (Vetores) e Listas Encadeadas (Dinâmicas), além de algoritmos clássicos de busca e ordenação.
Índice
Visão Geral
Funcionalidades
Estruturas de Dados
Análise de Complexidade Assintótica
Algoritmos Implementados
Busca
Ordenação
Como Compilar e Executar
Estrutura do Código
Exemplo de Uso e Métricas
Visão Geral
O projeto simula um inventário de itens de um jogo de RPG (com atributos como ID, Nome, Tipo e Raridade), permitindo que o usuário escolha em tempo de execução qual estrutura base deseja operar:
Vetor Estático: Alocação contígua com limite pré-fixado de capacidade (MAX_ITENS = 100). Suporta acesso indexado direto, ordenação e busca binária.
Lista Simplesmente Encadeada: Alocação puramente dinâmica via ponteiros em tempo de execução (malloc/free), sem limite rígido de memória, porém limitada ao acesso sequencial.
Funcionalidades
CRUD de Itens: Inserção, listagem formatada e remoção por chave primária (ID).
Prevenção de Duplicatas: Validação de unicidade para o identificador (ID).
Validação de Entrada: Tratamento de entradas inválidas e limpeza de buffer (stdin) para tipos numéricos e textuais.
Instrumentação de Métricas: Contadores em tempo real para comparações de chaves e movimentações/trocas de dados em memória.
Compatibilidade Multiplataforma: Suporte à limpeza de tela tanto em ambientes Windows (cls) quanto baseados em POSIX/Linux/macOS (clear).
Estruturas de Dados



C
typedef struct {
    int id;           // Identificador único
    char nome[50];    // Nome do item (ex: "Excalibur")
    char tipo[30];    // Categoria (ex: "Arma", "Poção", "Armadura")
    int raridade;     // Escala de 1 a 5
} Item;


ListaVetor: Vetor sequencial estático de tamanho 100, mantendo o total de itens ativos e uma flag ordenadoPorID que valida se o vetor está apto para Busca Binária.
No (Lista Encadeada): Estrutura contendo o dado (Item) e o ponteiro autorreferenciado struct No* proximo.
Análise de Complexidade Assintótica
Estruturas: Vetor vs. Lista Encadeada
Operação
Vetor Estático
Lista Encadeada
Detalhe Operacional
Inserção no Início
O(N)
O(1)
No vetor exige shift à direita; na lista basta atualizar ponteiros.
Inserção no Fim
O(1)
O(N)
Vetor insere diretamente no índice tamanho. (Nota: o projeto insere no início da lista encadeada, tornando-a O(1)).
Remoção por ID
O(N)
O(N)
Ambos precisam de O(N) para encontrar; o vetor gasta mais tempo deslocando os elementos subsequentes.
Acesso por Índice
O(1)
O(N)
Vetor possui aritmética de ponteiros direta; lista requer travessia nó a nó.
Uso de Memória
Fixo (sizeof(Item) * 100)
Dinâmico
A lista consome memória adicional para o ponteiro proximo por nó (8 bytes em arquiteturas 64-bit).

Algoritmos Implementados
Algoritmos de Busca
Busca Linear (O(N)):
Executada sequencialmente elemento a elemento.
Funciona em qualquer estrutura (Vetor ou Lista) independentemente do estado de ordenação.
Pior caso: N comparações.
Busca Binária (O(N)):
Disponível exclusivamente para o Vetor Estático.
Pré-requisito: O vetor deve estar previamente ordenado pelo atributo chave (ID).
Divide o espaço de busca pela metade a cada passo. Para 100 itens, o pior caso executa no máximo 7 iterações.
Algoritmos de Ordenação
Disponíveis para o vetor estático, ordenando por Raridade (ou por ID como rotina preparatória para a busca binária):
Algoritmo
Melhor Caso
Caso Médio
Pior Caso
Trocas (Pior Caso)
Estabilidade
Bubble Sort
O(N2)*
O(N2)
O(N2)
O(N2)
Estável
Selection Sort
O(N2)
O(N2)
O(N2)
O(N)
Não estável
Insertion Sort
O(N)
O(N2)
O(N2)
O(N2) movimentações
Estável

* Na implementação atual sem flag de parada antecipada.
Como Compilar e Executar
Pré-requisitos
Compilador C padrão (gcc, clang ou msvc).
Compilando via Terminal (GCC)



Bash
# Compilação com flags de otimização e exibição de avisos
gcc -Wall -Wextra -pedantic main.c -o inventario_rpg

# Executando no Linux/macOS:
./inventario_rpg

# Executando no Windows:
inventario_rpg.exe


Estrutura do Código



Plaintext
├── main.c              # Código-fonte unificado contendo:
│   ├── Estruturas      # Declaração das structs Item, ListaVetor e No
│   ├── Auxiliares      # Tratamento de buffer, inputs, pausas e formatação
│   ├── Vetor           # Operações de CRUD do vetor estático
│   ├── Lista           # Operações de alocação/desalocação e manipulação de nós
│   ├── Buscas          # Implementações instrumentadas de Busca Linear e Binária
│   ├── Ordenação       # Bubble, Selection e Insertion Sort com contadores
│   └── Interface       # Menus interativos via linha de comando
└── README.md           # Documentação técnica do projeto


Exemplo de Uso e Métricas
Ao executar o menu e ordenar o inventário pelo método de Selection Sort, o programa gera um relatório comparativo direto:



Plaintext
--- Escolha o Algoritmo de Ordenacao (por Raridade) ---
1. Bubble Sort
2. Selection Sort
3. Insertion Sort
0. Voltar
Escolha: 2

Ordenado com Selection Sort!
Relatorio de Analise:
- Comparacoes: 45
- Trocas:      4


Ao rodar a Busca Binária com o vetor devidamente ordenado:



Plaintext
--- Busca Binaria ---
Digite o ID do item a buscar: 42
Busca Binaria (Vetor): Item encontrado!
----------------------------------
ID:       42
Nome:     Espada Vorpal
Tipo:     Arma
Raridade: 5
----------------------------------
Relatorio de Analise: Busca Binaria realizou 4 comparacoes.


Considerações Acadêmicas
Busca Binária em Listas Encadeadas: A busca binária é inviável em listas simplesmente encadeadas clássicas, pois encontrar o elemento central (meio) exige percorrer N/2 nós ponteiro a ponteiro (O(N)), anulando o ganho assintótico de O(N).
Localidade de Referência: O vetor apresenta desempenho prático de cache superior (spatial locality), enquanto os nós da lista encadeada ficam espalhados na memória heap, ocasionando mais cache misses.
Gerenciamento de Memória: Ao encerrar o programa selecionando a lista encadeada, todos os nós alocados na Heap são sistematicamente liberados via free(), prevenindo memory leaks.
