# Fundamentos de Grafos (Graph Basics)

Esta pasta cobre as operações mais fundamentais em grafos: as duas travessias principais (DFS e BFS), detecção de ciclos, identificação de componentes conexas, ordenação topológica e **pontes/articulações**. Dominar esses algoritmos é o pré-requisito para tudo que vem depois.

---

## Visão Geral

| Algoritmo / Arquivo | Estrutura Base | Complexidade | Utilização Típica |
|---|---|---|---|
| `graph_DFS` | Recursão (Call Stack) | O(V + E) | Travessia, detecção de ciclos, SCCs |
| `graph_BFS` | `std::queue` | O(V + E) | Menor caminho (sem pesos), nível de cada nó |
| `components` | DFS | O(V + E) | Contar e identificar componentes conexas |
| `cycle` | DFS com 3 cores | O(V + E) | Detectar ciclos em grafos direcionados |
| `topologic_kahn` | BFS (Kahn's) | O(V + E) | Ordenação topológica, detecção de ciclos |
| `topologic_dfs` | DFS | O(V + E) | Ordenação topológica via pilha |
| Pontes e Articulações | DFS | O(V + E) | Arestas e vértices críticos à conectividade |

*(Onde V = número de vértices e E = número de arestas)*

---

## Representação do Grafo

Antes de qualquer travessia, é necessário representar o grafo. A forma mais comum em maratonas é a **Lista de Adjacência**, por ser eficiente em memória para grafos esparsos.

```cpp
const int MAXN = 1e5 + 5;
vector<int> adj[MAXN]; // Lista de adjacência simples

// Para grafos ponderados: vector<pair<int,int>> adj[MAXN];
//                         onde pair = {vizinho, peso}

// Lendo arestas não-direcionadas:
adj[u].push_back(v);
adj[v].push_back(u);

// Para grafos direcionados, omita a segunda linha.
```

> A **Matriz de Adjacência** (`int mat[MAXN][MAXN]`) é uma alternativa viável apenas para grafos densos ou quando V ≤ 1000. Para V = 10⁵, ela usa O(V²) de memória e causa MLE.

---

## DFS (Busca em Profundidade)

A DFS explora o grafo mergulhando o mais fundo possível por cada ramo antes de voltar. É a base de praticamente todos os algoritmos avançados em grafos (SCCs, pontes, articulações, LCA).

```cpp
vector<int> adj[MAXN];
bool vis[MAXN];

void dfs(int u) {
    vis[u] = true;
    for (int v : adj[u]) {
        if (!vis[v]) dfs(v);
    }
}
```

> **Use DFS quando** a ordem de visita não importar ou quando precisar explorar toda uma componente. Para grafos muito grandes (V > 10⁵), a DFS recursiva pode causar *Stack Overflow* — nesse caso, implemente com uma `std::stack` explícita.

---

## BFS (Busca em Largura)

A BFS explora o grafo em "camadas" radiais a partir da origem. Isso garante que a primeira vez que um nó é visitado, ele foi alcançado pelo caminho mais curto (em número de arestas).

```cpp
vector<int> adj[MAXN];
bool vis[MAXN];
int dist[MAXN];

void bfs(int start) {
    queue<int> q;
    q.push(start);
    vis[start] = true;
    dist[start] = 0;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (!vis[v]) {
                vis[v] = true;
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}
```

> **Use BFS quando** o problema perguntar pela menor distância (em número de arestas) entre dois nós. Para arestas com pesos, use Dijkstra.

---

## Componentes Conexas

Conta e rotula as componentes conexas de um grafo não-direcionado. Cada chamada de DFS a partir de um nó não visitado descobre uma nova componente inteira.

```cpp
int comp[MAXN]; // comp[i] = ID da componente do vértice i

void dfs_comp(int u, int c) {
    comp[u] = c;
    for (int v : adj[u])
        if (!comp[v]) dfs_comp(v, c);
}

int find_components(int n) {
    int num_comps = 0;
    for (int i = 1; i <= n; i++)
        if (!comp[i]) dfs_comp(i, ++num_comps);
    return num_comps;
}
```

> **Use quando** precisar responder "X e Y estão conectados?" ou "quantas ilhas existem no grafo?". Para consultas dinâmicas de conectividade (com inserção de arestas durante o processo), prefira o **Union-Find**.

---

## Detecção de Ciclos (Grafos Direcionados)

Usa o esquema de **3 cores** para distinguir nós ainda não visitados, nós em processo de visita (na pilha de recursão atual) e nós já completamente processados.

```cpp
int color[MAXN]; // 0 = não visitado, 1 = visitando, 2 = processado

bool has_cycle(int u) {
    color[u] = 1;
    for (int v : adj[u]) {
        if (color[v] == 1) return true;  // Aresta de retorno = ciclo
        if (color[v] == 0)
            if (has_cycle(v)) return true;
    }
    color[u] = 2;
    return false;
}
```

Um nó com `color == 1` significa que ele está na pilha de chamadas atual — encontrar uma aresta para ele é uma **aresta de retorno** e confirma a existência de um ciclo.

> **Para grafos não-direcionados**, a detecção é mais simples: na DFS, se um vizinho já foi visitado e não é o pai do nó atual, há um ciclo.

---

## Ordenação Topológica (Algoritmo de Kahn)

Ordena os vértices de um **grafo direcionado acíclico (DAG)** de forma que toda aresta u → v satisfaça u antes de v na ordenação. Se o grafo contiver um ciclo, a ordenação é impossível e o algoritmo detecta isso.

```cpp
int in_degree[MAXN];
vector<int> topo_order;

bool topological_sort(int n) {
    queue<int> q;
    for (int i = 1; i <= n; i++)
        if (in_degree[i] == 0) q.push(i);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        topo_order.push_back(u);
        for (int v : adj[u]) {
            if (--in_degree[v] == 0) q.push(v);
        }
    }
    return (int)topo_order.size() == n; // false = ciclo detectado
}

// Ao ler arestas direcionadas u -> v:
// adj[u].push_back(v);
// in_degree[v]++;
```

**Casos de uso clássicos:** escalonamento de tarefas com dependências, compilação com dependências entre módulos, verificar se um grafo direcionado é um DAG.

> A ordenação topológica também pode ser obtida via DFS: basta inserir cada vértice em uma pilha ao terminar de processá-lo e depois inverter a ordem. O algoritmo de Kahn é geralmente preferido em maratonas por ser iterativo e detectar ciclos mais naturalmente.

---

## Pontes e Articulações

**Pontes** são arestas cuja remoção desconecta o grafo. **Pontos de articulação** (vértices de corte) são vértices cuja remoção desconecta o grafo. Ambos são encontrados em uma única DFS usando os arrays `tin` (tempo de entrada) e `low` (menor tempo alcançável sem subir pela aresta de árvore que chegou ao nó).

```cpp
int tin[MAXN], low[MAXN], timer_val;
bool vis[MAXN];

vector<pair<int,int>> bridges;  // lista de pontes
vector<int> articulations;      // lista de pontos de articulação
```

### Pontes — O(V + E)

```cpp
void dfs_bridge(int u, int parent) {
    vis[u] = true;
    tin[u] = low[u] = ++timer_val;

    for (int v : adj[u]) {
        if (v == parent) continue; // ignora aresta de volta ao pai
        if (vis[v]) {
            low[u] = min(low[u], tin[v]);
        } else {
            dfs_bridge(v, u);
            low[u] = min(low[u], low[v]);

            // Se low[v] > tin[u], a aresta u-v é uma ponte
            if (low[v] > tin[u])
                bridges.push_back({u, v});
        }
    }
}
```

> **Atenção com arestas múltiplas:** se o grafo puder ter arestas paralelas, passe o índice da aresta usada em vez do nó pai, e ignore apenas a aresta específica (não todas as arestas para o pai).

### Pontos de Articulação — O(V + E)

```cpp
void dfs_articulation(int u, int parent) {
    vis[u] = true;
    tin[u] = low[u] = ++timer_val;
    int children = 0; // filhos na árvore DFS

    for (int v : adj[u]) {
        if (v == parent) continue;
        if (vis[v]) {
            low[u] = min(low[u], tin[v]);
        } else {
            children++;
            dfs_articulation(v, u);
            low[u] = min(low[u], low[v]);

            // u é articulação se:
            // 1) u é raiz da DFS com 2+ filhos
            // 2) u não é raiz e low[v] >= tin[u]
            if (parent == -1 && children > 1)
                articulations.push_back(u);
            if (parent != -1 && low[v] >= tin[u])
                articulations.push_back(u);
        }
    }
}
```

**Como chamar:**
```cpp
// Resetar antes de cada caso de teste
timer_val = 0;
fill(vis, vis + n + 1, false);
bridges.clear();
articulations.clear();

for (int i = 1; i <= n; i++)
    if (!vis[i]) {
        dfs_bridge(i, -1);          // para pontes
        dfs_articulation(i, -1);    // para articulações
    }

// Remover duplicatas de articulações (um nó pode ser adicionado múltiplas vezes)
sort(articulations.begin(), articulations.end());
articulations.erase(unique(articulations.begin(), articulations.end()), articulations.end());
```

### Componentes Biconexas (2-Edge / 2-Vertex Connected Components)

Após encontrar as pontes/articulações, é possível condensar o grafo:

- **Componentes 2-edge-connected:** grupos de vértices que permanecem conectados mesmo removendo qualquer aresta — são os nós do grafo condensado, ligados pelas pontes.
- **Componentes biconexas (2-vertex-connected):** grupos de vértices sem ponto de articulação interno — úteis para problemas de robustez de rede.

> **Use pontes/articulações quando** o problema envolve: "qual servidor desligar/aresta remover desconecta a rede?", "encontrar partes críticas da infraestrutura", ou "condensar o grafo em componentes 2-conectadas para aplicar outra técnica".