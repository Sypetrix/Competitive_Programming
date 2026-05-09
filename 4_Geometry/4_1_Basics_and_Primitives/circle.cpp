#include <iostream>
using namespace std;

const int MAX_RAIO = 5;
const int MAX_SETORES = 8; // Fatias da "pizza"

void percorrer_circular(int raio, int setor) {
    // Caso especial: o centro toca todos os setores do anel 1
    if (raio == 0) {
        for (int s = 0; s < MAX_SETORES; s++) {
            // Processar vizinho (1, s)
        }
        return;
    }

    // Vizinhos no mesmo anel (esquerda e direita, com módulo para dar a volta)
    int vizinhos_setor[] = {(setor + 1) % MAX_SETORES, (setor - 1 + MAX_SETORES) % MAX_SETORES};
    for (int ns : vizinhos_setor) {
        // Processar vizinho lateral (raio, ns)
    }

    // Vizinho no anel de dentro (se raio == 1, o vizinho interno é o centro único)
    if (raio == 1) {
        // Processar vizinho centro (0, 0)
    } else {
        // Processar vizinho interno (raio - 1, setor)
    }

    // Vizinho no anel de fora
    if (raio + 1 < MAX_RAIO) {
        // Processar vizinho externo (raio + 1, setor)
    }
}