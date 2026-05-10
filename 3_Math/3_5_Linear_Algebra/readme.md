# Álgebra Linear (Linear Algebra)

Álgebra Linear em CP tem dois usos principais: **Exponenciação de Matrizes** para resolver recorrências lineares com N até 10¹⁸, e **Eliminação Gaussiana** para sistemas de equações lineares (incluindo mod p e no campo GF(2)).

---

## Visão Geral

| Técnica | Complexidade | Uso típico |
|---|---|---|
| Exponenciação de Matrizes | O(M³ log N) | Recorrências lineares com N até 10¹⁸ |
| Eliminação Gaussiana (real) | O(N² M) | Sistemas Ax = b com floats |
| Eliminação Gaussiana (mod p) | O(N² M) | Sistemas lineares em aritmética modular |
| Eliminação Gaussiana (GF(2)) | O(N² / 64) com bitset | Base XOR, independência linear sobre bits |

*(M = tamanho da matriz; N = expoente/número de variáveis)*

---

## Exponenciação de Matrizes — O(M³ log N)

Resolve qualquer recorrência linear da forma:

```
[f(n+1)]   [a b c]   [f(n)  ]
[f(n)  ] = [1 0 0] × [f(n-1)]
[f(n-1)]   [0 1 0]   [f(n-2)]
```

Elevando a **matriz de transição** ao expoente N usando exponenciação rápida (igual a fast_pow, mas com multiplicação de matrizes no lugar de multiplicação escalar).

```cpp
typedef vector<vector<long long>> Matrix;
const long long MOD = 1e9 + 7;

Matrix multiply(const Matrix& A, const Matrix& B) {
    int n = A.size(), m = B[0].size(), k = B.size();
    Matrix C(n, vector<long long>(m, 0));
    for (int i = 0; i < n; i++)
        for (int p = 0; p < k; p++) if (A[i][p])
            for (int j = 0; j < m; j++)
                C[i][j] = (C[i][j] + A[i][p] * B[p][j]) % MOD;
    return C;
}

Matrix mat_pow(Matrix A, long long p) {
    int n = A.size();
    Matrix R(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++) R[i][i] = 1; // identidade
    while (p > 0) {
        if (p & 1) R = multiply(R, A);
        A = multiply(A, A);
        p >>= 1;
    }
    return R;
}
```

### Exemplo: N-ésimo Fibonacci — O(log N)

```
[F(n+1)]   [1 1]^n   [F(1)]   [1 1]^n   [1]
[F(n)  ] = [1 0]   × [F(0)] = [1 0]   × [0]
```

```cpp
long long fibonacci(long long n) {
    if (n == 0) return 0;
    Matrix T = {{1, 1}, {1, 0}};
    Matrix R = mat_pow(T, n);
    return R[0][1]; // F(n)
}
```

### Recorrência Geral

Para qualquer recorrência `f(n) = c₁f(n-1) + c₂f(n-2) + ... + cₖf(n-k)`:

**Matriz de transição (k×k):**
```
[c₁ c₂ c₃ ... cₖ]
[1  0  0  ...  0 ]
[0  1  0  ...  0 ]
[0  0  1  ...  0 ]
...
```

**Vetor de estado inicial:**
```
[f(k), f(k-1), ..., f(1)]ᵀ
```

**Resultado:** `(T^(n-k) × estado_inicial)[0]` = f(n)

### Exemplo: Caminhos em Grafo

Número de caminhos de comprimento exato K entre vértices u e v em grafo com N nós = `(Aᴷ)[u][v]`, onde A é a matriz de adjacência.

```cpp
long long caminhos(const Matrix& adj, int u, int v, long long K) {
    Matrix R = mat_pow(adj, K);
    return R[u][v];
}
```

---

## Montando a Matriz de Transição

O passo mais difícil é modelar o problema como uma multiplicação de matrizes. Regras gerais:

1. **Identifique o vetor de estado:** quais valores precisam ser propagados?
2. **Escreva a transição:** como cada componente do estado em n+1 depende do estado em n?
3. **Construa a matriz:** cada linha i da matriz descreve como calcular o componente i do próximo estado.

**Exemplo — tribonacci:** `f(n) = f(n-1) + f(n-2) + f(n-3)`

```
Estado: [f(n), f(n-1), f(n-2)]

Transição:
f(n+1)   = 1·f(n) + 1·f(n-1) + 1·f(n-2)
f(n)     = 1·f(n) + 0·f(n-1) + 0·f(n-2)
f(n-1)   = 0·f(n) + 1·f(n-1) + 0·f(n-2)

Matriz:
[1 1 1]
[1 0 0]
[0 1 0]
```

---

## Eliminação Gaussiana — O(N²M)

Resolve o sistema Ax = b reduzindo a matriz aumentada [A|b] à forma escalonada.

### Versão com Floats

```cpp
const double EPS = 1e-9;

// Retorna o rank do sistema. Modifica a e b in-place.
// Se sem solução: retorna -1. Se infinitas soluções: rank < n.
int gauss(vector<vector<double>>& a, vector<double>& b) {
    int n = a.size(), m = a[0].size();
    int row = 0;
    for (int col = 0; col < m && row < n; col++) {
        // Encontra pivô (maior valor absoluto na coluna)
        int pivot = row;
        for (int i = row + 1; i < n; i++)
            if (abs(a[i][col]) > abs(a[pivot][col])) pivot = i;

        if (abs(a[pivot][col]) < EPS) continue; // coluna toda zero

        swap(a[row], a[pivot]);
        swap(b[row], b[pivot]);

        // Normaliza a linha do pivô
        double div = a[row][col];
        for (int j = col; j < m; j++) a[row][j] /= div;
        b[row] /= div;

        // Elimina coluna nas outras linhas
        for (int i = 0; i < n; i++) {
            if (i == row || abs(a[i][col]) < EPS) continue;
            double factor = a[i][col];
            for (int j = col; j < m; j++) a[i][j] -= factor * a[row][j];
            b[i] -= factor * b[row];
        }
        row++;
    }
    return row; // rank
}
```

### Versão Modular (mod p primo)

Substitua `double` por `long long`, EPS por `== 0` e divida usando inverso modular.

```cpp
long long fast_pow(long long b, long long e, long long m) {
    long long r = 1; b %= m;
    for (; e > 0; e >>= 1) { if (e & 1) r = r*b%m; b = b*b%m; }
    return r;
}

int gauss_mod(vector<vector<long long>>& a, vector<long long>& b, long long MOD) {
    int n = a.size(), m = a[0].size(), row = 0;
    for (int col = 0; col < m && row < n; col++) {
        int pivot = -1;
        for (int i = row; i < n; i++)
            if (a[i][col] != 0) { pivot = i; break; }
        if (pivot == -1) continue;

        swap(a[row], a[pivot]);
        swap(b[row], b[pivot]);

        long long inv = fast_pow(a[row][col], MOD - 2, MOD);
        for (int j = col; j < m; j++) a[row][j] = a[row][j] * inv % MOD;
        b[row] = b[row] * inv % MOD;

        for (int i = 0; i < n; i++) {
            if (i == row || a[i][col] == 0) continue;
            long long f = a[i][col];
            for (int j = col; j < m; j++)
                a[i][j] = (a[i][j] - f * a[row][j] % MOD + MOD) % MOD;
            b[i] = (b[i] - f * b[row] % MOD + MOD) % MOD;
        }
        row++;
    }
    return row;
}
```

### Eliminação em GF(2) (bitset) — O(N² / 64)

Para sistemas de equações XOR (cada variável é 0 ou 1, operações são XOR/AND).

```cpp
// Cada linha é representada como bitset: bits 0..n-1 = coeficientes, bit n = b
int gauss_gf2(vector<bitset<1005>>& a, int n, int m) {
    int row = 0;
    for (int col = 0; col < m && row < n; col++) {
        int pivot = -1;
        for (int i = row; i < n; i++)
            if (a[i][col]) { pivot = i; break; }
        if (pivot == -1) continue;

        swap(a[row], a[pivot]);
        for (int i = 0; i < n; i++)
            if (i != row && a[i][col]) a[i] ^= a[row];
        row++;
    }
    return row; // rank
}
```

> **Aplicação:** base linear XOR — verificar se um conjunto de números é linearmente independente em GF(2), ou encontrar o XOR máximo de um subconjunto.

---

## Quando Usar Cada Técnica

```
Recorrência linear com N até 10^18?
└── Exponenciação de Matrizes

Sistema Ax = b com N variáveis?
├── Coeficientes reais → Eliminação Gaussiana com floats
├── Coeficientes inteiros mod p primo → Eliminação Gaussiana modular
└── Operações XOR (GF(2)) → Eliminação Gaussiana com bitset

Número de caminhos de comprimento K em grafo pequeno (N ≤ 100)?
└── Exponenciação da Matriz de Adjacência

Contagem de formas com restrições lineares?
└── Montar DP → identificar recorrência → Exponenciação de Matrizes
```