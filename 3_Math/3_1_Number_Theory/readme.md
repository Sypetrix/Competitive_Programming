# Teoria dos Números (Number Theory)

Problemas de Teoria dos Números raramente aceitam soluções de força bruta. Se o problema envolve divisibilidade, primos ou fatoriais gigantes, você precisará de algoritmos baseados em matemática discreta com complexidade sub-linear.

---

## Visão Geral

| Algoritmo / Arquivo | Complexidade | Utilização Típica |
|---|---|---|
| `erastotenes_sieve` | O(N log log N) | Pré-computar primos até 10⁷ |
| `dividers` | O(√N) ou O(π(N)) | Contar e somar divisores |
| `euclides` | O(log min(a,b)) | MDC, MMC, Inverso Modular |
| `diophantine` | O(log min(a,b)) | Soluções inteiras de Ax + By = C |
| `phi_euler` | O(√N) | Quantidade de coprimos com N |
| `legendre` | O(log_p N) | Potência de primo em N! |
| `cycle_floyd` | O(μ + λ) | Início e tamanho de ciclo em sequências |s

---

## Crivo de Eratóstenes — O(N log log N)

Gera todos os primos até N. Usando `bitset` para comprimir memória, é viável até ~10⁷ em tempo e espaço razoáveis.

```cpp
bitset<10000010> bs;
vector<int> primos;

void crivo(int limite) {
    bs.set();
    bs[0] = bs[1] = 0;
    for (int i = 2; i <= limite; i++) {
        if (bs[i]) {
            for (int j = i * i; j <= limite; j += i) bs[j] = 0;
            primos.push_back(i);
        }
    }
}
```

**Fatoração usando o crivo:**

```cpp
vector<int> fatorar(int N) {
    vector<int> fatores;
    for (int p : primos) {
        if (p * p > N) break;
        while (N % p == 0) { fatores.push_back(p); N /= p; }
    }
    if (N > 1) fatores.push_back(N);
    return fatores;
}
```

> Use o crivo quando precisar de múltiplas consultas de primalidade ou fatorar muitos números. Fatorar N números em O(√N) cada sem o crivo pode causar TLE.

---

## MDC, MMC e Euclides Estendido

```cpp
// MDC recursivo — O(log min(a,b))
long long mdc(long long a, long long b) { return b ? mdc(b, a % b) : a; }

// MMC — cuidado com overflow: divida antes de multiplicar
long long mmc(long long a, long long b) { return a / mdc(a, b) * b; }

// Euclides Estendido: encontra x, y tais que a*x + b*y = mdc(a,b)
long long extGCD(long long a, long long b, long long& x, long long& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    long long x1, y1;
    long long d = extGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

// Inverso modular: a^(-1) mod m (requer mdc(a,m) = 1)
long long invMod(long long a, long long m) {
    long long x, y;
    if (extGCD(a, m, x, y) != 1) return -1;
    return (x % m + m) % m;
}
```

---

## Equações Diofantinas Lineares

Encontra uma solução inteira para **Ax + By = C**.

Solução existe se e somente se `mdc(A, B) | C`.

```cpp
bool diofantina(long long a, long long b, long long c,
                long long& x0, long long& y0, long long& g) {
    g = extGCD(abs(a), abs(b), x0, y0);
    if (c % g != 0) return false;
    x0 *= c / g;
    y0 *= c / g;
    if (a < 0) x0 = -x0;
    if (b < 0) y0 = -y0;
    return true;
}
```

**Família de soluções:** se (x₀, y₀) é uma solução, então todas as soluções são:
```
x = x₀ + (b/g)·t
y = y₀ - (a/g)·t    para qualquer inteiro t
```

---

## Função Totiente de Euler — φ(N)

φ(N) = quantidade de inteiros em [1, N] coprimos com N.

```cpp
long long phi(long long n) {
    long long result = n;
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0) n /= i;
            result -= result / i;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}
```

**Propriedades úteis:**
- φ(p) = p − 1 para p primo
- φ(pᵏ) = pᵏ − pᵏ⁻¹
- φ é multiplicativa: se mdc(a,b) = 1, então φ(ab) = φ(a)·φ(b)
- **Teorema de Euler:** a^φ(m) ≡ 1 (mod m) quando mdc(a,m) = 1

---

## Contagem e Soma de Divisores

Se N = p₁^a₁ · p₂^a₂ · … · pₖ^aₖ:

- **Número de divisores:** (a₁ + 1)(a₂ + 1)…(aₖ + 1)
- **Soma dos divisores:** Π [(pᵢ^(aᵢ+1) − 1) / (pᵢ − 1)]

```cpp
long long numDiv(long long N) {
    long long ans = 1;
    for (long long p : primos) {
        if (p * p > N) break;
        int exp = 0;
        while (N % p == 0) { N /= p; exp++; }
        if (exp) ans *= (exp + 1);
    }
    if (N > 1) ans *= 2; // primo residual
    return ans;
}
```

---

## Fórmula de Legendre

Maior potência do primo `p` que divide `n!`:

```
νₚ(n!) = ⌊n/p⌋ + ⌊n/p²⌋ + ⌊n/p³⌋ + …
```

```cpp
long long legendre(long long n, long long p) {
    long long count = 0;
    while (n > 0) { count += n / p; n /= p; }
    return count;
}
```

**Aplicações:**
- Zeros à direita de N! → `legendre(N, 5)`
- Verificar se M divide N! → compare a potência de cada primo em M com `legendre(N, p)`

---

## Algoritmo de Floyd (Detecção de Ciclos)

Encontra o início (μ) e tamanho (λ) de um ciclo na sequência xᵢ = f(xᵢ₋₁) usando apenas O(1) de memória.

```cpp
pair<int,int> floyd(auto f, int x0) {
    // Fase 1: encontra o encontro
    int tartaruga = f(x0), lebre = f(f(x0));
    while (tartaruga != lebre) {
        tartaruga = f(tartaruga);
        lebre = f(f(lebre));
    }

    // Fase 2: encontra o início do ciclo (μ)
    int mu = 0;
    tartaruga = x0;
    while (tartaruga != lebre) {
        tartaruga = f(tartaruga);
        lebre = f(lebre);
        mu++;
    }

    // Fase 3: encontra o tamanho do ciclo (λ)
    int lambda = 1;
    lebre = f(tartaruga);
    while (tartaruga != lebre) { lebre = f(lebre); lambda++; }

    return {mu, lambda};
}
```

> Use quando o limite de memória impede salvar todos os estados em um `set`. Comum em hashing com ciclos, geradores pseudoaleatórios e problemas de sequências funcionais.

---

## Crivo Segmentado — O(N log log N) com memória O(√N)

Para gerar primos em um intervalo [L, R] grande (L, R até 10¹²), o crivo normal é inviável em memória. O crivo segmentado usa os primos até √R para peneirar blocos.

```cpp
void crivo_segmentado(long long L, long long R) {
    int raiz = sqrt(R) + 1;
    crivo(raiz); // gera primos pequenos até √R

    vector<bool> is_primo(R - L + 1, true);
    if (L == 1) is_primo[0] = false;

    for (int p : primos) {
        // primeiro múltiplo de p em [L, R]
        long long inicio = max((long long)p * p, (L + p - 1) / p * p);
        for (long long j = inicio; j <= R; j += p)
            is_primo[j - L] = false;
    }

    for (long long i = L; i <= R; i++)
        if (is_primo[i - L]) { /* i é primo */ }
}
```