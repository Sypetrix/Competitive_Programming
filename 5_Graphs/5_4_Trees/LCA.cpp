#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
const int LOG = 20; // log2(MAXN) arredondado para cima (2^20 > 10^5)

vector<int> adj[MAXN];
int up[MAXN][LOG]; // up[u][i] guarda o (2^i)-ésimo ancestral de 'u'
int tin[MAXN], tout[MAXN];
int timer;

// DFS para pré-computar a tabela 'up' e os timers
void dfs(int v, int p) {
    tin[v] = ++timer;
    
    up[v][0] = p; // O pai direto (2^0) de 'v' é 'p'
    
    // Constrói a tabela usando Binary Lifting (Programação Dinâmica)
    for (int i = 1; i < LOG; i++) {
        up[v][i] = up[ up[v][i - 1] ][i - 1];
    }
    
    for (int u : adj[v]) {
        if (u != p) {
            dfs(u, v);
        }
    }
    
    tout[v] = ++timer;
}

// Checa em O(1) se 'u' é ancestral de 'v'
bool is_ancestor(int u, int v) {
    // Se o tempo de entrada de 'u' é <= ao de 'v' e o de saída é >=
    // Se is_ancestor(0, x) (onde 0 seria uma raiz fake), sempre true.
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    
    // Sobe com 'u' o mais alto possível ANTES de cruzar a linha do LCA
    for (int i = LOG - 1; i >= 0; i--) {
        if (!is_ancestor(up[u][i], v)) {
            u = up[u][i];
        }
    }
    
    // Neste ponto, 'u' está exatamente um nível abaixo do LCA
    return up[u][0]; 
}

// Na main, para inicializar:
// timer = 0;
// dfs(raiz, raiz); // A raiz é pai dela mesma na inicialização