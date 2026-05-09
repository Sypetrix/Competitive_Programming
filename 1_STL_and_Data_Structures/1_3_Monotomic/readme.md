# Pilha e Fila Monotônicas (Monotonic Stack & Queue)

Pilhas e filas monotônicas são padrões de uso de `stack` e `deque` que mantêm os elementos em **ordem estritamente crescente ou decrescente**. Elas eliminam a necessidade de loops aninhados O(N²) em problemas onde você precisa, para cada elemento, encontrar o próximo (ou anterior) elemento maior ou menor.

---

## Visão Geral

| Estrutura | Operação principal | Complexidade | Uso típico |
|---|---|---|---|
| Pilha Monotônica | Próximo/anterior maior ou menor | O(N) total | Histogramas, temperaturas, spans |
| Fila Monotônica | Máximo/mínimo em janela deslizante | O(N) total | Sliding window max/min |

Apesar dos nomes diferentes, ambas processam cada elemento **no máximo uma vez** — a análise amortizada garante O(N) total, não O(N) por elemento.

---

## Pilha Monotônica

### Próximo Elemento Maior (Next Greater Element)

Para cada posição `i`, encontra o índice do primeiro elemento à direita que é maior que `arr[i]`.

```cpp
// resultado[i] = arr[j], onde j é o menor índice > i com arr[j] > arr[i]
// resultado[i] = -1 se não existir

vector<int> proximo_maior(const vector<int>& arr) {
    int n = arr.size();
    vector<int> res(n, -1);
    stack<int> idx; // guarda índices, não valores

    for (int i = 0; i < n; i++) {
        // Enquanto o topo é menor que o elemento atual, o atual é a resposta do topo
        while (!idx.empty() && arr[idx.top()] < arr[i]) {
            res[idx.top()] = arr[i];
            idx.pop();
        }
        idx.push(i);
    }
    return res;
}
```

**Exemplo:** `arr = [2, 1, 5, 3, 4]` → `res = [5, 5, -1, 4, -1]`

### Anterior Elemento Maior (Previous Greater Element)

```cpp
vector<int> anterior_maior(const vector<int>& arr) {
    int n = arr.size();
    vector<int> res(n, -1);
    stack<int> idx;

    for (int i = 0; i < n; i++) {
        while (!idx.empty() && arr[idx.top()] <= arr[i]) idx.pop();
        if (!idx.empty()) res[i] = arr[idx.top()];
        idx.push(i);
    }
    return res;
}
```

### Variações

Substituindo `<` por `>` e `>` por `<`, você obtém as versões para **menor** elemento.

| Problema | Sinal na condição do while |
|---|---|
| Próximo **maior** | `arr[topo] < arr[i]` → pop |
| Próximo **menor** | `arr[topo] > arr[i]` → pop |
| Anterior **maior** | Mesma lógica, mas olhando o topo após limpar |
| Anterior **menor** | Idem com sinal invertido |

### Problema Clássico: Maior Retângulo no Histograma

Dado um vetor de alturas, encontra a área do maior retângulo que pode ser formado.

```cpp
int maior_retangulo(const vector<int>& h) {
    int n = h.size();
    stack<int> s;
    int max_area = 0;

    for (int i = 0; i <= n; i++) {
        int altura = (i == n) ? 0 : h[i];
        while (!s.empty() && h[s.top()] > altura) {
            int alt = h[s.top()]; s.pop();
            int larg = s.empty() ? i : i - s.top() - 1;
            max_area = max(max_area, alt * larg);
        }
        s.push(i);
    }
    return max_area;
}
```

### Validação de Parênteses

Aplicação clássica de stack para verificar balanceamento.

```cpp
bool valida_parenteses(const string& expr) {
    stack<char> s;
    for (char c : expr) {
        if (c == '(' || c == '[' || c == '{') {
            s.push(c);
        } else {
            if (s.empty()) return false;
            char t = s.top();
            if ((c == ')' && t == '(') ||
                (c == ']' && t == '[') ||
                (c == '}' && t == '{')) s.pop();
            else return false;
        }
    }
    return s.empty();
}
```

---

## Fila Monotônica

### Sliding Window Maximum — O(N)

Para cada janela de tamanho `k`, encontra o elemento máximo. A deque mantém **índices** em ordem decrescente de valor — o front é sempre o índice do máximo da janela atual.

```cpp
vector<int> sliding_window_max(const vector<int>& arr, int k) {
    deque<int> dq; // guarda índices
    vector<int> res;

    for (int i = 0; i < (int)arr.size(); i++) {
        // Remove índices fora da janela
        if (!dq.empty() && dq.front() == i - k) dq.pop_front();

        // Remove índices cujos valores são menores que o atual (serão inúteis)
        while (!dq.empty() && arr[dq.back()] <= arr[i]) dq.pop_back();

        dq.push_back(i);

        // O front é sempre o máximo da janela atual
        if (i >= k - 1) res.push_back(arr[dq.front()]);
    }
    return res;
}
```

**Exemplo:** `arr = [1, 3, -1, -3, 5, 3, 6, 7]`, `k = 3` → `res = [3, 3, 5, 5, 6, 7]`

### Sliding Window Minimum

Substitua `<=` por `>=` na condição de limpeza da deque — o menor valor ficará no front.

---

## Quando usar

**Pilha Monotônica:** o problema pede, para cada elemento, o vizinho mais próximo com alguma propriedade de ordem (maior, menor, maior-ou-igual, etc.). Variantes: *stock span*, *temperaturas*, *histograma*, *sum of subarray minimums*.

**Fila Monotônica:** o problema tem uma **janela deslizante** de tamanho fixo `k` e pede o máximo ou mínimo de cada janela. Também útil em DP com transições que podem ser otimizadas via mínimo/máximo em intervalo móvel.

> Em ambos os casos, **cada elemento entra e sai da estrutura no máximo uma vez** — daí a complexidade O(N) total, mesmo que o loop interno pareça repetir trabalho.