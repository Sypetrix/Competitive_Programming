#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1005; // Ajuste conforme a quantidade de nós do Grupo B

// Lista de adjacência: Arestas indo do Grupo A (L) para o Grupo B (R)
vector<int> adj[MAXN]; 
int match[MAXN]; // match[v] = u indica que o nó 'v' (do grupo B) está conectado com 'u' (do grupo A)
bool vis[MAXN];  // Controle de visitados do Grupo B

// DFS para encontrar caminhos aumentantes
bool dfs(int u) {
    for (int v : adj[u]) {
        if (vis[v]) continue;
        vis[v] = true;
        
        // Se o nó v do grupo B está livre, OU o nó atual conectado a ele consegue achar outro par
        if (match[v] < 0 || dfs(match[v])) {
            match[v] = u; // Confirma o casamento
            return true;
        }
    }
    return false;
}

// Função principal para calcular o emparelhamento máximo
int max_bipartite_matching(int n_left) { // n_left = quantidade de nós no grupo A
    memset(match, -1, sizeof(match));
    int result = 0;
    
    for (int i = 0; i < n_left; i++) {
        memset(vis, false, sizeof(vis)); // Reseta visitados a cada nova tentativa
        if (dfs(i)) {
            result++; // Mais um casamento de sucesso
        }
    }
    
    return result;
}