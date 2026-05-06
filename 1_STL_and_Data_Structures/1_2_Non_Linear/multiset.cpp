#include <bits/stdc++.h>
using namespace std;

// ─── std::multiset (Árvore Balanceada - Permite Duplicatas) ──────────────────
void demo_multiset() {
    multiset<int> ms;

    // Inserção O(log N)
    ms.insert(10);
    ms.insert(20);
    ms.insert(10); // Agora existem dois '10' no conjunto

    // count(val) retorna a quantidade de vezes que o valor aparece - O(qtd + log N)
    int qtd = ms.count(10); 

    // ATENÇÃO NO ERASE:
    ms.erase(10);    // Remove TODAS as instâncias de 10
    
    auto it = ms.find(10);
    if (it != ms.end()) {
        ms.erase(it); // Remove apenas UMA instância (via iterator)
    }

    // Menor e Maior elemento
    int menor = *ms.begin();
    int maior = *ms.rbegin();

    // lower_bound e upper_bound funcionam como no set comum
    auto lb = ms.lower_bound(15); 
}