# Travessia em Grids (Grids and Matrices)

Em muitos problemas, mapas, labirintos e tabuleiros são representados por matrizes 2D. Em vez de construir um grafo explícito, tratamos o grid como um **grafo implícito**: cada célula é um vértice e o deslocamento para células vizinhas representa as arestas.

---

## Visão Geral

| Técnica | Estrutura | Complexidade | Uso típico |
|---|---|---|---|
| DFS (Flood Fill) | Recursão / Stack | O(L × C) | Contar área de regiões, marcar componentes |
| BFS (Menor Caminho) | `std::queue` | O(L × C) | Distância mínima sem pesos |
| BFS 0-1 | `std::deque` | O(L × C) | Arestas com peso 0 ou 1 |
| Dijkstra em grid | `priority_queue` | O(LC · log(LC)) | Arestas com pesos variados |

*(L = linhas, C = colunas)*

---

## Vetores de Deslocamento

Técnica essencial: evita dezenas de `if/else` para cada direção.

```cpp
int dr4[] = {-1, 1, 0, 0};
int dc4[] = {0, 0, -1, 1};

// 8 direções (inclui diagonais)
int dr8[] = {-1, 1, 0, 0, -1, -1, 1, 1};
int dc8[] = {0, 0, -1, 1, -1, 1, -1, 1};

bool isValid(int r, int c, int R, int C) {
    return r >= 0 && r < R && c >= 0 && c < C;
}
```

---

## DFS — Flood Fill — O(L × C)

Explora exaustivamente uma região conectada, como o balde de tinta do MS Paint. Ideal para contar componentes, medir área de ilhas ou marcar regiões.

```cpp
int dfs_flood(const vector<vector<int>>& grid,
              vector<vector<bool>>& vis, int r, int c) {
    int R = grid.size(), C = grid[0].size();
    vis[r][c] = true;
    int area = 1;

    for (int i = 0; i < 4; i++) {
        int nr = r + dr4[i], nc = c + dc4[i];
        if (isValid(nr, nc, R, C) && !vis[nr][nc] && grid[nr][nc] == 1)
            area += dfs_flood(grid, vis, nr, nc);
    }
    return area;
}
```

> **Atenção:** DFS recursiva em grids grandes (≥ 1000×1000) pode causar Stack Overflow. Nesses casos, use DFS iterativa com `std::stack` ou prefira BFS.

**DFS iterativa (segura para grids grandes):**

```cpp
int dfs_iterativa(const vector<vector<int>>& grid,
                  vector<vector<bool>>& vis, int sr, int sc) {
    int R = grid.size(), C = grid[0].size();
    stack<pair<int,int>> st;
    st.push({sr, sc});
    vis[sr][sc] = true;
    int area = 0;

    while (!st.empty()) {
        auto [r, c] = st.top(); st.pop();
        area++;
        for (int i = 0; i < 4; i++) {
            int nr = r + dr4[i], nc = c + dc4[i];
            if (isValid(nr, nc, R, C) && !vis[nr][nc] && grid[nr][nc] == 1) {
                vis[nr][nc] = true;
                st.push({nr, nc});
            }
        }
    }
    return area;
}
```

---

## BFS — Menor Caminho — O(L × C)

A BFS explora o grid em "camadas" radiais. A primeira vez que uma célula é alcançada, foi pelo caminho mais curto (em número de passos).

```cpp
int bfs(const vector<vector<int>>& grid, int sr, int sc, int tr, int tc) {
    int R = grid.size(), C = grid[0].size();
    vector<vector<int>> dist(R, vector<int>(C, -1));
    queue<pair<int,int>> q;

    q.push({sr, sc});
    dist[sr][sc] = 0;

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();

        if (r == tr && c == tc) return dist[r][c];

        for (int i = 0; i < 4; i++) {
            int nr = r + dr4[i], nc = c + dc4[i];
            if (isValid(nr, nc, R, C) && grid[nr][nc] == 1 && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }
    return -1; // impossível
}
```

---

## BFS Multi-Fonte

Quando há múltiplos pontos de partida (ex: "distância ao obstáculo mais próximo"), inicialize a fila com **todas** as origens antes de começar.

```cpp
// dist[r][c] = distância da célula (r,c) ao obstáculo mais próximo
void bfs_multi_fonte(const vector<vector<int>>& grid,
                     vector<vector<int>>& dist) {
    int R = grid.size(), C = grid[0].size();
    queue<pair<int,int>> q;
    dist.assign(R, vector<int>(C, -1));

    // todas as fontes entram na fila com dist 0
    for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++)
            if (grid[r][c] == 0) { dist[r][c] = 0; q.push({r, c}); }

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        for (int i = 0; i < 4; i++) {
            int nr = r + dr4[i], nc = c + dc4[i];
            if (isValid(nr, nc, R, C) && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }
}
```

---

## BFS 0-1 (Deque BFS)

Quando as arestas têm peso 0 ou 1, a BFS 0-1 com `deque` resolve em O(L × C) sem o overhead do heap do Dijkstra: arestas de peso 0 vão para a **frente** da deque, arestas de peso 1 vão para o **fundo**.

```cpp
// custo(r, c, nr, nc) retorna 0 ou 1
vector<vector<int>> bfs_01(const vector<vector<int>>& grid, int sr, int sc) {
    int R = grid.size(), C = grid[0].size();
    vector<vector<int>> dist(R, vector<int>(C, INT_MAX));
    deque<pair<int,int>> dq;

    dist[sr][sc] = 0;
    dq.push_back({sr, sc});

    while (!dq.empty()) {
        auto [r, c] = dq.front(); dq.pop_front();

        for (int i = 0; i < 4; i++) {
            int nr = r + dr4[i], nc = c + dc4[i];
            if (!isValid(nr, nc, R, C)) continue;

            int w = /* custo da aresta: 0 ou 1 */ 0;
            if (dist[r][c] + w < dist[nr][nc]) {
                dist[nr][nc] = dist[r][c] + w;
                if (w == 0) dq.push_front({nr, nc});
                else        dq.push_back({nr, nc});
            }
        }
    }
    return dist;
}
```

---

## Quando Usar Cada Técnica

```
Preciso explorar uma região / contar componentes?
└── DFS (Flood Fill)
    └── Grid > 1000×1000? → DFS iterativa ou BFS

Preciso do menor número de passos entre dois pontos?
├── Todas as arestas valem 1 → BFS
├── Arestas valem 0 ou 1 → BFS 0-1 com deque
└── Arestas com pesos variados → Dijkstra

Múltiplos pontos de origem simultaneamente?
└── BFS Multi-Fonte (inicializa com todas as fontes)
```

> **Dica de performance:** para BFS/DFS chamadas milhares de vezes, declare a matriz de distâncias/visitados globalmente e use um contador de versão em vez de `fill` a cada chamada.