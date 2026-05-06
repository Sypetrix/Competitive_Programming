#include <bits/stdc++.h>
using namespace std;

void demo_algorithm() {
    vector<int> v = {5, 2, 8, 1, 9, 2};

    // Ordenação e Inversão
    sort(v.begin(), v.end());           // {1, 2, 2, 5, 8, 9}
    sort(v.rbegin(), v.rend());         // decrescente: {9, 8, 5, 2, 2, 1}
    reverse(v.begin(), v.end());        // inverte a ordem atual

    // Operações úteis
    int mx = *max_element(v.begin(), v.end());
    int mn = *min_element(v.begin(), v.end());
    int soma = accumulate(v.begin(), v.end(), 0); // requer <numeric>
    int cnt = count(v.begin(), v.end(), 2);

    // Busca Binária (Obrigatório estar ordenado)
    sort(v.begin(), v.end());
    bool tem = binary_search(v.begin(), v.end(), 5);
    auto lb = lower_bound(v.begin(), v.end(), 5); // iterador pro 1º elemento >= 5
    auto ub = upper_bound(v.begin(), v.end(), 5); // iterador pro 1º elemento > 5

    // Remoção de duplicatas (Obrigatório estar ordenado)
    v.erase(unique(v.begin(), v.end()), v.end());

    // Permutações (Obrigatório estar ordenado para gerar todas)
    next_permutation(v.begin(), v.end());
}