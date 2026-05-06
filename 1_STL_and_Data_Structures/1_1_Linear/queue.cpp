#include <bits/stdc++.h>
using namespace std;

void demo_queue() {
    queue<int> q;
    q.push(10); 
    q.emplace(20); 
    q.push(30);
    q.front(); 
    q.back();
    q.pop();   // remove a frente (não retorna valor — leia front() antes)
    q.size();  
    q.empty();
    queue<int>().swap(q); // limpa em O(1) — não existe .clear()
}