# Geometria Computacional (Computational Geometry)

Problemas de geometria são conhecidos por serem os mais traiçoeiros das maratonas: a lógica parece simples no papel, mas a implementação esconde armadilhas de precisão numérica, casos degenerados e comparações inválidas com ponto flutuante. A chave para não perder pontos por bugs sutis é construir tudo sobre primitivas bem testadas, especialmente o **produto vetorial**.

---

## Visão Geral

| Arquivo | Complexidade | Utilização Típica |
|---|---|---|
| `point_vector` | O(1) por operação | Struct base: operadores, distâncias, rotação, projeção. |
| `cross_product` | O(1) | Orientação de pontos, interseção de segmentos, área de triângulo. |
| `convex_hull` | O(N log N) | Menor polígono convexo envolvendo N pontos (Monotone Chain). |
| `polygon` | O(N) | Área (Shoelace), ponto dentro do polígono (Ray Casting), centróide. |

---

## Pré-requisitos e Armadilhas

Antes de usar qualquer função deste módulo, internalize estas três regras:

**1. Nunca compare floats com `==`.**
Use sempre a constante de tolerância `EPS = 1e-9`:
```cpp
// ERRADO
if (a == b) { ... }

// CERTO
if (abs(a - b) < EPS) { ... }
```

**2. Use `long double` para máxima precisão.**
`double` tem ~15 dígitos significativos; `long double` tem ~18. Em geometria, a diferença importa.

**3. O produto vetorial é a base de tudo.**
`cross(A, B, C) = (B - A) × (C - A)` determina orientação, detecta colinearidade e calcula áreas. Quando em dúvida, reduza o problema ao produto vetorial.

---

## Point e Vetor (point_vector.cpp)

O `struct Point` representa tanto um ponto no plano quanto um vetor 2D. Toda a geometria é construída sobre ele.

```cpp
Point A(1, 2), B(4, 6);

ld d  = A.dist(B);        // Distância euclidiana
ld d2 = A.dist2(B);       // Distância ao quadrado (sem sqrt — preferir quando possível)
ld dp = (B - A).dot(C - A);   // Produto escalar
ld cp = (B - A).cross(C - A); // Produto vetorial

Point M = midpoint(A, B); // Ponto médio
ld h = distToSegment(P, A, B); // Distância de P ao segmento AB
```

> **Use `dist2()` no lugar de `dist()`** sempre que precisar apenas comparar distâncias (ex: "P é o mais próximo de Q?"). Isso evita a chamada custosa de `sqrtl` e elimina um nível de imprecisão.

---

## Produto Vetorial e Orientação (cross_product.cpp)

A função `orient(A, B, C)` é a primitiva mais importante desta seção. Ela retorna `+1` (CCW), `-1` (CW) ou `0` (colinear).

```cpp
// Sentido do giro A → B → C
int o = orient(A, B, C);
// o == +1: C está à esquerda de AB (anti-horário)
// o == -1: C está à direita de AB (horário)
// o ==  0: A, B, C são colineares

// Dois segmentos AB e CD se cruzam?
bool cruzam = segmentsIntersect(A, B, C, D);

// Ponto exato de interseção entre as retas AB e CD
Point P = lineIntersection(A, B, C, D);

// Área do triângulo ABC
ld area = triangleArea(A, B, C);
```

> **Atenção em `lineIntersection`:** a função pressupõe que as retas não são paralelas. Verifique antes com `orient` ou checando se o denominador é diferente de zero.

---

## Fecho Convexo — Monotone Chain (convex_hull.cpp)

O algoritmo de Andrew constrói o menor polígono convexo que envolve todos os N pontos, varrendo da esquerda para a direita para montar as cadeias inferior e superior separadamente.

```cpp
vector<Point> pts = { ... };
vector<Point> hull = convexHull(pts);
// hull está em ordem Anti-Horária (CCW)

ld area  = hullArea(hull);
ld perim = hullPerimeter(hull);
```

**Modo com colineares:**
```cpp
// Por padrão, pontos colineares nas arestas são removidos.
// Para mantê-los (ex: contar pontos na borda):
vector<Point> hull = convexHull(pts, true);
```

> **Use o Fecho Convexo quando** o problema pedir o "menor cerca" em torno de pontos, calcular o diâmetro de um conjunto de pontos (par mais distante) ou reduzir o espaço de busca a apenas os vértices extremos.

---

## Polígono (polygon.cpp)

```cpp
vector<Point> poly = { ... }; // Vértices em ordem (CCW ou CW)

ld area  = polygonArea(poly);      // Área via Shoelace — O(N)
ld perim = polygonPerimeter(poly); // Perímetro — O(N)
Point c  = polygonCentroid(poly);  // Centro de massa — O(N)
bool cv  = isConvex(poly);         // É convexo? — O(N)

// Ponto P dentro do polígono?
int res = pointInPolygon(P, poly);
//  1 → dentro
//  0 → na borda (sobre uma aresta)
// -1 → fora
```

**Fórmula do Cadarço (Shoelace):**

$$\text{Área} = \frac{1}{2} \left| \sum_{i=0}^{n-1} (x_i \cdot y_{i+1} - x_{i+1} \cdot y_i) \right|$$

O sinal da área não-absoluta (`polygonAreaSigned`) indica a orientação dos vértices: positivo = CCW, negativo = CW.

> **Use `pointInPolygon` com cuidado em bordas:** o retorno `0` indica que o ponto está exatamente sobre uma aresta. Decida antes como tratar esse caso de acordo com o enunciado do problema.

---

## Referência Rápida

| Problema | Função | Arquivo |
|---|---|---|
| Orientação de 3 pontos | `orient(A, B, C)` | `cross_product` |
| Segmentos se cruzam? | `segmentsIntersect(A, B, C, D)` | `cross_product` |
| Ponto de cruzamento de retas | `lineIntersection(A, B, C, D)` | `cross_product` |
| Área de triângulo | `triangleArea(A, B, C)` | `cross_product` |
| Distância ponto–segmento | `distToSegment(P, A, B)` | `point_vector` |
| Fecho convexo | `convexHull(pts)` | `convex_hull` |
| Área de polígono | `polygonArea(poly)` | `polygon` |
| Ponto dentro de polígono | `pointInPolygon(P, poly)` | `polygon` |