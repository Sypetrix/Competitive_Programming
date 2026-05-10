# Probabilidade (Probability)

Problemas de probabilidade em CP geralmente pedem o **valor esperado** de alguma quantidade ou a **probabilidade** de um evento, sempre mod p. O segredo está em reconhecer a estrutura correta: linearidade, independência, DP probabilístico ou probabilidade condicional.

---

## Conceitos Fundamentais

### Probabilidade

A probabilidade de um evento A é:

```
P(A) = (casos favoráveis) / (casos totais)
```

**Propriedades:**
- `0 ≤ P(A) ≤ 1`
- `P(A ∪ B) = P(A) + P(B) − P(A ∩ B)`
- Se A e B são **mutuamente exclusivos**: `P(A ∪ B) = P(A) + P(B)`
- Se A e B são **independentes**: `P(A ∩ B) = P(A) · P(B)`

### Probabilidade Condicional

```
P(A | B) = P(A ∩ B) / P(B)
```

Probabilidade de A dado que B já ocorreu.

**Fórmula de Bayes:**
```
P(A | B) = P(B | A) · P(A) / P(B)
```

---

## Valor Esperado (Expected Value)

O valor esperado de uma variável aleatória X é a média ponderada pelos resultados:

```
E[X] = Σ xᵢ · P(X = xᵢ)
```

### Linearidade do Valor Esperado

A propriedade mais poderosa da teoria: vale **mesmo para variáveis dependentes**.

```
E[X + Y] = E[X] + E[Y]
E[c · X] = c · E[X]
```

**Técnica:** decomponha X em variáveis indicadoras Xᵢ onde Xᵢ = 1 se o evento i ocorre, 0 caso contrário. Então `E[Xᵢ] = P(evento i)`.

```
E[X] = E[X₁ + X₂ + ... + Xₙ] = Σ P(evento i)
```

**Exemplo:** número esperado de pontos fixos em uma permutação aleatória de N elementos.

Seja Xᵢ = 1 se o elemento i está na posição i. P(Xᵢ = 1) = 1/N para todo i. Logo:

```
E[# pontos fixos] = Σᵢ E[Xᵢ] = N · (1/N) = 1
```

---

## DP Probabilístico

Quando a probabilidade de chegar a um estado depende de estados anteriores, modele como DP.

**Estado:** `dp[i]` = probabilidade de estar no estado i (ou valor esperado até o estado i).

**Exemplo clássico: número esperado de passos para absorção**

Dado um grafo onde de cada nó i você vai para o nó j com probabilidade p(i,j), calcule o número esperado de passos para chegar ao nó destino T a partir do nó S.

```cpp
// dp[i] = número esperado de passos de i até T
// dp[T] = 0
// dp[i] = 1 + Σⱼ p(i,j) * dp[j]   para i ≠ T

// Sistema linear — resolve com eliminação gaussiana
// ou iteração (se o grafo for um DAG)
```

**Exemplo: lançar um dado até somar ≥ N**

```cpp
// dp[i] = número esperado de lançamentos para sair de i
// dp[i] = 0 se i >= N
// dp[i] = 1 + (1/6) * Σₖ₌₁⁶ dp[i+k]   para i < N

vector<double> dp(N + 7, 0.0);
for (int i = N - 1; i >= 0; i--) {
    dp[i] = 1.0;
    for (int k = 1; k <= 6; k++)
        dp[i] += dp[min(i + k, N)] / 6.0;
    // Nota: dp[N] = 0 (absorvente)
}
```

---

## Probabilidade em Grafos — Caminhadas Aleatórias

**Random Walk:** em cada passo, o agente vai de i para um vizinho j uniformemente.

Para calcular a probabilidade de atingir T antes de S em uma cadeia de Markov:

```cpp
// p[i] = probabilidade de atingir T a partir de i antes de S
// p[S] = 0, p[T] = 1
// p[i] = Σⱼ P(i→j) * p[j]   para i ≠ S, T

// Sistema linear — resolve com eliminação gaussiana
```

---

## Probabilidade Geométrica

Para distribuições contínuas uniformes, a probabilidade é uma razão de medidas (comprimentos, áreas, volumes):

```
P(evento) = medida da região favorável / medida total
```

Em CP, isso frequentemente aparece como: "dois valores aleatórios em [0, 1] — qual a probabilidade de X + Y < 1?" → área do triângulo (1/2) sobre área total (1).

---

## Valor Esperado mod p

Quando o problema pede o valor esperado como fração p/q mod M:

```cpp
// E[X] = numerador / denominador
// Resposta = numerador * inv(denominador) % MOD

long long resposta = numerador % MOD * fast_pow(denominador, MOD - 2, MOD) % MOD;
```

**Cuidado:** certifique-se de que o denominador seja sempre coprimo com MOD (para MOD primo, basta que o denominador ≠ 0 mod MOD).

---

## Distribuições Clássicas

| Distribuição | Descrição | E[X] | Var(X) |
|---|---|---|---|
| Bernoulli(p) | 1 com prob p, 0 com prob 1−p | p | p(1−p) |
| Binomial(n,p) | # sucessos em n tentativas | np | np(1−p) |
| Geométrica(p) | # tentativas até 1º sucesso | 1/p | (1−p)/p² |
| Uniforme discreta(n) | valor em {1,...,n} igualmente provável | (n+1)/2 | (n²−1)/12 |

**Distribuição Geométrica em CP:** número esperado de tentativas até obter sucesso com probabilidade p em cada tentativa é **1/p**.

```
E[# tentativas para cupom i] = N / (N - i)   (problema do colecionador de cupons)
E[# total] = N · Σᵢ₌₁ᴺ 1/i = N · Hₙ  (Hₙ = número harmônico)
```

---

## Problema do Colecionador de Cupons

Há N cupons distintos. Cada compra dá um cupom aleatório uniforme. Qual o número esperado de compras para colecionar todos?

```
E[total] = N · (1/N + 1/(N-1) + ... + 1/1) = N · Hₙ ≈ N · ln(N)
```

```cpp
double colecionador(int N) {
    double esperado = 0;
    for (int i = 1; i <= N; i++) esperado += (double)N / i;
    return esperado;
}
```

---

## Checklist para Problemas de Probabilidade

1. O problema pede probabilidade ou valor esperado?
2. Os eventos são independentes? (→ multiplica)
3. É possível usar linearidade do valor esperado com indicadoras?
4. A estrutura permite DP (estado = configuração atual)?
5. O resultado deve ser mod p? (→ inverso modular do denominador)