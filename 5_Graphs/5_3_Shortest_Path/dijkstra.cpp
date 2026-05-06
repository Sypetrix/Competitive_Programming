#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
const int INF = 1e9; // Representa o infinito

// Lista de adjacência: guarda pares {vizinho, peso_da_aresta}
vector<pair<int, int>> adj[MAXN];
int dist[MAXN]; // Guarda a menor distância da origem até o nó

void dijkstra(int start) {
    // Inicializa todas as distâncias como infinito
    for (int i = 0; i < MAXN; i++) dist[i] = INF;
    
    // priority_queue configurada para retornar o MENOR elemento primeiro
    // Guarda pares {distancia_acumulada, no_atual}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    dist[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        
        // Se encontramos um caminho pior do que o já registrado, ignoramos
        if (d > dist[u]) continue;
        
        for (auto edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            
            // Relaxamento da aresta
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
}