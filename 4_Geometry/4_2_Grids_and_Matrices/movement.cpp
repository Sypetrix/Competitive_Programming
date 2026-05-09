#include <bits/stdc++.h>

using namespace std;

// Deslocamentos em 4 direções (Cima, Baixo, Esquerda, Direita)
int dr4[] = {-1, 1, 0, 0}; 
int dc4[] = {0, 0, -1, 1};

// Deslocamentos em 8 direções (Adicionando diagonais)
int dr8[] = {-1, 1, 0, 0, -1, -1, 1, 1};
int dc8[] = {0, 0, -1, 1, -1, 1, -1, 1};

// Função para checar se a posição é válida dentro do Grid R x C
bool isValid(int r, int c, int R, int C) {
    return r >= 0 && r < R && c >= 0 && c < C;
}

/* Exemplo de uso em uma BFS/DFS:
   for(int i = 0; i < 4; i++) {
       int nr = r + dr4[i];
       int nc = c + dc4[i];
       if(isValid(nr, nc, R, C) && !visited[nr][nc]) {
           // Continua a busca...
       }
   }
*/