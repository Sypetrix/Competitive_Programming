#include <bits/stdc++.h>
using namespace std;

struct FenwickTree {
    int n;
    vector<int> bit;

    FenwickTree(int n) : n(n) {
        bit.assign(n + 1, 0); // 1-based
    }

    // Adiciona 'val' na posição 'idx'
    void add(int idx, int val) {
        for (; idx <= n; idx += (idx & -idx))
            bit[idx] += val;
    }

    // Retorna a soma do prefixo [1, idx]
    int query(int idx) {
        int sum = 0;
        for (; idx > 0; idx -= (idx & -idx))
            sum += bit[idx];
        return sum;
    }

    // Retorna a soma no intervalo [l, r]
    int queryRange(int l, int r) {
        return query(r) - query(l - 1);
    }
};

void demo_fenwick() {
    FenwickTree ft(10); // Cria uma BIT para 10 elementos
    
    ft.add(3, 5);       // Adiciona 5 na posição 3
    ft.add(5, 2);       // Adiciona 2 na posição 5
    
    int s1 = ft.query(5);         // Soma de 1 até 5 (resultado: 7)
    int s2 = ft.queryRange(4, 5); // Soma de 4 até 5 (resultado: 2)
}