#include <bits/stdc++.h>
using namespace std;

void demo_pair_tuple() {
    // std::pair: agrupa exatamente dois elementos
    pair<int, string> p = {10, "uva"};
    p.first = 20; 
    p.second = "maca";

    // std::tuple: agrupa N elementos
    tuple<int, string, double> t = {1, "Maratona", 9.8};
    get<0>(t) = 10; // acesso e modificação por índice
    
    // Comparação lexicográfica nativa (útil para ordenação automática)
    pair<int, int> a = {1, 10}, b = {2, 1};
    if (a < b) {} // Compara o primeiro; se empatar, compara o segundo

    // Uso comum: Vetor de pares ordena por X e depois por Y
    vector<pair<int, int>> v = {{3, 5}, {1, 2}, {3, 2}};
    sort(v.begin(), v.end()); // Resultado: {1, 2}, {3, 2}, {3, 5}

    // Structured Bindings (C++17): descompactação rápida
    auto [id, nome, nota] = t;
    
    // tie e ignore: extração seletiva
    int v1;
    tie(v1, ignore, ignore) = t; // captura apenas o primeiro elemento
}