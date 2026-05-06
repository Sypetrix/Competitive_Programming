#include <bits/stdc++.h>
using namespace std;

// Pré-processa o padrão P e retorna o array pi (failure function) — O(M)
// pi[i] = tamanho do maior prefixo de P[0..i] que também é sufixo
vector<int> compute_pi(const string& P) {
    int m = P.size();
    vector<int> pi(m, 0);
    for (int i = 1, j = 0; i < m; i++) {
        while (j > 0 && P[i] != P[j]) j = pi[j - 1];
        if (P[i] == P[j]) j++;
        pi[i] = j;
    }
    return pi;
}

// Conta ocorrências do padrão P no texto T — O(N + M)
int kmp(const string& T, const string& P) {
    vector<int> pi = compute_pi(P);
    int count = 0;
    for (int i = 0, j = 0; i < (int)T.size(); i++) {
        while (j > 0 && T[i] != P[j]) j = pi[j - 1];
        if (T[i] == P[j]) j++;
        if (j == (int)P.size()) {
            count++;
            j = pi[j - 1]; // prepara para próxima ocorrência
        }
    }
    return count;
}