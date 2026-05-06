#include <bits/stdc++.h>
using namespace std;

void demo_vector() {
    vector<int> v1;
    vector<int> v2(5, -1);              // {-1, -1, -1, -1, -1}
    vector<int> v = {4, 1, 3, 5, 2};
    vector<vector<int>> mat(3, vector<int>(4, 0)); // matriz 3x4

    v.push_back(6);   // O(1) amortizado
    v.emplace_back(7);
    v.pop_back();

    v.size(); v.empty();
    v.front(); v.back(); v[2];

    sort(v.begin(), v.end());
    sort(v.rbegin(), v.rend());  // decrescente
    reverse(v.begin(), v.end());

    // Remove duplicatas
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());

    v.erase(v.begin() + 2);       // remove índice 2 — O(N)
    v.insert(v.begin() + 1, 99);  // insere 99 no índice 1 — O(N)
    v.resize(10);
    v.clear();
}