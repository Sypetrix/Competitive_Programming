# Manipulação de Bits (Bit Manipulation)

Operações bitwise trabalham diretamente com a representação binária dos inteiros. Em programação competitiva, elas aparecem em dois contextos distintos: **truques de baixo nível** para acelerar operações aritméticas e lógicas, e **bitmask** para representar subconjuntos de até ~20 elementos de forma compacta.

---

## Operações Fundamentais

| Operação | Símbolo | Exemplo | Resultado |
|---|---|---|---|
| AND | `&` | `6 & 3` → `110 & 011` | `010` = 2 |
| OR | `\|` | `6 \| 3` → `110 \| 011` | `111` = 7 |
| XOR | `^` | `6 ^ 3` → `110 ^ 011` | `101` = 5 |
| NOT | `~` | `~6` | Inverte todos os bits |
| Left Shift | `<<` | `1 << 3` | `1000` = 8 |
| Right Shift | `>>` | `8 >> 2` | `10` = 2 |

> `1 << k` = 2ᵏ. Use `1LL << k` quando k ≥ 31 para evitar overflow em `int`.

---

## Truques com Bits

### Operações comuns em O(1)

```cpp
// Verificar se o bit k está ligado
bool bit_ligado = (mask & (1 << k)) != 0;

// Ligar o bit k
mask |= (1 << k);

// Desligar o bit k
mask &= ~(1 << k);

// Inverter o bit k
mask ^= (1 << k);

// Isolat o bit mais à direita (Lowest Set Bit)
int lsb = mask & (-mask);

// Desligar o bit mais à direita
mask &= (mask - 1);

// Verificar se é potência de 2
bool pot2 = (n > 0) && (n & (n - 1)) == 0;

// Paridade (par/ímpar)
bool impar = n & 1;

// Contar bits ligados (popcount)
int bits = __builtin_popcount(mask);   // para int
int bits = __builtin_popcountll(mask); // para long long

// Posição do bit mais significativo (MSB)
int msb_pos = 31 - __builtin_clz(n);  // n > 0

// Posição do bit menos significativo (LSB)
int lsb_pos = __builtin_ctz(n);       // n > 0

// Trocar dois inteiros sem variável auxiliar
a ^= b; b ^= a; a ^= b;

// Multiplicar/dividir por potência de 2
int dobro = n << 1;
int metade = n >> 1;
```

---

## Bitmask e Subconjuntos

Um inteiro de N bits pode representar qualquer subconjunto de N elementos: o bit `i` ligado indica que o elemento `i` pertence ao subconjunto.

```
mask = 0b101  →  { elemento[0], elemento[2] }
```

### Iterar sobre todos os subconjuntos — O(N · 2ᴺ)

```cpp
for (int mask = 0; mask < (1 << n); mask++) {
    for (int i = 0; i < n; i++) {
        if (mask & (1 << i)) {
            // elemento i está no subconjunto 'mask'
        }
    }
}
```

### Iterar sobre todas as submáscaras de uma máscara — O(3ᴺ) total

Para cada `mask`, itera por todos os seus subconjuntos sem repetir. O truque `(sub - 1) & mask` garante que apenas subconjuntos válidos são gerados.

```cpp
for (int mask = 0; mask < (1 << n); mask++) {
    for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
        // 'sub' é um subconjunto de 'mask'
    }
    // O subconjunto vazio (sub == 0) existe para qualquer mask
}
```

> **Por que O(3ᴺ)?** Cada elemento pode estar: (1) fora de `mask`, (2) em `mask` mas fora de `sub`, ou (3) em ambos — 3 estados por elemento.

### Gosper's Hack — Próxima máscara com exatamente K bits

Gera todas as combinações de N bits tomados K a K em ordem crescente.

```cpp
int mask = (1 << k) - 1; // começa com os k bits menos significativos
int limite = (1 << n);

while (mask < limite) {
    // processa mask (tem exatamente k bits ligados)

    int c = mask & -mask;               // isola o bit mais à direita
    int r = mask + c;                   // vira o bloco de 1s para 0
    mask = (((r ^ mask) >> 2) / c) | r; // reposiciona os bits restantes
}
```

**Exemplo para n=4, k=2:** gera `0011 → 0101 → 0110 → 1001 → 1010 → 1100`

---

## Aplicações Clássicas

### DP em subconjuntos (Bitmask DP)

O estado da DP é uma máscara representando quais elementos já foram processados. Exemplo clássico: TSP.

```cpp
// dp[mask][i] = melhor resultado ao ter visitado os vértices em 'mask',
//               terminando no vértice i
vector<vector<int>> dp(1 << n, vector<int>(n, INF));
dp[1 << inicio][inicio] = 0;

for (int mask = 0; mask < (1 << n); mask++) {
    for (int u = 0; u < n; u++) {
        if (!(mask & (1 << u))) continue; // u não está no conjunto
        if (dp[mask][u] == INF) continue;
        for (int v = 0; v < n; v++) {
            if (mask & (1 << v)) continue; // v já visitado
            int nova_mask = mask | (1 << v);
            dp[nova_mask][v] = min(dp[nova_mask][v], dp[mask][u] + dist[u][v]);
        }
    }
}
```

### Verificar interseção de dois conjuntos

```cpp
bool intersecta = (A & B) != 0;
bool A_subconjunto_B = (A & B) == A;
bool disjuntos = (A & B) == 0;
int uniao = A | B;
int intersecao = A & B;
int diferenca = A & ~B; // elementos em A mas não em B
```

---

## Limites Práticos

| Tipo | Bits utilizáveis | Limite para `1 << k` |
|---|---|---|
| `int` (32 bits) | 30 bits seguros | k ≤ 30 |
| `long long` (64 bits) | 62 bits seguros | k ≤ 62 |
| `__int128` | 126 bits | k ≤ 126 |

> Para N > 20 com bitmask DP, verifique a memória: `2²⁰ × N × sizeof(int)` pode facilmente ultrapassar 256 MB.