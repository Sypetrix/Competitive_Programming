# FFT e NTT (Fast Fourier / Number Theoretic Transform)

FFT e NTT multiplicam dois polinômios de grau N em **O(N log N)** em vez de O(N²). Em CP, isso aparece em multiplicação de polinômios, convolução de arrays e multiplicação de inteiros grandes dígito a dígito.

---

## Visão Geral

| Algoritmo | Domínio | Complexidade | Uso típico |
|---|---|---|---|
| FFT | Números complexos | O(N log N) | Convolução geral; cuidado com erros float |
| NTT | Inteiros mod p | O(N log N) | Convolução modular; exata, sem float |

> **Prefira NTT quando houver módulo.** FFT pode acumular erros de ponto flutuante para N grande ou coeficientes grandes — arredonde o resultado com `llround`.

---

## FFT — Fast Fourier Transform

### Implementação iterativa (Cooley-Tukey) — O(N log N)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef complex<double> cd;
const double PI = acos(-1);

// inplace: transforma o vetor entre domínio de tempo e frequência
// invert = false: FFT direta; invert = true: FFT inversa
void fft(vector<cd>& a, bool invert) {
    int n = a.size();
    // Bit-reversal permutation
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    // Butterfly
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i+j], v = a[i+j+len/2] * w;
                a[i+j]         = u + v;
                a[i+j+len/2]   = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) for (cd& x : a) x /= n;
}

// Multiplica os polinômios A e B; retorna coeficientes do produto
vector<long long> multiply(vector<long long>& A, vector<long long>& B) {
    vector<cd> fa(A.begin(), A.end()), fb(B.begin(), B.end());
    int n = 1;
    while (n < (int)(A.size() + B.size())) n <<= 1;
    fa.resize(n); fb.resize(n);

    fft(fa, false); fft(fb, false);
    for (int i = 0; i < n; i++) fa[i] *= fb[i];
    fft(fa, true);

    vector<long long> res(n);
    for (int i = 0; i < n; i++) res[i] = llround(fa[i].real());
    return res;
}
```

---

## NTT — Number Theoretic Transform

A NTT é uma FFT sobre Z_p, onde p é um primo da forma `p = c · 2^k + 1` (primo de NTT). O resultado é **exato** — não há erros de ponto flutuante.

**Primos de NTT comuns:**

| Primo p | g (raiz primitiva) | Máximo N |
|---|---|---|
| 998 244 353 = 119 · 2²³ + 1 | 3 | 2²³ ≈ 8 · 10⁶ |
| 985 661 441 = 235 · 2²² + 1 | 3 | 2²² ≈ 4 · 10⁶ |
| 754 974 721 = 45 · 2²⁴ + 1 | 11 | 2²⁴ ≈ 1.6 · 10⁷ |

### Implementação — O(N log N)

```cpp
const long long MOD = 998244353;
const long long g_root = 3; // raiz primitiva de MOD

long long power(long long a, long long b, long long mod) {
    long long res = 1; a %= mod;
    for (; b > 0; b >>= 1) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
    }
    return res;
}

void ntt(vector<long long>& a, bool invert) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        long long w = invert ? power(g_root, MOD - 1 - (MOD-1)/len, MOD)
                             : power(g_root, (MOD-1)/len, MOD);
        for (int i = 0; i < n; i += len) {
            long long wn = 1;
            for (int j = 0; j < len/2; j++) {
                long long u = a[i+j];
                long long v = a[i+j+len/2] * wn % MOD;
                a[i+j]       = (u + v) % MOD;
                a[i+j+len/2] = (u - v + MOD) % MOD;
                wn = wn * w % MOD;
            }
        }
    }
    if (invert) {
        long long n_inv = power(n, MOD - 2, MOD);
        for (long long& x : a) x = x * n_inv % MOD;
    }
}

vector<long long> multiply_mod(vector<long long> A, vector<long long> B) {
    int result_size = A.size() + B.size() - 1;
    int n = 1;
    while (n < result_size) n <<= 1;
    A.resize(n); B.resize(n);

    ntt(A, false); ntt(B, false);
    for (int i = 0; i < n; i++) A[i] = A[i] * B[i] % MOD;
    ntt(A, true);

    A.resize(result_size);
    return A;
}
```

---

## Multiplicação de Inteiros Grandes

Para multiplicar dois inteiros grandes representados como vetores de dígitos:

```cpp
// Multiplica dois números não-negativos representados por dígitos (base 10)
vector<int> multiply_big(vector<int>& num1, vector<int>& num2) {
    vector<long long> a(num1.begin(), num1.end());
    vector<long long> b(num2.begin(), num2.end());
    auto res = multiply(a, b); // FFT/NTT multiply

    // Propaga carries
    for (int i = 0; i + 1 < (int)res.size(); i++) {
        res[i+1] += res[i] / 10;
        res[i]   %= 10;
    }
    // Remove zeros à esquerda
    while (res.size() > 1 && res.back() == 0) res.pop_back();
    return vector<int>(res.begin(), res.end());
}
```

---

## Aplicações em CP

### Convolução de Arrays

Dado `A[i]` e `B[j]`, calcular `C[k] = Σ A[i]·B[k-i]`:

```cpp
// Diretamente: multiply(A, B)[k] = C[k]
```

### Contar Pares com Soma S

```cpp
// Seja freq_A[x] = número de elementos iguais a x em A
// Seja freq_B[x] = número de elementos iguais a x em B
// conv[s] = número de pares (a, b) com a + b = s
auto conv = multiply(freq_A, freq_B);
long long pairs_with_sum_S = conv[S];
```

### Verificar se Soma S é Alcançável (Subconjunto)

Use FFT com `freq[x] = 1 se x ∈ conjunto, 0 caso contrário` e verifique se `conv[S] > 0`.

### String Matching com Wildcards — O(N log N)

Para verificar se padrão P (com wildcards `*`) ocorre em texto T:

```cpp
// Para cada posição i, compute:
// f(i) = Σⱼ T[i+j] · P[j] · (T[i+j] - P[j])²
// f(i) == 0 ⟺ P ocorre a partir de i (ignorando wildcards onde P[j] = 0)
```

---

## Escolhendo Entre FFT e NTT

```
O resultado deve ser mod p?
├── p é um primo de NTT (998244353, etc.) → NTT direto
├── p é outro primo → NTT com CRT (3 NTTs, combinar via CRT)
└── Sem módulo → FFT (arredondar com llround)

Os coeficientes são muito grandes (produto > 10^15)?
└── Use NTT para evitar perda de precisão da FFT

N é muito grande (> 10^6)?
└── NTT com MOD = 998244353 suporta até N = 2^23 ≈ 8·10^6
```

> **Tamanho do array:** sempre arredonde para a próxima potência de 2. O tamanho necessário para multiplicar A (grau N) por B (grau M) é pelo menos N + M − 1.