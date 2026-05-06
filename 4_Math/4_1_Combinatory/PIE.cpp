#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;

// Conta quantos números de 1 a N são múltiplos de pelo menos um primo da lista
ll inclusao_exclusao(ll n, const vector<ll>& primes) {
    int k = primes.size();
    ll ans = 0;
    
    // Itera por todos os subconjuntos de primos (de 1 até 2^k - 1)
    for (int mask = 1; mask < (1 << k); mask++) {
        ll mult = 1;
        int bits = 0;
        
        for (int i = 0; i < k; i++) {
            if (mask & (1 << i)) {
                bits++;
                mult *= primes[i]; // Multiplica os elementos do subconjunto
            }
        }
        
        // Se o subconjunto tem tamanho ímpar, soma. Se par, subtrai.
        if (bits % 2 == 1) {
            ans += (n / mult);
        } else {
            ans -= (n / mult);
        }
    }
    return ans;
}