#include <bits/stdc++.h>
using namespace std;

struct UF {
    vector<int> parent, sz;

    UF(int n) {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(parent.begin(), parent.end(), 0);
    }

    // Busca com Compressão de Caminho - O(alpha(N))
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }

    // União por Tamanho - O(alpha(N))
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (sz[root_i] < sz[root_j]) swap(root_i, root_j);
            parent[root_j] = root_i;
            sz[root_i] += sz[root_j];
        }
    }
};

void demo_dsu() {
    UF dsu(10);      // Inicializa para 10 elementos
    dsu.unite(1, 2);  // Conecta 1 e 2
    dsu.unite(2, 3);  // Conecta 2 e 3 (1, 2 e 3 agora estão no mesmo grupo)
    
    if (dsu.find(1) == dsu.find(3)) {
        // Estão no mesmo conjunto
    }
}