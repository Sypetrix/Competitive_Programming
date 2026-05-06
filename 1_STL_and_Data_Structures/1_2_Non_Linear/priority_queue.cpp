#include <bits/stdc++.h>
using namespace std;

// ─── std::priority_queue ─────────────────────────────────────────────────────
void demo_priority_queue() {
    priority_queue<int> pq;                              // Max-Heap (padrão)
    priority_queue<int, vector<int>, greater<int>> mpq;  // Min-Heap

    pq.push(10); pq.push(30); pq.push(20);
    pq.top(); // 30
    pq.pop();

    // Par {distância, nó} — padrão para Dijkstra
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> djk;
    djk.push({0, 1});
}