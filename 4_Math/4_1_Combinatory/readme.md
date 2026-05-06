# Combinatória

Combinatória é o ramo da matemática que conta, agrupa e organiza objetos. Em CP, aparece em problemas de contagem, probabilidade e otimização — muitas vezes a solução é uma fórmula direta, sem nenhum código complexo.

---

## Conceitos Fundamentais

### Permutação, Arranjo e Combinação

| Operação | Ordem importa? | Repetição? | Fórmula |
|---|---|---|---|
| Permutação de N | Sim | Não | $N!$ |
| Arranjo (N, K) | Sim | Não | $\frac{N!}{(N-K)!}$ |
| Combinação (N, K) | Não | Não | $\binom{N}{K} = \frac{N!}{K!(N-K)!}$ |
| Combinação com repetição | Não | Sim | $\binom{N+K-1}{K}$ |

### Propriedades Úteis do Coeficiente Binomial

```
C(N, K) = C(N, N-K)              → simetria
C(N, K) = C(N-1, K-1) + C(N-1, K) → identidade de Pascal
C(N, 0) = C(N, N) = 1
∑ C(N, K) = 2^N                  → soma de todos os subconjuntos
```

---

## Fórmulas Salva-Vidas

Problemas clássicos que se resolvem com a fórmula certa, sem código:

### Stars and Bars (Estrelas e Barras)

*"De quantas formas posso distribuir $N$ bolas idênticas em $K$ caixas distintas?"*

$$\binom{N + K - 1}{K - 1}$$

Se cada caixa deve ter **pelo menos uma** bola:

$$\binom{N - 1}{K - 1}$$

### Desarranjos (Derangements — $!N$)

*"De quantas formas $N$ pessoas podem trocar chapéus de modo que ninguém fique com o seu próprio?"*

Recorrência: $!N = (N-1) \times (!(N-1) + !(N-2))$

Casos base: $!0 = 1$, $!1 = 0$, $!2 = 1$

Aproximação: $!N \approx \frac{N!}{e}$ (arredondado ao inteiro mais próximo para $N \geq 1$)

### Teorema de Catalan

Conta estruturas com "balanceamento": árvores binárias, parênteses válidos, triangulações de polígonos.

$$C_n = \frac{1}{n+1}\binom{2n}{n}$$

Primeiros valores: $1, 1, 2, 5, 14, 42, 132, 429, \ldots$

### Princípio da Multiplicação e da Adição

- **Multiplicação:** eventos independentes em sequência → multiplica as contagens
- **Adição:** eventos mutuamente exclusivos → soma as contagens

### Triângulo de Pascal — Identidades Clássicas

```
Linha N soma 2^N
C(N,1) + C(N,2) + ... = 2^N - 1   (subconjuntos não vazios)
C(N,2) = N*(N-1)/2                 (pares de N elementos)
```

---

## Combinação — nCr em O(1) com Pré-computação

Para consultas repetidas de $\binom{N}{K}$ com módulo primo, pré-compute fatoriais e inversos modulares uma única vez.

<!-- Sugestão de imagem: diagrama mostrando fat[], inv[] e a fórmula final -->

```cpp
// Pré-processa fatoriais e inversos — chame UMA VEZ na main
void precompute() {
    fat[0] = inv[0] = 1;
    for (int i = 1; i < MAX; i++)
        fat[i] = fat[i-1] * i % MOD;
    inv[MAX-1] = fast_pow(fat[MAX-1], MOD - 2);
    for (int i = MAX-2; i >= 1; i--)
        inv[i] = inv[i+1] * (i+1) % MOD;
}

// Consulta em O(1)
ll nCr(int n, int k) {
    if (k < 0 || k > n) return 0;
    return fat[n] * inv[k] % MOD * inv[n-k] % MOD;
}
```

> **Quando usar:** N até ~10⁶, MOD primo. Para N pequeno (≤ 1000) sem módulo, use Pascal.

---

## Combinação — Triângulo de Pascal em O(N²)

Mais simples e funciona para **qualquer módulo** (inclusive sem módulo):

```cpp
// Preenche C[i][j] = C(i, j) para i, j < MAXN
void pascal_triangle() {
    for (int i = 0; i < MAXN; i++) {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++)
            C[i][j] = (C[i-1][j-1] + C[i-1][j]); // % MOD se necessário
    }
}
// Uso: C[n][k]
```

> **Quando usar:** N pequeno (≤ 1000), sem módulo, ou com módulo não-primo.

---

## Números de Catalan

$$C_n = \frac{\binom{2n}{n}}{n+1}$$

A divisão por $n+1$ **deve ser feita com inverso modular** (não divisão inteira direta com MOD):

```cpp
ll catalan(int n) {
    return nCr(2*n, n) * modInverse(n + 1) % MOD;
}
```

---

## Fibonacci em O(log N)

Calcula $F(N) \mod p$ em tempo logarítmico usando a identidade de duplicação:

$$F(2k) = F(k) \cdot [2F(k+1) - F(k)]$$
$$F(2k+1) = F(k)^2 + F(k+1)^2$$

```cpp
pair<ll,ll> fib_fast(ll n) {        // retorna {F(n), F(n+1)}
    if (n == 0) return {0, 1};
    auto [a, b] = fib_fast(n >> 1);
    ll c = a * ((2*b - a + MOD) % MOD) % MOD; // F(2k)
    ll d = (a*a + b*b) % MOD;                  // F(2k+1)
    return (n & 1) ? make_pair(d, (c+d) % MOD)
                   : make_pair(c, d);
}
// ll fn = fib_fast(N).first;
```

> **Quando usar:** N até 10¹⁸. Para N ≤ 10⁶, um simples `for` é suficiente.

---

## Inclusão-Exclusão (PIE)

Conta elementos que satisfazem **pelo menos uma** condição evitando dupla contagem:

$$|A \cup B \cup C| = |A| + |B| + |C| - |A \cap B| - |A \cap C| - |B \cap C| + |A \cap B \cap C|$$

**Regra geral:** soma subconjuntos de tamanho ímpar, subtrai os de tamanho par.

<!-- Sugestão de imagem: diagrama de Venn com 3 conjuntos mostrando as intersecções -->

```cpp
// Conta múltiplos de pelo menos um primo em [1, N]
ll inclusao_exclusao(ll n, const vector<ll>& primes) {
    int k = primes.size();
    ll ans = 0;
    for (int mask = 1; mask < (1 << k); mask++) {
        ll mult = 1;
        int bits = __builtin_popcount(mask);
        for (int i = 0; i < k; i++)
            if (mask & (1 << i)) mult *= primes[i];
        if (bits % 2 == 1) ans += n / mult;
        else               ans -= n / mult;
    }
    return ans;
}
```

> **Limite prático:** funciona bem com até ~20 condições (2²⁰ ≈ 10⁶ subconjuntos). Acima disso, busque DP ou outra abordagem.

---

## Referência Rápida

| Problema | Técnica | Complexidade |
|---|---|---|
| C(N,K) único, N grande | `fast_pow` + fórmula direta | O(log MOD) |
| C(N,K) múltiplas queries | Pré-computar fat/inv | O(N) prep + O(1) query |
| C(N,K) sem módulo, N ≤ 1000 | Pascal | O(N²) |
| F(N) com N ≤ 10¹⁸ | `fib_fast` | O(log N) |
| Contar com múltiplas condições | Inclusão-Exclusão | O(2^K · K) |
| Estruturas balanceadas | Catalan | O(1) com pré-comp |