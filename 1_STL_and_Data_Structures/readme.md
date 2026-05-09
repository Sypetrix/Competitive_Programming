# STL e Estruturas de Dados

Uma **Estrutura de Dados** define como os dados são organizados e acessados. A escolha certa determina se uma solução passa no tempo limite ou recebe **TLE** — e em maratonas de programação, essa escolha precisa ser rápida e segura.

O C++ resolve isso através da **STL** (Standard Template Library): um conjunto de contêineres, algoritmos e utilitários altamente otimizados, prontos para uso imediato. O princípio é simples: use sempre as ferramentas prontas da linguagem antes de implementar qualquer coisa do zero.

---

## Por que dominar a STL?

- **Velocidade de implementação** — contêineres prontos e testados, sem bugs de implementação manual.
- **Garantia de complexidade** — `std::map` é sempre O(log N), `std::unordered_map` é O(1) amortizado, `std::priority_queue` é O(log N). Sem surpresas.
- **Legibilidade** — código mais curto e claro significa menos tempo depurando durante a prova.

> **Regra de ouro:** use `std::priority_queue` ao invés de implementar um heap, `std::map` ao invés de uma BST manual, e `std::sort` ao invés de qualquer algoritmo de ordenação próprio. A única exceção é quando a STL genuinamente não oferece o que você precisa — aí entram as estruturas customizadas.

---

## Organização do Capítulo

| Seção | O que contém |
|---|---|
| **Linear** | `vector`, `stack`, `queue`, `deque`, `string` e KMP |
| **Non-Linear** | `set`, `map`, `multiset`, `priority_queue`, `unordered_set`, `unordered_map` |
| **Monotonic** | Pilha e fila monotônicas — padrões clássicos de otimização |
| **Bit Manipulation** | Operações bitwise, truques com bits, bitmask |
| **Utils** | Union-Find, Fenwick Tree, Segment Tree, Sparse Table, Trie, BigInt |

---

## Tabela Geral de Complexidades

| Estrutura | Acesso | Busca | Inserção | Remoção | Ordenado? |
|---|---|---|---|---|---|
| `vector` | O(1) | O(N) | O(1)* / O(N) | O(N) | Não |
| `deque` | O(1) | O(N) | O(1) nas pontas | O(1) nas pontas | Não |
| `stack` / `queue` | O(1) topo/frente | — | O(1) | O(1) | Não |
| `set` / `map` | — | O(log N) | O(log N) | O(log N) | Sim |
| `multiset` | — | O(log N) | O(log N) | O(log N) | Sim |
| `priority_queue` | O(1) topo | — | O(log N) | O(log N) | Parcial |
| `unordered_set/map` | — | O(1)** | O(1)** | O(1)** | Não |
| Union-Find | — | O(α(N)) | O(α(N)) | — | Não |
| Fenwick Tree | — | O(log N) | O(log N) | O(log N) | Não |
| Segment Tree | — | O(log N) | O(log N) | O(log N) | Não |
| Sparse Table | — | O(1) | O(N log N) | — | Não |
| Trie | — | O(L) | O(L) | O(L) | Prefixo |

*\* amortizado | \*\* médio, pior caso O(N)*

---

## Como escolher a estrutura certa

```
Preciso de acesso por índice?
├── Sim → vector (ou deque se inserir nas duas pontas)
└── Não
    ├── Preciso sempre do maior/menor?
    │   └── priority_queue (heap)
    ├── Preciso buscar por chave com ordenação?
    │   └── map / set
    ├── Preciso de busca por chave rápida (sem ordenação)?
    │   └── unordered_map / unordered_set
    ├── Preciso gerenciar componentes conexas?
    │   └── Union-Find
    ├── Preciso de queries em intervalos?
    │   ├── Apenas soma/ponto update → Fenwick Tree
    │   ├── Range query + range update → Segment Tree
    │   └── Range minimum query estático → Sparse Table
    └── Preciso buscar prefixos de strings?
        └── Trie
```

---

## Cuidados Gerais

**Overflow:** ao multiplicar dois valores da ordem de 10⁹, o resultado pode ultrapassar `int`. Use `long long` preventivamente em produtos e somas acumuladas. Priorize utilizar *BigInt* nesses casos ou uma linguagem que não contenha limites (como Python).

**Iteradores inválidos:** após `insert` ou `erase` em `vector`, todos os iteradores para aquele vetor são invalidados. Em `map`/`set`, apenas o iterador do elemento removido é invalidado.

**`unordered` no Codeforces:** tabelas hash podem sofrer ataques de colisão com inputs maliciosos. Use um custom hash ou prefira `std::map` quando a performance for crítica e o juiz for adversarial.

**`.clear()` vs swap:** `stack` e `queue` não possuem `.clear()`. Para limpar em O(1), use o truque `ContainerType().swap(c)`.