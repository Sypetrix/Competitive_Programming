# Divisão e Conquista (Divide and Conquer)

Divisão e Conquista resolve um problema **dividindo-o** em subproblemas menores do mesmo tipo, **resolvendo** cada subproblema recursivamente e **combinando** os resultados. Os subproblemas são **independentes** — isso é o que diferencia D&C de Programação Dinâmica, onde os subproblemas se sobrepõem.

```
D&C(problema):
    se problema é pequeno o suficiente: resolve diretamente
    divide em subproblemas menores
    resolve cada subproblema recursivamente
    combina os resultados
```

---

## Visão Geral

| Técnica | Complexidade | Uso típico |
|---|---|---|
| Busca Binária clássica | O(log N) | Encontrar elemento em array ordenado |
| Busca Binária na Resposta | O(N log V) | Otimizar valor com verificação gulosa |
| Busca Binária em função | O(log N) | Mínimo/máximo de função unimodal |
| Merge Sort / contagem de inversões | O(N log N) | Ordenação estável, contar inversões |
| Maximum Subarray (Kadane D&C) | O(N log N) | Subarray de soma máxima cruzando o meio |
| Potência de matriz | O(M³ log N) | Recorrências lineares com N até 10¹⁸ |
| Exponenciação rápida | O(log N) | aⁿ mod p |
| D&C em DP (Divide and Conquer DP) | O(N log N) | Otimização de DP com propriedade do ponto ótimo |

---

## Busca Binária Clássica — O(log N)

Divide o array ao meio e descarta metade a cada passo. Requer que o array esteja **ordenado**.

```cpp
// Retorna o índice de alvo, ou -1 se não encontrado
int busca_binaria(const vector<int>& arr, int alvo) {
    int l = 0, r = arr.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2; // evita overflow
        if (arr[mid] == alvo) return mid;
        else if (arr[mid] < alvo) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}
```

### Lower Bound e Upper Bound

```cpp
// Primeiro índice com arr[i] >= alvo
int lower_bound_manual(const vector<int>& arr, int alvo) {
    int l = 0, r = arr.size();
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (arr[mid] < alvo) l = mid + 1;
        else r = mid;
    }
    return l; // equivale a std::lower_bound
}

// Primeiro índice com arr[i] > alvo
int upper_bound_manual(const vector<int>& arr, int alvo) {
    int l = 0, r = arr.size();
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (arr[mid] <= alvo) l = mid + 1;
        else r = mid;
    }
    return l; // equivale a std::upper_bound
}
```

> Prefira `std::lower_bound` e `std::upper_bound` da STL em competições — menos código, menos erro.

---

## Busca Binária na Resposta — O(N log V)

Em vez de buscar um elemento, aplica busca binária sobre o **espaço de possíveis respostas** e verifica cada candidato com uma função `check()`.

**Estrutura padrão (minimizar):**

```cpp
// check(mid) retorna true se 'mid' é uma resposta viável
long long esq = valor_minimo, dir = valor_maximo, res = dir;
while (esq <= dir) {
    long long mid = esq + (dir - esq) / 2;
    if (check(mid)) { res = mid; dir = mid - 1; } // tenta melhorar
    else esq = mid + 1;
}
// res = menor valor viável
```

**Estrutura padrão (maximizar o mínimo):**

```cpp
long long esq = valor_minimo, dir = valor_maximo, res = esq;
while (esq <= dir) {
    long long mid = esq + (dir - esq) / 2;
    if (check(mid)) { res = mid; esq = mid + 1; }
    else dir = mid - 1;
}
// res = maior valor viável
```

**Exemplo — distribuir pesos em K caminhões:**

```cpp
bool check(long long cap, const vector<int>& pesos, int K) {
    int caminhoes = 1; long long soma = 0;
    for (int p : pesos) {
        if (p > cap) return false;
        if (soma + p > cap) { caminhoes++; soma = p; }
        else soma += p;
    }
    return caminhoes <= K;
}
```

> O `check()` quase sempre é uma simulação gulosa em O(N). A busca binária roda em O(log V) iterações — complexidade total O(N log V).

---

## Busca Binária em Função — O(log N)

Para funções **unimodais** (estritamente decrescentes depois de um mínimo, ou vice-versa), a busca ternária encontra o ponto ótimo em O(log N).

```cpp
// Minimizar f(x) em [l, r] — f deve ser convexa (unimodal)
double busca_ternaria(double l, double r, auto f) {
    for (int i = 0; i < 200; i++) { // 200 iterações → precisão de 10^-60
        double m1 = l + (r - l) / 3;
        double m2 = r - (r - l) / 3;
        if (f(m1) < f(m2)) r = m2;
        else l = m1;
    }
    return (l + r) / 2;
}
```

Para **inteiros** com função unimodal, use:

```cpp
// Encontra o mínimo inteiro de f em [l, r]
int busca_ternaria_int(int l, int r, auto f) {
    while (r - l > 2) {
        int m1 = l + (r - l) / 3;
        int m2 = r - (r - l) / 3;
        if (f(m1) <= f(m2)) r = m2;
        else l = m1;
    }
    int res = l;
    for (int x = l + 1; x <= r; x++)
        if (f(x) < f(res)) res = x;
    return res;
}
```

---

## Merge Sort e Contagem de Inversões — O(N log N)

O Merge Sort divide o array ao meio, ordena cada metade recursivamente e combina. A etapa de **merge** é onde o trabalho de O(N) acontece.

```cpp
long long inversoes = 0;

void merge(vector<int>& arr, int l, int mid, int r) {
    vector<int> tmp;
    int i = l, j = mid + 1;
    while (i <= mid && j <= r) {
        if (arr[i] <= arr[j]) tmp.push_back(arr[i++]);
        else {
            inversoes += mid - i + 1; // todos os elementos de i..mid são > arr[j]
            tmp.push_back(arr[j++]);
        }
    }
    while (i <= mid) tmp.push_back(arr[i++]);
    while (j <= r)   tmp.push_back(arr[j++]);
    for (int k = l; k <= r; k++) arr[k] = tmp[k - l];
}

void merge_sort(vector<int>& arr, int l, int r) {
    if (l >= r) return;
    int mid = l + (r - l) / 2;
    merge_sort(arr, l, mid);
    merge_sort(arr, mid + 1, r);
    merge(arr, l, mid, r);
}
```

---

## Exponenciação Rápida — O(log N)

Calcula `base^exp % mod` em O(log N) usando o fato de que `base^(2k) = (base^k)²`.

```cpp
long long fast_pow(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) res = res * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return res;
}
```

> Usado em praticamente todo problema de aritmética modular: inverso de Fermat, binomial, DP com módulo.

---

## Potência de Matrizes — O(M³ log N)

Resolve recorrências lineares para N até 10¹⁸ elevando a **matriz de transição** à N-ésima potência com exponenciação rápida.

```cpp
typedef vector<vector<long long>> Matrix;
const long long MOD = 1e9 + 7;

Matrix multiply(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) if (A[i][k])
            for (int j = 0; j < n; j++)
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % MOD;
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

**Exemplo — N-ésimo Fibonacci:**
```
[ F(n+1) ]   [ 1 1 ]^n   [ F(1) ]
[ F(n)   ] = [ 1 0 ]   × [ F(0) ]
```

---

## Maximum Subarray com D&C — O(N log N)

Divide o array ao meio. O subarray ótimo está:
- Inteiramente na metade esquerda.
- Inteiramente na metade direita.
- Cruzando o ponto médio (sufixo máximo da esquerda + prefixo máximo da direita).

```cpp
int max_cruzando(const vector<int>& arr, int l, int mid, int r) {
    int soma = 0, esq_max = INT_MIN;
    for (int i = mid; i >= l; i--) { soma += arr[i]; esq_max = max(esq_max, soma); }
    soma = 0;
    int dir_max = INT_MIN;
    for (int i = mid + 1; i <= r; i++) { soma += arr[i]; dir_max = max(dir_max, soma); }
    return esq_max + dir_max;
}

int max_subarray(const vector<int>& arr, int l, int r) {
    if (l == r) return arr[l];
    int mid = l + (r - l) / 2;
    return max({max_subarray(arr, l, mid),
                max_subarray(arr, mid + 1, r),
                max_cruzando(arr, l, mid, r)});
}
```

> Na prática, **Kadane's Algorithm** resolve isso em O(N) com uma linha de DP. Use D&C quando o problema exigir calcular algo adicional durante o merge (ex: contagem de pares que cruzam o meio).

---

## D&C DP (Divide and Conquer DP) — O(N log N)

Otimização de DP onde a função de custo satisfaz a **propriedade da monotonicidade do ponto ótimo**: se `opt(i)` é o índice que minimiza `dp[i]`, então `opt(i) ≤ opt(i+1)`. Em vez de O(N²), busca-se o ótimo apenas no intervalo `[opt(l), opt(r)]`.

```cpp
// dp[i] = min_{k < i} { dp[k] + custo(k+1, i) }
void solve(int l, int r, int opt_l, int opt_r,
           vector<long long>& dp, vector<long long>& dp_prev, auto custo) {
    if (l > r) return;
    int mid = l + (r - l) / 2;
    int opt = opt_l;
    dp[mid] = LLONG_MAX;
    for (int k = opt_l; k <= min(mid - 1, opt_r); k++) {
        long long val = dp_prev[k] + custo(k + 1, mid);
        if (val < dp[mid]) { dp[mid] = val; opt = k; }
    }
    solve(l, mid - 1, opt_l, opt, dp, dp_prev, custo);
    solve(mid + 1, r, opt, opt_r, dp, dp_prev, custo);
}
```

> Verifique a monotonicidade do ponto ótimo antes de aplicar. Problemas com essa estrutura incluem: divisão de arrays em K partes com custo mínimo, problema do correio, etc.