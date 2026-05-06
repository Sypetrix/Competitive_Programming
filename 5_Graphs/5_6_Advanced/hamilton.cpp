#include <bits/stdc++.h>

using namespace std;

// N máximo costuma ser 20 para caber no tempo de 1s~2s
const int MAXN = 20; 

// adj_mat[u][v] = true se existe aresta de u para v
bool adj_mat[MAXN][MAXN]; 

// dp[mask][i] = Se é possível visitar o conjunto de vértices representado por 'mask' 
// e terminar exatamente no vértice 'i'
bool dp[1 << MAXN][MAXN]; 

bool has_hamiltonian_path(int n) {
    memset(dp, false, sizeof(dp));
    
    // Casos base: caminhos começando em cada vértice
    for (int i = 0; i < n; i++) {
        dp[1 << i][i] = true;
    }
    
    // Itera sobre todos os subconjuntos de vértices (máscaras)
    for (int mask = 1; mask < (1 << n); mask++) {
        for (int u = 0; u < n; u++) {
            // Se a máscara inclui o vértice u e conseguimos terminar em u
            if ((mask & (1 << u)) && dp[mask][u]) {
                
                // Tenta expandir para o próximo vértice v
                for (int v = 0; v < n; v++) {
                    // Se existe aresta (u, v) e v NÃO está na máscara
                    if (adj_mat[u][v] && !(mask & (1 << v))) {
                        dp[mask | (1 << v)][v] = true;
                    }
                }
            }
        }
    }
    
    // Verifica se conseguimos visitar todos os nós (mask = todos os bits 1)
    int full_mask = (1 << n) - 1;
    for (int i = 0; i < n; i++) {
        if (dp[full_mask][i]) return true; // Para Caminho Hamiltoniano
        
        // Se o problema pedir CIRCUITO Hamiltoniano, basta checar se o 
        // vértice final 'i' tem aresta de volta para o vértice inicial.
    }
    
    return false;
}