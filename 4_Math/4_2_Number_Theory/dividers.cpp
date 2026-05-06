#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
typedef long long ll;

extern vector<ll> primos; // Requer o Crivo de Eratóstenes já executado

// Retorna a QUANTIDADE de divisores de N
ll numDiv(ll N) {
    ll ans = 1; // Pela combinatória, começamos com 1
    ll PF_idx = 0, PF = primos[PF_idx];
    
    while (PF * PF <= N) {
        ll power = 0; // Quantas vezes o primo 'PF' divide N?
        while (N % PF == 0) {
            N /= PF;
            power++;
        }
        ans *= (power + 1); // Regra do produto da combinatória
        PF = primos[++PF_idx];
    }
    // Se sobrou um primo gigante
    if (N != 1) ans *= 2; 
    
    return ans;
}

// Retorna a SOMA de todos os divisores de N
ll sumDiv(ll N) {
    ll ans = 1;
    ll PF_idx = 0, PF = primos[PF_idx];
    
    while (PF * PF <= N) {
        ll power = 0;
        while (N % PF == 0) { N /= PF; power++; }
        
        // Fórmula da soma da PG geométrica para o fator primo
        ans *= ((ll)pow((double)PF, power + 1.0) - 1) / (PF - 1);
        PF = primos[++PF_idx];
    }
    if (N != 1) {
        ans *= ((ll)pow((double)N, 2.0) - 1) / (N - 1);
    }
    return ans;
}