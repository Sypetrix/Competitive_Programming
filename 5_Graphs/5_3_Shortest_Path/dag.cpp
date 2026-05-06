#include <bits/stdc++.h>

using namespace std;

// ==============================================================================
// ALGORITMO: Menor Caminho em DAG — Relaxamento em Ordem Topológica
// Complexidade: O(V + E)
// ==============================================================================
// Descrição:
// - Funciona APENAS em grafos direcionados acíclicos (DAGs).
// - Vantagem sobre Dijkstra e Bellman-Ford: mais rápido (linear) e aceita
//   arestas com pesos NEGATIVOS, desde que não haja ciclos.
// - Ideia: processar os vértices em ordem topológica garante que, ao relaxar
//   as arestas de um vértice U, a distância dist[U] já é definitiva
//   (todos os caminhos que chegam em U já foram processados).
// ==============================================================================

const int MAXN = 1e5 + 5;
const int INF  = 1e9;

vector<pair<int, int>> adj[MAXN]; // {vizinho, peso}
int in_degree[MAXN];
int dist[MAXN];

// Retorna a ordem topológica via Kahn (BFS).
// Retorna vetor vazio se o grafo contiver ciclo.
vector<int> topological_sort(int n) {
    queue<int> q;
    vector<int> order;

    for (int i = 1; i <= n; i++)
        if (in_degree[i] == 0) q.push(i);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (auto [v, w] : adj[u])
            if (--in_degree[v] == 0) q.push(v);
    }

    if ((int)order.size() != n) return {}; // Ciclo detectado
    return order;
}

// Calcula a menor distância de 'start' a todos os vértices do DAG.
// Retorna false se o grafo contiver ciclo (não é um DAG válido).
bool dag_shortest_path(int start, int n) {
    // Salva in_degree original pois topological_sort o modifica
    // (neste código in_degree já é global — em múltiplos casos de teste,
    //  reconstrua in_degree a cada chamada ou passe por cópia)
    vector<int> order = topological_sort(n);
    if (order.empty()) return false;

    // Inicializa distâncias
    for (int i = 1; i <= n; i++) dist[i] = INF;
    dist[start] = 0;

    // Relaxa arestas na ordem topológica
    for (int u : order) {
        if (dist[u] == INF) continue; // Vértice inalcançável a partir de 'start'
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    return true;
}

// ==============================================================================
// VARIANTE: Maior Caminho em DAG (Longest Path)
// ==============================================================================
// Inicialize dist[] com -INF (exceto dist[start] = 0) e inverta o sinal
// da comparação no relaxamento. Útil para problemas de escalonamento
// (ex: maior número de tarefas encadeadas).
// ==============================================================================

bool dag_longest_path(int start, int n) {
    vector<int> order = topological_sort(n);
    if (order.empty()) return false;

    for (int i = 1; i <= n; i++) dist[i] = -INF;
    dist[start] = 0;

    for (int u : order) {
        if (dist[u] == -INF) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w > dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    return true;
}

// Como ler as arestas e construir in_degree na main:
// adj[u].push_back({v, w});
// in_degree[v]++;