#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;
long long tree[4 * MAXN];
long long lazy[4 * MAXN];
long long arr[MAXN];

// Constrói a SegTree em O(N)
void build(int node, int start, int end) {
    lazy[node] = 0; // Limpa o lazy inicial
    if (start == end) {
        tree[node] = arr[start];
        return;
    }
    int mid = (start + end) / 2;
    build(2 * node, start, mid);         // Filho esquerdo
    build(2 * node + 1, mid + 1, end);     // Filho direito
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

// Empurra a atualização pendente para os filhos
void push(int node, int start, int end) {
    if (lazy[node] != 0) {
        tree[node] += lazy[node] * (end - start + 1); // Atualiza o nó atual
        
        if (start != end) { // Se não for folha, repassa a preguiça pros filhos
            lazy[2 * node] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
        }
        lazy[node] = 0; // Limpa a preguiça do nó atual
    }
}

// Atualiza o intervalo [l, r] somando 'val'
void updateRange(int node, int start, int end, int l, int r, long long val) {
    push(node, start, end); // Sempre verifica o lazy antes de olhar o nó
    
    // Totalmente fora do intervalo
    if (start > end || start > r || end < l) return;
    
    // Totalmente dentro do intervalo
    if (start >= l && end <= r) {
        lazy[node] += val;
        push(node, start, end);
        return;
    }
    
    // Parcialmente dentro: divide e conquista
    int mid = (start + end) / 2;
    updateRange(2 * node, start, mid, l, r, val);
    updateRange(2 * node + 1, mid + 1, end, l, r, val);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

// Consulta a soma no intervalo [l, r]
long long queryRange(int node, int start, int end, int l, int r) {
    push(node, start, end); // Garante que o nó está atualizado
    
    // Totalmente fora
    if (start > end || start > r || end < l) return 0;
    
    // Totalmente dentro
    if (start >= l && end <= r) return tree[node];
    
    int mid = (start + end) / 2;
    long long p1 = queryRange(2 * node, start, mid, l, r);
    long long p2 = queryRange(2 * node + 1, mid + 1, end, l, r);
    return p1 + p2;
}

// Como chamar na main (considerando N elementos 1-based):
// build(1, 1, N);
// updateRange(1, 1, N, L, R, valor);
// queryRange(1, 1, N, L, R);