# Primitivas de Geometria (Basics and Primitives)

As primitivas são os blocos de construção de toda geometria computacional. Quase todo algoritmo complexo — fecho convexo, interseção de polígonos, Sweep Line — é construído sobre essas operações básicas. Implemente-as bem, teste-as e não as reimplemente do zero em cada problema.

---

## Visão Geral

| Arquivo | Complexidade | Conteúdo |
|---|---|---|
| `point_vector.cpp` | O(1) | Struct Point, operadores, distâncias, rotação, projeção |
| `cross_product.cpp` | O(1) | Orientação, interseção de segmentos, área de triângulo |
| Topologias de grid | O(1) por vizinho | Square, Hexagonal, Triangular, Circular, Polygon |

---

## Ponto e Vetor 2D

Um `Point` representa tanto um ponto no plano quanto um vetor 2D. Use `long double` para máxima precisão. Para geometria com coordenadas inteiras e sem distâncias, `long long` é exato.

```cpp
typedef long double ld;
const ld EPS = 1e-9;

struct Point {
    ld x, y;
    Point(ld x = 0, ld y = 0) : x(x), y(y) {}

    Point operator+(const Point& p) const { return {x+p.x, y+p.y}; }
    Point operator-(const Point& p) const { return {x-p.x, y-p.y}; }
    Point operator*(ld t)           const { return {x*t, y*t}; }
    Point operator/(ld t)           const { return {x/t, y/t}; }

    // Produto Escalar: A·B = |A||B|cos(θ)
    // > 0: ângulo agudo, = 0: perpendiculares, < 0: obtuso
    ld dot(const Point& p)   const { return x*p.x + y*p.y; }

    // Produto Vetorial: A×B = |A||B|sin(θ)
    // > 0: B à esquerda de A (CCW), < 0: B à direita (CW), = 0: paralelos
    ld cross(const Point& p) const { return x*p.y - y*p.x; }

    ld norm()   const { return x*x + y*y; }       // módulo²
    ld length() const { return sqrtl(norm()); }   // módulo

    ld dist(const Point& p)  const { return (*this - p).length(); }
    ld dist2(const Point& p) const { return (*this - p).norm(); }  // sem sqrt

    ld angle() const { return atan2l(y, x); } // ângulo em [-π, π]

    // Rotação em torno da origem
    Point rotate(ld ang) const {
        return {x*cosl(ang) - y*sinl(ang), x*sinl(ang) + y*cosl(ang)};
    }

    bool operator<(const Point& p) const {
        if (abs(x-p.x) > EPS) return x < p.x;
        return y < p.y - EPS;
    }
    bool operator==(const Point& p) const {
        return abs(x-p.x) < EPS && abs(y-p.y) < EPS;
    }
};

Point midpoint(Point A, Point B) { return (A + B) / 2; }
```

### Distância Ponto–Reta e Ponto–Segmento

```cpp
// Distância de P à reta definida por A e B
ld distToLine(Point P, Point A, Point B) {
    return abs((B-A).cross(P-A)) / (B-A).length();
}

// Distância de P ao segmento AB (projeção limitada a [0, 1])
ld distToSegment(Point P, Point A, Point B) {
    if ((B-A).norm() < EPS) return P.dist(A); // segmento degenerado
    ld t = (P-A).dot(B-A) / (B-A).norm();
    t = max((ld)0, min((ld)1, t));
    return P.dist(A + (B-A)*t);
}
```

> Use `dist2()` no lugar de `dist()` para **comparar** distâncias — evita `sqrtl` e reduz imprecisão.

---

## Produto Vetorial e Orientação

`cross(A, B, C) = (B−A) × (C−A)` é a primitiva mais importante da geometria computacional. O seu sinal determina a orientação relativa de três pontos.

```cpp
ld cross(Point A, Point B, Point C) { return (B-A).cross(C-A); }

// +1 = C à esquerda de AB (Anti-Horário / CCW)
// -1 = C à direita de AB (Horário / CW)
//  0 = A, B, C colineares
int orient(Point A, Point B, Point C) {
    ld v = cross(A, B, C);
    if (v >  EPS) return  1;
    if (v < -EPS) return -1;
    return 0;
}
```

---

## Interseção de Segmentos

```cpp
// Verifica se P está sobre o segmento AB (assumindo colinearidade)
bool onSegment(Point A, Point B, Point P) {
    return min(A.x,B.x)-EPS <= P.x && P.x <= max(A.x,B.x)+EPS &&
           min(A.y,B.y)-EPS <= P.y && P.y <= max(A.y,B.y)+EPS;
}

// Retorna true se os segmentos AB e CD se intersectam (inclui pontas)
bool segmentsIntersect(Point A, Point B, Point C, Point D) {
    int d1 = orient(C,D,A), d2 = orient(C,D,B);
    int d3 = orient(A,B,C), d4 = orient(A,B,D);

    if (d1*d2 < 0 && d3*d4 < 0) return true; // caso geral

    // casos degenerados: ponta sobre o outro segmento
    if (d1==0 && onSegment(C,D,A)) return true;
    if (d2==0 && onSegment(C,D,B)) return true;
    if (d3==0 && onSegment(A,B,C)) return true;
    if (d4==0 && onSegment(A,B,D)) return true;
    return false;
}

// Ponto de interseção entre as RETAS AB e CD (assumindo não paralelas)
Point lineIntersection(Point A, Point B, Point C, Point D) {
    ld a1 = cross(A,B,C), a2 = cross(A,B,D);
    return C + (D-C)*(a1/(a1-a2));
}
```

---

## Área de Triângulo

```cpp
// Área sinalizada: > 0 se CCW, < 0 se CW
ld triangleAreaSigned(Point A, Point B, Point C) { return cross(A,B,C) / 2.0L; }
ld triangleArea(Point A, Point B, Point C)       { return abs(triangleAreaSigned(A,B,C)); }
```

---

## Topologias de Grid

A maioria dos problemas usa a grade quadrada clássica, mas alguns problemas modelam geometrias alternativas. A chave é descobrir a fórmula dos vizinhos para tratar o grid como um **grafo implícito** e rodar DFS/BFS normalmente.

### Square (Quadrada) — 4 ou 8 direções

```cpp
int dr4[] = {-1, 1, 0, 0};
int dc4[] = {0, 0, -1, 1};

// 8 direções (inclui diagonais)
int dr8[] = {-1, 1, 0, 0, -1, -1, 1, 1};
int dc8[] = {0, 0, -1, 1, -1, 1, -1, 1};

bool isValid(int r, int c, int R, int C) {
    return r >= 0 && r < R && c >= 0 && c < C;
}
```

### Hexagonal — Sistema Axial (q, r)

```cpp
// 6 direções no sistema de coordenadas axiais
int dq[] = { 1,  1,  0, -1, -1,  0};
int dr[] = { 0, -1, -1,  0,  1,  1};
```

### Triangular — Paridade (r + c) % 2

```cpp
// Vizinhos horizontais sempre existem
int dc_h[] = {-1, 1};

// Vizinho vertical depende da orientação do triângulo
int nr_vert = ((r + c) % 2 == 0) ? r + 1 : r - 1; // base abaixo ou acima
```

### Circular — (raio, setor)

```cpp
// Lateral: wrap-around com módulo
int esq = (setor - 1 + MAX_SETORES) % MAX_SETORES;
int dir = (setor + 1) % MAX_SETORES;

// Centro (raio == 0) toca todos os setores do anel 1
// Dentro (raio == 1) toca o centro (0, 0)
```

### Polygon (Irregular) — Grafo Explícito

```cpp
vector<int> adj[MAX]; // lista de adjacência
void adicionar_fronteira(int A, int B) { adj[A].push_back(B); adj[B].push_back(A); }
```

### Tabela Comparativa

| Topologia | Vizinhos | Transição | Uso típico |
|---|---|---|---|
| Square | 4 ou 8 | `(r+dr[i], c+dc[i])` | Mapas, labirintos, jogos 2D |
| Hexagonal | 6 | Coordenadas axiais `(q+dq[i], r+dr[i])` | Estratégia, expansão |
| Triangular | 3 | Paridade `(r+c)%2` | Pavimentações específicas |
| Circular | 3–4 | `(raio±1, setor)` com módulo | Radares, alvos, anéis |
| Polygon | Variável | Lista de adjacência | Mapas geopolíticos |

> Use a topologia Square quando o problema fornecer uma matriz regular. Para as demais, identifique a fórmula matemática dos vizinhos antes de implementar.