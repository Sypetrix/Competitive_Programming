#include <iostream>
using namespace std;

typedef long long ll;

// Retorna a quantidade de números menores que n que são coprimos a n
ll phi(ll n) {
    ll result = n;
    for (ll i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            // Se 'i' é um fator primo, removemos todos os múltiplos dele
            while (n % i == 0) n /= i;
            result -= result / i;
        }
    }
    // Se sobrar algum fator primo maior que sqrt(n)
    if (n > 1) result -= result / n;
    
    return result;
}