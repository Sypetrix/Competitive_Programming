# Árvores (Trees)

Esta pasta cobre os algoritmos clássicos sobre árvores e grafos que se reduzem a problemas em árvores: construção da Árvore Geradora Mínima (MST) pelos algoritmos de Kruskal e Prim, consultas de Ancestral Comum Mais Baixo (LCA) com Binary Lifting, Heavy-Light Decomposition (HLD) para queries em caminhos, e Centroid Decomposition para problemas de distâncias.

---

## Visão Geral

| Algoritmo / Arquivo | Complexidade | Utilização Típica |
|---|---|---|
| `kruskal` | O(E log E) | MST em grafos esparsos; usa Union-Find |
| `prim` | O((V + E) log V) | MST em grafos densos; usa min-heap |
| `LCA` | O(N log N) prep + O(log N) query | Menor ancestral comum, distância entre nós |
| HLD | O(N log N) prep + O(log² N) query | Range queries em caminhos arbitrários da árvore |
| Centroid Decomp. | O(N log N) prep + O(log N) per path | Contagem/soma de caminhos com restrições |

---

## Árvore Geradora Mínima (MST)

Uma **Árvore Geradora Mínima** é um subconjunto de arestas que conecta todos os V vértices do grafo com custo total mínimo, sem formar ciclos. Toda MST possui exatamente V − 1 arestas.

Kruskal e Prim sempre produzem o mesmo custo total, mas diferem na abordagem.

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

> **Use Kruskal** para grafos esparsos (E ≈ V), quando as arestas já estiverem disponíveis em lista, ou quando precisar do Union-Find de qualquer forma (ex: para consultas de conectividade junto com a MST).

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

> **Use Prim** para grafos densos (E ≈ V²), onde Kruskal seria lento para ordenar tantas arestas. Prim também é preferido quando o grafo é dado como matriz de adjacência.

---

## LCA — Ancestral Comum Mais Baixo (Binary Lifting)

O **LCA** de dois nós u e v é o nó mais profundo da árvore que é ancestral de ambos. A técnica de **Binary Lifting** pré-computa, para cada nó u e cada potência de 2, qual é o 2ⁱ-ésimo ancestral de u. Com isso, qualquer LCA é resolvido em O(log N).

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

> **Use LCA quando** o problema perguntar sobre caminhos em árvores, ancestrais, ou qualquer consulta que envolva subir na árvore repetidamente.

---

## Heavy-Light Decomposition (HLD)

A HLD decompõe a árvore em **cadeias de nós** (chains) de forma que qualquer caminho entre dois nós atravessa no máximo O(log N) cadeias. Cada cadeia é mapeada para um segmento contíguo de um array, permitindo que uma **Segment Tree** ou **Fenwick Tree** responda queries de intervalo em caminhos arbitrários.

**Ideia central:** para cada nó, o filho com a maior subárvore é chamado de **filho pesado** (heavy child). A aresta até esse filho é uma aresta pesada; as demais são arestas leves. As cadeias são formadas concatenando arestas pesadas.

**Por que O(log N) cadeias?** Toda aresta leve atravessada pelo menos dobra o tamanho da subárvore. Logo, um caminho raiz-folha cruza no máximo log N arestas leves.

### Implementação

```cpp
const int MAXN = 1e5 + 5;
vector<int> adj[MAXN];

int sz[MAXN];    // tamanho da subárvore
int depth[MAXN]; // profundidade
int par[MAXN];   // pai
int heavy[MAXN]; // filho pesado (-1 se folha)
int head[MAXN];  // topo da cadeia do nó
int pos[MAXN];   // posição no array linearizado
int cur_pos;     // contador global de posições

// Passo 1: DFS para calcular sz e heavy
int dfs_sz(int v, int p, int d) {
    sz[v] = 1;
    depth[v] = d;
    par[v] = p;
    heavy[v] = -1;
    int max_sz = 0;
    for (int u : adj[v]) {
        if (u == p) continue;
        sz[v] += dfs_sz(u, v, d + 1);
        if (sz[u] > max_sz) {
            max_sz = sz[u];
            heavy[v] = u;
        }
    }
    return sz[v];
}

// Passo 2: DFS para atribuir heads e posições
void dfs_hld(int v, int h) {
    head[v] = h;
    pos[v] = ++cur_pos; // posição no array da Seg Tree
    if (heavy[v] != -1)
        dfs_hld(heavy[v], h);           // continua na mesma cadeia
    for (int u : adj[v]) {
        if (u == par[v] || u == heavy[v]) continue;
        dfs_hld(u, u);                  // nova cadeia começa em u
    }
}

// Inicialização:
// cur_pos = 0;
// dfs_sz(raiz, raiz, 0);
// dfs_hld(raiz, raiz);
```

### Query em Caminho — O(log² N)

```cpp
// Exemplo: soma dos valores nos nós do caminho u-v
// seg é uma Segment Tree sobre o array linearizado

long long query_path(int u, int v) {
    long long res = 0;
    while (head[u] != head[v]) {
        if (depth[head[u]] < depth[head[v]]) swap(u, v);
        // u está em cadeia mais profunda: query de head[u] até u
        res += seg.query(pos[head[u]], pos[u]);
        u = par[head[u]]; // sobe para o pai do topo da cadeia
    }
    // agora u e v estão na mesma cadeia
    if (depth[u] > depth[v]) swap(u, v);
    res += seg.query(pos[u], pos[v]);
    return res;
}

// Update de valor no nó v:
// seg.update(pos[v], novo_valor);
```

> O(log² N) vem de O(log N) cadeias, cada uma consultada em O(log N) pela Segment Tree. Usando uma Fenwick Tree ou Sparse Table pode-se chegar a O(log N) ou O(1) por cadeia dependendo da operação.

**Operações suportadas pela HLD:**
- Soma/mínimo/máximo de valores nos nós de um caminho
- Update pontual ou em intervalo de cadeia
- Qualquer operação que uma Segment Tree suporte, mas agora em caminhos da árvore

> **Use HLD quando** o problema fizer queries do tipo "valor acumulado no caminho entre u e v" com updates, o que não é possível com LCA puro.

---

## Centroid Decomposition (Decomposição por Centroide)

A Centroid Decomposition divide recursivamente a árvore pelo seu **centroide** — o nó cuja remoção produz componentes de tamanho no máximo N/2. Isso garante que a árvore de centróides tem altura O(log N), o que torna eficiente responder perguntas sobre pares de nós.

**Ideia central:** todo caminho na árvore ou passa pelo centroide da componente atual, ou está inteiramente em uma das subcomponentes. Processar os caminhos que passam pelo centroide e depois decompor recursivamente cada subcomponente garante que cada par de nós é processado exatamente uma vez, no nível de seu LCA na árvore de centróides.

### Implementação

```cpp
const int MAXN = 1e5 + 5;
vector<int> adj[MAXN];

int sz[MAXN];
bool removed[MAXN]; // centróides já removidos

// Calcula tamanho da componente (ignorando removidos)
int get_sz(int v, int p) {
    sz[v] = 1;
    for (int u : adj[v])
        if (u != p && !removed[u]) sz[v] += get_sz(u, v);
    return sz[v];
}

// Encontra o centroide da componente com 'total' nós
int get_centroid(int v, int p, int total) {
    for (int u : adj[v])
        if (u != p && !removed[u] && sz[u] > total / 2)
            return get_centroid(u, v, total);
    return v;
}

// Constrói a árvore de centróides e processa os caminhos
// 'par_centroid' guarda o pai na árvore de centróides
int par_centroid[MAXN];

void build(int v, int p) {
    int total = get_sz(v, -1);
    int c = get_centroid(v, -1, total);

    par_centroid[c] = p;
    removed[c] = true; // "remove" o centróide

    // Processar todos os caminhos que passam por c:
    // coletar distâncias de c para todos os nós da componente,
    // depois combinar pares (ver template abaixo)

    for (int u : adj[c])
        if (!removed[u]) build(u, c); // decompõe recursivamente

    // removed[c] permanece true — centroide não volta ao grafo
}
```

### Template de Contagem de Pares — O(N log² N)

```cpp
// Exemplo: contar pares (u, v) com distância exatamente K
int K;
map<int, int> freq; // frequência de distâncias vistas

void dfs_dist(int v, int p, int dist, bool add) {
    if (add) freq[dist]++;
    else /* remove */ freq[dist]--;
    for (int u : adj[v])
        if (u != p && !removed[u])
            dfs_dist(u, v, dist + 1, add);
}

long long ans = 0;

void solve(int v) {
    int total = get_sz(v, -1);
    int c = get_centroid(v, -1, total);
    removed[c] = true;

    freq[0] = 1; // distância 0: o próprio centroide
    for (int u : adj[c]) {
        if (removed[u]) continue;
        // Subtrair contribuições inválidas (que entram e saem pelo mesmo galho)
        dfs_dist(u, c, 1, false); // remove temporariamente
        // ans += freq[K - 1] seria adicionado antes — ver solução completa
        dfs_dist(u, c, 1, true);  // adiciona de volta
    }
    freq.clear();

    for (int u : adj[c])
        if (!removed[u]) solve(u);
}
```

> A Centroid Decomposition é mais simples de implementar do que parece — a dificuldade está em modelar corretamente o que processar em cada centroide. O padrão é: **coletar → combinar → limpar → recursão**.

**Casos de uso clássicos:**
- Contar pares de nós com distância exatamente K
- Contar caminhos com soma de pesos igual a um alvo
- Encontrar o caminho mais longo com restrições (valor ≤ X, número de nós de cor C, etc.)
- Qualquer problema de "quantos pares (u, v) satisfazem P(caminho u→v)?"

> **Use Centroid Decomposition quando** o problema fizer queries sobre todos (ou muitos) pares de caminhos na árvore, especialmente quando o LCA não é suficiente por precisar combinar informações de dois lados do caminho.