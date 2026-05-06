#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
vector<int> adj[MAXN];

int tin[MAXN], low[MAXN], timer;
bool in_stack[MAXN];
stack<int> st;

vector<vector<int>> sccs; // Guarda todas as SCCs encontradas

void dfs_tarjan(int u) {
    tin[u] = low[u] = ++timer;
    st.push(u);
    in_stack[u] = true;
    
    for (int v : adj[u]) {
        if (!tin[v]) { // Vértice não visitado
            dfs_tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (in_stack[v]) { // Aresta de retorno (Back-edge)
            low[u] = min(low[u], tin[v]);
        }
    }
    
    // Se u é a raiz de uma SCC (ninguém alcançou alguém mais "antigo")
    if (low[u] == tin[u]) {
        vector<int> current_scc;
        int v;
        do {
            v = st.top();
            st.pop();
            in_stack[v] = false;
            current_scc.push_back(v);
        } while (u != v);
        
        sccs.push_back(current_scc);
    }
}

void find_sccs(int n) {
    timer = 0;
    sccs.clear();
    // Preencher tin com 0 antes do caso de teste
    for(int i = 0; i <= n; i++) tin[i] = low[i] = 0; 
    
    for (int i = 1; i <= n; i++) {
        if (!tin[i]) dfs_tarjan(i);
    }
}