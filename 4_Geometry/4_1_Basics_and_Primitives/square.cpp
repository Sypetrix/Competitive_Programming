#include <iostream>
using namespace std;

// Deslocamentos para 4 direções (Cima, Baixo, Esquerda, Direita)
int dr4[] = {-1, 1, 0, 0};
int dc4[] = {0, 0, -1, 1};

// Deslocamentos para 8 direções (com diagonais)
int dr8[] = {-1, 1, 0, 0, -1, -1, 1, 1};
int dc8[] = {0, 0, -1, 1, -1, 1, -1, 1};

bool isValid(int r, int c, int R, int C) {
    return r >= 0 && r < R && c >= 0 && c < C;
}

void percorrer_quadrado(int r, int c, int R, int C) {
    for (int i = 0; i < 4; i++) {
        int nr = r + dr4[i];
        int nc = c + dc4[i];
        if (isValid(nr, nc, R, C)) {
            // Processar vizinho (nr, nc)
        }
    }
}