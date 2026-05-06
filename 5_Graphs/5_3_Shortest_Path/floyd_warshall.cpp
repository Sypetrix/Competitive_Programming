#include <bits/stdc++.h>

using namespace std;

const int MAXN = 505; // Grafos para Floyd-Warshall são sempre pequenos
const int INF = 1e9;

int dist[MAXN][MAXN];

// Inicialização antes de ler as arestas do caso de teste
void init(int n) {
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == j) dist[i][j] = 0; // Distância para si mesmo é 0
            else dist[i][j] = INF;
        }
    }
}

void floyd_warshall(int n) {
    // k é o vértice intermediário, i é a origem, j é o destino
    // A ordem dos loops (k por fora) é OBRIGATÓRIA!
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                // A condição < INF evita overflow ao somar INF + peso
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

/* Como ler as arestas na main:
    init(N);
    for(int i=0; i<M; i++){
        int u, v, w;
        cin >> u >> v >> w;
        // Se for não-direcionado, preencha os dois lados e pegue o mínimo
        // (importante caso haja arestas múltiplas entre os mesmos nós)
        dist[u][v] = min(dist[u][v], w);
        dist[v][u] = min(dist[v][u], w); 
    }
*/