#include <bits/stdc++.h>

using namespace std;

typedef long double ld;

// NUNCA compare floats com ==. Use sempre: abs(a - b) < EPS
const ld EPS = 1e-9;

// ==============================================================================
// STRUCT: Point (Ponto / Vetor 2D)
// ==============================================================================
// Representa tanto um ponto no plano quanto um vetor 2D.
// Usa 'long double' para máxima precisão. Para geometria inteira pura
// (sem distâncias ou ângulos), substitua 'ld' por 'long long'.
// ==============================================================================

struct Point {
    ld x, y;

    Point(ld x = 0, ld y = 0) : x(x), y(y) {}

    // ─── Operadores aritméticos ───────────────────────────────────────────────
    Point operator+(const Point& p) const { return {x + p.x, y + p.y}; }
    Point operator-(const Point& p) const { return {x - p.x, y - p.y}; }
    Point operator*(ld t)           const { return {x * t,   y * t};   }
    Point operator/(ld t)           const { return {x / t,   y / t};   }

    // ─── Comparadores (necessários para sort e set) ───────────────────────────
    bool operator==(const Point& p) const {
        return abs(x - p.x) < EPS && abs(y - p.y) < EPS;
    }
    bool operator<(const Point& p) const {
        if (abs(x - p.x) > EPS) return x < p.x;
        return y < p.y - EPS;
    }

    // ─── Produto Escalar (Dot Product): A · B = |A||B| cos(θ) ────────────────
    // Se dot > 0: ângulo agudo. Se dot == 0: perpendiculares. Se dot < 0: obtuso.
    ld dot(const Point& p) const { return x * p.x + y * p.y; }

    // ─── Produto Vetorial (Cross Product): A × B = |A||B| sin(θ) ────────────
    // Se cross > 0: B está à esquerda de A (giro anti-horário / CCW).
    // Se cross < 0: B está à direita de A (giro horário / CW).
    // Se cross == 0: A e B são paralelos/colineares.
    // O módulo do resultado é a área do paralelogramo formado por A e B.
    ld cross(const Point& p) const { return x * p.y - y * p.x; }

    // ─── Norma e Distâncias ───────────────────────────────────────────────────
    ld norm()   const { return x * x + y * y; }   // Módulo ao quadrado (evita sqrt)
    ld length() const { return sqrtl(norm()); }    // Módulo (distância à origem)

    ld dist(const Point& p) const { return (*this - p).length(); }

    // Distância ao quadrado (útil para comparar distâncias sem erro de float)
    ld dist2(const Point& p) const { return (*this - p).norm(); }

    // ─── Ângulo e Rotação ─────────────────────────────────────────────────────
    // Ângulo do vetor em relação ao eixo X positivo, em radianos [-π, π]
    ld angle() const { return atan2l(y, x); }

    // Rotaciona o ponto em 'ang' radianos em torno da ORIGEM (sentido anti-horário)
    Point rotate(ld ang) const {
        return {x * cosl(ang) - y * sinl(ang),
                x * sinl(ang) + y * cosl(ang)};
    }

    // Rotaciona o ponto em 'ang' radianos em torno de um PIVÔ arbitrário
    Point rotateAround(const Point& pivot, ld ang) const {
        return (*this - pivot).rotate(ang) + pivot;
    }

    // ─── I/O ──────────────────────────────────────────────────────────────────
    friend ostream& operator<<(ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
};

// ─── Funções auxiliares ───────────────────────────────────────────────────────

// Distância de um ponto P à reta definida por A e B
ld distToLine(Point P, Point A, Point B) {
    // Área do triângulo PAB = |AB × AP| / 2
    // Altura = Área * 2 / base = |AB × AP| / |AB|
    return abs((B - A).cross(P - A)) / (B - A).length();
}

// Distância de um ponto P ao segmento AB (não à reta infinita)
ld distToSegment(Point P, Point A, Point B) {
    if ((B - A).norm() < EPS) return P.dist(A); // Segmento degenerado (ponto)
    ld t = (P - A).dot(B - A) / (B - A).norm();
    t = max((ld)0, min((ld)1, t));    // Projeta na faixa [0, 1] do segmento
    Point proj = A + (B - A) * t;
    return P.dist(proj);
}

// Ponto médio entre dois pontos
Point midpoint(Point A, Point B) { return (A + B) / 2; }