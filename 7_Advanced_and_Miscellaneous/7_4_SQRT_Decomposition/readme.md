# Decomposição por Raiz Quadrada (SQRT Decomposition)

A decomposição por raiz quadrada divide um problema em blocos de tamanho ≈ √N, obtendo complexidade O(√N) por operação. É a solução padrão quando não existe estrutura suficiente para aplicar Segment Tree ou Fenwick Tree, e para queries offline de intervalo com o algoritmo de Mo.

---

## Visão Geral

| Técnica | Complexidade | Uso típico |
|---|---|---|
| Decomposição de array | O(√N) por query/update | Range sum/min com updates em bloco |
| Mo's Algorithm | O((N + Q) √N) | Queries offline de intervalo sem updates |
| Mo's com updates | O(N^(5/3)) | Queries offline com updates pontuais |
| SQRT em árvores (Mo on trees) | O(N √N) | Queries em caminhos de árvore |

---

## Decomposição de Array

Divide o array em blocos de tamanho B ≈ √N. Para cada bloco, mantém um valor agregado (soma, mínimo, etc.) pré-computado. Queries e updates tocam no máximo 2 blocos parciais + blocos inteiros intermediários.

```cpp
const int MAXN = 1e5 + 5;
int arr[MAXN], block[350]; // block[i] = agregado do bloco i
int B; // tamanho do bloco = (int)sqrt(n)

void build(int n) {
    B = max(1, (int)sqrt(n));
    fill(block, block + (n/B) + 1, 0);
    for (int i = 0; i < n; i++)
        block[i/B] += arr[i];
}

// Query de soma em [l, r]
long long query(int l, int r) {
    long long res = 0;
    int bl = l/B, br = r/B;
    if (bl == br) {
        for (int i = l; i <= r; i++) res += arr[i];
    } else {
        for (int i = l; i < (bl+1)*B; i++) res += arr[i]; // bloco esquerdo parcial
        for (int b = bl+1; b < br; b++) res += block[b];  // blocos inteiros
        for (int i = br*B; i <= r; i++) res += arr[i];    // bloco direito parcial
    }
    return res;
}

// Update pontual
void update(int pos, int val) {
    block[pos/B] += val - arr[pos];
    arr[pos] = val;
}
```

> Para problemas com range updates e range queries, adicione um array `lazy[bloco]` para atualizações pendentes nos blocos inteiros.

---

## Mo's Algorithm

O algoritmo de Mo processa queries offline de intervalo `[l, r]` em O((N + Q) √N) ordenando as queries de forma que os ponteiros `l` e `r` se movam minimamente no total.

**Ordenação de Mo:** ordena por `(l / B, r)` — se estiver no mesmo bloco, ordena por `r`; caso contrário, ordena por bloco de `l`. Para reduzir constante, alternar a direção de `r` a cada bloco (Mo zig-zag).

```cpp
struct Query { int l, r, idx; };

int B_mo; // tamanho do bloco para Mo

bool mo_cmp(const Query& a, const Query& b) {
    int ba = a.l / B_mo, bb = b.l / B_mo;
    if (ba != bb) return ba < bb;
    return (ba & 1) ? (a.r > b.r) : (a.r < b.r); // zig-zag
}

// Estado global da janela atual
int cur_l, cur_r;
long long cur_ans; // resposta atual (ex: contagem de distintos)
int freq[MAXN];

// Adiciona arr[pos] à janela
void add(int pos) {
    // Exemplo: contar distintos
    if (freq[arr[pos]]++ == 0) cur_ans++;
}

// Remove arr[pos] da janela
void remove(int pos) {
    if (--freq[arr[pos]] == 0) cur_ans--;
}

vector<long long> mo_solve(vector<int>& arr, vector<Query>& queries) {
    int n = arr.size(), q = queries.size();
    B_mo = max(1, (int)sqrt(n));
    sort(queries.begin(), queries.end(), mo_cmp);

    vector<long long> ans(q);
    cur_l = 0; cur_r = -1; cur_ans = 0;
    fill(freq, freq + n + 1, 0);

    for (auto& query : queries) {
        while (cur_r < query.r) add(++cur_r);
        while (cur_l > query.l) add(--cur_l);
        while (cur_r > query.r) remove(cur_r--);
        while (cur_l < query.l) remove(cur_l++);
        ans[query.idx] = cur_ans;
    }
    return ans;
}
```

### Escolhendo o Tamanho do Bloco B

O valor ótimo teórico é `B = √N`. Na prática, tente `B = max(1, (int)sqrt(n))` e ajuste se necessário. Para Mo's com updates, o ótimo é `B = N^(2/3)`.

---

## Mo's Algorithm com Updates (Mo 3D)

Extende Mo's para suportar updates pontuais offline. O estado é `(l, r, t)` onde `t` é o tempo (índice do update). As queries são ordenadas por blocos 3D de tamanho `N^(2/3)`.

```cpp
struct Query3D { int l, r, t, idx; };

int B3; // ≈ N^(2/3)

bool mo3d_cmp(const Query3D& a, const Query3D& b) {
    int ba = a.l/B3, bb = b.l/B3;
    int ra = a.r/B3, rb = b.r/B3;
    if (ba != bb) return ba < bb;
    if (ra != rb) return (ba & 1) ? ra > rb : ra < rb;
    return (ra & 1) ? a.t > b.t : a.t < b.t;
}
```

---

## Mo on Trees (Queries em Caminhos de Árvore)

Para responder queries do tipo "f(caminho u→v)" offline em O(N √N):

1. Achate a árvore usando a **ordem de entrada/saída da DFS** (euler tour).
2. Para um caminho u→v, use o intervalo `[in[u], in[v]]` se `lca(u,v) = u` ou v, caso contrário `[out[u]+1, in[v]]` com o LCA adicionado separadamente.
3. Aplique Mo's sobre esses intervalos na ordem linearizada.

---

## Quando Usar SQRT Decomposition

```
A operação add/remove é O(1) ou O(log N)?
└── Mo's Algorithm (offline)

Preciso de range query + update em array?
├── Operação simples (soma, XOR) → Fenwick ou Seg Tree (mais rápido)
└── Operação sem estrutura → SQRT decomposition

Updates são offline?
└── Mo's com updates (Mo 3D): O(N^(5/3))

Queries em caminhos de árvore?
└── Mo on Trees + Euler Tour

Operação não tem inverso (ex: moda, distintos com remoção)?
└── Mo's (add e remove separados não precisam ser inversos um do outro)
```

> **Regra prática:** se N, Q ≤ 10⁵ e cada operação de add/remove é O(1), Mo's resolve em ~10⁷ operações — confortável para 1s. Se add/remove for O(log N), serão ~10⁷ · 17 ≈ 1.7 · 10⁸ — pode precisar de otimização de constante.