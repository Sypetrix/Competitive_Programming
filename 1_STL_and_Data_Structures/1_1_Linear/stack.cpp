#include <bits/stdc++.h>
using namespace std;

void demo_stack() {
    stack<int> s;
    s.push(10); s.emplace(20); s.push(30);
    s.top();   // lê sem remover
    s.pop();   // remove (não retorna valor — leia top() antes)
    s.size();  s.empty();
    stack<int>().swap(s); // limpa em O(1) — não existe .clear()
}

// ─── Pilha Monotônica: Próximo Elemento Maior — O(N) ────────────────────────
vector<int> proximo_maior(const vector<int>& arr) {
    int n = arr.size();
    vector<int> res(n, -1);
    stack<int> idx;
    for (int i = 0; i < n; i++) {
        while (!idx.empty() && arr[idx.top()] < arr[i]) {
            res[idx.top()] = arr[i];
            idx.pop();
        }
        idx.push(i);
    }
    return res;
}

// ─── Validação de Parênteses ─────────────────────────────────────────────────
bool valida_parenteses(const string& expr) {
    stack<char> s;
    for (char c : expr) {
        if (c == '(' || c == '[' || c == '{') {
            s.push(c);
        } else {
            if (s.empty()) return false;
            char t = s.top();
            if ((c==')' && t=='(') || (c==']' && t=='[') || (c=='}' && t=='{'))
                s.pop();
            else return false;
        }
    }
    return s.empty();
}