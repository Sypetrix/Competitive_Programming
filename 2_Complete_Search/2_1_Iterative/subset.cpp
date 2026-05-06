#include <bits/stdc++.h>

using namespace std;

/*
Subconjuntos === O(n * 2^n), com um total de 2^n subsets

- Um conjunto de tamanho 'n' possui exatamente 2^n subconjuntos.
- Utilizamos um inteiro 'mask' que varia de 0 até (2^n) - 1. 
- A operação (1 << n) calcula 2^n usando deslocamento de bits.
- A representação binária do inteiro 'mask' atua como um mapa para o subconjunto:
- bit 1 na posição 'i' indica que o elemento 'i' está incluído no subconjunto atual.
- bit 0 indica que o elemento não está incluído.
- O loop interno (i de 0 até n-1) verifica o estado de cada bit usando a operação
- bitwise AND: (mask & (1 << i)). Se o resultado for diferente de zero, o i-ésimo
elemento é impresso.
*/
void gera_subconjuntos_bitmask(const vector<int>& conjunto) {
    int n = conjunto.size();
    for (int mask = 0; mask < (1 << n); mask++) {
        cout << "{ ";
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) cout << conjunto[i] << " ";
        }
        cout << "}\n";
    }
}

void gerasub(vector<bool>& c, int k, int n) {
    // Caso base: se terminou de gerar um subconjunto 
    if (k == n) { 
        // Imprime o subconjunto gerado
        cout << "{ ";
        for (int i = 0; i < n; ++i) {
            // Se a posição i for true, o elemento i está neste subconjunto
            if (c[i]) cout << i << " ";
        }
        cout << "}\n";
        return;
    }
    
    // Caminho 1: gera subconjuntos COM o elemento k [cite: 27]
    c[k] = true; 
    gerasub(c, k + 1, n);
    
    // Caminho 2: gera subconjuntos SEM o elemento k [cite: 28]
    c[k] = false; 
    gerasub(c, k + 1, n);
}