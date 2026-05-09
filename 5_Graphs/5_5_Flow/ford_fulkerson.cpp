#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1005; // Ajuste conforme o problema
const int INF = 1e9; // O (E * F)

vector<int> adj[MAXN];
int cap[MAXN][MAXN]; // Matriz de capacidades residuais
bool vis[MAXN];

// Busca em Profundidade (DFS) para encontrar um caminho aumentante
int dfs(int u, int t, int current_flow) {
    if (u == t) return current_flow; // Chegou no sorvedouro (sink)
    
    vis[u] = true;
    
    for (int v : adj[u]) {
        // Se o vizinho não foi visitado e a aresta ainda tem capacidade
        if (!vis[v] && cap[u][v] > 0) {
            
            // Tenta empurrar fluxo adiante (o gargalo é o mínimo entre o fluxo atual e a capacidade da aresta)
            int pushed = dfs(v, t, min(current_flow, cap[u][v]));
            
            // Se conseguiu empurrar fluxo até o fim, atualiza as arestas e volta
            if (pushed > 0) {
                cap[u][v] -= pushed; // Subtrai da aresta de ida
                cap[v][u] += pushed; // Soma na aresta de volta (aresta residual de arrependimento)
                return pushed;
            }
        }
    }
    return 0; // Não achou caminho por este ramo
}

// Algoritmo principal
int ford_fulkerson(int s, int t) {
    int total_flow = 0;
    
    while (true) {
        // A cada nova tentativa de caminho, resetamos o array de visitados
        memset(vis, false, sizeof(vis));
        
        // Tentamos empurrar o infinito a partir da fonte (source)
        int pushed = dfs(s, t, INF);
        
        // Se a DFS retornou 0, não há mais caminhos da fonte ao sorvedouro
        if (pushed == 0) break;
        
        total_flow += pushed;
    }
    
    return total_flow;
}

/* Como adicionar arestas (grafo direcionado):
void add_edge(int u, int v, int capacity) {
    adj[u].push_back(v);
    adj[v].push_back(u); // Aresta reversa para a rede residual
    cap[u][v] += capacity;
    // cap[v][u] fica como 0 se não for bidirecional
}
*/