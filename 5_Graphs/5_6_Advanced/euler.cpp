#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
vector<int> adj[MAXN];
bool vis[MAXN];

// DFS simples para checar conectividade
void dfs_con(int u) {
    vis[u] = true;
    for (int v : adj[u]) {
        if (!vis[v]) dfs_con(v);
    }
}

// Retorna 2 para Circuito Euleriano, 1 para Caminho Euleriano, 0 se não for
int check_eulerian(int n) {
    // 1. Checar conectividade (ignorando nós de grau 0)
    int start_node = -1;
    for (int i = 1; i <= n; i++) {
        if (adj[i].size() > 0) {
            start_node = i;
            break;
        }
    }
    
    if (start_node == -1) return 2; // Grafo sem arestas é tecnicamente euleriano
    
    dfs_con(start_node);
    for (int i = 1; i <= n; i++) {
        if (adj[i].size() > 0 && !vis[i]) {
            return 0; // Grafo desconexo
        }
    }
    
    // 2. Contar vértices com grau ímpar
    int odd_count = 0;
    for (int i = 1; i <= n; i++) {
        if (adj[i].size() % 2 != 0) odd_count++;
    }
    
    if (odd_count == 0) return 2; // Circuito (começa e termina no mesmo nó)
    if (odd_count == 2) return 1; // Caminho (começa em um ímpar, termina no outro)
    return 0;                     // Não é euleriano
}