#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1005;
long long C[MAXN][MAXN];

// Calcula nCr em O(N^2) - Seguro para qualquer módulo ou sem módulo (se couber em long long)
void pascal_triangle() {
    for (int i = 0; i < MAXN; i++) {
        C[i][0] = 1; // nC0 é sempre 1
        for (int j = 1; j <= i; j++) {
            C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]); 
            // C[i][j] %= MOD; // Descomente se houver módulo
        }
    }
}
// Uso: C[n][k] contém o valor de "n escolhe k"