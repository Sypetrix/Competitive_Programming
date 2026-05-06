#include <bits/stdc++.h>
using namespace std;

// ─── std::unordered_map (Tabela Hash) ────────────────────────────────────────
void demo_unordered_map() {
    unordered_map<string, int> um;

    // Acesso e inserção O(1) médio
    um["brasília"] = 61;
    um["viçosa"] = 31;

    // Se a chave não existir, o operador [] cria com valor padrão (ex: 0)
    int ddd = um["rio"]; 

    if (um.find("viçosa") != um.end()) {
        // Chave encontrada
    }

    um.erase("brasília");
    um.size();
    
    // Percorrendo (ordem é imprevisível)
    for (auto const& [cidade, ddd] : um) {
        // cidade (key), ddd (value)
    }
}