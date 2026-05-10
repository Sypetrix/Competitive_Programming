# Geometria (Geometry)

Problemas de geometria são conhecidos por serem os mais traiçoeiros das maratonas: a lógica parece simples no papel, mas a implementação esconde armadilhas de precisão numérica, casos degenerados e comparações inválidas com ponto flutuante.

A chave para não perder pontos é construir tudo sobre **primitivas bem testadas** — especialmente o produto vetorial — e tratar precisão com disciplina desde o início.

---

## Organização da Pasta

| Subpasta | Conteúdo |
|---|---|
| **Basics and Primitives** | Ponto/Vetor 2D, distâncias, produto vetorial, orientação, interseção de segmentos |
| **Grids and Matrices** | Topologias de grid (quadrado, hex, triangular, circular, poligonal), DFS/BFS em grids |
| **Algorithms on Polygons** | Área (Shoelace), ponto dentro de polígono (Ray Casting), fecho convexo, centróide |
| **Sweep Line** | Eventos ordenados por x, interseção de segmentos, área de união de retângulos |

---

## Regras de Ouro em Geometria

**1. Nunca compare floats com `==`.**
```cpp
const double EPS = 1e-9;
if (abs(a - b) < EPS) { /* iguais */ }
```

**2. Use `long double` para máxima precisão.**
`double` tem ~15 dígitos significativos; `long double` tem ~18. Em geometria, a diferença importa.

**3. Quando possível, trabalhe com inteiros.**
Se coordenadas são inteiras e você só precisa de orientação/área, use `long long` e evite ponto flutuante completamente. O produto vetorial de pontos inteiros é exato.

**4. O produto vetorial é a base de tudo.**
Quase todas as primitivas geométricas — orientação, interseção, área, fecho convexo — são construídas sobre `cross(A, B, C) = (B−A) × (C−A)`.

**5. Trate casos degenerados explicitamente.**
Pontos colineares, segmentos que se tocam nas pontas, polígonos com área zero — esses são os casos que quebram implementações apressadas.

---

## Tabela de Complexidades

| Algoritmo | Complexidade | Arquivo |
|---|---|---|
| Operações em ponto/vetor | O(1) | `point_vector.cpp` |
| Orientação e interseção | O(1) | `cross_product.cpp` |
| DFS/BFS em grid | O(L × C) | `grid_DFS.cpp`, `grid_BFS.cpp` |
| Fecho Convexo (Monotone Chain) | O(N log N) | `convex_hull.cpp` |
| Área / Ponto em polígono | O(N) | `polygon.cpp` |
| Interseção de N segmentos (Sweep) | O(N log N) | Sweep Line |
| Área de união de retângulos | O(N log N) | Sweep Line |

---

## Estrutura Base: Ponto 2D

```cpp
typedef long double ld;
const ld EPS = 1e-9;

struct Point {
    ld x, y;
    Point(ld x = 0, ld y = 0) : x(x), y(y) {}

    Point operator+(const Point& p) const { return {x + p.x, y + p.y}; }
    Point operator-(const Point& p) const { return {x - p.x, y - p.y}; }
    Point operator*(ld t)           const { return {x * t,   y * t};   }

    ld dot(const Point& p)   const { return x*p.x + y*p.y; }
    ld cross(const Point& p) const { return x*p.y - y*p.x; }
    ld norm()   const { return x*x + y*y; }
    ld length() const { return sqrtl(norm()); }

    bool operator<(const Point& p) const {
        if (abs(x - p.x) > EPS) return x < p.x;
        return y < p.y - EPS;
    }
    bool operator==(const Point& p) const {
        return abs(x-p.x) < EPS && abs(y-p.y) < EPS;
    }
};

// Produto vetorial de (B-A) × (C-A)
ld cross(Point A, Point B, Point C) { return (B-A).cross(C-A); }

// Orientação: +1 CCW, -1 CW, 0 colinear
int orient(Point A, Point B, Point C) {
    ld v = cross(A, B, C);
    if (v >  EPS) return  1;
    if (v < -EPS) return -1;
    return 0;
}
```