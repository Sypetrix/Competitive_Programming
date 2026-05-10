# Aritmética Modular (Modular Arithmetic)

Aritmética Modular surge em quase todo problema de contagem ou combinatória — o resultado é sempre pedido "módulo 10⁹+7" (ou outro primo). Entender suas regras é obrigatório para não introduzir bugs sutis.

---

## Visão Geral

| Técnica / Arquivo | Complexidade | Utilização Típica |
|---|---|---|
| Operações básicas mod | O(1) | Somar, multiplicar, subtrair sem overflow |
| `fast_pow` (exponenciação) | O(log exp) | aⁿ mod p, inverso de Fermat |
| `extGCD` + `invMod` | O(log m) | Inverso modular para qualquer mod (não primo) |
| `phi_euler` + Euler | O(√N) | a^φ(m) ≡ 1 (mod m) para mdc(a,m)=1 |
| Teorema Chinês dos Restos | O(log m) | Combinar sistemas de congruências |
| Fatorial + inverso pré-computado | O(N) prep + O(1) | nCr mod p para N até 10⁶ |

---

## Regras Fundamentais

Para qualquer inteiro a, b e módulo m:

```
(a + b) mod m = ((a mod m) + (b mod m)) mod m
(a - b) mod m = ((a mod m) - (b mod m) + m) mod m   ← +m para evitar negativo
(a * b) mod m = ((a mod m) * (b mod m)) mod m
(a / b) mod m = (a * b⁻¹) mod m                     ← divisão vira multiplicação pelo inverso
```

> **Regra de ouro:** aplique o módulo após **cada** operação. Nunca acumule sem modular — o produto de dois valores de ~10⁹ já excede `long long`.

```cpp
const long long MOD = 1e9 + 7;

long long add(long long a, long long b) { return (a + b) % MOD; }
long long sub(long long a, long long b) { return (a - b + MOD) % MOD; }
long long mul(long long a, long long b) { return (1LL * a % MOD) * (b % MOD) % MOD; }
```

---

## Exponenciação Rápida — O(log exp)

Calcula `base^exp mod p` dividindo o expoente ao meio a cada passo.

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

**Usos imediatos:**
```cpp
fast_pow(2, 10, MOD);          // 2^10 mod MOD = 1024
fast_pow(a, MOD - 2, MOD);     // inverso modular de a (MOD primo, mdc(a,MOD)=1)
fast_pow(a, phi_m - 1, m);     // inverso quando m não é primo (Teorema de Euler)
```

---

## Inverso Modular

O inverso de `a` módulo `m` é o valor `x` tal que `a * x ≡ 1 (mod m)`. É a forma de fazer **divisão modular**: `(a / b) mod m = a * inv(b) mod m`.

### Método 1: Pequeno Teorema de Fermat — O(log p)

Funciona apenas quando `m` é **primo** e `mdc(a, m) = 1`:

```cpp
// a^(p-1) ≡ 1 (mod p)  →  a^(p-2) ≡ a^(-1) (mod p)
long long inv_fermat(long long a, long long p) {
    return fast_pow(a, p - 2, p);
}
```

### Método 2: Euclides Estendido — O(log m)

Funciona para qualquer `m` (primo ou não), desde que `mdc(a, m) = 1`:

```cpp
long long extGCD(long long a, long long b, long long& x, long long& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    long long x1, y1;
    long long d = extGCD(b, a % b, x1, y1);
    x = y1; y = x1 - y1 * (a / b);
    return d;
}

long long inv_euclides(long long a, long long m) {
    long long x, y;
    if (extGCD(a, m, x, y) != 1) return -1; // inverso não existe
    return (x % m + m) % m;
}
```

### Método 3: Pré-computação Linear — O(N)

Computa `inv[1..N]` de uma vez. Ideal quando você precisar dos inversos de múltiplos valores:

```cpp
vector<long long> pre_inv(int N, long long p) {
    vector<long long> inv(N + 1);
    inv[1] = 1;
    for (int i = 2; i <= N; i++)
        inv[i] = (p - (p / i) * inv[p % i] % p) % p;
    return inv;
}
```

---

## Pré-computação de Fatoriais — O(N) prep + O(1) query

Essencial para calcular nCr mod p com N até ~10⁶.

```cpp
const int MAX = 1e6 + 5;
const long long MOD = 1e9 + 7;
long long fat[MAX], inv_fat[MAX];

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

---

## Teorema de Euler

Generalização do Pequeno Teorema de Fermat para módulos não primos:

```
Se mdc(a, m) = 1, então a^φ(m) ≡ 1 (mod m)
```

Portanto `a^(φ(m)-1) mod m` é o inverso de `a` quando `m` não é primo:

```cpp
long long inv_euler(long long a, long long m) {
    long long phi_m = phi(m); // função totiente de Euler
    return fast_pow(a, phi_m - 1, m);
}
```

---

## Teorema Chinês dos Restos (CRT)

Dado um sistema de congruências:
```
x ≡ r₁ (mod m₁)
x ≡ r₂ (mod m₂)
...
```
Se os módulos são **coprimos entre si** (mdc(mᵢ, mⱼ) = 1), existe solução única módulo M = m₁·m₂·…·mₖ.

```cpp
// Combina duas congruências: x ≡ r1 (mod m1) e x ≡ r2 (mod m2)
// Retorna {r, m} tal que x ≡ r (mod m), ou {-1, -1} se incompatível
pair<long long, long long> crt(long long r1, long long m1, long long r2, long long m2) {
    long long x, y;
    long long g = extGCD(m1, m2, x, y);
    if ((r2 - r1) % g != 0) return {-1, -1}; // sem solução

    long long lcm = m1 / g * m2;
    long long r = (r1 + m1 * ((r2 - r1) / g % (m2 / g) * x % (m2 / g))) % lcm;
    if (r < 0) r += lcm;
    return {r, lcm};
}
```

**Aplicação clássica:** combinar resultados de problemas independentes com módulos diferentes, ou recuperar N a partir de N mod p₁, N mod p₂, N mod p₃.

---

## Aritmética Modular em Floats

Quando o resultado é racional p/q e o problema pede `(p/q) mod M`, compute:

```cpp
long long resposta = p % MOD * inv_fermat(q, MOD) % MOD;
```

---

## Armadilhas Comuns

| Armadilha | Solução |
|---|---|
| `a - b` negativo após mod | Sempre use `(a - b + MOD) % MOD` |
| Overflow em `a * b` | Cast: `(long long)a * b % MOD` |
| `inv(0)` — divisão por zero | Verifique antes; `0` não tem inverso |
| Módulo não primo no Fermat | Use Euclides Estendido ou pré-computação linear |
| `pow(a, b)` da `<cmath>` | Usa ponto flutuante — **nunca** use para aritmética modular |