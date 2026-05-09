#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;

// Estrutura para as arestas
struct Edge {
    int u, v, weight;
    // Operador para ordenar as arestas pelo peso automaticamente
    bool operator<(Edge const& other) {
        return weight < other.weight;
    }
};

vector<Edge> edges;

// --- INÍCIO DO UNION-FIND (DSU) ---
int parent_node[MAXN];
int sz[MAXN]; // Guarda o tamanho do conjunto para otimização

void make_set(int v) {
    parent_node[v] = v;
    sz[v] = 1;
}

int find_set(int v) {
    if (v == parent_node[v]) return v;
    // Path compression: liga o nó diretamente à raiz
    return parent_node[v] = find_set(parent_node[v]); 
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        // Union by size: anexa a árvore menor na maior
        if (sz[a] < sz[b]) swap(a, b);
        parent_node[b] = a;
        sz[a] += sz[b];
    }
}
// --- FIM DO UNION-FIND ---

// Retorna o custo total da AGM (ou -1 se o grafo for desconexo)
long long kruskal(int n) {
    long long cost = 0;
    int edges_used = 0;
    
    // 1. Inicializa o DSU
    for (int i = 1; i <= n; i++) make_set(i);
    
    // 2. Ordena as arestas do menor para o maior peso
    sort(edges.begin(), edges.end());
    
    // 3. Processa cada aresta
    for (Edge e : edges) {
        // Se u e v estão em conjuntos diferentes, a aresta não forma ciclo
        if (find_set(e.u) != find_set(e.v)) {
            cost += e.weight;
            edges_used++;
            union_sets(e.u, e.v);
            
            // Se já usamos V-1 arestas, a AGM está pronta
            if (edges_used == n - 1) break;
        }
    }
    
    // Verifica se todos os nós foram conectados
    if (edges_used != n - 1) return -1; // Grafo desconexo
    return cost;
}