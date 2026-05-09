# Programação Dinâmica (Dynamic Programming)

Programação Dinâmica resolve problemas que possuem **subestrutura ótima** e **subproblemas sobrepostos**: a solução ótima do problema total é construída a partir das soluções ótimas de subproblemas, e esses subproblemas se repetem. Em vez de recalculá-los, armazenamos os resultados.

**Dois ingredientes obrigatórios:**
1. **Subestrutura ótima:** a solução ótima do problema contém soluções ótimas dos subproblemas.
2. **Subproblemas sobrepostos:** os mesmos subproblemas são resolvidos múltiplas vezes.

> Se os subproblemas são **independentes**, use Divisão e Conquista. Se há **escolha local ótima**, use Greedy. Se nenhum desses, provavelmente é DP.

---

## Visão Geral

| Variante | Ideia | Quando usar |
|---|---|---|
| Top-Down (Memoização) | Recursão + cache | Transições complexas, subproblemas esparsos |
| Bottom-Up (Tabulação) | Tabela iterativa | Performance máxima, ordem de dependência clara |
| Forward DP | Estado atual atualiza estados futuros | Transições naturalmente "para frente" |
| Backward DP | Estado atual depende de estados anteriores | Transições naturalmente "para trás" |
| Bitmask DP | Estado é subconjunto de N ≤ 20 elementos | TSP, particionamento, cobertura |
| DP em Intervalos | Estado é [l, r] | Corte ótimo, multiplicação de matrizes |
| DP em Grafos/Árvores | Estado é nó da árvore | Mochila em árvore, caminhos máximos |
| DP com Rollback | Reduz dimensão da memória | Quando dp[i] depende só de dp[i-1] |

---

## Top-Down (Memoização)

Implementa a recorrência naturalmente com recursão e armazena os resultados já calculados em um mapa ou array.

**Vantagem:** não precisa pensar na ordem de preenchimento. **Desvantagem:** overhead de chamadas de função e cache misses.

```cpp
// Exemplo: Fibonacci com memoização
unordered_map<int, long long> memo;

long long fib(int n) {
    if (n <= 1) return n;
    if (memo.count(n)) return memo[n];
    return memo[n] = fib(n - 1) + fib(n - 2);
}
```

**Com `@lru_cache` em Python:**

```python
from functools import lru_cache

@lru_cache(maxsize=None)
def dp(i, j):
    if caso_base: return 0
    return min(dp(i + 1, j), dp(i, j + 1)) + custo[i][j]
```

---

## Bottom-Up (Tabulação)

Preenche a tabela de DP de forma iterativa, dos casos base em direção ao resultado final.

**Vantagem:** sem overhead de recursão, acesso sequencial à memória. **Desvantagem:** a ordem de preenchimento precisa ser correta.

```cpp
// Exemplo: Mochila 0/1
// dp[j] = maior valor com capacidade j
vector<long long> dp(W + 1, 0);
for (int i = 0; i < n; i++) {
    // percorre de trás para frente para não usar o item i duas vezes
    for (int j = W; j >= peso[i]; j--) {
        dp[j] = max(dp[j], dp[j - peso[i]] + valor[i]);
    }
}
```

---

## Forward DP vs Backward DP

**Backward DP** (pull): `dp[i]` é calculado a partir de estados anteriores. Mais intuitivo.

```cpp
// dp[i] = menor custo para chegar em i
for (int i = 1; i <= n; i++)
    for (int j = 0; j < i; j++)
        dp[i] = min(dp[i], dp[j] + custo(j, i));
```

**Forward DP** (push): o estado atual **empurra** contribuições para estados futuros. Útil quando as transições são mais naturais nessa direção (ex: BFS-like, distribuição de items).

```cpp
// dp[i] contribui para dp[j] (j > i)
for (int i = 0; i < n; i++) {
    if (dp[i] == INF) continue;
    for (int j = i + 1; j <= n; j++)
        dp[j] = min(dp[j], dp[i] + custo(i, j));
}
```

---

## Problemas Clássicos

### Longest Increasing Subsequence (LIS) — O(N log N)

```cpp
// tails[i] = menor elemento final de uma IS de tamanho i+1
int lis(const vector<int>& arr) {
    vector<int> tails;
    for (int x : arr) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return tails.size();
}
```

> Para LIS estritamente crescente use `lower_bound`. Para não-decrescente use `upper_bound`.

### Longest Common Subsequence (LCS) — O(N·M)

```cpp
int lcs(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (a[i-1] == b[j-1]) dp[i][j] = dp[i-1][j-1] + 1;
            else dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
    return dp[n][m];
}
```

### Edit Distance — O(N·M)

```cpp
int edit_distance(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 0; i <= n; i++) dp[i][0] = i;
    for (int j = 0; j <= m; j++) dp[0][j] = j;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (a[i-1] == b[j-1]) dp[i][j] = dp[i-1][j-1];
            else dp[i][j] = 1 + min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
    return dp[n][m];
}
```

### Mochila 0/1 — O(N·W)

```cpp
// dp[j] = maior valor com exatamente capacidade j disponível
vector<long long> dp(W + 1, 0);
for (int i = 0; i < n; i++)
    for (int j = W; j >= peso[i]; j--)
        dp[j] = max(dp[j], dp[j - peso[i]] + valor[i]);
```

### Mochila Ilimitada (Unbounded Knapsack) — O(N·W)

```cpp
// percorre de frente para trás — permite usar o mesmo item múltiplas vezes
vector<long long> dp(W + 1, 0);
for (int i = 0; i < n; i++)
    for (int j = peso[i]; j <= W; j++)
        dp[j] = max(dp[j], dp[j - peso[i]] + valor[i]);
```

---

## Bitmask DP — O(2ᴺ · N)

O estado da DP é uma máscara de bits representando quais elementos já foram usados. Essencial para problemas com N ≤ 20 onde os subconjuntos precisam ser explorados.

**Caso clássico: Caminho Hamiltoniano (TSP)**

```cpp
// dp[mask][i] = menor custo de visitar os vértices em 'mask', terminando em i
const int INF = 1e9;
int n;
int dist[20][20]; // dist[u][v] = custo da aresta u -> v
int dp[1 << 20][20];

int tsp() {
    memset(dp, 0x3f, sizeof(dp));
    for (int i = 0; i < n; i++) dp[1 << i][i] = 0;

    for (int mask = 1; mask < (1 << n); mask++) {
        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u))) continue;
            if (dp[mask][u] == 0x3f3f3f3f) continue;
            for (int v = 0; v < n; v++) {
                if (mask & (1 << v)) continue;
                int nova = mask | (1 << v);
                dp[nova][v] = min(dp[nova][v], dp[mask][u] + dist[u][v]);
            }
        }
    }

    int full = (1 << n) - 1;
    int res = INF;
    for (int i = 0; i < n; i++)
        res = min(res, dp[full][i] + dist[i][0]); // fecha o ciclo
    return res;
}
```

**Caminho Hamiltoniano (sem retorno):**
```cpp
for (int i = 0; i < n; i++)
    if (dp[full][i] < INF) return dp[full][i]; // não fecha o ciclo
```

---

## DP em Intervalos — O(N³)

O estado é `dp[l][r]`, representando a solução ótima para o subproblema no intervalo `[l, r]`. A recorrência divide o intervalo em dois e testa todos os pontos de corte.

```cpp
// Exemplo: Multiplicação encadeada de matrizes
// dp[l][r] = custo mínimo de multiplicar as matrizes l..r
// dim[i] = dimensão i da cadeia (matriz i tem dimensão dim[i] x dim[i+1])
vector<vector<long long>> dp(n, vector<long long>(n, 0));

for (int len = 2; len <= n; len++) {        // comprimento do intervalo
    for (int l = 0; l + len - 1 < n; l++) {
        int r = l + len - 1;
        dp[l][r] = LLONG_MAX;
        for (int k = l; k < r; k++) {       // ponto de corte
            long long custo = dp[l][k] + dp[k+1][r]
                            + (long long)dim[l] * dim[k+1] * dim[r+1];
            dp[l][r] = min(dp[l][r], custo);
        }
    }
}
```

> **Outros problemas de DP em intervalos:** corte ótimo de varas, triangulação de polígonos, número de formas de parênteses.

---

## DP com Rollback (Redução de Memória)

Quando `dp[i]` depende apenas de `dp[i-1]`, é possível reduzir de O(N·M) para O(M) usando dois arrays alternados.

```cpp
// LCS com O(M) de memória em vez de O(N·M)
int lcs_memoria_reduzida(const string& a, const string& b) {
    int m = b.size();
    vector<int> prev(m + 1, 0), curr(m + 1, 0);
    for (char ca : a) {
        fill(curr.begin(), curr.end(), 0);
        for (int j = 1; j <= m; j++)
            if (ca == b[j-1]) curr[j] = prev[j-1] + 1;
            else curr[j] = max(prev[j], curr[j-1]);
        swap(prev, curr);
    }
    return prev[m];
}
```

---

## Identificando DP

Perguntas que indicam DP:

1. O problema pede o **mínimo, máximo ou contagem** de algo?
2. As decisões tomadas agora afetam as opções futuras (estado)?
3. O mesmo subproblema aparece múltiplas vezes em caminhos diferentes?

Se as três respostas forem sim, é quase certamente DP. O próximo passo é definir:

- **Estado:** o que precisa ser armazenado para descrever um subproblema unicamente?
- **Transição:** como o estado `dp[i]` se relaciona com estados anteriores?
- **Caso base:** qual é o valor inicial (sem decisões tomadas)?
- **Resposta:** qual entrada da tabela contém o resultado final?

> **Dica de ordem de preenchimento:** garanta que ao calcular `dp[i]`, todos os estados dos quais ele depende já foram calculados. Em Bottom-Up, isso geralmente significa percorrer os índices em ordem crescente, por comprimento de intervalo, ou em ordem topológica.