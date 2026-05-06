#include <iostream>

using namespace std;

typedef long long ll;

/* * FÓRMULA DE LEGENDRE
 * Retorna a maior potência do primo 'p' que divide 'n!' (n fatorial).
 * Complexidade: O(log_p(n))
 */
ll legendre(ll n, ll p) {
    ll count = 0;
    while (n > 0) {
        count += n / p;
        n /= p;
    }
    return count;
}

/*
 * FACTOVISORS (M divide N! ?)
 * Verifica se 'm' divide perfeitamente 'n!' sem calcular o fatorial.
 * Complexidade: O(sqrt(m) + log(n))
 */
bool divides_factorial(ll n, ll m) {
    if (m == 0) return false;
    if (m <= n) return true; // Se m <= n, m já é um dos termos de n!

    // Fatoração de m em O(sqrt(m))
    for (ll i = 2; i * i <= m; ++i) {
        if (m % i == 0) {
            ll count_m = 0;
            
            // Descobre qual a potência do primo 'i' em 'm'
            while (m % i == 0) {
                count_m++;
                m /= i;
            }

            // Usa a Fórmula de Legendre para contar 'i' em 'n!'
            ll count_n = legendre(n, i);

            // Se n! tem o primo 'i' menos vezes do que 'm' precisa
            if (count_n < count_m) {
                return false; 
            }
        }
    }

    // Se sobrou um fator primo gigante em m (maior que a raiz quadrada inicial)
    if (m > 1) {
        // Para n! ter esse primo gigante, n precisaria ser >= a ele
        if (n < m) return false;
    }

    return true;
}

/* * DICA PARA O CADERNO:
 * Se o problema pedir "Quantos zeros à direita tem o número N!", 
 * basta chamar legendre(N, 5). 
 * (A quantidade de zeros é determinada pelos pares de (2 * 5). 
 * Como há muito mais fatores 2 do que 5, a potência de 5 é o gargalo).
 */