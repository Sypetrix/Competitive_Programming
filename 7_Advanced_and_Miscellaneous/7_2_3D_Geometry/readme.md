# Geometria 3D (3D Geometry)

Geometria tridimensional em CP é relativamente rara, mas quando aparece exige implementação cuidadosa das primitivas 3D. A maioria dos problemas envolve distâncias, volumes, interseções e fecho convexo em três dimensões.

---

## Visão Geral

| Operação | Complexidade | Uso típico |
|---|---|---|
| Produto vetorial 3D | O(1) | Normal de plano, área de triângulo, orientação |
| Produto misto (escalar triplo) | O(1) | Volume de tetraedro, coplanaridade |
| Distância ponto–plano | O(1) | Separação, projeção |
| Distância segmento–segmento | O(1) | Colisão, menor distância |
| Fecho convexo 3D | O(N log N) | Envoltória mínima de pontos |

---

## Estrutura Base: Ponto/Vetor 3D

```cpp
typedef long double ld;
const ld EPS = 1e-9;

struct P3 {
    ld x, y, z;
    P3(ld x=0, ld y=0, ld z=0) : x(x), y(y), z(z) {}

    P3 operator+(const P3& p) const { return {x+p.x, y+p.y, z+p.z}; }
    P3 operator-(const P3& p) const { return {x-p.x, y-p.y, z-p.z}; }
    P3 operator*(ld t)        const { return {x*t, y*t, z*t}; }
    P3 operator/(ld t)        const { return {x/t, y/t, z/t}; }

    // Produto escalar (dot product): A·B = |A||B|cos(θ)
    ld dot(const P3& p) const { return x*p.x + y*p.y + z*p.z; }

    // Produto vetorial (cross product): A×B ⊥ A e ⊥ B
    // Módulo = |A||B|sin(θ) = área do paralelogramo
    P3 cross(const P3& p) const {
        return { y*p.z - z*p.y,
                 z*p.x - x*p.z,
                 x*p.y - y*p.x };
    }

    ld norm()   const { return x*x + y*y + z*z; }
    ld length() const { return sqrtl(norm()); }
    ld dist(const P3& p) const { return (*this - p).length(); }
    P3 unit()   const { return *this / length(); }

    bool operator==(const P3& p) const {
        return abs(x-p.x)<EPS && abs(y-p.y)<EPS && abs(z-p.z)<EPS;
    }
};
```

---

## Produto Misto (Scalar Triple Product)

```cpp
// Produto misto: A · (B × C)
// |resultado| = volume do paralelepípedo formado por A, B, C
// > 0: A está no lado positivo do plano B×C (CCW)
// = 0: A, B, C são coplanares
ld triple(P3 A, P3 B, P3 C) {
    return A.dot(B.cross(C));
}

// Volume do tetraedro (A, B, C, D)
ld tetrahedron_volume(P3 A, P3 B, P3 C, P3 D) {
    return abs(triple(B-A, C-A, D-A)) / 6.0L;
}
```

---

## Planos

```cpp
struct Plane {
    P3 n;   // vetor normal (não precisa ser unitário)
    ld  d;  // a·x + b·y + c·z = d

    // Plano pelos três pontos A, B, C
    Plane(P3 A, P3 B, P3 C) {
        n = (B-A).cross(C-A);
        d = n.dot(A);
    }

    // Distância sinalizada de P ao plano
    // > 0: P está do lado da normal; < 0: lado oposto
    ld signed_dist(P3 P) const {
        return (n.dot(P) - d) / n.length();
    }

    ld dist(P3 P) const { return abs(signed_dist(P)); }

    // Projeção de P no plano
    P3 project(P3 P) const {
        return P - n * ((n.dot(P) - d) / n.norm());
    }
};
```

---

## Distâncias

### Distância Ponto–Reta

```cpp
ld dist_point_line(P3 P, P3 A, P3 B) {
    P3 AB = B - A, AP = P - A;
    return AB.cross(AP).length() / AB.length();
}
```

### Distância Ponto–Segmento

```cpp
ld dist_point_segment(P3 P, P3 A, P3 B) {
    P3 AB = B-A, AP = P-A;
    ld t = AP.dot(AB) / AB.norm();
    t = max((ld)0, min((ld)1, t));
    return (AP - AB*t).length();
}
```

### Distância Segmento–Segmento

```cpp
ld dist_segment_segment(P3 A, P3 B, P3 C, P3 D) {
    P3 AB = B-A, CD = D-C, AC = C-A;
    ld a = AB.norm(), b = AB.dot(CD), c = CD.norm();
    ld d = AB.dot(AC), e = CD.dot(AC);
    ld denom = a*c - b*b;

    ld s, t;
    if (abs(denom) < EPS) { // Segmentos paralelos
        s = 0; t = e/c;
    } else {
        s = (b*e - c*d) / denom;
        t = (a*e - b*d) / denom;
    }
    s = max((ld)0, min((ld)1, s));
    t = max((ld)0, min((ld)1, t));

    // Recalcula com s/t fixos
    t = max((ld)0, min((ld)1, (AB*s + AC).dot(CD) / c));
    s = max((ld)0, min((ld)1, (CD*t - AC).dot(AB) / a));

    return (A + AB*s - C - CD*t).length();
}
```

---

## Interseção Reta–Plano

```cpp
// Retorna o ponto onde a reta (A + t*(B-A)) intersecta o plano
// Retorna A se a reta for paralela ao plano
P3 line_plane_intersect(P3 A, P3 B, Plane pl) {
    P3 AB = B - A;
    ld denom = pl.n.dot(AB);
    if (abs(denom) < EPS) return A; // paralelo
    ld t = (pl.d - pl.n.dot(A)) / denom;
    return A + AB * t;
}
```

---

## Esfera

```cpp
struct Sphere {
    P3 center;
    ld radius;

    bool contains(P3 P) const {
        return center.dist(P) <= radius + EPS;
    }

    // Área da superfície
    ld surface_area() const { return 4 * acosl(-1) * radius * radius; }

    // Volume
    ld volume() const { return (4.0L/3) * acosl(-1) * radius * radius * radius; }
};
```

---

## Área de Triângulo 3D

```cpp
ld triangle_area(P3 A, P3 B, P3 C) {
    return (B-A).cross(C-A).length() / 2.0L;
}
```

---

## Fecho Convexo 3D — O(N log N)

O fecho convexo 3D é significativamente mais complexo que o 2D. O algoritmo de **Incremental Convex Hull** (ou QuickHull 3D) adiciona pontos um a um, mantendo as faces visíveis do ponto atual.

```cpp
// Representação: lista de triângulos (faces do fecho)
struct Face { int a, b, c; };

// Verifica se o ponto P está acima da face (a, b, c) — no lado da normal
bool above(const vector<P3>& pts, Face f, P3 P) {
    return (pts[f.b]-pts[f.a]).cross(pts[f.c]-pts[f.a]).dot(P-pts[f.a]) > EPS;
}

vector<Face> convex_hull_3d(vector<P3> pts) {
    int n = pts.size();
    if (n < 4) return {}; // degenerado

    vector<Face> faces;

    // Inicializa com um tetraedro não-degenerado
    // (encontrar 4 pontos não coplanares)
    // ... (implementação completa omitida por extensão)
    // Recomendação: use biblioteca como CGAL ou implemente QuickHull 3D
    // para problemas que exijam fecho convexo 3D completo.

    return faces;
}
```

> O fecho convexo 3D raramente é cobrado em sua forma geral em maratonas. Problemas mais comuns envolvem apenas primitivas (distâncias, volumes, orientação).

---

## Formulário Rápido

| Quantidade | Fórmula |
|---|---|
| Área do triângulo ABC | `\|(B−A)×(C−A)\| / 2` |
| Volume do tetraedro ABCD | `\|( B−A) · ((C−A)×(D−A))\| / 6` |
| Normal do plano ABC | `(B−A) × (C−A)` |
| Dist. ponto P ao plano | `\|n·P − d\| / \|n\|` |
| Dist. ponto P à reta AB | `\|(B−A)×(P−A)\| / \|B−A\|` |
| Ângulo entre vetores A e B | `arccos(A·B / (\|A\|·\|B\|))` |
| Projeção de P sobre reta AB | `A + (B−A) · ((P−A)·(B−A)) / \|B−A\|²` |