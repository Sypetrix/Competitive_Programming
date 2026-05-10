# Algoritmos Avançados em Grafos (Advanced)

Esta pasta reúne algoritmos que exigem técnicas mais sofisticadas: decomposição em componentes fortemente conexas (Tarjan), satisfatibilidade em 2 variáveis (2-SAT), problemas clássicos de teoria dos grafos (caminhos Eulerianos e Hamiltonianos) e extensões do Dijkstra para espaços de estado multidimensionais.

---

## Visão Geral

| Algoritmo / Arquivo | Complexidade | Utilização Típica |
|---|---|---|
| `tarjan` | O(V + E) | Componentes Fortemente Conexas (SCCs) em grafos direcionados |
| 2-SAT | O(V + E) | Satisfatibilidade de fórmulas com cláusulas de 2 literais |
| `euler` | O(V + E) | Verificar existência de caminho ou circuito Euleriano |
| `hamilton` | O(2^N · N²) | Caminho ou Circuito Hamiltoniano com DP bitmask |
| `layers` | O(K · (V + E) log V) | Dijkstra em espaço de estados com K dimensões extras |

---

## Tarjan — Componentes Fortemente Conexas (SCCs)

Um subgrafo direcionado é **fortemente conexo** se existe um caminho entre qualquer par de vértices em ambas as direções. O algoritmo de Tarjan encontra todas as SCCs em uma única DFS, usando dois valores por nó: `tin` (tempo de entrada) e `low` (menor tempo alcançável sem subir por uma aresta de árvore já processada).

```cpp
int tin[MAXN], low[MAXN], timer_scc;
bool in_stack[MAXN];
stack<int> st;
vector<vector<int>> sccs;

void dfs_tarjan(int u) {
    tin[u] = low[u] = ++timer_scc;
    st.push(u); in_stack[u] = true;

    for (int v : adj[u]) {
        if (!tin[v]) {
            dfs_tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (in_stack[v]) {
            low[u] = min(low[u], tin[v]); // Aresta de retorno
        }
    }

    // u e raiz de uma SCC se ninguém acima dele e alcancavel
    if (low[u] == tin[u]) {
        vector<int> scc;
        int v;
        do { v = st.top(); st.pop(); in_stack[v] = false; scc.push_back(v); }
        while (v != u);
        sccs.push_back(scc);
    }
}
```

**Condensação:** após encontrar todas as SCCs, é comum condensar o grafo — cada SCC vira um único "super-nó". O grafo condensado é sempre um **DAG**, o que permite aplicar programação dinâmica ou ordenação topológica sobre ele.

> **Use Tarjan quando** precisar encontrar grupos de vértices mutuamente alcançáveis, detectar ciclos em grafos direcionados, ou reduzir um grafo a um DAG para aplicar outras técnicas.

---

## 2-SAT

O **2-SAT** resolve instâncias do problema de satisfatibilidade onde cada cláusula tem exatamente 2 literais. Dado um conjunto de variáveis booleanas x₁, ..., xₙ e cláusulas da forma `(aᵢ ∨ bᵢ)`, determine se existe uma atribuição que satisfaz todas as cláusulas.

**Ideia central:** cada cláusula `(a ∨ b)` é equivalente a duas implicações: `¬a → b` e `¬b → a`. Representamos cada variável xᵢ por dois nós no grafo de implicações: `xᵢ` (índice i) e `¬xᵢ` (índice i + n). A fórmula é satisfatível se e somente se nenhuma variável e sua negação estão na mesma SCC.

```cpp
const int MAXN = 2e5 + 5; // 2*n nós: [0,n) = xi, [n,2n) = ¬xi
vector<int> adj[MAXN], radj[MAXN]; // grafo e grafo reverso
int order_cnt, comp[MAXN];
bool vis[MAXN];
vector<int> order; // ordem de saída da DFS

// Adiciona a cláusula (a ∨ b)
// Para xi use i, para ¬xi use i+n
void add_clause(int a, int b, int n) {
    adj[a ^ 1].push_back(b);   // ¬a → b
    adj[b ^ 1].push_back(a);   // ¬b → a
    radj[b].push_back(a ^ 1);
    radj[a].push_back(b ^ 1);
}
// Convenção: variável xi = 2*i, ¬xi = 2*i+1
// add_clause(2*i, 2*j+1, n) = cláusula (xi ∨ ¬xj)
```

> Nota: existem duas convenções comuns de indexação. A seguir usamos a mais simples: xi = 2*i, ¬xi = 2*i+1. Com isso, o literal complementar de `v` é `v ^ 1`.

```cpp
void dfs1(int v) {
    vis[v] = true;
    for (int u : adj[v]) if (!vis[u]) dfs1(u);
    order.push_back(v);
}

void dfs2(int v, int c) {
    comp[v] = c;
    for (int u : radj[v]) if (comp[u] == -1) dfs2(u, c);
}

// Algoritmo de Kosaraju para SCCs (alternativa ao Tarjan)
bool solve_2sat(int n) {
    // n = número de variáveis; 2n nós no total
    order.clear();
    fill(vis, vis + 2*n, false);
    fill(comp, comp + 2*n, -1);

    for (int i = 0; i < 2*n; i++)
        if (!vis[i]) dfs1(i);

    int c = 0;
    for (int i = 2*n - 1; i >= 0; i--) {
        if (comp[order[i]] == -1) dfs2(order[i], c++);
    }

    // Verificar satisfatibilidade: xi e ¬xi não podem estar na mesma SCC
    for (int i = 0; i < n; i++)
        if (comp[2*i] == comp[2*i+1]) return false;
    return true;
}

// Recuperar a atribuição (após confirmar satisfatibilidade):
// xi = true  se comp[2*i] > comp[2*i+1]  (xi está em SCC posterior a ¬xi)
// xi = false caso contrário
bool get_value(int i) {
    return comp[2*i] > comp[2*i+1];
}
```

### Cláusulas Comuns em Competições

| Restrição | Cláusula | Implicações adicionadas |
|---|---|---|
| xi ∨ xj | add_clause(2i, 2j, n) | ¬xi → xj, ¬xj → xi |
| xi deve ser true | add_clause(2i, 2i, n) | ¬xi → xi |
| xi deve ser false | add_clause(2i+1, 2i+1, n) | xi → ¬xi |
| xi = xj (igualdade) | duas cláusulas | xi → xj, xj → xi, ¬xi → ¬xj, ¬xj → ¬xi |
| xi ≠ xj (diferença) | duas cláusulas | xi → ¬xj, xj → ¬xi, ¬xi → xj, ¬xj → xi |
| xi → xj (implicação) | add_clause(2i+1, 2j, n) | ¬xi → xj (equivalente a ¬xi ∨ xj) |

> **Use 2-SAT quando** o problema tiver variáveis booleanas e restrições do tipo "A ou B deve ser verdadeiro" — coloração de grafos com 2 cores, atribuição de turnos, questões de consistência entre pares de condições.

---

## Caminhos e Circuitos Eulerianos

Um **caminho Euleriano** percorre cada **aresta** do grafo exatamente uma vez. Se começa e termina no mesmo vértice, é um **circuito Euleriano**.

A existência é determinada apenas pelas condições de grau, sem precisar construir o caminho:

**Grafo não-direcionado:**

| Condição | Resultado |
|---|---|
| Todos os vértices com grau par | Circuito Euleriano |
| Exatamente 2 vértices com grau ímpar | Caminho Euleriano (entre esses dois) |
| Mais de 2 vértices com grau ímpar | Não existe |

**Grafo direcionado:**

| Condição | Resultado |
|---|---|
| Todo vértice: grau_entrada = grau_saída | Circuito Euleriano |
| Um vértice com saída − entrada = 1 e um com entrada − saída = 1 | Caminho Euleriano |
| Qualquer outra configuração | Não existe |

Em ambos os casos, o grafo deve ser **conexo** (ignorando vértices isolados).

```cpp
int check_eulerian(int n) {
    // Retorna 2 = Circuito, 1 = Caminho, 0 = Não existe
    // (ver implementação completa em euler.cpp)
}
```

> **Não confunda com Hamiltoniano:** Euleriano passa por todas as *arestas* uma vez; Hamiltoniano passa por todos os *vértices* uma vez. Verificar Euleriano é O(V + E); verificar Hamiltoniano é NP-completo.

---

## Caminho Hamiltoniano — DP com Bitmask

Um **caminho Hamiltoniano** visita cada **vértice** exatamente uma vez. Diferente do Euleriano, não existe critério simples de existência — o problema é NP-completo. Para N ≤ 20, a solução padrão é **DP com bitmask**:

`dp[mask][i]` = é possível visitar exatamente os vértices em `mask` terminando no vértice i?

```cpp
bool dp[1 << MAXN][MAXN];

bool has_hamiltonian_path(int n) {
    memset(dp, false, sizeof(dp));
    for (int i = 0; i < n; i++) dp[1 << i][i] = true;

    for (int mask = 1; mask < (1 << n); mask++)
        for (int u = 0; u < n; u++)
            if ((mask & (1 << u)) && dp[mask][u])
                for (int v = 0; v < n; v++)
                    if (adj_mat[u][v] && !(mask & (1 << v)))
                        dp[mask | (1 << v)][v] = true;

    int full = (1 << n) - 1;
    for (int i = 0; i < n; i++)
        if (dp[full][i]) return true;
    return false;
}
// Para Circuito Hamiltoniano: verificar também se existe aresta i -> vértice_inicial.
```

**Complexidade:** O(2^N · N²) em tempo e O(2^N · N) em memória. Para N = 20: aproximadamente 4 × 10⁸ operações e 20 MB.

> Este é também o algoritmo base para o **TSP (Caixeiro Viajante)**: substitua o `bool` por um custo e minimize ao invés de verificar existência.

---

## Dijkstra em Camadas (Layered Graph)

Técnica para problemas onde o estado não é apenas o vértice atual, mas inclui uma dimensão extra — por exemplo, "quantas vezes usei um poder especial". Modela-se isso como K cópias do grafo original empilhadas em **camadas**, onde transições normais ficam dentro da mesma camada e usar o poder move para a próxima.

O estado é a tupla `{distância, vértice, camada}` e o Dijkstra roda sobre esse espaço ampliado.

```cpp
int dist[MAXN][MAXK]; // dist[u][k] = menor distancia ate u usando k poderes

int dijkstra_camadas(int start, int end, int n, int max_k) {
    // Inicializa dist[i][j] = INF para todo i, j
    priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<>> pq;
    dist[start][0] = 0;
    pq.push({0, start, 0});

    while (!pq.empty()) {
        auto [d, u, k] = pq.top(); pq.pop();
        if (d > dist[u][k]) continue;

        for (auto [v, w] : adj[u]) {
            // Transição normal (mesma camada)
            if (dist[u][k] + w < dist[v][k]) {
                dist[v][k] = dist[u][k] + w;
                pq.push({dist[v][k], v, k});
            }
            // Usar o poder (próxima camada) — ex: zerar peso da aresta
            if (k < max_k && dist[u][k] < dist[v][k + 1]) {
                dist[v][k + 1] = dist[u][k];
                pq.push({dist[v][k + 1], v, k + 1});
            }
        }
    }

    int ans = INF;
    for (int k = 0; k <= max_k; k++) ans = min(ans, dist[end][k]);
    return (ans == INF) ? -1 : ans;
}
```

**Variações comuns do "poder":**
- Remover o peso de uma aresta (viajar de graça)
- Atravessar um obstáculo (paredes em grids)
- Usar um atalho com capacidade limitada

> Sempre que um problema de menor caminho disser "você pode fazer X no máximo K vezes", pense em Dijkstra em camadas. A complexidade é O(K · (V + E) log V), multiplicando a do Dijkstra normal por K.