# Casamento de Padrões (Pattern Matching)

Algoritmos de casamento de padrões encontram ocorrências de uma (ou mais) strings dentro de um texto em tempo linear, evitando a abordagem ingênua de O(N·M).

---

## Visão Geral

| Algoritmo | Pré-processamento | Busca | Uso típico |
|---|---|---|---|
| KMP | O(M) | O(N) | Busca de padrão único; contagem de borders |
| Z-function | O(N + M) | O(N + M) | Ocorrências de prefixo; período mínimo da string |
| Aho-Corasick | O(Σ\|Pi\| * \|Σ\|) | O(N) | Busca simultânea de múltiplos padrões |

---

## KMP — Knuth-Morris-Pratt

O KMP evita recomparar caracteres que já foram confirmados. O segredo está no array `pi` (failure function): `pi[i]` é o tamanho do maior **borda** de `P[0..i]` — o maior prefixo que também é sufixo.

### Pré-computação do array pi — O(M)

```cpp
vector<int> compute_pi(const string& P) {
    int m = P.size();
    vector<int> pi(m, 0);
    for (int i = 1, j = 0; i < m; i++) {
        while (j > 0 && P[i] != P[j]) j = pi[j-1];
        if (P[i] == P[j]) j++;
        pi[i] = j;
    }
    return pi;
}
```

### Busca de P em T — O(N + M)

```cpp
// Retorna lista de posições (0-indexed) onde P começa em T
vector<int> kmp(const string& T, const string& P) {
    vector<int> pi = compute_pi(P);
    vector<int> occ;
    int n = T.size(), m = P.size();
    for (int i = 0, j = 0; i < n; i++) {
        while (j > 0 && T[i] != P[j]) j = pi[j-1];
        if (T[i] == P[j]) j++;
        if (j == m) {
            occ.push_back(i - m + 1);
            j = pi[j-1]; // prepara para próxima ocorrência
        }
    }
    return occ;
}
```

### Usos Além da Busca Simples

**Período mínimo:** `m - pi[m-1]` é o comprimento do período mínimo de P. Se `m % (m - pi[m-1]) == 0`, a string tem período exato.

**Contar borders:** todos os tamanhos de bordas de P são `pi[m-1]`, `pi[pi[m-1]-1]`, ... (cadeia de pi).

**Verificar se P é rotação de Q:** concatene `Q + Q` e busque P com KMP.

---

## Z-function

`z[i]` é o comprimento do maior prefixo de `S` que começa na posição `i`. Por definição, `z[0]` não é definido (pode ser considerado `|S|` ou 0).

### Computação — O(N)

```cpp
vector<int> z_function(const string& s) {
    int n = s.size();
    vector<int> z(n, 0);
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) { l = i; r = i + z[i]; }
    }
    return z;
}
```

### Busca de Padrão P em Texto T

```cpp
vector<int> z_search(const string& T, const string& P) {
    string s = P + "$" + T; // '$' não aparece em P nem T
    auto z = z_function(s);
    int m = P.size();
    vector<int> occ;
    for (int i = m + 1; i < (int)s.size(); i++)
        if (z[i] == m) occ.push_back(i - m - 1);
    return occ;
}
```

> O separador `'$'` evita que o prefixo "vaze" do padrão para o texto.

### Comparação KMP vs Z-function

| | KMP | Z-function |
|---|---|---|
| Estrutura gerada | `pi[i]` = tamanho da maior borda de P[0..i] | `z[i]` = comprimento do match com prefixo |
| Mais natural para | Contagem de ocorrências, borders | Verificar prefixos, período |
| Implementação | Ligeiramente mais complexa | Mais direta |

Na prática, ambos resolvem os mesmos problemas — use o que você memoriza melhor.

---

## Aho-Corasick

O Aho-Corasick é a extensão do KMP para múltiplos padrões. Constrói um **autômato** (trie + failure links) que processa o texto em O(N) após construção O(Σ|Pi|), reportando todas as ocorrências de todos os padrões simultaneamente.

### Estrutura

```cpp
struct AhoCorasick {
    vector<array<int,26>> go;   // transições
    vector<int> fail;           // failure links (como pi do KMP)
    vector<int> out;            // índice do padrão que termina aqui (-1 se nenhum)
    vector<int> dict;           // dict link: próximo estado com saída

    AhoCorasick() {
        go.push_back({});
        go[0].fill(0); // estado 0 = raiz
        fail.push_back(0);
        out.push_back(-1);
        dict.push_back(0);
    }

    // Insere padrão P com ID pid
    void insert(const string& P, int pid) {
        int cur = 0;
        for (char c : P) {
            int ch = c - 'a';
            if (!go[cur][ch]) {
                go[cur][ch] = go.size();
                go.push_back({});
                go.back().fill(0);
                fail.push_back(0);
                out.push_back(-1);
                dict.push_back(0);
            }
            cur = go[cur][ch];
        }
        out[cur] = pid;
    }

    // Constrói failure links e dict links via BFS — O(Σ|Pi| · |Σ|)
    void build() {
        queue<int> q;
        for (int c = 0; c < 26; c++) {
            if (go[0][c]) {
                fail[go[0][c]] = 0;
                q.push(go[0][c]);
            }
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            dict[u] = (out[fail[u]] != -1) ? fail[u] : dict[fail[u]];
            for (int c = 0; c < 26; c++) {
                int v = go[u][c];
                if (v) {
                    fail[v] = go[fail[u]][c];
                    q.push(v);
                } else {
                    go[u][c] = go[fail[u]][c]; // atalho: comprime transições
                }
            }
        }
    }

    // Processa texto T: chama callback(pos, pid) para cada ocorrência
    void search(const string& T, auto callback) {
        int cur = 0;
        for (int i = 0; i < (int)T.size(); i++) {
            cur = go[cur][T[i] - 'a'];
            // Reporta todos os padrões que terminam em i
            for (int v = (out[cur] != -1 ? cur : dict[cur]); v; v = dict[v])
                callback(i, out[v]);
        }
    }
};
```

### Uso

```cpp
AhoCorasick ac;
vector<string> patterns = {"he", "she", "his", "hers"};
for (int i = 0; i < (int)patterns.size(); i++)
    ac.insert(patterns[i], i);
ac.build();

string text = "ushers";
ac.search(text, [&](int pos, int pid) {
    // padrão pid termina na posição pos do texto
    cout << "Padrão '" << patterns[pid] << "' termina em " << pos << "\n";
});
```

### Quando usar Aho-Corasick

- Múltiplos padrões (dicionário) para buscar no mesmo texto
- Contar quantas palavras de um dicionário aparecem em cada substring
- Problemas de DP onde o estado inclui o nó atual no autômato (ex: contar strings de comprimento N que contêm todas as palavras do dicionário)