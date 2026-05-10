# Palíndromos e Anagramas (Palindromes and Anagrams)

---

## Visão Geral

| Algoritmo | Complexidade | Uso típico |
|---|---|---|
| Manacher | O(N) | Raio do maior palíndromo centrado em cada posição |
| Eertree (Palindromic Tree) | O(N) | Todos os palíndromos distintos, contagem de ocorrências |
| Hash + busca binária | O(N log N) | Verificar se substring é palíndromo em O(log N) |
| Frequência de caracteres | O(N) | Verificar anagramas, contagem de permutações |

---

## Manacher

O algoritmo de Manacher encontra, em O(N), o maior palíndromo centrado em cada posição da string. Evita a abordagem ingênua de O(N²) usando a simetria dos palíndromos já computados.

**Transformação:** para tratar palíndromos de comprimento par e ímpar uniformemente, intercala a string com um separador (ex: `#`):

```
"abba"  →  "#a#b#b#a#"
"aba"   →  "#a#b#a#"
```

### Implementação — O(N)

```cpp
// Retorna p[i] = raio do maior palindromo centrado em i
// na string transformada (com separadores '#')
vector<int> manacher(const string& s) {
    // Transforma: s = "abc" -> t = "#a#b#c#"
    string t = "#";
    for (char c : s) { t += c; t += '#'; }
    int n = t.size();
    vector<int> p(n, 0);

    for (int i = 0, l = 0, r = -1; i < n; i++) {
        // Usa simetria do palíndromo atual para inicializar p[i]
        int k = (i > r) ? 1 : min(p[l + r - i], r - i + 1);
        while (i - k >= 0 && i + k < n && t[i-k] == t[i+k]) k++;
        p[i] = k - 1;
        if (i + k - 1 > r) { l = i - k + 1; r = i + k - 1; }
    }
    return p;
}

// Comprimento do maior palíndromo da string original centrado em i
// p[2*i+1] = raio para centros em letras (palíndromos ímpares)
// p[2*i+2] = raio para centros entre letras (palíndromos pares)
```

### Usos do Manacher

**Maior palíndromo na string:**
```cpp
int longest_palindrome(const string& s) {
    auto p = manacher(s);
    return *max_element(p.begin(), p.end());
}
```

**Todos os palíndromos de comprimento ≥ K:**
```cpp
// p[i] é o raio; comprimento do palíndromo original é p[i]
// (para posições ímpares na string transformada)
```

**Verificar se s[l..r] é palíndromo em O(1):**
depois de computar `p`, o centro do palíndromo que abrange `[l, r]` na string transformada é `l + r + 1`; basta checar se `p[l+r+1] >= r - l + 1`.

---

## Eertree (Palindromic Tree)

O Eertree (também chamado de Palindromic Tree ou PalTree) é uma estrutura que representa **todos os palíndromos distintos** de uma string como nós de uma árvore, construída em O(N).

**Estrutura:** dois nós raiz especiais com comprimento `-1` (raiz imaginária) e `0` (string vazia). Cada nó armazena o comprimento do palíndromo que representa. As arestas `link[c]` conectam um nó ao maior palíndromo sufixo do palíndromo atual após adicionar o caractere `c` em ambas as pontas.

```cpp
struct EerTree {
    struct Node {
        int len, link;           // comprimento e suffix link
        map<char,int> children;
        int cnt;                 // número de vezes que aparece como sufixo
        // Adicione outros campos conforme o problema
    };

    vector<Node> t;
    int last; // nó do maior palíndromo sufixo atual

    EerTree() {
        t.push_back({-1, 0, {}, 0}); // raiz imaginária (id=0)
        t.push_back({0,  0, {}, 0}); // string vazia (id=1)
        last = 1;
    }

    // Encontra o maior palíndromo sufixo de s[0..i] que pode ser estendido
    int get_link(const string& s, int v, int i) {
        while (i - t[v].len - 1 < 0 || s[i - t[v].len - 1] != s[i])
            v = t[v].link;
        return v;
    }

    // Adiciona o caractere s[i]
    void extend(const string& s, int i) {
        char c = s[i];
        int cur = get_link(s, last, i);

        if (!t[cur].children.count(c)) {
            // Cria novo nó
            int now = t.size();
            int new_len = t[cur].len + 2;
            int new_link;
            if (new_len == 1) {
                new_link = 1; // palíndromo de tamanho 1 tem suffix link para vazio
            } else {
                new_link = t[get_link(s, t[cur].link, i)].children[c];
            }
            t.push_back({new_len, new_link, {}, 0});
            t[cur].children[c] = now;
        }
        last = t[cur].children[c];
        t[last].cnt++;
    }

    void build(const string& s) {
        for (int i = 0; i < (int)s.size(); i++) extend(s, i);
    }

    // Propaga cnt pelos suffix links (conta todas as ocorrências)
    void propagate() {
        // Processa em ordem decrescente de len (topological order)
        // Nós mais novos têm ids maiores e lens maiores
        for (int i = (int)t.size() - 1; i >= 2; i--)
            t[t[i].link].cnt += t[i].cnt;
    }

    // Número de palíndromos distintos (excluindo as duas raízes)
    int count_distinct() { return (int)t.size() - 2; }
};
```

### Usos do Eertree

**Contar palíndromos distintos:** `eertree.count_distinct()`

**Número de ocorrências de cada palíndromo:** após `propagate()`, `t[v].cnt` = número de vezes que o palíndromo `v` ocorre como substring.

**Maior palíndromo sufixo em cada posição:** `last` após cada `extend(i)` é o nó do maior palíndromo sufixo de `s[0..i]`.

**Fatoração palindrômica mínima (DP com Eertree):**
```cpp
// dp[i] = mínimo de palíndromos para cobrir s[0..i-1]
vector<int> dp(n+1, INF);
dp[0] = 0;
// Para cada posição i, percorra os suffix links do Eertree
// e use dp[i - t[v].len] + 1 para cada palíndromo sufixo v
```

---

## Anagramas

Verificar se duas strings são anagramas e contar permutações são problemas clássicos que se resolvem com frequência de caracteres.

### Verificar Anagrama — O(N)

```cpp
bool is_anagram(const string& a, const string& b) {
    if (a.size() != b.size()) return false;
    int freq[26] = {};
    for (char c : a) freq[c-'a']++;
    for (char c : b) freq[c-'a']--;
    for (int x : freq) if (x != 0) return false;
    return true;
}
```

### Busca de Anagrama em Texto (Sliding Window) — O(N)

Encontra todas as posições onde uma permutação de P aparece em T:

```cpp
vector<int> find_anagrams(const string& T, const string& P) {
    int n = T.size(), m = P.size();
    if (n < m) return {};

    int freq[26] = {}, window[26] = {};
    for (char c : P) freq[c-'a']++;
    for (int i = 0; i < m; i++) window[T[i]-'a']++;

    vector<int> res;
    auto match = [&]() {
        for (int i = 0; i < 26; i++) if (freq[i] != window[i]) return false;
        return true;
    };
    if (match()) res.push_back(0);

    for (int i = m; i < n; i++) {
        window[T[i]-'a']++;
        window[T[i-m]-'a']--;
        if (match()) res.push_back(i - m + 1);
    }
    return res;
}
```

### Número de Palíndromos de uma String (Combinatória)

Uma string é rearranjo de um palíndromo se e somente se **no máximo um caractere tem frequência ímpar**.

```cpp
bool can_form_palindrome(const string& s) {
    int freq[26] = {};
    for (char c : s) freq[c-'a']++;
    int odd = 0;
    for (int x : freq) odd += (x % 2);
    return odd <= 1;
}
```