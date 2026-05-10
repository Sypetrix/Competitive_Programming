# Programação Dinâmica em Strings (Strings DP)

Problemas de similaridade e alinhamento entre strings são resolvidos classicamente com DP 2D sobre as posições de duas strings. A maioria dos problemas desta seção compartilha a mesma estrutura de tabela `dp[i][j]` representando algum valor ótimo para os prefixos `a[0..i-1]` e `b[0..j-1]`.

---

## Visão Geral

| Problema | Complexidade | Estado DP |
|---|---|---|
| Edit Distance (Levenshtein) | O(N·M) | dp[i][j] = edições para transformar a[0..i] em b[0..j] |
| LCS (Longest Common Subsequence) | O(N·M) | dp[i][j] = tamanho da LCS de a[0..i] e b[0..j] |
| LPS (Longest Palindromic Subsequence) | O(N²) | dp[i][j] = LPS de s[i..j] |
| Minimum Insertions para Palíndromo | O(N²) | dp[i][j] = inserções mínimas para s[i..j] virar palíndromo |
| String Matching com DP | O(N·M) | dp[i][j] = match de padrão[0..i] com texto[0..j] |

---

## Edit Distance (Distância de Levenshtein) — O(N·M)

Número mínimo de **inserções, remoções e substituições** para transformar a string `a` em `b`.

```cpp
int edit_distance(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n+1, vector<int>(m+1));

    // Casos base: transformar prefixo em string vazia
    for (int i = 0; i <= n; i++) dp[i][0] = i; // remover i chars de a
    for (int j = 0; j <= m; j++) dp[0][j] = j; // inserir j chars

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i-1] == b[j-1]) {
                dp[i][j] = dp[i-1][j-1]; // caracteres iguais: sem custo
            } else {
                dp[i][j] = 1 + min({
                    dp[i-1][j],   // remoção de a[i-1]
                    dp[i][j-1],   // inserção de b[j-1]
                    dp[i-1][j-1]  // substituição
                });
            }
        }
    }
    return dp[n][m];
}
```

### Variantes

**Apenas inserção e remoção (sem substituição):** remova a opção `dp[i-1][j-1] + 1`.

**Operações com custo diferente:** atribua pesos diferentes a cada operação.

**Distância de Hamming** (apenas substituições, strings de mesmo tamanho):
```cpp
int hamming(const string& a, const string& b) {
    int diff = 0;
    for (int i = 0; i < (int)a.size(); i++) diff += (a[i] != b[i]);
    return diff;
}
```

### Otimização de Memória — O(M)

```cpp
int edit_distance_mem(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<int> dp(m+1), prev(m+1);
    iota(dp.begin(), dp.end(), 0);

    for (int i = 1; i <= n; i++) {
        prev = dp;
        dp[0] = i;
        for (int j = 1; j <= m; j++) {
            if (a[i-1] == b[j-1]) dp[j] = prev[j-1];
            else dp[j] = 1 + min({prev[j], dp[j-1], prev[j-1]});
        }
    }
    return dp[m];
}
```

---

## LCS — Longest Common Subsequence — O(N·M)

Comprimento da maior subsequência comum entre `a` e `b` (os caracteres não precisam ser contíguos).

```cpp
int lcs(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n+1, vector<int>(m+1, 0));

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (a[i-1] == b[j-1]) dp[i][j] = dp[i-1][j-1] + 1;
            else dp[i][j] = max(dp[i-1][j], dp[i][j-1]);

    return dp[n][m];
}
```

### Reconstruindo a LCS

```cpp
string lcs_reconstruct(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (a[i-1] == b[j-1]) dp[i][j] = dp[i-1][j-1] + 1;
            else dp[i][j] = max(dp[i-1][j], dp[i][j-1]);

    string res;
    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (a[i-1] == b[j-1]) { res += a[i-1]; i--; j--; }
        else if (dp[i-1][j] > dp[i][j-1]) i--;
        else j--;
    }
    reverse(res.begin(), res.end());
    return res;
}
```

### LCS e Edit Distance

As duas medidas estão relacionadas:
```
edit_distance(a, b) = n + m - 2 * lcs(a, b)
```
(quando o custo de substituição é 2, ou seja, apenas inserções e remoções).

### Otimização de Memória — O(M)

```cpp
int lcs_mem(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<int> dp(m+1, 0), prev(m+1, 0);
    for (int i = 1; i <= n; i++) {
        prev = dp;
        for (int j = 1; j <= m; j++)
            if (a[i-1] == b[j-1]) dp[j] = prev[j-1] + 1;
            else dp[j] = max(dp[j], dp[j-1]);
    }
    return dp[m];
}
```

---

## LPS — Longest Palindromic Subsequence — O(N²)

Comprimento da maior subsequência de `s` que é palíndromo.

```cpp
int lps(const string& s) {
    int n = s.size();
    vector<vector<int>> dp(n, vector<int>(n, 0));

    // Caso base: substrings de tamanho 1
    for (int i = 0; i < n; i++) dp[i][i] = 1;

    // Preenche por comprimento crescente do intervalo
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            if (s[i] == s[j]) dp[i][j] = dp[i+1][j-1] + 2;
            else dp[i][j] = max(dp[i+1][j], dp[i][j-1]);
        }
    }
    return dp[0][n-1];
}
```

> Equivalente a `lcs(s, reverse(s))`.

---

## Mínimo de Inserções para Palíndromo — O(N²)

Número mínimo de inserções para transformar `s` em um palíndromo.

```cpp
int min_insertions_palindrome(const string& s) {
    int n = s.size();
    // Relação: inserções mínimas = n - LPS(s)
    return n - lps(s);
}
```

**Intuição:** os `lps(s)` caracteres do LPS já estão no lugar certo; os `n - lps(s)` restantes precisam de um caractere espelhado inserido.

---

## Shortest Common Supersequence (SCS)

Menor string que contém `a` e `b` como subsequências.

```cpp
int scs_length(const string& a, const string& b) {
    return a.size() + b.size() - lcs(a, b);
}
```

---

## Referência Rápida

| Problema | Fórmula / Técnica |
|---|---|
| Edit Distance | DP 2D: insert/delete/substitute |
| LCS | DP 2D: match ou pular |
| LPS | `lcs(s, reverse(s))` ou DP em intervalos |
| Min inserções → palíndromo | `n - lps(s)` |
| Distância de Hamming | contagem direta O(N) |
| SCS | `n + m - lcs(a, b)` |
| Edit dist. com apenas insert/delete | `n + m - 2 * lcs(a, b)` |