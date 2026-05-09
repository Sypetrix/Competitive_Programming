#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1005; // Grafos de fluxo costumam ser menores
const int INF = 1e9; // O(E^2 * V)

int cap[MAXN][MAXN];   // Matriz para guardar as capacidades residuais
vector<int> adj[MAXN]; // Lista de adjacência (deve guardar a aresta de ida e de volta!)
int parent[MAXN];      // Guarda o caminho encontrado pela BFS

// Encontra um caminho aumentante da fonte (s) ao sorvedouro (t)
int bfs_flow(int s, int t) {
    fill(parent, parent + MAXN, -1);
    parent[s] = s;
    queue<pair<int, int>> q; // {nó, gargalo_atual}
    q.push({s, INF});
    
    while (!q.empty()) {
        int u = q.front().first;
        int flow = q.front().second;
        q.pop();
        
        for (int v : adj[u]) {
            // Se ainda não visitei o vizinho e a aresta tem capacidade sobrando
            if (parent[v] == -1 && cap[u][v] > 0) {
                parent[v] = u;
                int new_flow = min(flow, cap[u][v]); // O gargalo do caminho
                
                if (v == t) return new_flow; // Chegamos no destino
                q.push({v, new_flow});
            }
        }
    }
    return 0; // Não achou caminho
}

// Algoritmo principal
int max_flow(int s, int t) {
    int flow = 0;
    int new_flow;
    
    // Enquanto existir caminho da fonte ao destino com capacidade > 0
    while ((new_flow = bfs_flow(s, t))) {
        flow += new_flow;
        int curr = t;
        
        // Atualiza as capacidades residuais (subtrai da ida, soma na volta)
        while (curr != s) {
            int prev = parent[curr];
            cap[prev][curr] -= new_flow;
            cap[curr][prev] += new_flow; // Aresta reversa (crucial para o algoritmo!)
            curr = prev;
        }
    }
    return flow;
}

/* Como adicionar arestas no fluxo:
void add_edge(int u, int v, int capacity) {
    adj[u].push_back(v);
    adj[v].push_back(u); // Adiciona na volta para permitir desfazer o fluxo
    cap[u][v] += capacity;
}
*/