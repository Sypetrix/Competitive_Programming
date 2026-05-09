#include <iostream>
using namespace std;

// 6 Direções no sistema Axial (q = coluna inclinada, r = linha)
int dq[] = { 1,  1,  0, -1, -1,  0};
int dr[] = { 0, -1, -1,  0,  1,  1};

void percorrer_hexagonal(int q, int r) {
    for (int i = 0; i < 6; i++) {
        int nq = q + dq[i];
        int nr = r + dr[i];
        
        // Em grids hexagonais, os limites dependem do formato do tabuleiro 
        // (ex: um raio máximo a partir do centro (0,0)).
        // if (limite_valido(nq, nr)) {
            // Processar vizinho (nq, nr)
        // }
    }
}