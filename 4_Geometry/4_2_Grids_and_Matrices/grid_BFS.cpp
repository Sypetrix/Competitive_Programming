#include <bits/stdc++.h>

using namespace std;

// Vetores de deslocamento para as 4 direções (Cima, Baixo, Esquerda, Direita)
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

/*
Encontra a menor distância entre um ponto de partida e um destino em um grid usando BFS. === O(L * C)

- A BFS (Busca em Largura) explora o grid em "camadas" radiais a partir da origem.
- Utiliza uma fila (std::queue) para processar as células. Armazena pares de coordenadas.
- Uma matriz 'dist' guarda a menor distância da origem até cada célula e também serve
para controle de visitação (distância diferente de -1 significa que já foi visitada).
- A fila garante que a primeira vez que alcançamos a célula de destino, fizemos isso
pelo caminho mais curto possível (em grids sem pesos nas arestas).

* Vantagem sobre DFS: Encontra o caminho mínimo e não causa Stack Overflow em grids gigantes.
*/
int bfs_menor_caminho(const vector<vector<int>>& grid, int sr, int sc, int tr, int tc) {
    int linhas = grid.size();
    int colunas = grid[0].size();
    
    // Na maratona, se a BFS for chamada milhares de vezes, 
    // considere usar uma matriz global em vez de alocar uma nova a cada chamada.
    vector<vector<int>> dist(linhas, vector<int>(colunas, -1));
    queue<pair<int, int>> fila;
    
    fila.push({sr, sc});
    dist[sr][sc] = 0;
    
    while (!fila.empty()) {
        int r = fila.front().first;
        int c = fila.front().second;
        fila.pop();
        
        if (r == tr && c == tc) {
            return dist[r][c];
        }
        
        for (int i = 0; i < 4; i++) {
            int novo_r = r + dr[i];
            int novo_c = c + dc[i];
            
            if (novo_r >= 0 && novo_r < linhas && novo_c >= 0 && novo_c < colunas && 
                grid[novo_r][novo_c] == 1 && dist[novo_r][novo_c] == -1) {
                
                dist[novo_r][novo_c] = dist[r][c] + 1;
                fila.push({novo_r, novo_c});
            }
        }
    }
    
    return -1; // Impossível chegar
}