# Emparelhamento em Grafos (Graph Matching)

Emparelhamento (matching) é o problema de parear vértices de um grafo de forma que nenhum vértice apareça em mais de um par. Esta pasta cobre os dois algoritmos fundamentais para **grafos bipartidos**: verificação de bipartição e o algoritmo de Kuhn para emparelhamento máximo.

---

## Visão Geral

| Algoritmo / Arquivo | Complexidade | Utilização Típica |
|---|---|---|
| `bipartite` | O(V + E) | Verificar se o grafo pode ser dividido em dois conjuntos independentes. |
| `kuhn` | O(V * E) | Encontrar o emparelhamento máximo em grafos bipartidos. |

---

## Grafo Bipartido

Um grafo é **bipartido** se seus vértices podem ser divididos em dois conjuntos $A$ e $B$ tal que toda aresta conecta um vértice de $A$ a um vértice de $B$ — ou seja, não existem arestas dentro do mesmo conjunto.

A verificação é feita com uma **BFS de 2-coloração**: tenta-se pintar o grafo com duas cores alternando a cada aresta. Se em algum momento dois vizinhos tiverem a mesma cor, o grafo contém um ciclo de tamanho ímpar e não é bipartido.

```cpp
int color[MAXN]; // Inicializar com -1

bool is_bipartite(int start) {
    queue<int> q;
    q.push(start);
    color[start] = 0;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (color[v] == -1) {
                color[v] = 1 - color[u]; // Cor oposta ao pai
                q.push(v);
            } else if (color[v] == color[u]) {
                return false; // Ciclo ímpar: não é bipartido
            }
        }
    }
    return true;
}
```

> Um grafo é bipartido **se e somente se** não contém ciclos de comprimento ímpar. Esta verificação é pré-requisito para aplicar o algoritmo de Kuhn.

---

## Emparelhamento Máximo Bipartido — Algoritmo de Kuhn

O algoritmo de Kuhn encontra o **emparelhamento máximo** em um grafo bipartido: o maior conjunto de arestas tal que nenhum vértice é coberto mais de uma vez.

O algoritmo funciona pela busca de **caminhos aumentantes**: partindo de cada vértice do conjunto $A$, tenta-se encontrar, via DFS, um caminho que termine em um vértice livre do conjunto $B$. Sempre que um caminho é encontrado, o emparelhamento cresce em 1.

```cpp
vector<int> adj[MAXN]; // Arestas de A para B
int match[MAXN];       // match[v] = nó de A emparelhado com v (em B)
bool vis[MAXN];

bool dfs(int u) {
    for (int v : adj[u]) {
        if (vis[v]) continue;
        vis[v] = true;
        // Se v está livre OU o parceiro atual de v consegue outro par
        if (match[v] < 0 || dfs(match[v])) {
            match[v] = u;
            return true;
        }
    }
    return false;
}

int max_bipartite_matching(int n_left) {
    memset(match, -1, sizeof(match));
    int result = 0;
    for (int i = 0; i < n_left; i++) {
        memset(vis, false, sizeof(vis));
        if (dfs(i)) result++;
    }
    return result;
}
```

**Como montar o grafo e chamar:**

```cpp
// Leitura das arestas (vértices de A indexados de 0, B de 0 também)
adj[u].push_back(v);

// Resultado
int pareamentos = max_bipartite_matching(n_left);
```

### Teorema de König

Em grafos bipartidos, o tamanho do **emparelhamento máximo** é igual ao tamanho da **cobertura mínima por vértices** (menor conjunto de vértices que toca todas as arestas). Isso permite resolver dois problemas com uma única chamada ao Kuhn:

$$\text{Emparelhamento Máximo} = \text{Cobertura Mínima por Vértices}$$

$$\text{Conjunto Independente Máximo} = V - \text{Cobertura Mínima por Vértices}$$

> **Use o Kuhn quando** o problema envolver alocação ótima entre dois grupos: tarefas e trabalhadores, alunos e salas, times e torneios. Para grafos não-bipartidos ou emparelhamentos com peso, são necessários algoritmos mais avançados (Blossom, Húngaro).