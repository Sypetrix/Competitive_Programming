# Caminhos Mais Curtos (Shortest Paths)

Encontrar o caminho de menor custo entre vértices é um dos problemas mais recorrentes em competições. A escolha do algoritmo depende de três fatores: o grafo tem pesos negativos? A consulta é de uma única origem ou de todos os pares? O grafo é esparso ou denso?

---

## Visão Geral

| Algoritmo / Arquivo | Complexidade | Pesos Negativos | Consultas |
|---|---|---|---|
| `dijkstra` | O((V + E) log V) | Não | Fonte única |
| `bellman_ford` | O(V * E) | Sim | Fonte única |
| `floyd_warshall` | O(V³) | Sim | Todos os pares |

---

## Como Escolher

```
Tem ciclo negativo?
├── Sim → Bellman-Ford (detecta o ciclo)
└── Não
    ├── Consulta de todos os pares (V ≤ 500)?
    │   └── Floyd-Warshall
    └── Consulta de fonte única?
        ├── Tem arestas negativas? → Bellman-Ford
        └── Todos os pesos ≥ 0?   → Dijkstra
```

---

## Dijkstra

O algoritmo mais usado em competições para grafos com pesos não-negativos. Processa os vértices em ordem crescente de distância usando uma **fila de prioridade (min-heap)**, garantindo que a primeira vez que um vértice é extraído da fila, sua distância já é definitiva.

```cpp
vector<pair<int,int>> adj[MAXN]; // {vizinho, peso}
int dist[MAXN];

void dijkstra(int start) {
    fill(dist, dist + MAXN, INF);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue; // Entrada desatualizada na fila

        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}
```

**Detalhe importante:** a fila de prioridade pode conter entradas duplicadas para o mesmo vértice. A linha `if (d > dist[u]) continue` descarta entradas desatualizadas em O(1) e é essencial para a corretude.

> **Não use Dijkstra** com arestas de peso negativo — o algoritmo pode dar respostas erradas. Para esses casos, use Bellman-Ford.

---

## Bellman-Ford

Relaxa todas as arestas $V - 1$ vezes. Funciona com pesos negativos e, como bônus, detecta ciclos negativos (se ainda for possível relaxar na $V$-ésima iteração, existe um ciclo de custo negativo).

```cpp
struct Edge { int u, v, weight; };
vector<Edge> edges;
int dist[MAXN];

bool bellman_ford(int n, int start) {
    fill(dist, dist + n + 1, INF);
    dist[start] = 0;

    // Relaxa V-1 vezes
    for (int i = 1; i <= n - 1; i++)
        for (auto& e : edges)
            if (dist[e.u] < INF && dist[e.u] + e.weight < dist[e.v])
                dist[e.v] = dist[e.u] + e.weight;

    // Verifica ciclo negativo
    for (auto& e : edges)
        if (dist[e.u] < INF && dist[e.u] + e.weight < dist[e.v])
            return false; // Ciclo negativo detectado

    return true;
}
```

**Por que $V - 1$ iterações?** O caminho mais curto sem ciclos tem no máximo $V - 1$ arestas. Cada iteração garante que caminhos de até $i$ arestas estão corretos.

> Bellman-Ford é mais lento que Dijkstra (O(V·E) vs O((V+E) log V)), então use-o **apenas quando houver pesos negativos** ou quando precisar detectar ciclos negativos.

---

## Floyd-Warshall

Calcula a menor distância entre **todos os pares** de vértices com três loops aninhados. Aceita pesos negativos, mas não funciona corretamente na presença de **ciclos negativos** (que tornam o problema mal definido para pares de vértices afetados).

```cpp
const int INF = 1e9;
int dist[MAXN][MAXN];

void init(int n) {
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            dist[i][j] = (i == j) ? 0 : INF;
}

void floyd_warshall(int n) {
    // k = vértice intermediário — a ordem dos loops é obrigatória
    for (int k = 1; k <= n; k++)
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                if (dist[i][k] < INF && dist[k][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
}

// Após rodar: dist[i][j] contém a menor distância de i até j.
// Se dist[i][j] == INF, i e j não estão conectados.
```

**Detecção de ciclo negativo:** após rodar o algoritmo, se `dist[i][i] < 0` para algum $i$, existe um ciclo negativo passando por $i$.

**Reconstrução do caminho:** para rastrear o caminho real (não só o custo), mantenha uma matriz `next[i][j]` inicializada com $j$ e atualizada para `next[i][k]` sempre que o caminho via $k$ for melhor.

> **Use Floyd-Warshall quando** $V \leq 500$ e você precisar de distâncias entre todos os pares, ou quando o grafo for pequeno e denso e a simplicidade do código importar mais que a performance.

---

## Comparativo Prático

| Situação | Algoritmo |
|---|---|
| Grafo com pesos não-negativos, fonte única | Dijkstra |
| Grafo com pesos negativos, fonte única | Bellman-Ford |
| Todos os pares, $V \leq 500$ | Floyd-Warshall |
| Grid sem pesos | BFS (O(V)) |
| DAG com pesos | Relaxamento em ordem topológica (O(V + E)) |
| Verificar se existe ciclo negativo | Bellman-Ford ou Floyd-Warshall |