#include <bits/stdc++.h>
using namespace std;

void demo_array() {
    array<int, 5> a = {10, 20, 30, 40, 50};
    array<int, 5> b;
    b.fill(0);

    a.size(); a.front(); a.back(); a[2];
    sort(a.begin(), a.end());
    b = a; // cópia direta (impossível com int[])
}