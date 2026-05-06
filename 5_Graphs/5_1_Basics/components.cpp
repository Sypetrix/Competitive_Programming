#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
vector<int> adj[MAXN];
int comp[MAXN]; // comp[i] guarda o ID da componente do vértice i

void dfs_comp(int u, int c) {
    comp[u] = c;
    for (int v : adj[u]) {
        if (!comp[v]) { // Se ainda não tem componente
            dfs_comp(v, c);
        }
    }
}

int find_components(int n) {
    // fill(comp, comp + n + 1, 0); // Resetar antes do caso de teste
    int num_comps = 0;
    
    for (int i = 1; i <= n; i++) {
        if (!comp[i]) {
            num_comps++; // Nova componente encontrada
            dfs_comp(i, num_comps);
        }
    }
    return num_comps;
}