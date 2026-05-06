#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const int MAX = 1e6 + 5;
const int MOD = 1e9 + 7;

ll fat[MAX];
ll inv[MAX]; // Inverso modular do fatorial

// Exponenciação Rápida: (base^exp) % MOD em O(log exp)
ll fast_pow(ll base, ll exp) {
    ll res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

// Inverso modular usando Fermat (só funciona se MOD for primo)
ll modInverse(ll n) {
    return fast_pow(n, MOD - 2);
}

// Chame precompute() UMA VEZ no início da main!
void precompute() {
    fat[0] = 1;
    inv[0] = 1;
    for (int i = 1; i < MAX; i++) {
        fat[i] = (fat[i - 1] * i) % MOD;
    }
    // Calcula o inverso do último elemento
    inv[MAX - 1] = modInverse(fat[MAX - 1]);
    
    // Calcula os outros inversos de trás para frente em O(1) cada
    for (int i = MAX - 2; i >= 1; i--) {
        inv[i] = (inv[i + 1] * (i + 1)) % MOD;
    }
}

// Calcula Combinação N escolhe K em O(1)
ll nCr(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fat[n] * inv[k] % MOD * inv[n - k] % MOD;
}

ll catalan(int n) {
    ll comb = nCr(2 * n, n);
    ll div = modInverse(n + 1); // Tem que dividir usando inverso modular!
    return (comb * div) % MOD;
}