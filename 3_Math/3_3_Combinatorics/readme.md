# Combinatória (Combinatorics)

Combinatória é o ramo da matemática que conta, agrupa e organiza objetos. Em CP, aparece em problemas de contagem, probabilidade e otimização — muitas vezes a solução é uma fórmula direta, sem nenhum código complexo.

---

## Conceitos Fundamentais

| Operação | Ordem importa? | Repetição? | Fórmula |
|---|---|---|---|
| Permutação de N | Sim | Não | N! |
| Arranjo (N, K) | Sim | Não | N! / (N−K)! |
| Combinação (N, K) | Não | Não | C(N,K) = N! / (K!(N−K)!) |
| Combinação com repetição | Não | Sim | C(N+K−1, K) |
| Permutação com repetição | Sim | Sim | N! / (n₁! · n₂! · …) |

---

## Coeficiente Binomial — nCr

### Pré-computação com Fatoriais — O(N) prep + O(1) query

Para N até ~10⁶ com MOD primo. Requer `precompute()` chamado uma vez na `main`.

```cpp
const int MAX = 1e6 + 5;
const long long MOD = 1e9 + 7;
long long fat[MAX], inv_fat[MAX];

long long fast_pow(long long b, long long e, long long m) {
    long long r = 1; b %= m;
    for (; e > 0; e >>= 1) { if (e & 1) r = r * b % m; b = b * b % m; }
    return r;
}

void precompute() {
    fat[0] = 1;
    for (int i = 1; i < MAX; i++) fat[i] = fat[i-1] * i % MOD;
    inv_fat[MAX-1] = fast_pow(fat[MAX-1], MOD - 2, MOD);
    for (int i = MAX-2; i >= 0; i--) inv_fat[i] = inv_fat[i+1] * (i+1) % MOD;
}

long long nCr(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fat[n] * inv_fat[k] % MOD * inv_fat[n-k] % MOD;
}
```

### Triângulo de Pascal — O(N²)

Para N ≤ ~1000, sem módulo, ou com qualquer módulo (inclusive não primo):

```cpp
const int MAXN = 1005;
long long C[MAXN][MAXN];

void pascal() {
    for (int i = 0; i < MAXN; i++) {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++)
            C[i][j] = (C[i-1][j-1] + C[i-1][j]); // % MOD se necessário
    }
}
// Uso: C[n][k]
```

### Identidades Úteis

```
C(N, K) = C(N, N-K)                  ← simetria
C(N, K) = C(N-1, K-1) + C(N-1, K)   ← identidade de Pascal
C(N, 1) + C(N, 2) + ... = 2^N - 1   ← soma dos subconjuntos não vazios
C(N, 2) = N(N-1)/2                   ← número de pares
```

---

## Stars and Bars (Estrelas e Barras)

Distribui N objetos idênticos em K recipientes distintos.

| Variante | Fórmula |
|---|---|
| Recipientes podem ser vazios | C(N+K−1, K−1) |
| Cada recipiente com pelo menos 1 | C(N−1, K−1) |

---

## Números de Catalan

Conta estruturas "balanceadas": pares de parênteses válidos, árvores binárias completas, triangulações de polígonos, caminhos que não cruzam a diagonal.

```
C_n = C(2n, n) / (n+1)
```

Primeiros valores: 1, 1, 2, 5, 14, 42, 132, 429, …

```cpp
long long catalan(int n) {
    return nCr(2*n, n) * fast_pow(n + 1, MOD - 2, MOD) % MOD;
}
```

> A divisão por (n+1) **deve** usar inverso modular — não divisão inteira direta com MOD.

---

## Fibonacci em O(log N)

Calcula F(N) mod p em O(log N) usando a identidade de duplicação:

```
F(2k)   = F(k) · [2·F(k+1) − F(k)]
F(2k+1) = F(k)² + F(k+1)²
```

```cpp
pair<long long,long long> fib(long long n) { // retorna {F(n), F(n+1)}
    if (n == 0) return {0, 1};
    auto [a, b] = fib(n >> 1);
    long long c = a * ((2*b - a + MOD) % MOD) % MOD;
    long long d = (a*a + b*b) % MOD;
    return (n & 1) ? make_pair(d, (c+d) % MOD) : make_pair(c, d);
}
// F(N) = fib(N).first
```

---

## Desarranjos (Derangements — !N)

Permutações onde **nenhum** elemento está na posição original.

```
!0 = 1,  !1 = 0,  !N = (N−1) · (!(N−1) + !(N−2))
```

Aproximação: !N ≈ N! / e (arredondado ao inteiro mais próximo para N ≥ 1).

```cpp
vector<long long> derang(int N) {
    vector<long long> d(N + 1);
    d[0] = 1; d[1] = 0;
    for (int i = 2; i <= N; i++)
        d[i] = (long long)(i-1) % MOD * ((d[i-1] + d[i-2]) % MOD) % MOD;
    return d;
}
```

---

## Inclusão-Exclusão (PIE)

Conta elementos que satisfazem **pelo menos uma** de K condições, evitando dupla contagem.

```
|A₁ ∪ A₂ ∪ … ∪ Aₖ| = Σ|Aᵢ| − Σ|Aᵢ∩Aⱼ| + Σ|Aᵢ∩Aⱼ∩Aₖ| − …
```

Regra: soma subconjuntos de tamanho ímpar, subtrai os de tamanho par.

```cpp
// Conta múltiplos de pelo menos um primo em [1, N]
long long pie(long long n, const vector<long long>& primes) {
    int k = primes.size();
    long long ans = 0;
    for (int mask = 1; mask < (1 << k); mask++) {
        long long mult = 1;
        int bits = __builtin_popcount(mask);
        for (int i = 0; i < k; i++)
            if (mask & (1 << i)) mult *= primes[i];
        if (bits % 2 == 1) ans += n / mult;
        else               ans -= n / mult;
    }
    return ans;
}
```

> Limite prático: funciona bem com até ~20 condições (2²⁰ ≈ 10⁶ subconjuntos).

---

## Referência Rápida

| Problema | Técnica | Complexidade |
|---|---|---|
| C(N,K) mod p, N até 10⁶ | Pré-computar fat/inv_fat | O(N) prep + O(1) |
| C(N,K) sem módulo, N ≤ 1000 | Pascal | O(N²) |
| C(N,K) único, N grande | fast_pow + fórmula direta | O(log MOD) |
| F(N) com N até 10¹⁸ | fib rápido | O(log N) |
| Estruturas balanceadas | Catalan | O(1) com pré-comp |
| Contagem com múltiplas condições | Inclusão-Exclusão | O(2^K · K) |
| Permutação sem ponto fixo | Desarranjos | O(N) pré-comp |