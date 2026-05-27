# Estruturas de Sufixos (Suffix Structures)

Estruturas de sufixos indexam **todos os sufixos** de uma string de uma vez, permitindo responder queries complexas sobre substrings em tempo sub-linear após a construção.

---

## Visão Geral

| Estrutura | Construção | Query | Uso típico |
|---|---|---|---|
| Suffix Array + LCP | O(N log N) | O(M log N) busca | Substrings distintas, LCP entre sufixos, buscas repetidas |
| Suffix Automaton | O(N) | O(M) | Contar substrings distintas, número de ocorrências |
| Trie | O(Σ\|Si\|) | O(M) | Busca de prefixo, autocompletar, XOR máximo |

---

## Suffix Array (SA) + LCP Array

O **Suffix Array** é um array `sa[i]` contendo os índices dos sufixos em ordem lexicográfica. O **LCP array** `lcp[i]` guarda o comprimento do maior prefixo comum entre `sa[i]` e `sa[i-1]`.

### Construção — O(N log N)

```cpp
vector<int> suffix_array(const string& s) {
    int n = s.size();
    vector<int> sa(n), rank_(n), tmp(n);

    // Inicialização
    iota(sa.begin(), sa.end(), 0);
    for (int i = 0; i < n; i++) rank_[i] = s[i];

    for (int k = 1; k < n; k <<= 1) {
        // Comparador: ordena por (rank_[i], rank_[i+k])
        auto cmp = [&](int a, int b) {
            if (rank_[a] != rank_[b]) return rank_[a] < rank_[b];
            int ra = a+k < n ? rank_[a+k] : -1;
            int rb = b+k < n ? rank_[b+k] : -1;
            return ra < rb;
        };
        sort(sa.begin(), sa.end(), cmp);

        tmp[sa[0]] = 0;
        for (int i = 1; i < n; i++)
            tmp[sa[i]] = tmp[sa[i-1]] + (cmp(sa[i-1], sa[i]) ? 1 : 0);
        rank_ = tmp;
        if (rank_[sa[n-1]] == n-1) break; // todos os ranks são únicos
    }
    return sa;
}
```

> Para competições com strings muito grandes (N > 10⁵) e limite de tempo apertado, use o algoritmo SA-IS (O(N)) ou a implementação com radix sort.

### LCP Array com Algoritmo de Kasai — O(N)

```cpp
vector<int> build_lcp(const string& s, const vector<int>& sa) {
    int n = s.size();
    vector<int> rank_(n), lcp(n, 0);
    for (int i = 0; i < n; i++) rank_[sa[i]] = i;

    int h = 0;
    for (int i = 0; i < n; i++) {
        if (rank_[i] > 0) {
            int j = sa[rank_[i] - 1];
            while (i+h < n && j+h < n && s[i+h] == s[j+h]) h++;
            lcp[rank_[i]] = h;
            if (h > 0) h--;
        }
    }
    return lcp;
}
```

### Aplicações do SA + LCP

**Número de substrings distintas:**
```cpp
long long count_distinct(const string& s) {
    int n = s.size();
    auto sa  = suffix_array(s);
    auto lcp = build_lcp(s, sa);
    long long total = (long long)n*(n+1)/2;
    for (int x : lcp) total -= x; // remove duplicatas
    return total;
}
```

**Busca de padrão P — O(M log N):**
```cpp
// Retorna range [lo, hi) de SA onde P ocorre
pair<int,int> sa_search(const string& s, const vector<int>& sa,
                        const string& P) {
    int n = s.size(), m = P.size();
    // lower_bound: primeiro sufixo >= P
    int lo = lower_bound(sa.begin(), sa.end(), 0, [&](int a, int) {
        return s.substr(a, m) < P;
    }) - sa.begin();
    // upper_bound: primeiro sufixo > P
    int hi = upper_bound(sa.begin(), sa.end(), 0, [&](int, int a) {
        return P < s.substr(a, m);
    }) - sa.begin();
    return {lo, hi};
}
```

**LCP entre dois sufixos em O(1)** com Sparse Table sobre o LCP array (Range Minimum Query).

---

## Suffix Automaton (SAM)

O **Suffix Automaton** é o menor autômato finito que aceita todos os sufixos de S. Cada estado representa um conjunto de substrings (uma classe de equivalência), e o número total de estados é O(N).

### Construção — O(N)

```cpp
struct SAM {
    struct State {
        int len, link;
        map<char,int> next;
    };

    vector<State> st;
    int last;

    SAM() {
        st.push_back({0, -1, {}});
        last = 0;
    }

    void extend(char c) {
        // Verifica se transição já existe (otimização)
        if (st[last].next.count(c)) {
            int q = st[last].next[c];
            if (st[q].len == st[last].len + 1) {
                last = q;
                return;
            }
            int clone = st.size();
            st.push_back({st[last].len + 1, st[q].link, st[q].next});
            while (last != -1 && st[last].next[c] == q) {
                st[last].next[c] = clone;
                last = st[last].link;
            }
            st[q].link = clone;
            last = clone;
            return;
        }

        int cur = st.size();
        st.push_back({st[last].len + 1, -1, {}});
        int p = last;
        while (p != -1 && !st[p].next.count(c)) {
            st[p].next[c] = cur;
            p = st[p].link;
        }
        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c];
            if (st[q].len == st[p].len + 1) {
                st[cur].link = q;
            } else {
                int clone = st.size();
                st.push_back({st[p].len + 1, st[q].link, st[q].next});
                while (p != -1 && st[p].next.count(c) && st[p].next[c] == q) {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    void build(const string& s) {
        for (char c : s) extend(c);
    }
};
```

### Aplicações do SAM

**Número de substrings distintas:**
```cpp
// Cada estado v representa (st[v].len - st[st[v].link].len) substrings distintas
long long count_distinct(SAM& sam) {
    long long ans = 0;
    for (int i = 1; i < (int)sam.st.size(); i++)
        ans += sam.st[i].len - sam.st[sam.st[i].link].len;
    return ans;
}
```

**Verificar se P é substring de S — O(M):**
```cpp
bool is_substring(SAM& sam, const string& P) {
    int cur = 0;
    for (char c : P) {
        if (!sam.st[cur].next.count(c)) return false;
        cur = sam.st[cur].next[c];
    }
    return true;
}
```

**Número de ocorrências de cada substring:** marque os estados criados por `extend` (não clones) com `cnt = 1`; faça topological sort por `len` decrescente e propague `cnt` pelos suffix links.

---

## Trie (Árvore de Prefixos)

Cada nó representa uma letra. Permite inserção e busca em **O(M)** onde M é o tamanho da string, independente do número de palavras armazenadas.

```cpp
const int MAXN = 1e5 + 5;
const int ALPHA = 26;

int trie[MAXN][ALPHA];
int is_end[MAXN];
int node_cnt = 0;

void init_trie() {
    for (int i = 0; i <= node_cnt; i++) {
        is_end[i] = 0;
        fill(trie[i], trie[i] + ALPHA, 0);
    }
    node_cnt = 0;
}

void insert(const string& s) {
    int u = 0;
    for (char c : s) {
        int idx = c - 'a';
        if (!trie[u][idx]) trie[u][idx] = ++node_cnt;
        u = trie[u][idx];
    }
    is_end[u]++;
}

bool search(const string& s) {
    int u = 0;
    for (char c : s) {
        int idx = c - 'a';
        if (!trie[u][idx]) return false;
        u = trie[u][idx];
    }
    return is_end[u] > 0;
}

bool starts_with(const string& prefix) {
    int u = 0;
    for (char c : prefix) {
        int idx = c - 'a';
        if (!trie[u][idx]) return false;
        u = trie[u][idx];
    }
    return true;
}
```

### Trie Binária (XOR Máximo)

Para encontrar o XOR máximo entre um número e qualquer elemento de um conjunto, insira os números bit a bit (do MSB ao LSB) e, ao buscar, tente sempre o bit oposto.

```cpp
int trie_bin[MAXN * 32][2];
int node_cnt_bin = 0;

void insert_bin(int x) {
    int u = 0;
    for (int i = 30; i >= 0; i--) {
        int bit = (x >> i) & 1;
        if (!trie_bin[u][bit]) trie_bin[u][bit] = ++node_cnt_bin;
        u = trie_bin[u][bit];
    }
}

int max_xor(int x) {
    int u = 0, res = 0;
    for (int i = 30; i >= 0; i--) {
        int bit = (x >> i) & 1;
        int want = 1 - bit; // tenta o bit oposto para maximizar
        if (trie_bin[u][want]) { res |= (1 << i); u = trie_bin[u][want]; }
        else u = trie_bin[u][bit];
    }
    return res;
}
```

---

## Quando Usar Cada Estrutura

```
Busca de substrings com LCP e ranking?
└── Suffix Array

Contar substrings distintas / ocorrências?
└── Suffix Automaton (mais compacto e versátil)

Busca de prefixos / inserção de palavras?
└── Trie

XOR máximo de pares?
└── Trie Binária
```