# Strings

Problemas de strings são uma das categorias mais frequentes em maratonas. A maioria das técnicas transforma operações ingênuas de O(N²) em algo linear, explorando a estrutura interna das strings — prefixos, sufixos e padrões repetidos.

---

## Organização da Pasta

| Subpasta | Principais Tópicos |
|---|---|
| **Hashing** | Polynomial Rolling Hash, hashing duplo, comparação em O(1) |
| **Pattern Matching** | KMP, Z-function, Aho-Corasick |
| **Suffix Structures** | Suffix Array + LCP, Suffix Automaton, Trie |
| **Palindromes and Anagrams** | Manacher, Eertree (Palindromic Tree) |
| **Strings DP** | Edit Distance, LCS, LPS, alinhamento |

---

## Tabela Geral de Complexidades

| Algoritmo | Pré-proc | Query / Operação | Uso típico |
|---|---|---|---|
| Rolling Hash | O(N) | O(1) substring | Comparação de substrings, busca de padrões |
| KMP | O(M) | O(N) busca | Busca de padrão P em texto T |
| Z-function | O(N) | O(1) por posição | Ocorrências de prefixo, border da string |
| Aho-Corasick | O(Σ|Pi|) | O(N · |Σ|) | Busca de múltiplos padrões simultaneamente |
| Suffix Array | O(N log N) | O(M log N) busca | Substrings distintas, LCP, buscas repetidas |
| Suffix Automaton | O(N) | O(M) busca | Contagem de substrings distintas, ocorrências |
| Manacher | O(N) | O(1) por posição | Maior palíndromo centrado em cada posição |
| Eertree | O(N) | — | Todos os palíndromos distintos, contagens |
| Edit Distance | O(N·M) | — | Distância de edição entre duas strings |
| LCS | O(N·M) | — | Subsequência comum mais longa |

---

## Como Escolher a Técnica

```
Buscar padrão(ões) em um texto?
├── Padrão único → KMP ou Z-function
└── Múltiplos padrões → Aho-Corasick

Comparar substrings rapidamente (O(1))?
└── Rolling Hash (duplo para evitar colisões)

Problema envolve todos os sufixos / substrings?
├── Busca, LCP, ranking → Suffix Array
└── Contagem de substrings distintas / ocorrências → Suffix Automaton

Inserir e buscar prefixos de strings?
└── Trie

Problema envolve palíndromos?
├── Maior palíndromo centrado em i → Manacher
└── Contar / listar todos os palíndromos distintos → Eertree

Problema de similaridade / alinhamento entre strings?
└── Strings DP (Edit Distance, LCS)
```

---

## Dicas Gerais

**Indexação:** a maioria dos algoritmos de strings usa indexação 0-based. Cuidado ao combinar com estruturas 1-based (Segment Tree, Fenwick sobre Suffix Array).

**`s.length()` é `size_t` (unsigned):** evite `s.length() - 1` com string vazia — o resultado underflowa para um número gigante. Prefira `(int)s.size() - 1`.

**Módulo no Hash:** use dois módulos primos grandes para resistir a anti-hash tests em juízes adversariais.