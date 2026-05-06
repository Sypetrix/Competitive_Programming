#include <bits/stdc++.h>

using namespace std;

typedef long double ld;
const ld EPS = 1e-9;

struct Point {
    ld x, y;
    Point(ld x = 0, ld y = 0) : x(x), y(y) {}
    Point operator+(const Point& p) const { return {x + p.x, y + p.y}; }
    Point operator-(const Point& p) const { return {x - p.x, y - p.y}; }
    Point operator*(ld t)           const { return {x * t,   y * t};   }
    ld cross(const Point& p)        const { return x * p.y - y * p.x;  }
    ld dot(const Point& p)          const { return x * p.x + y * p.y;  }
    ld norm()                       const { return x * x + y * y;       }
};

// ==============================================================================
// PRODUTO VETORIAL E ORIENTAÇÃO
// ==============================================================================
// A função orient() é a primitiva central da geometria computacional.
// Quase todos os algoritmos (convex hull, interseção, point-in-polygon)
// são construídos sobre ela.
// ==============================================================================

// Retorna o sinal do produto vetorial (B - A) × (C - A):
//   > 0  →  C está à ESQUERDA de AB  (giro Anti-Horário / CCW)
//   < 0  →  C está à DIREITA de AB   (giro Horário / CW)
//   = 0  →  A, B, C são COLINEARES
ld cross(Point A, Point B, Point C) {
    return (B - A).cross(C - A);
}

// Versão com sinal: retorna -1, 0 ou +1
int orient(Point A, Point B, Point C) {
    ld val = cross(A, B, C);
    if (val >  EPS) return  1;  // CCW
    if (val < -EPS) return -1;  // CW
    return 0;                   // Colinear
}

// ==============================================================================
// INTERSEÇÃO DE SEGMENTOS
// ==============================================================================

// Verifica se o ponto P está sobre o segmento AB (pressupõe que são colineares)
bool onSegment(Point A, Point B, Point P) {
    // P está no bounding box de AB?
    return min(A.x, B.x) - EPS <= P.x && P.x <= max(A.x, B.x) + EPS &&
           min(A.y, B.y) - EPS <= P.y && P.y <= max(A.y, B.y) + EPS;
}

// Retorna true se os segmentos AB e CD se intersectam (inclui pontas)
bool segmentsIntersect(Point A, Point B, Point C, Point D) {
    int d1 = orient(C, D, A);
    int d2 = orient(C, D, B);
    int d3 = orient(A, B, C);
    int d4 = orient(A, B, D);

    // Caso geral: os segmentos se cruzam de lado a lado
    if (d1 * d2 < 0 && d3 * d4 < 0) return true;

    // Casos degenerados: ponto de uma extremidade sobre o outro segmento
    if (d1 == 0 && onSegment(C, D, A)) return true;
    if (d2 == 0 && onSegment(C, D, B)) return true;
    if (d3 == 0 && onSegment(A, B, C)) return true;
    if (d4 == 0 && onSegment(A, B, D)) return true;

    return false;
}

// Encontra o ponto exato de interseção entre as RETAS AB e CD.
// ATENÇÃO: chame apenas se souber que as retas não são paralelas (cross != 0).
Point lineIntersection(Point A, Point B, Point C, Point D) {
    ld a1 = cross(A, B, C);  // (B-A) × (C-A)
    ld a2 = cross(A, B, D);  // (B-A) × (D-A)
    // t = a1 / (a1 - a2): parâmetro no segmento CD onde ocorre a interseção
    return C + (D - C) * (a1 / (a1 - a2));
}

// ==============================================================================
// ÁREA DO TRIÂNGULO
// ==============================================================================

// Retorna a área SINALIZADA do triângulo ABC
// (positiva se CCW, negativa se CW, zero se colinear)
ld triangleAreaSigned(Point A, Point B, Point C) {
    return cross(A, B, C) / 2.0L;
}

// Retorna a área ABSOLUTA do triângulo ABC
ld triangleArea(Point A, Point B, Point C) {
    return abs(triangleAreaSigned(A, B, C));
}