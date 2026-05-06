#include <bits/stdc++.h>

using namespace std;

// -------------------------------------------------------------------------------------
// Busca em Profundidade Iterativa (IDDFS)
// -------------------------------------------------------------------------------------
// Combina a otimização de memória da DFS com a garantia de caminho mais curto da BFS.
// Funciona rodando uma DFS com limite de profundidade (Depth-Limited Search).
// A cada iteração, se o objetivo não for achado, o limite aumenta e tudo recomeça.
// Parece ineficiente recalcular os nós rasos, mas a complexidade de tempo se mantém 
// assintoticamente a mesma de uma BFS normal, usando espaço O(Profundidade).
// -------------------------------------------------------------------------------------

// Variáveis globais para facilitar (comum em maratonas)
int estado_objetivo = 10;

// Função simulando as transições de estado (ex: movimentos em um jogo)
vector<int> gera_vizinhos(int estado) {
    return {estado * 2, estado * 2 + 1};
}

// DFS que não desce além de um certo nível (limite_profundidade)
bool dfs_limitada(int estado, int profundidade_atual, int limite_profundidade) {
    if (estado == estado_objetivo) return true;
    
    // Poda: se atingiu o limite estipulado, corta este ramo e volta
    if (profundidade_atual == limite_profundidade) return false;

    for (int proximo : gera_vizinhos(estado)) {
        if (dfs_limitada(proximo, profundidade_atual + 1, limite_profundidade)) {
            return true;
        }
    }
    
    return false;
}

// Função principal da IDDFS
int iddfs(int inicio) {
    int max_profundidade_possivel = 20; // Previne loops infinitos se não houver resposta
    
    for (int limite = 0; limite <= max_profundidade_possivel; limite++) {
        // Dica: Se o seu estado puder formar ciclos, você precisará de um vetor/map 
        // de 'visitado' que DEVE ser limpo a cada nova iteração deste loop.
        
        if (dfs_limitada(inicio, 0, limite)) {
            return limite; // Encontrou! A profundidade é igual ao número de passos
        }
    }
    return -1; // Objetivo inalcançável dentro do limite máximo
}