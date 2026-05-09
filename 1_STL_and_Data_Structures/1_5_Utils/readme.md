# Utilitários e Estruturas Customizadas (Utils)

Esta pasta contém estruturas de dados que **não possuem implementação nativa na STL** mas são indispensáveis em competições: Union-Find, Fenwick Tree, Segment Tree, Sparse Table e Trie. Todas compartilham a característica de responder queries complexas em tempo sub-linear após uma pré-computação.

---

## Visão Geral

| Estrutura | Pré-computação | Query | Update | Uso típico |
|---|---|---|---|---|
| Union-Find | O(N) | O(α(N)) | O(α(N)) | Componentes conexas dinâmicas, Kruskal |
| Fenwick Tree | O(N) | O(log N) | O(log N) | Soma de prefixo com updates pontuais |
| Segment Tree | O(N) | O(log N) | O(log N) | Range query + range/point update |
| Sparse Table | O(N log N) | O(1) | ✗ imutável | Range Minimum Query (RMQ) estático |
| Trie | O(L) por inserção | O(L) | O(L) | Busca de palavras e prefixos |
| BigInt | — | O(N) | O(N) | Aritmética com números > 18 dígitos |

*α(N) = função inversa de Ackermann ≈ constante para todo N prático; N = número de dígitos para BigInt*

---

## Union-Find (Disjoint Set Union — DSU)

Mantém uma partição de N elementos em conjuntos disjuntos, respondendo em tempo quase constante se dois elementos pertencem ao mesmo conjunto e unindo conjuntos.

**Duas otimizações essenciais:**
- **Path Compression:** ao encontrar a raiz, aponta todos os nós do caminho diretamente para ela.
- **Union by Size:** sempre anexa a árvore menor na maior, mantendo a altura baixa.

```cpp
struct UF {
    vector<int> parent, sz;

    UF(int n) : parent(n + 1), sz(n + 1, 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]); // path compression
    }

    void unite(int i, int j) {
        int a = find(i), b = find(j);
        if (a == b) return;
        if (sz[a] < sz[b]) swap(a, b); // union by size
        parent[b] = a;
        sz[a] += sz[b];
    }

    bool same(int i, int j) { return find(i) == find(j); }
    int size(int i) { return sz[find(i)]; }
};
```

**Usos clássicos:** detectar ciclos em grafos não-direcionados, algoritmo de Kruskal (MST), perguntas offline de conectividade, problema do labirinto dinâmico.

---

## Fenwick Tree (Binary Indexed Tree — BIT)

Mantém um array com suporte a **atualização pontual** e **query de prefixo** em O(log N) cada. A operação `idx & -idx` isola o bit menos significativo, que define o intervalo de responsabilidade de cada nó.

```cpp
struct FenwickTree {
    int n;
    vector<int> bit;

    FenwickTree(int n) : n(n), bit(n + 1, 0) {}

    // Adiciona val na posição idx (1-based)
    void add(int idx, int val) {
        for (; idx <= n; idx += idx & -idx)
            bit[idx] += val;
    }

    // Retorna a soma do prefixo [1, idx]
    int query(int idx) {
        int sum = 0;
        for (; idx > 0; idx -= idx & -idx)
            sum += bit[idx];
        return sum;
    }

    // Retorna a soma do intervalo [l, r]
    int query(int l, int r) {
        return query(r) - query(l - 1);
    }
};
```

> **Use Fenwick Tree** quando as operações forem: (1) update pontual e (2) query de soma de prefixo/intervalo. É mais simples e tem constante menor que a Segment Tree para esse caso específico.

---

## Segment Tree com Lazy Propagation

A estrutura mais versátil para **range queries e range updates** em O(log N). O Lazy Propagation posterga atualizações de intervalos grandes para quando os filhos forem realmente acessados.

```cpp
const int MAXN = 1e5 + 5;
long long tree[4 * MAXN], lazy[4 * MAXN];

void build(int node, int s, int e, long long arr[]) {
    lazy[node] = 0;
    if (s == e) { tree[node] = arr[s]; return; }
    int mid = (s + e) / 2;
    build(2*node, s, mid, arr);
    build(2*node+1, mid+1, e, arr);
    tree[node] = tree[2*node] + tree[2*node+1];
}

void push(int node, int s, int e) {
    if (lazy[node]) {
        tree[node] += lazy[node] * (e - s + 1);
        if (s != e) {
            lazy[2*node]   += lazy[node];
            lazy[2*node+1] += lazy[node];
        }
        lazy[node] = 0;
    }
}

void update(int node, int s, int e, int l, int r, long long val) {
    push(node, s, e);
    if (s > r || e < l) return;
    if (s >= l && e <= r) { lazy[node] += val; push(node, s, e); return; }
    int mid = (s + e) / 2;
    update(2*node, s, mid, l, r, val);
    update(2*node+1, mid+1, e, l, r, val);
    tree[node] = tree[2*node] + tree[2*node+1];
}

long long query(int node, int s, int e, int l, int r) {
    push(node, s, e);
    if (s > r || e < l) return 0;
    if (s >= l && e <= r) return tree[node];
    int mid = (s + e) / 2;
    return query(2*node, s, mid, l, r) + query(2*node+1, mid+1, e, l, r);
}
// Uso: build(1, 1, N, arr); update(1, 1, N, L, R, val); query(1, 1, N, L, R)
```

> A função de combinação (`tree[node] = tree[left] + tree[right]`) pode ser trocada por `min`, `max`, `gcd`, etc. O Lazy Propagation deve ser adaptado de acordo com a operação.

---

## Sparse Table

Estrutura estática (sem updates) para **Range Minimum Query (RMQ) em O(1)**. Pré-computa, para cada posição i e cada potência k, o mínimo do intervalo `[i, i + 2ᵏ - 1]`. Queries se resolvem sobrepondo dois intervalos de potência de 2.

```cpp
const int LOG = 20;
int sparse[MAXN][LOG];
int lg[MAXN]; // pré-computação de log2

void build(const vector<int>& arr, int n) {
    lg[1] = 0;
    for (int i = 2; i <= n; i++) lg[i] = lg[i / 2] + 1;

    for (int i = 0; i < n; i++) sparse[i][0] = arr[i];
    for (int j = 1; (1 << j) <= n; j++)
        for (int i = 0; i + (1 << j) <= n; i++)
            sparse[i][j] = min(sparse[i][j-1], sparse[i + (1 << (j-1))][j-1]);
}

// Query O(1): mínimo em [l, r] (0-based)
int query(int l, int r) {
    int k = lg[r - l + 1];
    return min(sparse[l][k], sparse[r - (1 << k) + 1][k]);
}
```

> **Use Sparse Table** quando o array for **estático** (sem updates) e você precisar de RMQ ultra-rápido — por exemplo, como subestrutura em LCA ou em soluções que fazem muitas queries em intervalos.

---

## Trie (Árvore de Prefixos)

Cada nó representa uma letra. Permite inserção e busca em **O(L)** onde L é o tamanho da string, independente do número de palavras armazenadas.

```cpp
const int MAXN = 1e5 + 5;
const int ALPHA = 26;

int trie[MAXN][ALPHA];
int is_end[MAXN];
int node_cnt = 0;

void init() {
    fill(&trie[0][0], &trie[0][0] + sizeof(trie)/sizeof(int), 0);
    fill(is_end, is_end + MAXN, 0);
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

// Retorna true se a palavra exata existe
bool search(const string& s) {
    int u = 0;
    for (char c : s) {
        int idx = c - 'a';
        if (!trie[u][idx]) return false;
        u = trie[u][idx];
    }
    return is_end[u] > 0;
}

// Retorna true se alguma palavra começa com o prefixo
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

> **Use Trie** em problemas com grandes dicionários de strings onde é preciso verificar existência de prefixos, autocompletar ou agrupar strings por prefixo comum. Variante: **Trie binária** (bit a bit) para problemas de XOR máximo.

---

## BigInt — Inteiros de Precisão Arbitrária

Quando o resultado é um número com mais de 18 dígitos (ultrapassando `long long`), é necessário uma estrutura customizada. A implementação armazena os dígitos em **base 10⁹** — cada "dígito" interno cabe em um `int`, economizando memória e reduzindo o número de operações.

```cpp
struct BigInt {
    static const int BASE = 1e9;
    vector<int> d; // d[0] = menos significativo
    bool neg = false;

    BigInt() : d(1, 0) {}

    // Construtor a partir de long long
    BigInt(long long v) {
        neg = (v < 0); v = abs(v);
        do { d.push_back(v % BASE); v /= BASE; } while (v);
    }

    // Construtor a partir de string
    BigInt(const string& s) {
        neg = (!s.empty() && s[0] == '-');
        int i = neg ? 1 : 0;
        for (int j = (int)s.size(); j > i; j -= 9) {
            int from = max(i, j - 9);
            d.push_back(stoi(s.substr(from, j - from)));
        }
        trim();
    }

    // Remove zeros à esquerda (mantém pelo menos um dígito)
    void trim() {
        while (d.size() > 1 && d.back() == 0) d.pop_back();
        if (d.size() == 1 && d[0] == 0) neg = false;
    }

    bool isZero() const { return d.size() == 1 && d[0] == 0; }

    // Comparação de módulo (ignora sinal)
    int cmpAbs(const BigInt& o) const {
        if (d.size() != o.d.size()) return d.size() < o.d.size() ? -1 : 1;
        for (int i = (int)d.size() - 1; i >= 0; i--)
            if (d[i] != o.d[i]) return d[i] < o.d[i] ? -1 : 1;
        return 0;
    }

    // Operadores de comparação
    bool operator==(const BigInt& o) const { return neg == o.neg && cmpAbs(o) == 0; }
    bool operator<(const BigInt& o) const {
        if (neg != o.neg) return neg;
        return neg ? cmpAbs(o) > 0 : cmpAbs(o) < 0;
    }
    bool operator>(const BigInt& o) const { return o < *this; }
    bool operator<=(const BigInt& o) const { return !(o < *this); }
    bool operator>=(const BigInt& o) const { return !(*this < o); }

    // Adição de módulos (assume cmpAbs(a, b) >= 0)
    static BigInt addAbs(const BigInt& a, const BigInt& b) {
        BigInt res;
        res.d.resize(max(a.d.size(), b.d.size()) + 1, 0);
        long long carry = 0;
        for (int i = 0; i < (int)res.d.size(); i++) {
            long long s = carry;
            if (i < (int)a.d.size()) s += a.d[i];
            if (i < (int)b.d.size()) s += b.d[i];
            res.d[i] = s % BASE;
            carry = s / BASE;
        }
        res.trim();
        return res;
    }

    // Subtração de módulos (assume cmpAbs(a, b) >= 0)
    static BigInt subAbs(const BigInt& a, const BigInt& b) {
        BigInt res;
        res.d.resize(a.d.size(), 0);
        long long borrow = 0;
        for (int i = 0; i < (int)a.d.size(); i++) {
            long long diff = (long long)a.d[i] - borrow;
            if (i < (int)b.d.size()) diff -= b.d[i];
            if (diff < 0) { diff += BASE; borrow = 1; }
            else borrow = 0;
            res.d[i] = diff;
        }
        res.trim();
        return res;
    }

    // Adição com sinal
    BigInt operator+(const BigInt& o) const {
        if (neg == o.neg) {
            BigInt res = addAbs(*this, o);
            res.neg = neg;
            return res;
        }
        if (cmpAbs(o) >= 0) {
            BigInt res = subAbs(*this, o);
            res.neg = neg;
            res.trim();
            return res;
        }
        BigInt res = subAbs(o, *this);
        res.neg = o.neg;
        res.trim();
        return res;
    }

    // Subtração com sinal
    BigInt operator-(const BigInt& o) const {
        BigInt tmp = o;
        tmp.neg = !o.neg;
        return *this + tmp;
    }

    BigInt& operator+=(const BigInt& o) { return *this = *this + o; }
    BigInt& operator-=(const BigInt& o) { return *this = *this - o; }

    // Multiplicação — O(N·M) onde N, M = número de dígitos internos
    BigInt operator*(const BigInt& o) const {
        BigInt res;
        res.d.assign(d.size() + o.d.size(), 0);
        for (int i = 0; i < (int)d.size(); i++) {
            long long carry = 0;
            for (int j = 0; j < (int)o.d.size() || carry; j++) {
                long long cur = (long long)res.d[i + j] + carry;
                if (j < (int)o.d.size()) cur += (long long)d[i] * o.d[j];
                res.d[i + j] = cur % BASE;
                carry = cur / BASE;
            }
        }
        res.neg = neg ^ o.neg;
        res.trim();
        return res;
    }

    BigInt& operator*=(const BigInt& o) { return *this = *this * o; }

    // I/O — imprime com cuidado (zeros à esquerda em cada dígito interno)
    friend ostream& operator<<(ostream& os, const BigInt& b) {
        if (b.neg) os << '-';
        os << b.d.back();
        for (int i = (int)b.d.size() - 2; i >= 0; i--)
            os << setw(9) << setfill('0') << b.d[i];
        return os;
    }

    friend istream& operator>>(istream& is, BigInt& b) {
        string s;
        is >> s;
        b = BigInt(s);
        return is;
    }
};

// Exemplo de uso
int main() {
    BigInt a("12345678901234567890");
    BigInt b("98765432109876543210");
    cout << a + b << "\n";  // soma
    cout << a * b << "\n";  // multiplicação
    
    // Fatorial de 20 (ultrapassa long long)
    BigInt fat(1);
    for (int i = 2; i <= 20; i++) fat *= BigInt(i);
    cout << "20! = " << fat << "\n";

    return 0;
}
```

**Limitações e otimizações:**
- Não implementa **divisão** (seria O(N²) naive). Se precisar, use apenas para casos bem específicos.
- **Multiplicação é cara** — para N muito grande (1000+ dígitos), considere **FFT/NTT** em problemas que exijam.
- Não trata **notação científica** — armazena sempre como inteiro.
- Para **problemas com módulo**, prefira sempre `long long` com `% MOD` em vez de BigInt.

> **Use BigInt quando** o problema exigir exatamente o resultado em forma decimal com precisão completa (ex: "imprima o fatorial de 100" ou "N! mod 10⁹+7... sem módulo"). Para operações simples de soma/subtração, é eficiente; para multiplicação de números muito grandes, considere outras estratégias.

---

## Como Escolher

```
Preciso gerenciar grupos/componentes?
└── Union-Find

Preciso de queries em intervalos numéricos?
├── Array estático, query O(1) → Sparse Table
├── Update pontual + query de soma → Fenwick Tree
└── Range update + range query qualquer → Segment Tree

Preciso buscar strings ou prefixos?
└── Trie

Resultado é um número > 18 dígitos sem módulo?
└── BigInt
```