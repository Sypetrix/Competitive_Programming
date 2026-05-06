#include <bits/stdc++.h>

using namespace std;

typedef long double ld;
const ld EPS = 1e-9;

struct Point {
    ld x, y;
    Point(ld x = 0, ld y = 0) : x(x), y(y) {}
    Point operator-(const Point& p) const { return {x - p.x, y - p.y}; }
    ld cross(const Point& p)        const { return x * p.y - y * p.x;  }
    bool operator<(const Point& p)  const {
        if (abs(x - p.x) > EPS) return x < p.x;
        return y < p.y - EPS;
    }
    bool operator==(const Point& p) const {
        return abs(x - p.x) < EPS && abs(y - p.y) < EPS;
    }
};

// Produto vetorial de (B - A) × (C - A)
ld cross(Point A, Point B, Point C) {
    return (B - A).cross(C - A);
}

// ==============================================================================
// ALGORITMO: Fecho Convexo (Convex Hull) — Monotone Chain de Andrew
// Complexidade: O(N log N)
// ==============================================================================
// Descrição:
// - Constrói o menor polígono convexo que contém todos os pontos de entrada.
// - Ordena os pontos lexicograficamente (por X, depois por Y).
// - Constrói a cadeia inferior (lower hull) e a superior (upper hull) separadamente.
// - Usa o produto vetorial para decidir se um ponto "dobra" para a esquerda
//   (CCW), mantendo o invariante de que o hull é sempre convexo.
// ==============================================================================

// Retorna os vértices do fecho convexo em ordem ANTI-HORÁRIA (CCW).
// Se 'incluir_colineares' for true, inclui pontos colineares nas arestas do hull.
vector<Point> convexHull(vector<Point> pts, bool incluir_colineares = false) {
    int n = pts.size();
    if (n < 2) return pts;

    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    n = pts.size();
    if (n < 2) return pts;

    vector<Point> hull;
    hull.reserve(2 * n);

    // ── Cadeia inferior (left → right) ───────────────────────────────────────
    for (int i = 0; i < n; i++) {
        while (hull.size() >= 2) {
            ld c = cross(hull[hull.size() - 2], hull[hull.size() - 1], pts[i]);
            // Remove se o giro for horário (CW) ou colinear (dependendo do modo)
            if (c < (incluir_colineares ? -EPS : EPS)) 
                hull.pop_back();
            else break;
        }
        hull.push_back(pts[i]);
    }

    // ── Cadeia superior (right → left) ───────────────────────────────────────
    int lower_size = hull.size();
    for (int i = n - 2; i >= 0; i--) {
        while ((int)hull.size() > lower_size) {
            ld c = cross(hull[hull.size() - 2], hull[hull.size() - 1], pts[i]);
            if (c < (incluir_colineares ? -EPS : EPS))
                hull.pop_back();
            else break;
        }
        hull.push_back(pts[i]);
    }

    hull.pop_back(); // O primeiro ponto foi adicionado duas vezes
    return hull;
}

// ==============================================================================
// UTILITÁRIOS DO FECHO CONVEXO
// ==============================================================================

// Perímetro do fecho convexo (soma dos comprimentos das arestas)
ld hullPerimeter(const vector<Point>& hull) {
    ld perim = 0;
    int n = hull.size();
    for (int i = 0; i < n; i++) {
        Point diff = hull[(i + 1) % n] - hull[i];
        perim += sqrtl(diff.cross(diff) + diff.x * diff.x + diff.y * diff.y);
        // Simplificando: perim += (hull[(i+1)%n] - hull[i]).length()
    }
    return perim;
}

// Área do fecho convexo usando a Fórmula do Cadarço (Shoelace)
ld hullArea(const vector<Point>& hull) {
    ld area = 0;
    int n = hull.size();
    for (int i = 0; i < n; i++) {
        area += hull[i].cross(hull[(i + 1) % n]);
    }
    return abs(area) / 2.0L;
}