# Estruturas Lineares

Estruturas lineares organizam os dados em sequência — cada elemento tem um predecessor e um sucessor. São a base de quase todo algoritmo em programação competitiva.

<!-- Sugestão de imagem: diagrama comparando vector, stack, queue e deque lado a lado -->

---

## Visão Geral

| Estrutura | Acesso | Inserção/Remoção | Uso típico |
|---|---|---|---|
| `vector` | O(1) aleatório | O(1) no final | Array dinâmico geral |
| `deque` | O(1) aleatório | O(1) nas duas pontas | Sliding window, BFS com custo |
| `array` | O(1) aleatório | Tamanho fixo | Substituir `int arr[N]` com STL |
| `stack` | O(1) no topo | O(1) no topo | DFS, parênteses, pilha monotônica |
| `queue` | O(1) nas pontas | O(1) nas pontas | BFS |
| `string` | O(1) por índice | O(1) no final | Manipulação de texto |
| `bitset` | O(1) por bit | O(1) por bit | DP de bitmask, popcount rápido |

---

## Vector

Array dinâmico que dobra de capacidade automaticamente. O contêiner mais usado em CP.

```cpp
vector<int> v = {1, 2, 3};
v.push_back(4);       // [1, 2, 3, 4]
v.pop_back();         // [1, 2, 3]
v[0];                 // acesso O(1)
v.size(); v.empty();
sort(v.begin(), v.end());
reverse(v.begin(), v.end());
```

**Remover duplicatas (sort → unique → erase):**
```cpp
sort(v.begin(), v.end());
v.erase(unique(v.begin(), v.end()), v.end());
```

**Matriz 2D:**
```cpp
vector<vector<int>> mat(linhas, vector<int>(colunas, 0));
```

> **Evite** `v = v + elemento` em loops — cria uma cópia O(N) a cada iteração. Use sempre `push_back`.

---

## Deque

Permite inserção e remoção eficiente em **ambas as pontas**. Base da técnica de Sliding Window Maximum.

```cpp
deque<int> dq;
dq.push_back(10);   dq.push_front(20);
dq.pop_back();      dq.pop_front();
dq.front();         dq.back();
dq[i]; // acesso aleatório O(1)
```

**Sliding Window Maximum — O(N):**
```cpp
deque<int> dq; // guarda índices
for (int i = 0; i < n; i++) {
    if (!dq.empty() && dq.front() == i - k) dq.pop_front();
    while (!dq.empty() && arr[dq.back()] <= arr[i]) dq.pop_back();
    dq.push_back(i);
    if (i >= k - 1) cout << arr[dq.front()] << " ";
}
```

---

## Stack

Princípio **LIFO** (Last-In, First-Out). Inserção e remoção apenas no topo.

```cpp
stack<int> s;
s.push(10); s.emplace(20);
s.top();    // lê o topo sem remover
s.pop();    // remove o topo (não retorna valor — leia antes!)
s.size();   s.empty();
stack<int>().swap(s); // limpa em O(1) (não existe .clear())
```

**Pilha Monotônica — Próximo Elemento Maior em O(N):**
```cpp
stack<int> idx;
for (int i = 0; i < n; i++) {
    while (!idx.empty() && arr[idx.top()] < arr[i]) {
        resultado[idx.top()] = arr[i];
        idx.pop();
    }
    idx.push(i);
}
```

**Validação de Parênteses:**
```cpp
for (char c : expr) {
    if (c == '(' || c == '[' || c == '{') s.push(c);
    else {
        if (s.empty()) return false;
        if ((c==')' && s.top()=='(') || ...) s.pop();
        else return false;
    }
}
return s.empty();
```

---

## Queue

**Queue** — princípio **FIFO** (First-In, First-Out). Base da BFS.

```cpp
queue<int> q;
q.push(10); q.emplace(20);
q.front();  q.back();
q.pop();    // remove a frente (não retorna valor!)
q.size();   q.empty();
queue<int>().swap(q); // limpa em O(1) (não existe .clear())
```

> **Atenção:** tanto `stack` quanto `queue` não possuem `.clear()`. Use o truque do swap.

---

## String

Funciona como um `vector<char>` com métodos extras para texto.

```cpp
string s = "maratona";
s.length();           // tamanho
s[i];                 // acesso O(1)
s += "s";             // concatenação O(1) — use +=, não s = s + "x"
s.substr(0, 4);       // "mara" — O(K)
s.find("rat");        // posição ou string::npos
s.rfind('a');         // busca da direita
s.erase(0, 3);        // remove os 3 primeiros chars
s.insert(0, "mar");   // insere no início
s.replace(3, 4, "X"); // substitui 4 chars a partir do índice 3
to_string(123);       // int → string
stoi("123");          // string → int
stoll("123456789");   // string → long long
sort(s.begin(), s.end());
reverse(s.begin(), s.end());
```

> **Armadilha:** após `cin >> n`, use `cin.ignore()` antes de `getline(cin, s)` para consumir o `\n` que sobrou.

---

## Algoritmo KMP - String

Busca um padrão `P` dentro de um texto `T` em **O(N + M)**, evitando comparações redundantes com o array `pi` (failure function).

<!-- Sugestão de imagem: diagrama do array pi para o padrão "ABABC" -->

```
pi[i] = tamanho do maior prefixo de P[0..i] que também é sufixo
```

```cpp
// 1. Pré-processa o padrão — O(M)
vector<int> compute_pi(const string& P) {
    int m = P.size();
    vector<int> pi(m, 0);
    for (int i = 1, j = 0; i < m; i++) {
        while (j > 0 && P[i] != P[j]) j = pi[j - 1];
        if (P[i] == P[j]) j++;
        pi[i] = j;
    }
    return pi;
}

// 2. Busca no texto — O(N)
int kmp(const string& T, const string& P) {
    vector<int> pi = compute_pi(P);
    int count = 0;
    for (int i = 0, j = 0; i < T.size(); i++) {
        while (j > 0 && T[i] != P[j]) j = pi[j - 1];
        if (T[i] == P[j]) j++;
        if (j == (int)P.size()) { count++; j = pi[j - 1]; }
    }
    return count;
}
```

> **Use KMP quando** `N` e `M` forem grandes (ex: N = 10⁶). Para padrões pequenos, `s.find()` é suficiente.

---

## Bitset

Vetor de bits compacto com operações bitwise em **O(N/64)** — muito mais rápido que `vector<bool>` para operações em massa.

```cpp
bitset<1024> b;
b.set(10);    // liga o bit 10
b.reset(10);  // desliga o bit 10
b.flip(5);    // inverte o bit 5
b.test(5);    // lê o bit 5
b.count();    // popcount — quantidade de bits ligados

bitset<1024> a, c;
a |= c;           // OR
a &= (c << 2);    // AND com deslocamento
a ^= c;           // XOR
```

> **Uso clássico em CP:** DP de subconjuntos, crivo de Eratóstenes otimizado, e problemas de contagem onde o estado cabe em bits.

---

## BigInt

Inteiro de precisão arbitrária para quando os valores ultrapassam o limite do `long long` (~9.2 × 10¹⁸).

```cpp
BigInt a("12345678901234567890");
BigInt b("98765432109876543210");

BigInt c = a + b;
BigInt d = a * b;
bool maior = (b > a);
cout << c << "\n";
```

> **Use BigInt quando** o enunciado pedir resultados com mais de 18 dígitos e não houver módulo. Se houver módulo, prefira `long long` com `% MOD`.