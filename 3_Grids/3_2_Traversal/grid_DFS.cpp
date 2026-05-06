#include <bits/stdc++.h>

using namespace std;

// Vetores de deslocamento para as 4 direções (Cima, Baixo, Esquerda, Direita)
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

/*
Realiza uma Busca Exaustiva (Flood Fill) em um grid 2D usando DFS. === O(L * C)

- A função recebe as coordenadas (r, c) atuais. A primeira ação é marcar a célula 
como visitada para evitar ciclos infinitos.
- Iteramos sobre as 4 direções possíveis usando os vetores de deslocamento (dr, dc).
- Para cada direção, calculamos as novas coordenadas (novo_r, novo_c) e verificamos
limites da matriz (boundary check) e se a célula já foi visitada ou se é um obstáculo.
- Se for um movimento válido, chamamos a recursão para a nova célula.
- Neste exemplo, a função retorna o tamanho (área) da região conectada.
*/
int dfs_flood_fill(const vector<vector<int>>& grid, int r, int c, vector<vector<bool>>& visitado) {
    int linhas = grid.size();
    int colunas = grid[0].size();
    
    visitado[r][c] = true;
    int tamanho_regiao = 1;

    for (int i = 0; i < 4; i++) {
        int novo_r = r + dr[i];
        int novo_c = c + dc[i];

        // Verifica limites da matriz, validade do caminho e se não foi visitado
        if (novo_r >= 0 && novo_r < linhas && novo_c >= 0 && novo_c < colunas && 
            grid[novo_r][novo_c] == 1 && !visitado[novo_r][novo_c]) {
            tamanho_regiao += dfs_flood_fill(grid, novo_r, novo_c, visitado);
        }
    }
    
    return tamanho_regiao;
}