#include <iostream>
using namespace std;

bool isValid(int r, int c, int R, int C) {
    return r >= 0 && r < R && c >= 0 && c < C;
}

void percorrer_triangular(int r, int c, int R, int C) {
    // Vizinhos horizontais (sempre existem)
    int dr_horiz[] = {0, 0};
    int dc_horiz[] = {-1, 1};
    
    for (int i = 0; i < 2; i++) {
        int nr = r + dr_horiz[i];
        int nc = c + dc_horiz[i];
        if (isValid(nr, nc, R, C)) { /* Processar vizinho (nr, nc) */ }
    }

    // O terceiro vizinho depende se o triângulo aponta para cima ou para baixo
    int nr_vert = r;
    if ((r + c) % 2 == 0) {
        nr_vert += 1; // Aponta para cima, vizinho de aresta fica embaixo
    } else {
        nr_vert -= 1; // Aponta para baixo, vizinho de aresta fica em cima
    }
    
    if (isValid(nr_vert, c, R, C)) {
        // Processar vizinho vertical (nr_vert, c)
    }
}