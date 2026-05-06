# Estruturas Não-Lineares

Estruturas não-lineares organizam os dados de forma hierárquica ou associativa, permitindo buscas, inserções e remoções muito mais eficientes do que arrays simples em diversos cenários. Em C++, a maioria é implementada como Árvores Binárias de Busca Balanceadas ou Tabelas Hash.

---

## Visão Geral

| Estrutura | Ordenação | Inserção/Busca | Utilização típica |
|---|---|---|---|
| `set` | Crescente | O(log N) | Elementos únicos e ordenados |
| `map` | Por Chave | O(log N) | Dicionário ou frequência ordenada |
| `multiset` | Crescente | O(log N) | Elementos repetidos e ordenados |
| `priority_queue` | Parcial (Heap) | O(log N) | Dijkstra, maior/menor elemento |
| `unordered_set` | Nenhuma | O(1) médio | Busca ultra rápida de existência |
| `unordered_map` | Nenhuma | O(1) médio | Contagem de frequência rápida |

---

## Set

Armazena elementos únicos numa árvore balanceada. Útil para manter um conjunto de valores sempre ordenados e sem duplicados.

```cpp
set<int> s;
s.insert(10);
s.insert(30);
s.insert(20);
s.insert(20); // Ignorado (o set não aceita duplicados)

if (s.count(10)) { /* o elemento existe */ }
auto it = s.find(20);

// Acesso ao menor e ao maior elemento
int menor = *s.begin();
int maior = *s.rbegin();

// Consultas de intervalo em O(log N)
auto it_lb = s.lower_bound(15); // Retorna iterador para o primeiro elemento >= 15
```

> **Use `set` quando** precisar de manter elementos distintos e realizar consultas de intervalo (como `lower_bound` e `upper_bound`) ou manter os dados sempre ordenados.

---

## Map

Associa uma chave a um valor (Key-Value), mantendo as chaves sempre ordenadas.

```cpp
map<string, int> m;
m["banana"] = 5;
m["laranja"] = 10;

// ATENÇÃO: o operador m[chave] cria a chave com valor padrão (ex: 0) se não existir
if (m.count("uva")) { /* existe */ }

// Iteração ordenada (usando Structured Bindings do C++17)
for (auto const& [chave, valor] : m) {
    cout << chave << ": " << valor << endl;
}
```

> **Use `map` quando** precisar de um dicionário (mapeamento) que mantenha as chaves em ordem alfabética ou numérica.

---

## Multiset

Semelhante ao `set`, mas permite elementos duplicados. É ideal para listas ordenadas dinâmicas onde existem repetições de valores.

```cpp
multiset<int> ms;
ms.insert(10);
ms.insert(10); // Aceite (duplicado)

// CUIDADO NA REMOÇÃO:
ms.erase(10);        // Remove TODAS as instâncias do valor 10
ms.erase(ms.find(10)); // Remove apenas UMA instância (via iterador)
```

> **Use `multiset` quando** precisar de manter vários valores iguais ordenados e remover apenas o menor ou o maior de forma eficiente.

---

## Priority Queue

Implementação de um Heap que dá acesso rápido ao elemento de maior (ou menor) prioridade.

```cpp
// Max-Heap (padrão: o maior elemento fica no topo)
priority_queue<int> pq; 

// Min-Heap (o menor elemento fica no topo)
priority_queue<int, vector<int>, greater<int>> min_pq;

pq.push(10); pq.push(30); pq.push(20);
int topo = pq.top(); // Retorna 30
pq.pop();            // Remove o topo
```

> **Use `priority_queue` quando** precisar processar elementos por ordem de importância, como nos algoritmos de Dijkstra ou Prim.

---

## Unordered Set / Map

Utilizam Tabelas Hash para fornecer operações em tempo constante médio. Não mantêm qualquer ordem nos elementos.

```cpp
unordered_set<int> us;
us.insert(10);
us.count(10); // O(1) médio

unordered_map<string, int> um;
um["vicosa"] = 31; // O(1) médio
```

> **Use as versões `unordered` quando** a ordem não for relevante e precisar da máxima performance de busca. **Nota:** Em maratonas no Codeforces, utilize sempre com um *Custom Hash* para evitar ataques de colisão que forçam o pior caso O(N).