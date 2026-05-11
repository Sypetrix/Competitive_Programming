# Avançado e Miscelânea (Advanced and Miscellaneous)

Esta pasta reúne tópicos que não se encaixam perfeitamente em uma categoria anterior, mas que aparecem com frequência suficiente em maratonas para merecer estudo dedicado. O conteúdo cresce conforme novos temas são explorados.

---

## Índice de Subpastas

| Subpasta | Tópicos |
|---|---|
| `7_1_Game_Theory` | Nim, Sprague-Grundy, jogos combinatórios |
| `7_2_3D_Geometry` | Produto vetorial 3D, poliedros, interseção em 3D |
| `7_3_FFT_and_NTT` | Fast Fourier Transform, Number Theoretic Transform, multiplicação de polinômios |
| `7_4_SQRT_Decomposition` | Mo's Algorithm, Square Root Decomposition em arrays e árvores |

> **Novos tópicos a serem adicionados com o tempo**

---

## Tópicos Avançados — Visão Rápida

### Game Theory
Teoria dos jogos combinatórios. O conceito central é o **valor de Grundy (nimber)**: todo jogo imparcial equivale a uma pilha de Nim de certo tamanho. XOR dos Grundy values de jogos independentes determina o vencedor.

**Quando usar:** o enunciado descreve dois jogadores alternando movimentos em um estado, ambos jogando otimamente, e pede "quem vence?".

### 3D Geometry
Extensão das primitivas 2D para o espaço tridimensional. O produto vetorial em 3D produz um **vetor** (não um escalar), e é usado para calcular normais a planos, volumes de tetraedros e interseção de raios com superfícies.

**Quando usar:** problema explicitamente em três dimensões — esferas, caixas, tetraedros, projeções.

### FFT / NTT
Multiplicação de dois polinômios de grau N em **O(N log N)** em vez de O(N²). A NTT (Number Theoretic Transform) é a versão modular da FFT, evitando erros de ponto flutuante.

**Quando usar:** multiplicação de polinômios; convolução de arrays; contagem de pares com soma/diferença específica; multiplicação de números grandes dígito a dígito.

### SQRT Decomposition
Divide o array em blocos de tamanho √N. Cada query ou update examina no máximo O(√N) blocos inteiros mais O(√N) elementos nas bordas — total O(√N) por operação.

**Mo's Algorithm** é a aplicação offline desta ideia a queries de intervalo: ordena as queries por `(bloco do l, r)` para minimizar o número de movimentos dos ponteiros.

**Quando usar:** range queries sem estrutura suficiente para Segment Tree (ex: contagem de distintos, moda); Mo's quando as queries são offline e a operação não tem inverso fácil.

---

## Outros Tópicos que Podem Entrar Aqui

Lista de referência para novos conteúdos a adicionar conforme necessário:

- **Treap / Splay Tree** — BST balanceada com merge/split, sequências implícitas
- **Link-Cut Tree** — Union-Find dinâmico em árvores com path queries
- **Wavelet Tree** — queries de ordem em subarrays em O(log N)
- **Persistent Segment Tree** — versões históricas da Seg Tree, k-ésimo elemento em subarray
- **Convex Hull Trick** — otimização de DP com transições lineares
- **Aliens Trick (Lambda Optimization)** — otimização de DP com restrição de contagem
- **Matroid Intersection** — combinação de estruturas matroides
- **Randomized Algorithms** — hashing aleatório, treaps, quickselect
- **Simulated Annealing / Hill Climbing** — otimização para problemas sem solução exata viável
- **Interactive Problems** — estratégias de busca binária e adaptação para juízes interativos
- **Constructive Algorithms** — técnicas de construção explícita de respostas