#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;

// Lista de adjacência: guarda pares {vizinho, peso}
vector<pair<int, int>> adj[MAXN];
bool in_mst[MAXN]; // Marca se o vértice já está na AGM

long long prim(int start, int n) {
    long long cost = 0;
    int edges_used = 0;
    
    // Reseta visitados para múltiplos casos de teste
    for(int i = 0; i <= n; i++) in_mst[i] = false;
    
    // priority_queue configurada para retornar o MENOR elemento primeiro
    // Guarda pares {peso_da_aresta, no_destino}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    // Começa pelo nó 'start'
    pq.push({0, start});
    
    while (!pq.empty() && edges_used < n) {
        int weight = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        
        if (in_mst[u]) continue; // Se o nó já está na AGM, ignora
        
        in_mst[u] = true;
        cost += weight;
        edges_used++; // O primeiro nó retirado conta como aresta de peso 0
        
        // Adiciona todas as arestas vizinhas na fila
        for (auto edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;
            if (!in_mst[v]) {
                pq.push({w, v});
            }
        }
    }
    
    if (edges_used != n) return -1; // Grafo desconexo
    return cost;
}