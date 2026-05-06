#include <bits/stdc++.h>
using namespace std;

void demo_set() {
    set<int> s = {30, 10, 20}; // armazenado como {10, 20, 30}
    set<int> b;

    s.insert(40); // O(log N)
    s.insert(20); // ignorado (elementos são únicos)

    s.size(); s.empty(); s.count(20);
    
    auto it = s.find(30);
    if (it != s.end()) s.erase(it);

    s.lower_bound(20); // iterador para o primeiro elemento >= 20
    s.upper_bound(20); // iterador para o primeiro elemento > 20
    s.erase(40);

    for (int x : s) {} // iteração sempre em ordem crescente

    b = s; // cópia direta
    s.clear();
}