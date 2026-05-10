# Algoritmos em Polígonos (Algorithms on Polygons)

Polígonos são a estrutura central da geometria computacional em CP. Esta seção cobre os algoritmos mais cobrados: área, perímetro, ponto dentro do polígono, fecho convexo e centróide.

---

## Visão Geral

| Algoritmo / Arquivo | Complexidade | Uso típico |
|---|---|---|
| `polygon.cpp` — Área (Shoelace) | O(N) | Área de qualquer polígono simples |
| `polygon.cpp` — Ponto em polígono | O(N) | Ray Casting: dentro / fora / borda |
| `polygon.cpp` — Centróide | O(N) | Centro de massa geométrico |
| `polygon.cpp` — Is Convex | O(N) | Verificar convexidade |
| `convex_hull.cpp` — Monotone Chain | O(N log N) | Menor polígono convexo contendo N pontos |
| `cross_product.cpp` — Interseção | O(1) | Segmentos se cruzam? Ponto de cruzamento? |

---

## Área do Polígono — Fórmula do Cadarço (Shoelace)

Funciona para qualquer polígono **simples** (sem auto-interseções), convexo ou côncavo. Os vértices podem estar em ordem CCW ou CW.

```
Área = (1/2) |Σᵢ (xᵢ · yᵢ₊₁ − xᵢ₊₁ · yᵢ)|
```

```cpp
ld polygonAreaSigned(const vector<Point>& poly) {
    ld area = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++)
        area += poly[i].cross(poly[(i+1) % n]);
    return area / 2.0L;
}

ld polygonArea(const vector<Point>& poly) {
    return abs(polygonAreaSigned(poly));
}
```

O **sinal** da área não-absoluta indica a orientação:
- `> 0` → vértices em ordem Anti-Horária (CCW)
- `< 0` → vértices em ordem Horária (CW)

---

## Perímetro

```cpp
ld polygonPerimeter(const vector<Point>& poly) {
    ld perim = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++)
        perim += (poly[(i+1)%n] - poly[i]).length();
    return perim;
}
```

---

## Ponto Dentro do Polígono — Ray Casting — O(N)

Lança um raio horizontal para a direita a partir do ponto P e conta quantas arestas ele cruza. Número ímpar → dentro; par → fora. Trata bordas explicitamente.

```cpp
// Retorna: 1 = dentro, 0 = na borda, -1 = fora
int pointInPolygon(Point P, const vector<Point>& poly) {
    int n = poly.size(), inside = 0;

    for (int i = 0, j = n-1; i < n; j = i++) {
        Point A = poly[i], B = poly[j];

        // P está sobre a aresta AB?
        if (abs((B-A).cross(P-A)) < EPS &&
            min(A.x,B.x)-EPS <= P.x && P.x <= max(A.x,B.x)+EPS &&
            min(A.y,B.y)-EPS <= P.y && P.y <= max(A.y,B.y)+EPS)
            return 0; // na borda

        // Raio horizontal cruza a aresta?
        if ((A.y <= P.y+EPS) != (B.y <= P.y+EPS)) {
            ld xCross = A.x + (P.y-A.y)*(B.x-A.x)/(B.y-A.y);
            if (P.x < xCross + EPS) inside ^= 1;
        }
    }
    return inside ? 1 : -1;
}
```

> Use com cuidado em bordas: o retorno `0` indica que o ponto está **exatamente** sobre uma aresta. Decida antes como tratar esse caso conforme o enunciado.

---

## Centróide do Polígono — O(N)

```cpp
Point polygonCentroid(const vector<Point>& poly) {
    ld area = 0, cx = 0, cy = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        Point A = poly[i], B = poly[(i+1)%n];
        ld c = A.cross(B);
        area += c;
        cx += (A.x + B.x) * c;
        cy += (A.y + B.y) * c;
    }
    area /= 2.0L;
    return {cx / (6.0L * area), cy / (6.0L * area)};
}
```

---

## Verificar Convexidade — O(N)

Um polígono é convexo se todos os giros consecutivos são no mesmo sentido.

```cpp
bool isConvex(const vector<Point>& poly) {
    int n = poly.size(), sign = 0;
    for (int i = 0; i < n; i++) {
        ld c = (poly[(i+1)%n]-poly[i]).cross(poly[(i+2)%n]-poly[(i+1)%n]);
        if (abs(c) > EPS) {
            int s = (c > 0) ? 1 : -1;
            if (sign == 0) sign = s;
            else if (sign != s) return false;
        }
    }
    return true;
}
```

---

## Fecho Convexo — Monotone Chain — O(N log N)

Constrói o menor polígono convexo que contém todos os N pontos. O algoritmo de Andrew varre os pontos da esquerda para a direita construindo as cadeias inferior e superior separadamente.

```cpp
vector<Point> convexHull(vector<Point> pts, bool incluirColineares = false) {
    int n = pts.size();
    if (n < 2) return pts;
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    n = pts.size();
    if (n < 2) return pts;

    vector<Point> hull;
    hull.reserve(2 * n);

    // Cadeia inferior
    for (int i = 0; i < n; i++) {
        while (hull.size() >= 2) {
            ld c = cross(hull[hull.size()-2], hull.back(), pts[i]);
            if (c < (incluirColineares ? -EPS : EPS)) hull.pop_back();
            else break;
        }
        hull.push_back(pts[i]);
    }

    // Cadeia superior
    int lower = hull.size();
    for (int i = n-2; i >= 0; i--) {
        while ((int)hull.size() > lower) {
            ld c = cross(hull[hull.size()-2], hull.back(), pts[i]);
            if (c < (incluirColineares ? -EPS : EPS)) hull.pop_back();
            else break;
        }
        hull.push_back(pts[i]);
    }

    hull.pop_back(); // primeiro ponto adicionado duas vezes
    return hull;     // vértices em ordem CCW
}
```

**Utilitários do fecho convexo:**

```cpp
ld hullArea(const vector<Point>& hull) {
    return polygonArea(hull);
}

ld hullPerimeter(const vector<Point>& hull) {
    return polygonPerimeter(hull);
}
```

> **Use o Fecho Convexo quando** o problema pedir o "menor cerca" em torno de pontos, calcular o diâmetro de um conjunto de pontos (par mais distante via Rotating Calipers) ou reduzir o espaço de busca aos vértices extremos.

---

## Teorema de Pick

Para polígonos com vértices em pontos inteiros:

```
Área = I + B/2 − 1
```

Onde I = pontos inteiros **dentro** do polígono, B = pontos inteiros **na borda**.

Logo: `I = Área − B/2 + 1` e `B = Σᵢ mdc(|Δx|, |Δy|)` (soma dos MDCs de cada aresta).

```cpp
long long pontosNaBorda(const vector<Point>& poly) {
    long long B = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        long long dx = abs((long long)(poly[(i+1)%n].x - poly[i].x));
        long long dy = abs((long long)(poly[(i+1)%n].y - poly[i].y));
        B += __gcd(dx, dy);
    }
    return B;
}
// Área usando Shoelace (inteiros) → sem divisão por 2 ainda
// Área_2x = valor inteiro exato da fórmula de Shoelace sem o /2
// I = (Área_2x - B + 2) / 2
```

---

## Referência Rápida

| Problema | Função | Complexidade |
|---|---|---|
| Área do polígono | `polygonArea` | O(N) |
| Ponto dentro? | `pointInPolygon` | O(N) |
| Centróide | `polygonCentroid` | O(N) |
| É convexo? | `isConvex` | O(N) |
| Fecho convexo | `convexHull` | O(N log N) |
| Pontos inteiros na borda | `pontosNaBorda` + MDC | O(N) |
| Pontos inteiros dentro | Teorema de Pick | O(N) |
| Orientação de 3 pontos | `orient(A,B,C)` | O(1) |
| Segmentos se cruzam? | `segmentsIntersect` | O(1) |
| Ponto de cruzamento | `lineIntersection` | O(1) |