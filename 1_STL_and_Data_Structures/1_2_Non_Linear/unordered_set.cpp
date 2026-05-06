#include <bits/stdc++.h>
using namespace std;

// ─── std::unordered_set (Tabela Hash) ────────────────────────────────────────
void demo_unordered_set() {
    unordered_set<int> us;

    // Inserção e busca média O(1)
    us.insert(10);
    us.insert(20);
    us.insert(10); // Duplicatas são ignoradas

    if (us.count(10)) { /* elemento existe */ }
    
    // Retorna iterator para o elemento ou us.end()
    auto it = us.find(20); 

    us.erase(10); // Remove por valor
    us.size(); us.empty();
    
    // Nota: Em maratonas, o unordered_set pode sofrer TLE por colisões 
    // propositais. Se isso ocorrer, use um Custom Hash ou std::set O(log N).
}