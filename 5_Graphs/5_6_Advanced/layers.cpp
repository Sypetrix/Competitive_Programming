#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
const int MAXK = 25; // Máximo de camadas (ex: até 20 usos de poder)
const int INF = 1e9;

vector<pair<int, int>> adj[MAXN];
// dist[u][k] = menor distância até o vértice 'u' tendo usado 'k' poderes
int dist[MAXN][MAXK]; 

int dijkstra_camadas(int start, int end, int n, int max_k) {
    // 1. Inicializa todas as distâncias para o infinito
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= max_k; j++) {
            dist[i][j] = INF;
        }
    }

    // 2. Fila de Prioridade guardando tuplas: {distancia_acumulada, no_atual, poderes_usados}
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;

    dist[start][0] = 0;
    pq.push({0, start, 0});

    while (!pq.empty()) {
        // C++17: Structured bindings para desempacotar a tupla
        auto [d, u, k] = pq.top(); 
        pq.pop();

        // Se encontrou um caminho pior do que o registrado no mesmo estado, ignora
        if (d > dist[u][k]) continue;
        
        // Opcional: Se só queremos chegar no fim, independentemente de quantos poderes gastamos
        // if (u == end) return d; 

        for (auto edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            // --- TRANSIÇÃO 1: Movimento normal (continua na mesma camada 'k') ---
            if (dist[u][k] + weight < dist[v][k]) {
                dist[v][k] = dist[u][k] + weight;
                pq.push({dist[v][k], v, k});
            }

            // --- TRANSIÇÃO 2: Usar o poder (Muda para a próxima camada 'k+1') ---
            // Nesse exemplo o poder zera o peso da aresta (podia ser weight/2, por exemplo)
            if (k < max_k) {
                if (dist[u][k] + 0 < dist[v][k + 1]) { 
                    dist[v][k + 1] = dist[u][k] + 0;
                    pq.push({dist[v][k + 1], v, k + 1});
                }
            }
        }
    }
    
    // 3. A resposta será o menor valor para chegar no destino considerando qualquer 'k'
    int ans = INF;
    for (int k = 0; k <= max_k; k++) {
        ans = min(ans, dist[end][k]);
    }
    
    return (ans == INF) ? -1 : ans;
}