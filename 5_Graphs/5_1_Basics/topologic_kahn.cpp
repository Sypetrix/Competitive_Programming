#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
vector<int> adj[MAXN];
int in_degree[MAXN]; // Grau de entrada (quantas arestas chegam no nó)
vector<int> topo_order;

// Retorna true se conseguiu ordenar, false se encontrou um ciclo
bool topological_sort(int n) {
    queue<int> q;
    topo_order.clear();
    
    // 1. Coloca na fila todos os vértices com grau de entrada 0 (sem dependências)
    for (int i = 1; i <= n; i++) {
        if (in_degree[i] == 0) q.push(i);
    }
    
    // 2. Processa a fila
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo_order.push_back(u);
        
        // Remove virtualmente as arestas que saem de 'u'
        for (int v : adj[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }
    
    // Se o tamanho da ordem for igual a N, deu tudo certo.
    // Se for menor, significa que sobraram nós presos em um ciclo!
    return topo_order.size() == n;
}

/* Na leitura das arestas (direcionadas u -> v):
    adj[u].push_back(v);
    in_degree[v]++;
*/