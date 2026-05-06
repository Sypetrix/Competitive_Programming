#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
vector<int> adj[MAXN];
int color[MAXN]; // Inicializar com 0

bool has_cycle(int u) {
    color[u] = 1; // Marca como "visitando"
    
    for (int v : adj[u]) {
        if (color[v] == 1) {
            return true; // Encontrou ciclo!
        }
        if (color[v] == 0) {
            if (has_cycle(v)) return true;
        }
    }
    
    color[u] = 2; // Marca como "totalmente processado"
    return false;
}