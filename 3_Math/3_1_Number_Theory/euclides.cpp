#include <iostream>
using namespace std;

typedef long long ll;

// Resolve: a*x + b*y = mdc(a, b)
// Retorna o Máximo Divisor Comum (MDC) e altera x e y por referência
ll extGCD(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll d = extGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

// Retorna o inverso modular de 'a' módulo 'm'
// Retorna -1 se o inverso não existir (a e m têm de ser coprimos)
ll invMod(ll a, ll m) {
    ll x, y;
    ll g = extGCD(a, m, x, y);
    if (g != 1) return -1; // Não existe inverso
    return (x % m + m) % m; // Garante que o resultado é positivo
}