#include <bits/stdc++.h>

using namespace std;

/* ==========================================================================
 * ALGORITMO 1: Problema do Caixeiro Viajante (TSP)
 * Paradigma: Backtracking com Poda de Otimalidade (Pruning)
 * Complexidade: O(N!) no pior caso, mas otimizado fortemente pela poda.
 * ==========================================================================
 * - A função tenta construir o caminho visitando cidades não visitadas.
 * - 'custo_atual' armazena o custo do caminho construído até agora.
 * - 'melhor_custo' guarda a resposta global (passado por referência).
 * - PODA (Pruning): Se o 'custo_atual' for MAIOR OU IGUAL ao 'melhor_custo' 
 * já encontrado, abortamos essa ramificação (return). 
 * - Caso Base: Se visitamos todas as 'n' cidades, somamos o custo de voltar.
 * - Backtracking: Marcamos, visitamos e desmarcamos a cidade.
 */
void tsp_backtracking(const vector<vector<int>>& dist, vector<bool>& visitado, 
                      int cidade_atual, int visitadas_count, int custo_atual, 
                      int& melhor_custo) {
    int n = dist.size();

    // PODA DE OTIMALIDADE: Aborta se o caminho atual já é pior que o melhor conhecido
    if (custo_atual >= melhor_custo) {
        return; 
    }

    // Caso base: Todas as cidades foram visitadas
    if (visitadas_count == n) {
        int custo_final = custo_atual + dist[cidade_atual][0]; // Volta para a origem
        melhor_custo = min(melhor_custo, custo_final);
        return;
    }

    // Tenta visitar as próximas cidades
    for (int proxima = 0; proxima < n; proxima++) {
        if (!visitado[proxima]) {
            // 1. Marca estado
            visitado[proxima] = true;
            
            // 2. Transição (Chamada recursiva)
            tsp_backtracking(dist, visitado, proxima, visitadas_count + 1, 
                             custo_atual + dist[cidade_atual][proxima], melhor_custo);
            
            // 3. Desfaz estado (Backtracking)
            visitado[proxima] = false;
        }
    }
}


/* ==========================================================================
 * ALGORITMO 2: Problema das N-Rainhas
 * Paradigma: Backtracking com Poda de Viabilidade (Vetores de Marcação)
 * Complexidade: O(N!)
 * ==========================================================================
 * - O objetivo é colocar N rainhas num tabuleiro NxN sem que se ataquem.
 * - Usamos vetores booleanos para verificar ataques em tempo O(1):
 * 1. 'colunas[c]': Verifica a coluna 'c'.
 * 2. 'diag1[linha + c]': Diagonais principais (soma linha+coluna).
 * 3. 'diag2[linha - c + n - 1]': Diagonais secundárias (diferença linha-coluna).
 * - Se a posição for válida, marca os vetores, avança a linha, e depois desmarca.
 */
void resolver_n_rainhas(int linha, int n, vector<bool>& colunas, 
                        vector<bool>& diag1, vector<bool>& diag2, int& solucoes) {
    // Caso base: Todas as rainhas foram posicionadas
    if (linha == n) {
        solucoes++;
        return;
    }

    for (int c = 0; c < n; c++) {
        // PODA DE VIABILIDADE: Verifica ataque em O(1)
        if (colunas[c] || diag1[linha + c] || diag2[linha - c + n - 1]) {
            continue; // Posição inválida, tenta a próxima coluna
        }

        // 1. MARCAR estado
        colunas[c] = diag1[linha + c] = diag2[linha - c + n - 1] = true;

        // 2. MERGULHAR (Recursão)
        resolver_n_rainhas(linha + 1, n, colunas, diag1, diag2, solucoes);

        // 3. DESMARCAR (Backtrack)
        colunas[c] = diag1[linha + c] = diag2[linha - c + n - 1] = false;
    }
}