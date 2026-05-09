# Estruturas Lineares

Estruturas lineares organizam dados em sequência: cada elemento tem um predecessor e um sucessor direto. São a base de quase todo algoritmo em programação competitiva — desde a BFS mais simples até parsers de strings complexos.

---

## Visão Geral

| Estrutura | Acesso | Inserção/Remoção | Uso típico |
|---|---|---|---|
| `vector` | O(1) aleatório | O(1) no final* | Array dinâmico geral |
| `deque` | O(1) aleatório | O(1) nas duas pontas | Sliding window, BFS com custo |
| `array` | O(1) aleatório | Tamanho fixo | Substituir `int arr[N]` com STL |
| `stack` | O(1) no topo | O(1) no topo | DFS iterativa, parênteses, pilha monotônica |
| `queue` | O(1) nas pontas | O(1) nas pontas | BFS |
| `string` | O(1) por índice | O(1) no final* | Manipulação de texto |

*\* amortizado*

---

## Vector

Array dinâmico que dobra de capacidade automaticamente. O contêiner mais usado em CP.

```cpp
vector<int> v = {1, 2, 3};
v.push_back(4);                    // O(1) amortizado
v.pop_back();                      // O(1)
v[0];                              // acesso O(1)
v.size(); v.empty();
sort(v.begin(), v.end());
reverse(v.begin(), v.end());

// Remover duplicatas (sort → unique → erase)
sort(v.begin(), v.end());
v.erase(unique(v.begin(), v.end()), v.end());

// Matriz 2D
vector<vector<int>> mat(linhas, vector<int>(colunas, 0));
```

> **Nunca faça** `v = v + elemento` em loops — cria uma cópia O(N) a cada iteração. Use sempre `push_back`.

---

## Deque

Permite inserção e remoção eficiente em **ambas as pontas**. Base da técnica de Sliding Window Maximum.

```cpp
deque<int> dq;
dq.push_back(10);   dq.push_front(20);
dq.pop_back();      dq.pop_front();
dq.front();         dq.back();
dq[i];              // acesso aleatório O(1)
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
s.push(10);
s.top();    // lê o topo sem remover
s.pop();    // remove o topo (não retorna valor — leia top() antes!)
s.size();   s.empty();
stack<int>().swap(s); // limpa em O(1) — não existe .clear()
```

> **Atenção:** os padrões de **pilha monotônica** (próximo elemento maior/menor) estão na seção **Monotonic**.

---

## Queue

Princípio **FIFO** (First-In, First-Out). Base da BFS.

```cpp
queue<int> q;
q.push(10);
q.front();  q.back();
q.pop();    // remove a frente (não retorna valor — leia front() antes!)
q.size();   q.empty();
queue<int>().swap(q); // limpa em O(1) — não existe .clear()
```

> `stack` e `queue` não possuem `.clear()`. Use o truque do swap para limpar em O(1).

---

## String

Funciona como um `vector<char>` com métodos extras para texto.

```cpp
string s = "maratona";
s.length();           // tamanho
s[i];                 // acesso O(1)
s += "s";             // concatenação O(1) amortizado — use +=, nunca s = s + "x" em loop
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

> **Armadilha:** após `cin >> n`, use `cin.ignore()` antes de `getline(cin, s)` para consumir o `\n` que ficou no buffer.

---

## KMP — Busca de Padrão em String

Busca um padrão `P` dentro de um texto `T` em **O(N + M)**, evitando comparações redundantes com o array `pi` (failure function).

```
pi[i] = tamanho do maior prefixo de P[0..i] que também é sufixo
```

```cpp
// Pré-processa o padrão — O(M)
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

// Conta ocorrências de P em T — O(N)
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

> **Use KMP quando** N e M forem grandes (ex: N = 10⁶). Para padrões pequenos, `s.find()` é suficiente.

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