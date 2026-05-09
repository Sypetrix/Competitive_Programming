#include <bits/stdc++.h>

using namespace std;

// ==============================================================================
// ALGORITMO: Algoritmo Húngaro (Hungarian Algorithm / Assignment Problem)
// Paradigma: Grafos (Emparelhamento Bipartido com Pesos)
// Complexidade: O(N^3)
// ==============================================================================
// Descrição:
// - Designa N trabalhadores para M tarefas (N <= M) minimizando o custo global.
// - IMPORTANTE: A matriz de custos DEVE ser BASE 1 (índices 1 a N). A linha/coluna 0
//   são usadas internamente pelo algoritmo para os potenciais.
// - Para MAXIMIZAR (ex: lucro máximo), inverta o sinal de todos os elementos 
//   na matriz antes de chamar a função e inverta de volta a resposta.
// ==============================================================================

const long long INF = 1e18; // Usar 1e18 para long long

// Retorna o custo mínimo da designação
long long hungarian_min_cost(const vector<vector<long long>>& cost_matrix, int n, int m) {
    // n: número de trabalhadores (linhas 1..n)
    // m: número de tarefas (colunas 1..m)
    
    vector<long long> u(n + 1), v(m + 1); 
    // p[j] guarda a linha atual designada para a coluna j
    vector<int> p(m + 1), way(m + 1); 

    for (int i = 1; i <= n; ++i) {
        p[0] = i; 
        int j0 = 0;
        vector<long long> minv(m + 1, INF); 
        vector<bool> used(m + 1, false);
        
        do {
            used[j0] = true;
            int i0 = p[j0], j1 = 0;
            long long delta = INF;
            
            // Procura a melhor coluna (tarefa)
            for (int j = 1; j <= m; ++j) {
                if (!used[j]) {
                    long long cur = cost_matrix[i0][j] - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }
            
            // Atualiza os potenciais
            for (int j = 0; j <= m; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (p[j0] != 0); // Repete até achar uma coluna livre

        // Alterna o caminho aumentante
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0 != 0);
    }
    
    // A posição v[0] invertida acumula o custo mínimo.
    return -v[0]; 
}