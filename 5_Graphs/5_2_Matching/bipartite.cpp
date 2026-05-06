#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
vector<int> adj[MAXN];
int color[MAXN]; // Inicializar com -1 antes de cada caso de teste

bool is_bipartite(int start) {
    queue<int> q;
    
    q.push(start);
    color[start] = 0; // Pinta o nó inicial com a cor 0
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v : adj[u]) {
            if (color[v] == -1) {
                // Se não foi visitado, pinta com a cor inversa (1 - 0 = 1, ou 1 - 1 = 0)
                color[v] = 1 - color[u];
                q.push(v);
            } else if (color[v] == color[u]) {
                // Se o vizinho já foi visitado e tem a mesma cor, encontrou um ciclo ímpar!
                return false; 
            }
        }
    }
    return true; // Se terminou sem conflitos, é bipartido
}