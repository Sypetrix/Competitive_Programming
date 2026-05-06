#include <bits/stdc++.h>

using namespace std;

// ==============================================================================
// ALGORITMO: Min-Cost Max-Flow (MCMF) - Successive Shortest Path com SPFA
// Paradigma: Grafos (Fluxo com Custos)
// Complexidade: O(F * V * E) onde F é o fluxo máximo (Pior caso).
// ==============================================================================
// Descrição:
// - Envia fluxo pelo caminho mais barato disponível da fonte (S) ao sorvedouro (T).
// - Usa o SPFA (Shortest Path Faster Algorithm) para lidar com arestas residuais
//   que possuem custos negativos.
// - Retorna um par {Fluxo Total, Custo Total}.
// ==============================================================================

const int INF = 1e9; // Valor para representar infinito

struct Edge {
    int to;
    int flow; // Fluxo atual
    int cap;  // Capacidade máxima
    int cost; // Custo unitário para passar fluxo por aqui
    int rev;  // Índice da aresta reversa
};

vector<vector<Edge>> adj;

void add_edge(int from, int to, int cap, int cost) {
    adj[from].push_back({to, 0, cap, cost, (int)adj[to].size()});
    // Aresta residual (capacidade inicial 0, custo invertido)
    adj[to].push_back({from, 0, 0, -cost, (int)adj[from].size() - 1});
}

pair<int, int> mcmf(int n, int s, int t) {
    int max_flow = 0;
    int min_cost = 0;

    vector<int> dist(n);
    vector<int> parent_node(n);
    vector<int> parent_edge(n);
    vector<bool> in_queue(n);

    // Repetir até não haver mais caminhos de S a T no grafo residual
    while (true) {
        fill(dist.begin(), dist.end(), INF);
        fill(in_queue.begin(), in_queue.end(), false);

        queue<int> q;
        q.push(s);
        dist[s] = 0;
        in_queue[s] = true;

        // SPFA para encontrar o caminho mais barato
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            in_queue[u] = false;

            for (int i = 0; i < adj[u].size(); i++) {
                Edge& e = adj[u][i];
                // Se ainda há capacidade e relaxa o caminho mais barato
                if (e.cap - e.flow > 0 && dist[e.to] > dist[u] + e.cost) {
                    dist[e.to] = dist[u] + e.cost;
                    parent_node[e.to] = u;
                    parent_edge[e.to] = i;

                    if (!in_queue[e.to]) {
                        q.push(e.to);
                        in_queue[e.to] = true;
                    }
                }
            }
        }

        // Se não achou caminho, terminamos
        if (dist[t] == INF) {
            break;
        }

        // Encontrar o gargalo (menor capacidade residual) no caminho
        int push = INF;
        int curr = t;
        while (curr != s) {
            int p = parent_node[curr];
            int idx = parent_edge[curr];
            push = min(push, adj[p][idx].cap - adj[p][idx].flow);
            curr = p;
        }

        max_flow += push;
        min_cost += push * dist[t]; // Custo = fluxo adicionado * custo do caminho

        // Atualizar as arestas ao longo do caminho
        curr = t;
        while (curr != s) {
            int p = parent_node[curr];
            int idx = parent_edge[curr];
            int rev_idx = adj[p][idx].rev;

            adj[p][idx].flow += push;
            adj[curr][rev_idx].flow -= push; // Aresta reversa
            curr = p;
        }
    }

    return {max_flow, min_cost};
}