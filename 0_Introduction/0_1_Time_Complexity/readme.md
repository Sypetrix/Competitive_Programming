# Complexidade de Tempo (Time Complexity)

Antes de implementar qualquer solução, um competidor experiente estima mentalmente se ela caberá no tempo limite. Essa heurística — analisar a complexidade antes de codar — é o que separa soluções que recebem **AC** das que recebem **TLE**. Entender notação Big-O não é apenas teoria: é uma ferramenta de tomada de decisão usada a todo momento durante uma competição.

---

## Notação Big-O

Big-O descreve o comportamento do tempo de execução de um algoritmo em função do tamanho da entrada $N$, **ignorando constantes e termos de menor ordem**.

```
T(N) = 3N² + 7N + 12  →  O(N²)
T(N) = N log N + 500  →  O(N log N)
T(N) = 42             →  O(1)
```

Ao analisar um algoritmo, pergunte: *se eu dobrar o tamanho de N, quanto o tempo cresce?*

---

## Complexidades Comuns (do mais rápido ao mais lento)

| Complexidade | Nome | Exemplo |
|---|---|---|
| $O(1)$ | Constante | Acesso a array, operação aritmética |
| $O(\log N)$ | Logarítmica | Busca binária, operações em heap |
| $O(\sqrt{N})$ | Raiz quadrada | Fatoração prima, crivo segmentado |
| $O(N)$ | Linear | Percorrer um array, BFS/DFS |
| $O(N \log N)$ | Linearítmica | Merge sort, Dijkstra, construção de Suffix Array |
| $O(N \sqrt{N})$ | — | Algoritmo de Mo, SQRT decomposition |
| $O(N^2)$ | Quadrática | Bubble sort, Floyd-Warshall para N pequeno |
| $O(N^3)$ | Cúbica | Floyd-Warshall, multiplicação de matrizes |
| $O(2^N)$ | Exponencial | Busca completa em subconjuntos |
| $O(N!)$ | Fatorial | Geração de permutações |

---

## Regra Prática: Operações por Segundo

A maioria dos juízes online consegue executar aproximadamente **$10^8$ operações simples por segundo**. Com um limite de tempo de 1 segundo, use esta tabela para estimar qual complexidade é viável:

| $N$ (tamanho da entrada) | Complexidade máxima viável |
|---|---|
| $N \leq 10$ | $O(N!)$ ou $O(N^N)$ |
| $N \leq 20$ | $O(2^N \cdot N)$ |
| $N \leq 500$ | $O(N^3)$ |
| $N \leq 5.000$ | $O(N^2)$ |
| $N \leq 10^5$ | $O(N \log N)$ |
| $N \leq 10^6$ | $O(N)$ ou $O(N \log N)$ com constante pequena |
| $N \leq 10^8$ | $O(N)$ — cuidado com constantes |
| $N \leq 10^{18}$ | $O(\log N)$ ou $O(\sqrt{N})$ |

> **Atenção ao fator constante:** dois algoritmos $O(N \log N)$ podem diferir por 5x na prática dependendo de acessos à memória, uso de cache e operações por iteração. Se a estimativa estiver no limite, considere otimizações de constante ou um algoritmo mais rápido.

---

## Como Analisar a Complexidade do Seu Código

**Regra dos loops:** a complexidade de loops aninhados se multiplica.

```cpp
for (int i = 0; i < N; i++)           // O(N)
    for (int j = 0; j < N; j++)       // O(N)
        // operação O(1)              // → O(N²) total
```

**Regra da soma:** blocos sequenciais se somam — o maior domina.

```cpp
sort(v.begin(), v.end());             // O(N log N)
for (int i = 0; i < N; i++) { ... }  // O(N)
// Total: O(N log N) + O(N) = O(N log N)
```

**Recursão:** use o Teorema Mestre ou pense intuitivamente.

```
T(N) = 2T(N/2) + O(N)  →  O(N log N)   [Merge Sort]
T(N) = T(N-1) + O(1)   →  O(N)         [Recursão linear]
T(N) = 2T(N-1) + O(1)  →  O(2^N)       [Recursão exponencial]
```

---

## Complexidade de Espaço

Além do tempo, o juiz também limita a **memória** (tipicamente 256 MB). Algumas referências úteis:

| Tipo | Tamanho |
|---|---|
| `int` (4 bytes) | $10^8$ inteiros ≈ 400 MB — **cuidado** |
| `long long` (8 bytes) | $10^7$ long longs ≈ 80 MB — ok |
| `bool` / `char` (1 byte) | $2.5 \times 10^8$ ≈ 250 MB — no limite |
| `double` (8 bytes) | igual ao `long long` |

```cpp
int arr[100005];           // ~400 KB — seguro
int mat[1000][1000];       // ~4 MB — seguro
int mat[10000][10000];     // ~400 MB — MLE!
```

> **Dica de declaração:** declare arrays grandes **globalmente** (fora da `main`). Arrays locais grandes podem causar Stack Overflow, pois a pilha de execução tem limite de ~1-8 MB dependendo do sistema.

---

## Exemplos de Raciocínio

**Problema:** dado um array de $N \leq 10^5$ inteiros, encontre o par de elementos com menor diferença absoluta.

- Força bruta: testar todos os pares → $O(N^2) = 10^{10}$ → TLE.
- Ordenar e verificar adjacentes → $O(N \log N) \approx 1.7 \times 10^6$ → AC.

**Problema:** dado um grafo de $N \leq 500$ vértices, calcule a menor distância entre todos os pares.

- Floyd-Warshall → $O(N^3) = 1.25 \times 10^8$ → no limite, mas geralmente AC com 1-2s.
- $N = 1000$? → $10^9$ → TLE. Use Dijkstra $N$ vezes: $O(N^2 \log N) \approx 10^7$ → AC.