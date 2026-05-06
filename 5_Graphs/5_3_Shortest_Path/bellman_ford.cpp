#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
const int INF = 1e9;

// Estrutura simples para armazenar a lista de arestas
struct Edge {
    int u, v, weight;
};

vector<Edge> edges;
int dist[MAXN];

// Retorna 'true' se não houver ciclo negativo, 'false' se houver
bool bellman_ford(int n, int start) {
    // Inicializa distâncias
    for (int i = 0; i <= n; i++) dist[i] = INF;
    dist[start] = 0;
    
    // Relaxamento das arestas V-1 vezes
    for (int i = 1; i <= n - 1; i++) {
        for (auto e : edges) {
            if (dist[e.u] < INF && dist[e.u] + e.weight < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.weight;
            }
        }
    }
    
    // Verificação de ciclo negativo (se relaxar na V-ésima vez, tem ciclo)
    for (auto e : edges) {
        if (dist[e.u] < INF && dist[e.u] + e.weight < dist[e.v]) {
            return false; // Ciclo negativo detetado!
        }
    }
    
    return true;
}