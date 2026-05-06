# Técnicas Iterativas de Busca Completa

Esta seção cobre as abordagens iterativas para enumerar todos os elementos de um espaço de busca: subconjuntos, combinações, permutações e operações com bitmask. São a base de muitos algoritmos de força bruta e programação dinâmica com máscaras de bits.

---

## Subconjuntos

Um conjunto de `n` elementos possui exatamente **2ⁿ subconjuntos**. Cada subconjunto pode ser representado por um inteiro `mask` onde cada bit indica se o elemento está incluído ou não.

```
exemplo de mask com n = 3 (101 em binário)
mask = 0b101  →  { elemento[0], elemento[2] }
         ↑↑↑
         bit 0 ligado → inclui elemento 0
         bit 1 desligado → não inclui elemento 1
         bit 2 ligado → inclui elemento 2
```

### Versão Bitmask — O(n · 2ⁿ)

Itera por todos os `2ⁿ` inteiros e verifica cada bit:

```cpp
for (int mask = 0; mask < (1 << n); mask++) {
    for (int i = 0; i < n; i++) {
        if (mask & (1 << i))  // bit i está ligado?
            // elemento i está no subconjunto
    }
}
```

### Versão Recursiva

Toma uma decisão binária para cada elemento: **incluir** ou **não incluir**.

```cpp
void gerasub(vector<bool>& c, int k, int n) {
    if (k == n) { /* processa subconjunto */ return; }

    c[k] = true;  gerasub(c, k + 1, n); // COM o elemento k
    c[k] = false; gerasub(c, k + 1, n); // SEM o elemento k
}
```

> **Quando usar:** problemas de mochila, cobertura de conjuntos, ou qualquer situação onde você precise testar todas as combinações de `n ≤ 20` elementos.

---

## Combinações

Gera todos os subconjuntos de tamanho exato `k` a partir de um array de `n` elementos, sem repetição e sem importar a ordem. Complexidade: **O(k · C(n,k))**.

A poda é o detalhe mais importante: se os elementos restantes forem insuficientes para completar a combinação, o laço para mais cedo.

```cpp
void gera_combinacoes(const vector<int>& arr, vector<int>& atual,
                      int inicio, int indice, int k) {
    if (indice == k) { /* combinação completa */ return; }

    int n = arr.size();
    // Poda: elementos restantes (n - i) devem ser >= vagas restantes (k - indice)
    for (int i = inicio; i < n && (n - i) >= (k - indice); i++) {
        atual[indice] = arr[i];
        gera_combinacoes(arr, atual, i + 1, indice + 1, k);
    }
}
```

**Como chamar:**
```cpp
vector<int> arr = {1, 2, 3, 4};
vector<int> atual(k);
gera_combinacoes(arr, atual, 0, 0, k);
```

> **Quando usar:** escolher `k` elementos de `n` para testar — ex: testar todos os subgrupos de jogadores, todas as combinações de ingredientes, etc.

---

## Permutações

Gera todas as ordenações possíveis de um array. Complexidade: **O(n!)**.

### Versão por Swap — O(n!)

Fixa um elemento na posição atual via `swap`, recursa para o restante e desfaz:

```cpp
void gera_permutacoes(vector<int>& arr, int l, int r) {
    if (l == r) { /* permutação completa */ return; }

    for (int i = l; i <= r; i++) {
        swap(arr[l], arr[i]);       // Fixa arr[i] na posição l
        gera_permutacoes(arr, l + 1, r);
        swap(arr[l], arr[i]);       // Desfaz (backtrack)
    }
}
```

### Versão por Vetor de Usados

Mais flexível quando os elementos não são contíguos ou têm restrições:

```cpp
void gera_permutacao2(int n, int k, vector<int>& perm, vector<bool>& usados) {
    if (n == k) { /* permutação completa */ return; }

    for (int i = 0; i < n; i++) {
        if (!usados[i]) {
            usados[i] = true;
            perm[k] = i;
            gera_permutacao2(n, k + 1, perm, usados);
            usados[i] = false;  // Backtrack
        }
    }
}
```

### Alternativa com STL

Para competições, `std::next_permutation` é o mais rápido de escrever:

```cpp
sort(arr.begin(), arr.end()); // OBRIGATÓRIO: array deve começar ordenado
do {
    // processa arr
} while (next_permutation(arr.begin(), arr.end()));
```

> **Atenção:** esquecer o `sort` antes do `do-while` é um erro clássico — você só vai iterar pelas permutações maiores que a ordem inicial.

---

## Bitmask Tricks

Operações avançadas com máscaras de bits que aparecem frequentemente em DP com bitmask e enumeração eficiente.

### Submáscaras de uma Máscara — O(3ⁿ)

Itera por todos os subconjuntos de um subconjunto `mask` sem repetir nenhum. O truque `(sub - 1) & mask` garante que só pulamos para subconjuntos válidos:

```cpp
for (int mask = 0; mask < (1 << n); mask++) {
    for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
        // 'sub' é um subconjunto de 'mask'
    }
    // sub == 0 (subconjunto vazio) sempre existe
}
```

> **Por que O(3ⁿ)?** Cada elemento pode estar: fora de `mask`, em `mask` mas fora de `sub`, ou em ambos — 3 opções por elemento.

### Gosper's Hack — O(1) por combinação

Gera todas as máscaras com exatamente `k` bits ligados em ordem crescente, sem precisar verificar todas as `2ⁿ` máscaras:

```cpp
int mask = (1 << k) - 1; // Começa com os k bits menos significativos ligados
int limite = (1 << n);

while (mask < limite) {
    // processa mask (tem exatamente k bits ligados)

    int c = mask & -mask;               // Isola o bit mais à direita
    int r = mask + c;                   // Vira o bloco de 1s para 0, liga o próximo
    mask = (((r ^ mask) >> 2) / c) | r;
}
```

**Exemplo para n=4, k=2:** gera `0011 → 0101 → 0110 → 1001 → 1010 → 1100`

> **Quando usar Bitmask Tricks:** DP em subconjuntos (ex: TSP com DP), problemas de cobertura, ou qualquer situação onde você precise iterar sobre combinações de bits de forma eficiente.