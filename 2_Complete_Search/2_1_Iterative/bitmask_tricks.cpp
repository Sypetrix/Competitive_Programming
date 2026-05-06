#include <bits/stdc++.h>

using namespace std;

// ----------------------------------------------------------------------
// Truque 1: Iterar sobre todas as submáscaras de todas as máscaras
// Complexidade Total: O(3^N) em vez de O(4^N)
// ----------------------------------------------------------------------
void enumera_submascaras(int n) {
    // Para n = 3, o limite é 2^3 = 8
    for (int mask = 0; mask < (1 << n); mask++) {
        cout << "Mascara (base 10): " << mask << " | Submascaras: ";
        
        // Itera decrescentemente por todas as submáscaras estritas. 
        // A mágica (sub - 1) & mask garante que só pulemos para subconjuntos válidos.
        // Para no 0 (pois a condição é sub > 0).
        for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
            cout << sub << " ";
        }
        
        // O subconjunto vazio (0) é sempre submáscara de qualquer máscara
        cout << "0\n";
    }
}

// ----------------------------------------------------------------------
// Truque 2: Gosper's Hack
// Gera todas as combinações de tamanho N com exatamente K bits ligados.
// Complexidade: O(1) por combinação gerada.
// ----------------------------------------------------------------------
/*
  Matemática por trás:
  - 'c = mask & -mask' isola o bit ligado mais à direita (Lowest Set Bit).
  - 'r = mask + c' vira o bloco de 1s mais à direita para 0 e liga o bit logo à esquerda.
  - A parte final rearranja os bits remanescentes arrastando-os para a ponta direita.
*/
void gospers_hack(int n, int k) {
    if (k == 0) {
        cout << 0 << "\n";
        return;
    }
    
    // Inicializa com os primeiros K bits ligados
    // Nota: use 1LL se n > 30
    int mask = (1 << k) - 1; 
    int limite = (1 << n);
    
    while (mask < limite) {
        cout << mask << " ";
        
        int c = mask & -mask;
        int r = mask + c;
        mask = (((r ^ mask) >> 2) / c) | r;
    }
    cout << "\n";
}