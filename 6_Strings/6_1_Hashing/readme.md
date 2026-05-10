# Hashing de Strings (String Hashing)

O Hashing de Strings transforma substrings em números inteiros, permitindo **comparar duas substrings em O(1)** após pré-computação de O(N). É a ferramenta mais versátil para comparação de substrings, detecção de repetições e busca de padrões.

---

## Visão Geral

| Operação | Complexidade |
|---|---|
| Pré-computação | O(N) |
| Hash de s[l..r] | O(1) |
| Comparação de duas substrings | O(1) |
| Busca de padrão em texto | O(N + M) |
| LCP entre dois sufixos (com busca binária) | O(log N) |

---

## Polynomial Rolling Hash

Cada caractere é multiplicado por uma potência da base `B` e somado módulo um primo `MOD`. O hash de `s[l..r]` é obtido por subtração de prefixos:

```cpp
const long long MOD1 = 1e9 + 7, MOD2 = 1e9 + 9;
const long long B1   = 131,     B2   = 137;
const int MAXN = 1e5 + 5;

long long h1[MAXN], h2[MAXN];
long long pw1[MAXN], pw2[MAXN];

void build(const string& s) {
    int n = s.size();
    pw1[0] = pw2[0] = 1;
    h1[0]  = h2[0]  = 0;
    for (int i = 0; i < n; i++) {
        pw1[i+1] = pw1[i] * B1 % MOD1;
        pw2[i+1] = pw2[i] * B2 % MOD2;
        h1[i+1]  = (h1[i] * B1 + s[i]) % MOD1;
        h2[i+1]  = (h2[i] * B2 + s[i]) % MOD2;
    }
}

// Hash de s[l..r] — 0-indexed, inclusive
pair<long long,long long> get(int l, int r) {
    long long v1 = (h1[r+1] - h1[l] * pw1[r-l+1] % MOD1 + MOD1*2) % MOD1;
    long long v2 = (h2[r+1] - h2[l] * pw2[r-l+1] % MOD2 + MOD2*2) % MOD2;
    return {v1, v2};
}

bool equal_sub(int l1, int r1, int l2, int r2) {
    return get(l1, r1) == get(l2, r2);
}
```

> O `+ MOD * 2` garante que a subtração nunca resulte em valor negativo, independente do compilador.

---

## Por que Hashing Duplo?

Com um único módulo de ~10⁹, a probabilidade de colisão por par de strings é ~10⁻⁹. Em juízes adversariais (Codeforces), é possível construir entradas que forçam colisões sistematicamente. Com dois módulos independentes, a probabilidade cai para ~10⁻¹⁸, tornando ataques impraticáveis.

---

## Busca de Padrão em Texto — O(N + M)

```cpp
vector<int> find_pattern(const string& text, const string& pat) {
    int n = text.size(), m = pat.size();
    vector<int> occ;

    // Pré-computa hash do texto
    build(text);

    // Computa hash do padrão separadamente
    long long ph1 = 0, ph2 = 0;
    for (char c : pat) {
        ph1 = (ph1 * B1 + c) % MOD1;
        ph2 = (ph2 * B2 + c) % MOD2;
    }

    for (int i = 0; i + m <= n; i++) {
        auto [v1, v2] = get(i, i + m - 1);
        if (v1 == ph1 && v2 == ph2) occ.push_back(i);
    }
    return occ;
}
```

---

## LCP com Busca Binária — O(log N)

Comprimento do maior prefixo comum entre `s[l1..]` e `s[l2..]`:

```cpp
int lcp(int l1, int l2, int n) {
    int lo = 0, hi = min(n - l1, n - l2);
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (equal_sub(l1, l1+mid-1, l2, l2+mid-1)) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}
```

---

## Verificar se Substring é Palíndromo — O(1)

Mantendo o hash do reverso da string:

```cpp
long long hr1[MAXN], hr2[MAXN]; // hash do reverso

void build_both(const string& s) {
    build(s);
    string r(s.rbegin(), s.rend());
    // Preenche hr1/hr2 com a mesma lógica de build() usando r
}

bool is_palindrome(int l, int r, int n) {
    // Posição correspondente no reverso
    int rl = n - 1 - r, rr = n - 1 - l;
    long long fwd1 = get(l, r).first,  fwd2 = get(l, r).second;
    long long rev1 = get_rev(rl, rr).first, rev2 = get_rev(rl, rr).second;
    return fwd1 == rev1 && fwd2 == rev2;
}
```

---

## Armadilhas Comuns

| Problema | Solução |
|---|---|
| Colisões em juízes adversariais | Use hashing duplo (dois módulos independentes) |
| Resultado negativo após subtração | Adicione `MOD * 2` antes do `% MOD` |
| Overflow em `h[i] * pw[j]` | Garanta que ambos os operandos são `long long` |
| Comparar strings de comprimentos diferentes | Verifique `r1-l1 == r2-l2` antes de comparar hashes |
| Base muito pequena (B = 31 ou 26) | Use B ≥ 131 para reduzir colisões com strings longas |