# Travessia em Grids (Grid Traversal)

Em muitos problemas, mapas, labirintos e tabuleiros são representados por matrizes 2D. Em vez de construirmos um grafo explícito (com Listas de Adjacência) e desperdiçarmos memória, tratamos o Grid como um **Grafo Implícito**, onde cada célula é um vértice e o deslocamento para células vizinhas representa as arestas.

---

## Visão Geral

| Técnica / Algoritmo | Estrutura Base | Utilização Típica | Complexidade |
|---|---|---|---|
| **DFS (Flood Fill)** | Recursão (Call Stack) | Contar área de ilhas, pintar/marcar regiões conectadas. | O(L * C) |
| **BFS (Shortest Path)** | `std::queue` | Encontrar a distância mínima (menor caminho) sem pesos. | O(L * C) |

*(Onde `L` é o número de linhas e `C` o número de colunas).*

---

## Vetores de Deslocamento e Limites (Movement)

A técnica mais importante para evitar dezenas de `if/else` no seu código. Usamos arrays constantes de deslocamento de linha (`dr`) e coluna (`dc`) para gerar as coordenadas dos vizinhos através de um loop simples.

```cpp
// Deslocamentos em 4 direções (Cima, Baixo, Esquerda, Direita)
int dr4[] = {-1, 1, 0, 0}; 
int dc4[] = {0, 0, -1, 1};

// Função para checar se a nova posição (r, c) é válida dentro da matriz R x C
bool isValid(int r, int c, int R, int C) {
    return r >= 0 && r < R && c >= 0 && c < C;
}
```

> **Use Vetores de Deslocamento em** TODOS os problemas de grid. Eles deixam o código muito mais limpo e diminuem drasticamente a chance de você cometer um erro de digitação nos índices.

---

## DFS em Grids (Flood Fill)

A Busca em Profundidade (DFS) é a ferramenta ideal para explorar exaustivamente uma região, de forma análoga ao balde de tinta do MS Paint.

```cpp
int dfs_flood_fill(const vector<vector<int>>& grid, int r, int c, vector<vector<bool>>& visitado) {
    visitado[r][c] = true;
    int tamanho_regiao = 1; // Conta a célula atual

    for (int i = 0; i < 4; i++) {
        int nr = r + dr4[i];
        int nc = c + dc4[i];

        // Se for válido, não foi visitado e não for obstáculo
        if (isValid(nr, nc, grid.size(), grid[0].size()) && !visitado[nr][nc] && grid[nr][nc] != 1) {
            tamanho_regiao += dfs_flood_fill(grid, nr, nc, visitado);
        }
    }
    return tamanho_regiao;
}
```

> **Use a DFS quando** precisar descobrir a quantidade de componentes conexas ("quantas ilhas existem no mapa?") ou o tamanho de uma região específica. 
> **Atenção:** Em grids gigantescos (ex: $1000 \times 1000$), a DFS recursiva atinge $10^6$ de profundidade e pode causar *Stack Overflow*. Nesses casos raros, prefira a BFS.

---

## BFS em Grids (Menor Caminho)

A Busca em Largura (BFS) explora o grid em "camadas" radiais a partir do ponto de origem. Por conta disso, ela é a única que **garante o caminho mais curto** num grid não ponderado.

```cpp
int bfs_menor_caminho(const vector<vector<int>>& grid, int sr, int sc, int tr, int tc) {
    int linhas = grid.size(), colunas = grid[0].size();
    
    // A matriz de distâncias também funciona como o controle de 'visitados'
    vector<vector<int>> dist(linhas, vector<int>(colunas, -1));
    queue<pair<int, int>> fila;
    
    fila.push({sr, sc});
    dist[sr][sc] = 0; // Distância da origem para si mesma é 0
    
    while (!fila.empty()) {
        auto [r, c] = fila.front();
        fila.pop();
        
        if (r == tr && c == tc) return dist[r][c]; // Chegou no destino!
        
        for (int i = 0; i < 4; i++) {
            int nr = r + dr4[i];
            int nc = c + dc4[i];
            
            // Se for válido, for caminhável e AINDA NÃO foi visitado (-1)
            if (isValid(nr, nc, linhas, colunas) && grid[nr][nc] != 1 && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1; // A distância é a do pai + 1
                fila.push({nr, nc});
            }
        }
    }
    return -1; // Impossível chegar ao destino
}
```

> **Use a BFS quando** o problema perguntar "qual é o número mínimo de passos para chegar ao destino?" ou "qual é o menor caminho saindo do labirinto?".