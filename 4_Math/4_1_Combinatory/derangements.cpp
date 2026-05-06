#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
const ll MOD = 1e9 + 7;

// ==============================================================================
// DESARRANJOS (Derangements — !N)
// Complexidade: O(N) para pré-computar, O(1) por consulta
// ==============================================================================
// Definição:
//   Um desarranjo de N elementos é uma permutação em que NENHUM elemento
//   ocupa sua posição original. !N conta quantas dessas permutações existem.
//
// Recorrência:
//   !0 = 1  (o conjunto vazio tem exatamente uma permutação "vazia")
//   !1 = 0  (um único elemento sempre ocupa a própria posição)
//   !N = (N - 1) * (!(N-1) + !(N-2))
//
// Intuição da recorrência:
//   Coloca-se o elemento N em alguma posição k (N-1 escolhas).
//   - Se k troca com N (elemento k vai para posição N): restam !(N-2) formas.
//   - Se k NÃO vai para a posição N: os N-1 elementos restantes formam um
//     subproblema de tamanho N-1 (cada um "proibido" de uma posição) → !(N-1).
//
// Aproximação para N grande:
//   !N ≈ N! / e  (arredondado ao inteiro mais próximo para N ≥ 1)
// ==============================================================================

const int MAXN = 1e6 + 5;
ll derang[MAXN]; // derang[i] = !i mod MOD

// Pré-computa todos os desarranjos de 0 até 'limite' — chame UMA VEZ na main
void precompute_derangements(int limite) {
    derang[0] = 1;
    derang[1] = 0;
    for (int i = 2; i <= limite; i++) {
        derang[i] = (ll)(i - 1) % MOD * ((derang[i - 1] + derang[i - 2]) % MOD) % MOD;
    }
}

// Versão recursiva com memoização (útil para consultas esparsas)
ll memo[MAXN];

ll derangement(int n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (memo[n] != -1) return memo[n];
    return memo[n] = (ll)(n - 1) % MOD * ((derangement(n - 1) + derangement(n - 2)) % MOD) % MOD;
}

// Fórmula de Inclusão-Exclusão direta (sem pré-computação, requer fat[])
// !N = N! * sum_{k=0}^{N} (-1)^k / k!
// Para uso com módulo, requer inverso modular de cada k!

// Como chamar na main:
// precompute_derangements(MAXN - 1);
// cout << derang[N] << "\n";
//
// Ou com memoização:
// memset(memo, -1, sizeof(memo));
// cout << derangement(N) << "\n";