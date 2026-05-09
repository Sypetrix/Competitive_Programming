#include <bits/stdc++.h>

using namespace std;

typedef long double ld;
const ld EPS = 1e-9;

struct Point {
    ld x, y;
    Point(ld x = 0, ld y = 0) : x(x), y(y) {}
    Point operator-(const Point& p) const { return {x - p.x, y - p.y}; }
    Point operator+(const Point& p) const { return {x + p.x, y + p.y}; }
    ld cross(const Point& p)        const { return x * p.y - y * p.x;  }
    ld dot(const Point& p)          const { return x * p.x + y * p.y;  }
    ld norm()                       const { return x * x + y * y;       }
    ld length()                     const { return sqrtl(norm());        }
};

// ==============================================================================
// ÁREA DO POLÍGONO — Fórmula do Cadarço (Shoelace / Gauss)
// Complexidade: O(N)
// ==============================================================================
// Funciona para qualquer polígono simples (sem auto-interseções),
// convexo ou côncavo. O vértices devem estar em ordem (CCW ou CW).
//
// Área Sinalizada:
//   > 0  →  vértices em ordem Anti-Horária (CCW)
//   < 0  →  vértices em ordem Horária (CW)
// ==============================================================================

ld polygonAreaSigned(const vector<Point>& poly) {
    ld area = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        area += poly[i].cross(poly[(i + 1) % n]);
    }
    return area / 2.0L;
}

ld polygonArea(const vector<Point>& poly) {
    return abs(polygonAreaSigned(poly));
}

// Perímetro do polígono (soma das arestas)
ld polygonPerimeter(const vector<Point>& poly) {
    ld perim = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        perim += (poly[(i + 1) % n] - poly[i]).length();
    }
    return perim;
}

// ==============================================================================
// PONTO DENTRO DO POLÍGONO — Ray Casting
// Complexidade: O(N)
// ==============================================================================
// Lança um raio horizontal para a direita a partir de P e conta quantas
// arestas do polígono ele cruza. Se o número for ímpar, P está dentro.
//
// Retorna:
//   1  →  P está DENTRO do polígono
//   0  →  P está SOBRE uma aresta (na borda)
//  -1  →  P está FORA do polígono
// ==============================================================================
int pointInPolygon(Point P, const vector<Point>& poly) {
    int n = poly.size();
    int inside = 0;

    for (int i = 0, j = n - 1; i < n; j = i++) {
        Point A = poly[i], B = poly[j];

        // Verifica se P está exatamente sobre a aresta AB
        // (colinear e dentro do bounding box)
        ld c = (B - A).cross(P - A);
        if (abs(c) < EPS) {
            // Colinear: checa se P está no segmento [A, B]
            if (min(A.x, B.x) - EPS <= P.x && P.x <= max(A.x, B.x) + EPS &&
                min(A.y, B.y) - EPS <= P.y && P.y <= max(A.y, B.y) + EPS) {
                return 0; // Na borda
            }
        }

        // Raio horizontal para a direita de P cruza a aresta AB?
        if ((A.y <= P.y + EPS) != (B.y <= P.y + EPS)) {
            // Ponto de cruzamento da aresta com a linha horizontal y = P.y
            ld xIntersect = A.x + (P.y - A.y) * (B.x - A.x) / (B.y - A.y);
            if (P.x < xIntersect + EPS) {
                inside ^= 1;
            }
        }
    }

    return inside ? 1 : -1;
}

// ==============================================================================
// CENTRÓIDE DO POLÍGONO
// Complexidade: O(N)
// ==============================================================================
// O centróide geométrico (centro de massa) de um polígono simples.
Point polygonCentroid(const vector<Point>& poly) {
    ld area = 0, cx = 0, cy = 0;
    int n = poly.size();

    for (int i = 0; i < n; i++) {
        Point A = poly[i], B = poly[(i + 1) % n];
        ld c = A.cross(B);
        area += c;
        cx += (A.x + B.x) * c;
        cy += (A.y + B.y) * c;
    }

    area /= 2.0L;
    return {cx / (6.0L * area), cy / (6.0L * area)};
}

// Verifica se o polígono é convexo (todos os giros no mesmo sentido)
bool isConvex(const vector<Point>& poly) {
    int n = poly.size();
    int sign = 0;
    for (int i = 0; i < n; i++) {
        ld c = (poly[(i + 1) % n] - poly[i]).cross(poly[(i + 2) % n] - poly[(i + 1) % n]);
        if (abs(c) > EPS) {
            int s = (c > 0) ? 1 : -1;
            if (sign == 0) sign = s;
            else if (sign != s) return false;
        }
    }
    return true;
}