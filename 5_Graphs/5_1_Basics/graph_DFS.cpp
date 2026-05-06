#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5; // Ajuste conforme o problema

vector<int> adj[MAXN];
bool vis[MAXN];

// Função recursiva da DFS
void dfs(int u) {
    vis[u] = true;
    
    // Processar o nó u (ex: cout << u << " ";)
    
    for (int v : adj[u]) {
        if (!vis[v]) {
            dfs(v);
        }
    }
}

// Como inicializar antes de múltiplos casos de teste:
// for(int i=0; i<=N; i++) { adj[i].clear(); vis[i] = false; }