#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

typedef long long ll;

ll tam_crivo;
bitset<10000010> bs; // 10^7 é o limite usual seguro para maratonas
vector<ll> primos;

// 1. Gera os primos (Chamar apenas UMA VEZ no início do main)
void crivo(ll limite) {
    tam_crivo = limite + 1;
    bs.set(); // Marca todos os bits como verdadeiros
    bs[0] = bs[1] = 0; // 0 e 1 não são primos
    
    for (ll i = 2; i <= tam_crivo; i++) {
        if (bs[i]) {
            // Elimina os múltiplos começando em i*i
            for (ll j = i * i; j <= tam_crivo; j += i) bs[j] = 0;
            primos.push_back(i);
        }
    }
}

// 2. Fatorização prima usando a lista pré-calculada
vector<ll> fatoracao(ll N) {
    vector<ll> fatores;
    ll PF_idx = 0, PF = primos[PF_idx];
    
    while (PF * PF <= N) {
        while (N % PF == 0) {
            N /= PF;
            fatores.push_back(PF);
        }
        PF = primos[++PF_idx];
    }
    if (N != 1) fatores.push_back(N); // O resto é um primo maior que sqrt(N)
    return fatores;
}