#include <iostream>
using namespace std;

typedef long long ll;
const int MOD = 1e9 + 7;

// Retorna um par contendo {F(n), F(n+1)}
pair<ll, ll> fib_fast(ll n) {
    if (n == 0) return {0, 1};
    
    auto p = fib_fast(n >> 1); // Calcula para n/2
    ll c = p.first * ((2 * p.second % MOD - p.first + MOD) % MOD) % MOD;
    ll d = (p.first * p.first % MOD + p.second * p.second % MOD) % MOD;
    
    if (n & 1) return {d, (c + d) % MOD}; // Se n for ímpar
    else return {c, d};                   // Se n for par
}

// Como chamar na main:
// ll answer = fib_fast(N).first;