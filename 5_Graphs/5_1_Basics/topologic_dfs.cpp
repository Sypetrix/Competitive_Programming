#include <bits/stdc++.h>

using namespace std;

// ==============================================================================
// ALGORITMO: Ordenação Topológica via DFS
// Complexidade: O(V + E)
// ==============================================================================
// Descrição:
// - Alternativa ao algoritmo de Kahn (BFS com grau de entrada).
// - Ideia: ao terminar de processar um vértice U (todos os seus descendentes
//   já foram visitados), insere-se U em uma pilha. Ao final, a pilha
//   invertida contém a ordenação topológica válida.
// - Detecção de ciclo: usa o esquema de 3 cores (igual ao cycle.cpp):
//     0 = não visitado
//     1 = visitando (na pilha de recursão atual)
//     2 = processado
//   Se durante a DFS encontrarmos um vizinho com cor 1, há um ciclo.
//
// Diferença em relação ao algoritmo de Kahn:
// - Kahn é iterativo (BFS) e detecta ciclo pelo tamanho da ordem final.
// - DFS é recursivo e detecta ciclo em tempo real pela cor dos vértices.
// - Ambos produzem ordenações topológicas válidas (pode haver mais de uma).
// ==============================================================================

const int MAXN = 1e5 + 5;

vector<int> adj[MAXN];
int color[MAXN];          // 0 = não visitado, 1 = visitando, 2 = processado
stack<int> topo_stack;    // Vértices em ordem topológica reversa
bool has_cycle;

void dfs_topo(int u) {
    color[u] = 1; // Marca como "visitando"

    for (int v : adj[u]) {
        if (color[v] == 1) {
            has_cycle = true; // Aresta de retorno = ciclo
            return;
        }
        if (color[v] == 0) {
            dfs_topo(v);
            if (has_cycle) return; // Propaga a detecção de ciclo
        }
    }

    color[u] = 2;          // Totalmente processado
    topo_stack.push(u);    // Insere na pilha ao terminar
}

// Retorna a ordenação topológica ou vetor vazio se houver ciclo.
vector<int> topological_sort_dfs(int n) {
    fill(color, color + n + 1, 0);
    while (!topo_stack.empty()) topo_stack.pop();
    has_cycle = false;

    for (int i = 1; i <= n; i++) {
        if (color[i] == 0) {
            dfs_topo(i);
            if (has_cycle) return {}; // Ciclo detectado
        }
    }

    // Desempilha para obter a ordem topológica correta
    vector<int> order;
    order.reserve(n);
    while (!topo_stack.empty()) {
        order.push_back(topo_stack.top());
        topo_stack.pop();
    }
    return order;
}

// Como inicializar antes de múltiplos casos de teste:
// for (int i = 0; i <= N; i++) { adj[i].clear(); color[i] = 0; }
// while (!topo_stack.empty()) topo_stack.pop();
// has_cycle = false;