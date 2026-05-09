#include <iostream>
using namespace std;

// Exemplo de uma função de transição qualquer
int f(int x) { return (x * x + 1) % 1000; }

// Retorna um par: {início_do_ciclo (mu), tamanho_do_ciclo (lambda)}
pair<int, int> floyd_cycle_finding(int (*func)(int), int x0) {
    int tart = func(x0), lebr = func(func(x0));
    
    // 1. Avançar até se encontrarem
    while (tart != lebr) {
        tart = func(tart);
        lebr = func(func(lebr));
    }
    
    // 2. Encontrar o início do ciclo (mu)
    int mu = 0;
    tart = x0;
    while (tart != lebr) {
        tart = func(tart);
        lebr = func(lebr);
        mu++;
    }
    
    // 3. Encontrar o tamanho do ciclo (lambda)
    int lambda = 1;
    lebr = func(tart);
    while (tart != lebr) {
        lebr = func(lebr);
        lambda++;
    }
    
    return {mu, lambda};
}