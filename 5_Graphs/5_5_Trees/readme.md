# Árvores (Trees)

Esta pasta cobre os algoritmos clássicos sobre árvores e grafos que se reduzem a problemas em árvores: construção da Árvore Geradora Mínima (MST) pelos algoritmos de Kruskal e Prim, e consultas de Ancestral Comum Mais Baixo (LCA) com Binary Lifting.

---

## Visão Geral

| Algoritmo / Arquivo | Complexidade | Utilização Típica |
|---|---|---|
| `kruskal` | O(E log E) | MST em grafos esparsos; usa Union-Find. |
| `prim` | O((V + E) log V) | MST em grafos densos; usa min-heap. |
| `LCA` | O(N log N) prep + O(log N) query | Menor ancestral comum, distância entre nós. |

---

## Árvore Geradora Mínima (MST)

Uma **Árvore Geradora Mínima** é um subconjunto de arestas que conecta todos os $V$ vértices do grafo com custo total mínimo, sem formar ciclos. Toda MST possui exatamente $V - 1$ arestas.

Kruskal e Prim sempre produzem o mesmo custo total, mas diferem na abordagem. A escolha depende da densidade do grafo.

---

## Kruskal

Ordena todas as arestas por peso e as adiciona à MST uma a uma, pulando as que formariam ciclo. A detecção de ciclo é feita em tempo quase constante pelo **Union-Find**.

```cpp
struct Edge {
    int u, v, weight;
    bool operator<(const Edge& o) { return weight < o.weight; }
};
vector<Edge> edges;

long long kruskal(int n) {
    sort(edges.begin(), edges.end());
    for (int i = 1; i <= n; i++) make_set(i);

    long long cost = 0;
    int used = 0;
    for (Edge& e : edges) {
        if (find_set(e.u) != find_set(e.v)) {
            union_sets(e.u, e.v);
            cost += e.weight;
            if (++used == n - 1) break;
        }
    }
    return (used == n - 1) ? cost : -1; // -1 = grafo desconexo
}
```

> **Use Kruskal** para grafos esparsos ($E \approx V$), quando as arestas já estiverem disponíveis em lista, ou quando precisar do Union-Find de qualquer forma (ex: para consultas de conectividade junto com a MST).

---

## Prim

Cresce a MST a partir de um vértice inicial, sempre adicionando a aresta de menor peso que conecta um vértice já na MST a um vértice ainda fora dela. Usa uma **min-heap** para encontrar essa aresta eficientemente.

```cpp
vector<pair<int,int>> adj[MAXN]; // {vizinho, peso}
bool in_mst[MAXN];

long long prim(int start, int n) {
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    pq.push({0, start});
    long long cost = 0;
    int used = 0;

    while (!pq.empty() && used < n) {
        auto [w, u] = pq.top(); pq.pop();
        if (in_mst[u]) continue;
        in_mst[u] = true;
        cost += w;
        used++;
        for (auto [v, peso] : adj[u])
            if (!in_mst[v]) pq.push({peso, v});
    }
    return (used == n) ? cost : -1;
}
```

> **Use Prim** para grafos densos ($E \approx V^2$), onde Kruskal seria lento para ordenar tantas arestas. Prim também é preferido quando o grafo é dado como matriz de adjacência.

---

## LCA — Ancestral Comum Mais Baixo (Binary Lifting)

O **LCA** de dois nós $u$ e $v$ é o nó mais profundo da árvore que é ancestral de ambos. A técnica de **Binary Lifting** pré-computa, para cada nó $u$ e cada potência de 2, qual é o $2^i$-ésimo ancestral de $u$. Com isso, qualquer LCA é resolvido em $O(\log N)$.

### Pré-computação — O(N log N)

```cpp
const int LOG = 20; // 2^20 > 10^5
int up[MAXN][LOG];  // up[u][i] = 2^i-esimo ancestral de u
int tin[MAXN], tout[MAXN], timer_lca;

void dfs(int v, int p) {
    tin[v] = ++timer_lca;
    up[v][0] = p;
    for (int i = 1; i < LOG; i++)
        up[v][i] = up[up[v][i-1]][i-1];
    for (int u : adj[v])
        if (u != p) dfs(u, v);
    tout[v] = ++timer_lca;
}
// Inicializar: timer_lca = 0; dfs(raiz, raiz);
```

### Consulta — O(log N)

```cpp
bool is_ancestor(int u, int v) {
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int i = LOG - 1; i >= 0; i--)
        if (!is_ancestor(up[u][i], v))
            u = up[u][i];
    return up[u][0];
}
```

**Distância entre dois nós** em uma árvore com pesos: pré-compute `dist[]` durante a DFS e use `dist[u] + dist[v] - 2 * dist[lca(u, v)]`.

> **Use LCA quando** o problema perguntar sobre caminhos em árvores, ancestrais, ou qualquer consulta que envolva subir na árvore repetidamente. O Binary Lifting também serve como base para consultas de mínimo/máximo em caminhos (substituindo `up` por uma tabela de sparse table sobre a árvore).