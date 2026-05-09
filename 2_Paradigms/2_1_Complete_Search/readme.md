# Busca Completa (Complete Search)

A Busca Completa resolve um problema percorrendo todo (ou parte) do espaço de busca até encontrar a solução. **Se a solução existir, ela sempre a encontrará.** Use-a quando N é pequeno o suficiente, quando não existe algoritmo mais eficiente, ou para validar soluções mais sofisticadas.

> Um Complete Search sem bugs nunca recebe Wrong Answer — apenas, potencialmente, TLE. Implemente primeiro, analise a complexidade, e só otimize se necessário.

---

## Visão Geral

| Técnica | Complexidade | Quando usar |
|---|---|---|
| Subconjuntos (bitmask) | O(N · 2ᴺ) | N ≤ 20, testar todos os subconjuntos |
| Subconjuntos (recursivo) | O(2ᴺ) | Igual, com mais flexibilidade |
| Combinações | O(K · C(N,K)) | Subgrupos de tamanho fixo K |
| Permutações | O(N · N!) | N ≤ 10, todas as ordenações |
| Backtracking | O(N!) pior caso (podado) | N-Rainhas, TSP, Sudoku |
| IDDFS | Igual à BFS assintoticamente | Menor caminho + memória O(profundidade) |
| Meet in the Middle | O(2^(N/2) · N) | N ≤ 40, quebra 2ᴺ em 2^(N/2) |
| Submáscaras | O(3ᴺ) total | DP de subconjuntos |

---

## Subconjuntos

Um conjunto de N elementos possui exatamente **2ᴺ subconjuntos**. Cada subconjunto é representado por um inteiro `mask`: o bit `i` ligado indica que o elemento `i` pertence ao subconjunto.

### Versão Bitmask — O(N · 2ᴺ)

```cpp
for (int mask = 0; mask < (1 << n); mask++) {
    for (int i = 0; i < n; i++) {
        if (mask & (1 << i)) {
            // elemento i está no subconjunto 'mask'
        }
    }
}
```

### Versão Recursiva

Toma uma decisão binária para cada elemento: incluir ou não incluir.

```cpp
void gerasub(vector<bool>& c, int k, int n) {
    if (k == n) {
        // processa subconjunto
        return;
    }
    c[k] = true;  gerasub(c, k + 1, n); // com o elemento k
    c[k] = false; gerasub(c, k + 1, n); // sem o elemento k
}
```

---

## Combinações — O(K · C(N,K))

Gera todos os subconjuntos de tamanho exato K sem repetição. A poda `(n - i) >= (k - indice)` para o loop cedo quando os elementos restantes são insuficientes.

```cpp
void gera_combinacoes(const vector<int>& arr, vector<int>& atual,
                      int inicio, int indice, int k) {
    if (indice == k) {
        // combinação completa
        return;
    }
    int n = arr.size();
    // Poda: elementos restantes (n - i) >= vagas restantes (k - indice)
    for (int i = inicio; i < n && (n - i) >= (k - indice); i++) {
        atual[indice] = arr[i];
        gera_combinacoes(arr, atual, i + 1, indice + 1, k);
    }
}
// Chamar: vector<int> atual(k); gera_combinacoes(arr, atual, 0, 0, k);
```

---

## Permutações — O(N · N!)

Gera todas as ordenações possíveis de um array.

### Versão por Swap (backtracking)

```cpp
void gera_permutacoes(vector<int>& arr, int l, int r) {
    if (l == r) {
        // permutação completa
        return;
    }
    for (int i = l; i <= r; i++) {
        swap(arr[l], arr[i]);
        gera_permutacoes(arr, l + 1, r);
        swap(arr[l], arr[i]); // desfaz
    }
}
```

### Com STL

```cpp
sort(arr.begin(), arr.end()); // OBRIGATÓRIO: começa da menor permutação
do {
    // processa arr
} while (next_permutation(arr.begin(), arr.end()));
```

> Esquecer o `sort` antes do `do-while` é um erro clássico — você só iterará pelas permutações maiores que a ordem inicial.

---

## Backtracking

O Backtracking é Complete Search com **poda**: ao perceber que um ramo não pode levar a uma solução válida ou melhor, abandona-o imediatamente. Existem dois tipos de poda:

- **Poda de Viabilidade:** o estado atual já viola uma restrição (ex: rainha atacando outra).
- **Poda de Otimalidade:** o custo atual já excede o melhor encontrado (ex: TSP).

```
void backtrack(estado) {
    if (inviável || pior que o melhor) return;  // PODA
    if (caso_base) { registra solução; return; }

    for (cada escolha válida) {
        aplica escolha
        backtrack(próximo estado)
        desfaz escolha
    }
}
```

### TSP — Poda de Otimalidade

```cpp
void tsp(const vector<vector<int>>& dist, vector<bool>& vis,
         int atual, int count, int custo, int& melhor) {
    if (custo >= melhor) return; // poda: já é pior

    if (count == (int)dist.size()) {
        melhor = min(melhor, custo + dist[atual][0]);
        return;
    }
    for (int prox = 0; prox < (int)dist.size(); prox++) {
        if (!vis[prox]) {
            vis[prox] = true;
            tsp(dist, vis, prox, count + 1, custo + dist[atual][prox], melhor);
            vis[prox] = false;
        }
    }
}
```

### N-Rainhas — Poda de Viabilidade

Verifica ataques em O(1) com vetores booleanos de coluna e diagonais.

```cpp
void n_rainhas(int linha, int n, vector<bool>& cols,
               vector<bool>& diag1, vector<bool>& diag2, int& sol) {
    if (linha == n) { sol++; return; }

    for (int c = 0; c < n; c++) {
        // diag1[linha + c] = diagonal principal, diag2[linha - c + n - 1] = secundária
        if (cols[c] || diag1[linha + c] || diag2[linha - c + n - 1]) continue;

        cols[c] = diag1[linha + c] = diag2[linha - c + n - 1] = true;
        n_rainhas(linha + 1, n, cols, diag1, diag2, sol);
        cols[c] = diag1[linha + c] = diag2[linha - c + n - 1] = false;
    }
}
```

---

## IDDFS — Iterative Deepening DFS

Combina o **baixo uso de memória da DFS** com a **garantia de caminho mínimo da BFS**: executa repetidas DFS com limite de profundidade crescente (0, 1, 2, ...).

Apesar de revisitar nós rasos, a complexidade assintótica é igual à BFS — os nós mais profundos dominam o custo. A vantagem é que usa apenas **O(profundidade)** de memória, enquanto a BFS usa O(largura do grafo).

```cpp
bool dfs_limitada(int estado, int prof, int limite) {
    if (estado == objetivo) return true;
    if (prof == limite) return false; // poda de profundidade

    for (int prox : vizinhos(estado))
        if (dfs_limitada(prox, prof + 1, limite)) return true;

    return false;
}

int iddfs(int inicio) {
    for (int limite = 0; limite <= MAX_PROF; limite++)
        if (dfs_limitada(inicio, 0, limite)) return limite;
    return -1; // não encontrado
}
```

> Se o espaço de estados puder ter ciclos, mantenha um set `visitados` e **limpe-o a cada nova iteração** do loop externo.

---

## Meet in the Middle — O(2^(N/2) · N)

Divide o problema em **duas metades**, resolve cada uma independentemente em O(2^(N/2)) e combina os resultados com busca binária. Transforma O(2ᴺ) em O(2^(N/2) · log N).

```
N = 40:
  Força bruta:         2⁴⁰ ≈ 10¹²  ✗
  Meet in the Middle:  2²⁰ ≈ 10⁶   ✓
```

**Caso clássico:** verificar se algum subconjunto soma exatamente `alvo`.

```cpp
bool meet_in_middle(const vector<long long>& arr, long long alvo) {
    int n = arr.size();

    // Gera todas as somas de um sub-intervalo
    auto get_sums = [&](int start, int end) {
        int len = end - start;
        vector<long long> sums(1 << len, 0);
        for (int mask = 0; mask < (1 << len); mask++)
            for (int i = 0; i < len; i++)
                if (mask & (1 << i)) sums[mask] += arr[start + i];
        return sums;
    };

    vector<long long> esq = get_sums(0, n / 2);
    vector<long long> dir = get_sums(n / 2, n);

    sort(dir.begin(), dir.end());

    for (long long s : esq)
        if (binary_search(dir.begin(), dir.end(), alvo - s)) return true;

    return false;
}
```

> **Extensão:** para **contar** pares em vez de apenas verificar existência, substitua `binary_search` por `upper_bound - lower_bound`.

---

## Submáscaras — O(3ᴺ) total

Para cada `mask`, itera por todos os seus subconjuntos sem repetir. O truque `(sub - 1) & mask` garante que apenas subconjuntos válidos são visitados. Muito usado como base de DP em subconjuntos.

```cpp
for (int mask = 0; mask < (1 << n); mask++) {
    for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
        // 'sub' é subconjunto de 'mask'
    }
    // sub == 0 também é subconjunto de qualquer mask
}
```

> **Por que O(3ᴺ)?** Cada elemento tem 3 estados: fora de `mask`, em `mask` mas fora de `sub`, ou em ambos.