# Técnicas de Ponteiros (Pointer Techniques)

Técnicas de ponteiros exploram a **monototonicidade** de arrays ordenados ou com propriedades estruturais para reduzir O(N²) para O(N). Em vez de testar todos os pares com dois loops aninhados, dois ponteiros se movem de forma coordenada — cada elemento é visitado no máximo uma ou duas vezes.

---

## Visão Geral

| Técnica | Estrutura | Complexidade | Uso típico |
|---|---|---|---|
| Two Pointers (opostos) | Array ordenado | O(N) | Par com soma alvo, container with most water |
| Two Pointers (mesmo sentido) | Array | O(N) | Remover duplicatas, subarrays sem repetição |
| Sliding Window (tamanho fixo) | Array/string | O(N) | Máximo/mínimo/soma em janela de tamanho K |
| Sliding Window (tamanho variável) | Array/string | O(N) | Menor subarray com soma ≥ S, substring sem repetição |

---

## Two Pointers — Ponteiros Opostos

Os dois ponteiros começam nas extremidades e avançam um em direção ao outro. Funciona quando o array está **ordenado** e a função de decisão é monotônica: se `arr[l] + arr[r] > alvo`, diminuir `r` só pode reduzir a soma; se `< alvo`, aumentar `l` só pode aumentá-la.

**Caso clássico: par com soma igual ao alvo**

```cpp
// arr deve estar ordenado
bool two_sum(vector<int>& arr, int alvo) {
    int l = 0, r = arr.size() - 1;
    while (l < r) {
        int soma = arr[l] + arr[r];
        if (soma == alvo) return true;
        else if (soma < alvo) l++;
        else r--;
    }
    return false;
}
```

**Contar pares com soma ≤ alvo**

```cpp
long long contar_pares(vector<int>& arr, int alvo) {
    sort(arr.begin(), arr.end());
    long long count = 0;
    int l = 0, r = arr.size() - 1;
    while (l < r) {
        if (arr[l] + arr[r] <= alvo) {
            count += r - l; // todos os pares (l, l+1), (l, l+2), ..., (l, r) são válidos
            l++;
        } else {
            r--;
        }
    }
    return count;
}
```

---

## Two Pointers — Mesmo Sentido

Ambos os ponteiros avançam na mesma direção. `r` expande a janela e `l` a contrai quando uma condição é violada.

**Remover duplicatas in-place (array ordenado)**

```cpp
int remover_duplicatas(vector<int>& arr) {
    if (arr.empty()) return 0;
    int l = 0;
    for (int r = 1; r < (int)arr.size(); r++) {
        if (arr[r] != arr[l]) {
            l++;
            arr[l] = arr[r];
        }
    }
    return l + 1; // novo tamanho
}
```

**Maior subarray sem elementos repetidos**

```cpp
int maior_subarray_unico(const string& s) {
    unordered_map<char, int> freq;
    int l = 0, res = 0;
    for (int r = 0; r < (int)s.size(); r++) {
        freq[s[r]]++;
        while (freq[s[r]] > 1) { // violou a condição: encolhe pela esquerda
            freq[s[l]]--;
            l++;
        }
        res = max(res, r - l + 1);
    }
    return res;
}
```

---

## Sliding Window — Tamanho Fixo — O(N)

Mantém uma janela de exatamente K elementos, deslizando da esquerda para a direita. Em vez de recalcular tudo do zero, **remove o elemento que saiu e adiciona o que entrou**.

**Soma máxima de subarray de tamanho K**

```cpp
long long soma_max_janela(const vector<int>& arr, int k) {
    long long soma = 0;
    for (int i = 0; i < k; i++) soma += arr[i]; // janela inicial

    long long res = soma;
    for (int i = k; i < (int)arr.size(); i++) {
        soma += arr[i] - arr[i - k]; // adiciona novo, remove antigo
        res = max(res, soma);
    }
    return res;
}
```

**Média de todas as janelas de tamanho K**

```cpp
vector<double> medias_janela(const vector<int>& arr, int k) {
    vector<double> res;
    double soma = 0;
    for (int i = 0; i < k; i++) soma += arr[i];
    res.push_back(soma / k);
    for (int i = k; i < (int)arr.size(); i++) {
        soma += arr[i] - arr[i - k];
        res.push_back(soma / k);
    }
    return res;
}
```

---

## Sliding Window — Tamanho Variável — O(N)

O ponteiro `r` expande a janela e `l` a contrai até satisfazer a condição. Cada elemento é adicionado e removido no máximo uma vez, logo O(N) amortizado.

**Menor subarray com soma ≥ S**

```cpp
int menor_subarray(const vector<int>& arr, int S) {
    int l = 0, soma = 0, res = INT_MAX;
    for (int r = 0; r < (int)arr.size(); r++) {
        soma += arr[r];
        while (soma >= S) {
            res = min(res, r - l + 1);
            soma -= arr[l++];
        }
    }
    return res == INT_MAX ? 0 : res;
}
```

**Maior substring com no máximo K caracteres distintos**

```cpp
int maior_com_k_distintos(const string& s, int k) {
    unordered_map<char, int> freq;
    int l = 0, res = 0;
    for (int r = 0; r < (int)s.size(); r++) {
        freq[s[r]]++;
        while ((int)freq.size() > k) {
            freq[s[l]]--;
            if (freq[s[l]] == 0) freq.erase(s[l]);
            l++;
        }
        res = max(res, r - l + 1);
    }
    return res;
}
```

---

## Quando Usar

**Two Pointers com ponteiros opostos:** array **ordenado**, você quer encontrar pares ou triplas satisfazendo uma condição de soma. A ordenação garante que mover `l` aumenta e mover `r` diminui a soma — a decisão de qual ponteiro mover é determinística.

**Two Pointers com mesmo sentido:** você quer o maior/menor subarray satisfazendo uma condição que é **monotônica**: ampliar a janela nunca melhora a condição que está sendo violada.

**Sliding Window de tamanho fixo:** qualquer agregado (soma, máximo, frequência) sobre janelas de tamanho exatamente K.

**Sliding Window de tamanho variável:** o problema pede o menor/maior subarray que satisfaz uma condição; a condição pode ser checada em O(1) ou O(log N) ao adicionar/remover um elemento.

> Se a condição de uma Sliding Window não é monotônica (ampliar pode tanto ajudar como prejudicar), a técnica não se aplica diretamente — considere Divide and Conquer ou DP.