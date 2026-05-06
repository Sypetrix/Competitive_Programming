# Utilitários e Estruturas Customizadas (Utils)

Esta pasta contém algoritmos da STL, métodos utilitários de estado (pares e tuplas), algoritmos de ordenação e Estruturas de Dados Avançadas que **não possuem implementação nativa pronta no C++** (como Segment Trees, Fenwick Trees, Tries e Union-Find). 

---

## Visão Geral

| Estrutura / Utilitário | Complexidade Principal | Utilização típica |
|---|---|---|
| `pair` e `tuple` | O(1) | Agrupamento de estados e ordenação lexicográfica múltipla. |
| `Fenwick Tree` (BIT) | O(log N) | Soma em prefixos e atualização de pontos muito veloz. |
| `Segment Tree` | O(log N) | Consultas e atualizações (Lazy) em intervalos arbitrários. |
| `Trie` | O(Tamanho da String) | Busca ultra rápida de palavras completas e prefixos. |
| `Union-Find` (DSU) | O(α(N)) ≈ O(1) | Gestão de componentes conexos dinâmicos e algoritmo de Kruskal. |
| `Algoritmos de Ordenação` | O(N log N) ~ O(N²) | Referência comparativa de estabilidade, memória e casos de uso. |

---

## Algorithm e Pair/Tuple

O C++ já possui ferramentas geniais para não termos de reinventar a roda durante uma competição, seja para buscas binárias super compactas ou para criar estruturas com múltiplos valores.

```cpp
// Busca Binária da STL
auto it = lower_bound(v.begin(), v.end(), 30); // Primeiro elemento >= 30

// Pair e Tuple com C++17 (Structured Bindings)
pair<int, int> p = {10, 20};
tuple<int, string, double> t = {1, "Maratona", 9.8};

auto [x, y] = p;
auto [id, nome, nota] = t; // Muito melhor do que usar get<1>(t)
```

> **Use `<algorithm>`, `pair` e `tuple` quando** precisar empacotar informações complexas de forma rápida (como nas filas do Dijkstra) ou aplicar pesquisas sem escrever um loop manual.

---

## Fenwick Tree (Binary Indexed Tree)

A Fenwick Tree permite calcular somas de prefixos de um array e atualizar elementos individuais, ambas as operações em O(log N). É muito mais simples e rápida de codar do que a Segment Tree, mas é limitada a operações inversíveis (como a soma).

```cpp
void add(int idx, int val) {
    for (; idx <= n; idx += (idx & -idx)) {
        bit[idx] += val;
    }
}

int query(int idx) {
    int sum = 0;
    for (; idx > 0; idx -= (idx & -idx)) {
        sum += bit[idx];
    }
    return sum;
}
```

> **Use `Fenwick Tree` quando** precisar fazer somas/frequências em intervalos e os dados estiverem em constante atualização pontual.

---

## Segment Tree (SegTree) com Lazy Propagation

A "rainha" das maratonas. Permite consultas e atualizações de **intervalos** (Range Queries) em O(log N). Muito mais poderosa e flexível que a Fenwick Tree, suportando operações não-inversíveis como Mínimo/Máximo.

```cpp
// A lógica central do Lazy Propagation (empurrar preguiça pros filhos)
void push(int node, int start, int end) {
    if (lazy[node] != 0) {
        tree[node] += lazy[node] * (end - start + 1); 
        if (start != end) {
            lazy[2 * node] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
        }
        lazy[node] = 0; 
    }
}
```

> **Use `Segment Tree` quando** precisar atualizar um intervalo inteiro ao mesmo tempo (ex: "somar X a todos os elementos de L a R") e consultar o Mínimo/Máximo/Soma num intervalo específico.

---

## Trie (Árvore de Prefixos)

Uma estrutura de árvore onde cada nó representa uma letra. Usada em processamento avançado de strings para verificar prefixos comuns.

```cpp
void insert(const string& s) {
    int u = 0; // Raiz
    for (char c : s) {
        int idx = c - 'a'; 
        if (!trie[u][idx]) trie[u][idx] = ++node_cnt; // Aloca novo nó
        u = trie[u][idx];
    }
    is_end[u]++;
}
```

> **Use `Trie` quando** o problema envolver um grande dicionário de palavras e for necessário procurar rapidamente pela existência de prefixos em comum ou autocompletar.

---

## Union-Find (Disjoint Set Union - DSU)

Uma estrutura brilhante para gerir partições e responder se dois elementos pertencem ao mesmo conjunto em tempo amortizado praticamente constante O(1).

```cpp
int find_set(int v) { // Com Path Compression
    if (v == parent_node[v]) return v;
    return parent_node[v] = find_set(parent_node[v]); 
}

void union_sets(int a, int b) { // Com Union by Size
    a = find_set(a); b = find_set(b);
    if (a != b) {
        if (size_node[a] < size_node[b]) swap(a, b);
        parent_node[b] = a;
        size_node[a] += size_node[b];
    }
}
```

> **Use `Union-Find` quando** precisar juntar grafos, encontrar ciclos não-direcionados, ligar componentes dinamicamente ou implementar a Árvore Geradora Mínima (MST) de Kruskal.

---

## Algoritmos de Ordenação

A STL oferece `std::sort` (introsort, O(N log N)) que resolve quase todos os casos. Conhecer os algoritmos clássicos é importante para entender as trocas entre estabilidade, memória e desempenho.

| Algoritmo | Complexidade | Estável | In-place | Melhor caso |
|---|---|---|---|---|
| Merge Sort | O(N log N) | Sim | Não | O(N log N) |
| Quick Sort | O(N log N) médio | Não | Sim | O(N log N) |
| Counting Sort | O(N + K) | Sim | Não | O(N + K) |
| Radix Sort | O(N · D) | Sim | Não | O(N · D) |
| Insertion Sort | O(N²) | Sim | Sim | O(N) quase ordenado |
| Selection Sort | O(N²) | Não | Sim | O(N²) |

**Merge Sort** é a escolha clássica para ordenação estável. Também é a base para contar inversões em um array em O(N log N): basta contar, durante o `merge`, quantas vezes um elemento da metade direita é menor do que um da metade esquerda ainda não processada.

**Quick Sort** é rápido na prática por ter boa localidade de cache, mas sofre no pior caso O(N²) com pivôs ruins. Em competições, prefira sempre `std::sort`.

**Counting Sort** e **Radix Sort** são lineares mas exigem restrições no domínio dos valores (inteiros em intervalo limitado). Use quando $K$ (amplitude dos valores) for pequeno em relação a $N$.

**Insertion Sort** degenera para O(N²) em geral, mas atinge O(N) em arrays quase ordenados. É usado internamente pelo `std::sort` para subarrays pequenos (introsort).

```cpp
// Ordenação estável com std::stable_sort — O(N log N)
stable_sort(v.begin(), v.end());

// Critério customizado (ex: ordenar por segundo elemento decrescente)
sort(v.begin(), v.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
    return a.second > b.second;
});
```

> **Regra prática:** use `std::sort` para o caso geral, `std::stable_sort` quando a ordem relativa de elementos iguais importar, e Counting/Radix Sort quando os valores forem inteiros pequenos e a performance linear fizer diferença.