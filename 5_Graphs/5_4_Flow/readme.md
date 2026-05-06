# Fluxo em Redes (Network Flow)

Problemas de fluxo modelam situações onde um recurso é enviado de uma **fonte** (source, $s$) a um **sorvedouro** (sink, $t$) através de uma rede de capacidades. Esta pasta cobre os algoritmos de fluxo máximo e suas extensões para custo mínimo e alocação ótima.

---

## Visão Geral

| Algoritmo / Arquivo | Complexidade | Utilização Típica |
|---|---|---|
| `ford_fulkerson` | O(E * F) | Fluxo máximo; base conceitual dos demais. |
| `edmond_karp` | O(V * E²) | Fluxo máximo; Ford-Fulkerson com BFS. Use no lugar do anterior. |
| `min_max` (MCMF) | O(F * V * E) | Fluxo máximo com custo mínimo (Min-Cost Max-Flow). |
| `hungaro` | O(N³) | Alocação ótima entre N trabalhadores e M tarefas. |

---

## Conceitos Fundamentais

**Grafo residual:** para cada aresta $u \to v$ com capacidade $c$ e fluxo $f$, existem duas arestas no grafo residual: $u \to v$ com capacidade restante $c - f$ (quanto ainda pode passar) e $v \to u$ com capacidade $f$ (quanto pode ser "devolvido"). A aresta reversa é o mecanismo que permite corrigir decisões ruins ao longo do algoritmo.

**Caminho aumentante:** qualquer caminho de $s$ a $t$ no grafo residual com capacidade positiva em todas as arestas. Enviar fluxo por ele aumenta o fluxo total.

**Teorema Max-Flow Min-Cut:** o fluxo máximo de $s$ a $t$ é igual à capacidade do corte mínimo (o menor conjunto de arestas cuja remoção desconecta $s$ de $t$).

---

## Ford-Fulkerson

Encontra repetidamente caminhos aumentantes via **DFS** e envia fluxo por eles até não existir mais nenhum.

```cpp
int cap[MAXN][MAXN]; // Capacidades residuais
bool vis[MAXN];

int dfs(int u, int t, int flow) {
    if (u == t) return flow;
    vis[u] = true;
    for (int v : adj[u]) {
        if (!vis[v] && cap[u][v] > 0) {
            int pushed = dfs(v, t, min(flow, cap[u][v]));
            if (pushed > 0) {
                cap[u][v] -= pushed;
                cap[v][u] += pushed; // Aresta reversa
                return pushed;
            }
        }
    }
    return 0;
}

int ford_fulkerson(int s, int t) {
    int total = 0, pushed;
    while ((memset(vis, 0, sizeof(vis)), pushed = dfs(s, t, INF)))
        total += pushed;
    return total;
}
```

> Ford-Fulkerson com DFS pode ser lento se o fluxo máximo $F$ for grande (O(E * F)). **Prefira Edmonds-Karp** em competições.

---

## Edmonds-Karp

É o Ford-Fulkerson com **BFS** no lugar de DFS. Isso garante que sempre se usa o caminho aumentante mais curto (em número de arestas), o que limita a complexidade a O(V * E²), independente do valor do fluxo.

```cpp
int cap[MAXN][MAXN];
int parent[MAXN];

int bfs_flow(int s, int t) {
    fill(parent, parent + MAXN, -1);
    parent[s] = s;
    queue<pair<int,int>> q;
    q.push({s, INF});
    while (!q.empty()) {
        auto [u, flow] = q.front(); q.pop();
        for (int v : adj[u]) {
            if (parent[v] == -1 && cap[u][v] > 0) {
                parent[v] = u;
                int new_flow = min(flow, cap[u][v]);
                if (v == t) return new_flow;
                q.push({v, new_flow});
            }
        }
    }
    return 0;
}

int max_flow(int s, int t) {
    int flow = 0, f;
    while ((f = bfs_flow(s, t))) {
        flow += f;
        for (int v = t; v != s; v = parent[v]) {
            cap[parent[v]][v] -= f;
            cap[v][parent[v]] += f;
        }
    }
    return flow;
}
```

**Como adicionar arestas:**

```cpp
void add_edge(int u, int v, int capacity) {
    adj[u].push_back(v);
    adj[v].push_back(u); // Necessário para a aresta reversa
    cap[u][v] += capacity;
    // cap[v][u] começa em 0 para arestas direcionadas
}
```

---

## Min-Cost Max-Flow (MCMF)

Estende o fluxo máximo adicionando **custo por unidade** em cada aresta. O objetivo é enviar o fluxo máximo possível com o menor custo total. O algoritmo usa **SPFA** (Bellman-Ford em fila) para encontrar o caminho mais barato no grafo residual, que pode conter arestas de custo negativo (arestas reversas).

```cpp
struct Edge { int to, cap, cost, flow, rev; };
vector<Edge> graph[MAXN];

void add_edge(int from, int to, int cap, int cost) {
    graph[from].push_back({to, cap, cost, 0, (int)graph[to].size()});
    graph[to].push_back({from, 0, -cost, 0, (int)graph[from].size() - 1});
}

pair<int,int> mcmf(int s, int t, int n);
// Retorna {fluxo_total, custo_total}
// (ver implementação completa em min_max.cpp)
```

**Aplicações típicas:**
- Alocação de tarefas com custos diferentes por trabalhador
- Transporte de mercadorias com custo por rota
- Qualquer problema de matching com peso que não caiba no Húngaro

---

## Algoritmo Húngaro

Resolve o **Problema de Alocação**: dados $N$ trabalhadores e $M$ tarefas ($N \leq M$) com uma matriz de custos, encontra a alocação de custo mínimo em O(N³).

É equivalente ao MCMF para grafos bipartidos completos, mas muito mais simples de implementar para esse caso específico.

```cpp
// cost_matrix[i][j] = custo de alocar o trabalhador i na tarefa j
// Indexação BASE 1 (linhas e colunas de 1 a N)
// Para MAXIMIZAR: inverta o sinal de todos os custos antes de chamar.

long long hungarian_min_cost(
    const vector<vector<long long>>& cost_matrix, int n, int m);
// Retorna o custo mínimo total da alocação
// (ver implementação completa em hungaro.cpp)
```

> **Use o Húngaro quando** o problema for de alocação 1-para-1 entre dois grupos de tamanho $N$ (ex: N times para N torneios, N robôs para N posições). Para emparelhamentos sem custo, o Kuhn é mais simples.

---

## Modelagens Clássicas com Fluxo

Muitos problemas não parecem ser de fluxo à primeira vista, mas se reduzem a ele com a modelagem certa:

| Problema | Modelagem |
|---|---|
| Emparelhamento bipartido máximo | Fluxo máximo com capacidades unitárias |
| Cobertura mínima de caminhos em DAG | $V$ - Emparelhamento máximo (König) |
| Conectividade de vértices | Dividir cada vértice em dois com capacidade 1 |
| Circulação com demandas | Adicionar super-fonte e super-sorvedouro |