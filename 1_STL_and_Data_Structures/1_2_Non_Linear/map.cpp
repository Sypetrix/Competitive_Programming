#include <bits/stdc++.h>
using namespace std;

void demo_map() {
    map<string, int> m = {{"uva", 3}, {"maca", 8}};
    map<string, int> b;

    m["banana"] = 5; // insere ou atualiza
    m.insert({"laranja", 10});

    m.size(); m.empty(); m.count("laranja"); m["banana"];
    
    auto it = m.find("uva");
    if (it != m.end()) m.erase(it);

    m.lower_bound("m"); // O(log N)
    m.erase("banana");

    for (auto const& [chave, valor] : m) {} // iteração sempre ordenada pela chave

    b = m; // cópia direta
    m.clear();
}