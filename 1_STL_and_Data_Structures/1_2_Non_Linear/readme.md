# Estruturas Não-Lineares

Estruturas não-lineares organizam dados de forma hierárquica ou associativa. Em C++, as versões ordenadas (`set`, `map`, `multiset`) são implementadas como **Árvores Rubro-Negras** (O(log N) garantido), enquanto as versões hash (`unordered_*`) oferecem O(1) amortizado sem garantia de ordem.

---

## Visão Geral

| Estrutura | Ordenação | Inserção/Busca | Uso típico |
|---|---|---|---|
| `set` | Crescente | O(log N) | Elementos únicos e ordenados |
| `map` | Por chave | O(log N) | Dicionário ou frequência ordenada |
| `multiset` | Crescente | O(log N) | Elementos repetidos e ordenados |
| `priority_queue` | Parcial (Heap) | O(log N) | Dijkstra, maior/menor elemento |
| `unordered_set` | Nenhuma | O(1) médio | Busca ultra-rápida de existência |
| `unordered_map` | Nenhuma | O(1) médio | Contagem de frequência rápida |

---

## Set

Armazena elementos **únicos** em ordem crescente. Útil para manter um conjunto dinâmico sempre ordenado.

```cpp
set<int> s;
s.insert(10); s.insert(30); s.insert(20);
s.insert(20);                     // Ignorado (elementos únicos)

if (s.count(10)) { /* existe */ }
auto it = s.find(20);             // O(log N)

int menor = *s.begin();
int maior = *s.rbegin();

// Consultas de intervalo em O(log N)
auto lb = s.lower_bound(15);      // primeiro elemento >= 15
auto ub = s.upper_bound(15);      // primeiro elemento > 15

s.erase(30);                      // remove por valor
s.erase(it);                      // remove por iterador
```

> **Use `set` quando** precisar de elementos distintos com consultas de intervalo (`lower_bound`, `upper_bound`) ou manter dados sempre ordenados.

---

## Map

Associa uma **chave** a um **valor** (key-value), mantendo as chaves em ordem crescente.

```cpp
map<string, int> m;
m["banana"] = 5;
m["laranja"] = 10;

// ATENÇÃO: m[chave] cria a chave com valor padrão (0) se não existir
if (m.count("uva")) { /* existe */ }
auto it = m.find("banana");       // O(log N), não cria a chave

// Iteração ordenada (C++17 Structured Bindings)
for (auto const& [chave, valor] : m) {
    cout << chave << ": " << valor << "\n";
}

m.erase("banana");
m.lower_bound("m");               // primeiro par com chave >= "m"
```

> **Use `map` quando** precisar de um dicionário que mantenha as chaves em ordem. Prefira `.find()` a `m[chave]` para buscas, pois `[]` insere a chave com valor padrão se ela não existir.

---

## Multiset

Semelhante ao `set`, mas permite **elementos duplicados**. Ideal para listas ordenadas dinâmicas com repetições.

```cpp
multiset<int> ms;
ms.insert(10); ms.insert(10);     // Dois 10s aceitos

int qtd = ms.count(10);           // Quantas vezes 10 aparece — O(qtd + log N)

// CUIDADO NA REMOÇÃO:
ms.erase(10);                     // Remove TODAS as instâncias de 10
ms.erase(ms.find(10));            // Remove apenas UMA instância

int menor = *ms.begin();
int maior = *ms.rbegin();
```

> **Use `multiset` quando** precisar de uma coleção ordenada que permita duplicatas e onde remover apenas a menor ou maior ocorrência seja comum (use `erase(find(val))` para remover apenas uma).

---

## Priority Queue

Implementação de **Heap** que dá acesso O(1) ao elemento de maior (ou menor) prioridade, com inserção e remoção em O(log N).

```cpp
// Max-Heap (padrão): maior elemento no topo
priority_queue<int> pq;

// Min-Heap: menor elemento no topo
priority_queue<int, vector<int>, greater<int>> min_pq;

pq.push(10); pq.push(30); pq.push(20);
int topo = pq.top(); // 30
pq.pop();

// Par {distância, nó} — padrão para Dijkstra
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> djk;
djk.push({0, 1});
```

> **Use `priority_queue` quando** precisar processar elementos por ordem de prioridade — Dijkstra, Prim, algoritmos gulosos. Para Min-Heap, use `greater<T>` como comparador.

---

## Unordered Set / Map

Tabelas hash que fornecem operações em **tempo constante médio** sem manter nenhuma ordem.

```cpp
unordered_set<int> us;
us.insert(10); us.insert(20);
us.insert(10);                    // Duplicata ignorada
if (us.count(10)) { /* existe */ }

unordered_map<string, int> um;
um["viçosa"] = 31;
um["brasília"] = 61;
if (um.find("viçosa") != um.end()) { /* existe */ }
um.erase("brasília");

// Iteração (ordem imprevisível)
for (auto const& [chave, valor] : um) { /* ... */ }
```

> **Use as versões `unordered` quando** a ordem não importar e você precisar da máxima performance de busca/inserção.
>
> **Atenção no Codeforces:** `unordered_map` e `unordered_set` podem sofrer ataques de colisão com inputs adversariais, forçando o pior caso O(N). Nesses casos, use um **custom hash** ou prefira `std::map` O(log N).

---

## Qual usar: `map` ou `unordered_map`?

| Situação | Escolha |
|---|---|
| Precisa de ordem ou `lower_bound` | `map` / `set` |
| Só precisa de busca/inserção rápida | `unordered_map` / `unordered_set` |
| Chave é `string` e N é grande | `unordered_map` (mas cuidado com colisões) |
| Juiz adversarial (Codeforces) | `map` ou custom hash |
| Frequência de caracteres (alfabeto pequeno) | `array<int, 26>` — mais rápido que qualquer map |