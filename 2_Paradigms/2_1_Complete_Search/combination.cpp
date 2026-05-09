#include <bits/stdc++.h>

using namespace std;

/*
Combinações (Backtracking) === O(k * binomio(n, k))

A função constrói combinações de tamanho k em um array temporário (atual).
A variável indice controla a posição sendo preenchida em atual, enquanto inicio até fim representa os elementos disponíveis no array original.
O laço for insere um elemento válido na posição corrente e chama a recursão para o próximo índice.
A condição fim - i + 1 >= k - indice é uma poda (pruning): interrompe o laço prematuramente se a quantidade de elementos restantes no array original for insuficiente para preencher o resto do array atual.
*/
void gera_combinacoes(const vector<int>& arr, vector<int>& atual, int inicio, int indice, int k) {
    // Caso base: a combinação atingiu o tamanho k
    if (indice == k) {
        for (int x : atual) cout << x << " ";
        cout << "\n";
        return;
    }

    int n = arr.size();
    // Poda: (n - i) >= (k - indice) garante que há elementos suficientes restantes
    for (int i = inicio; i < n && (n - i) >= (k - indice); i++) {
        atual[indice] = arr[i]; // Escolhe o elemento
        
        // Chama a recursão para o próximo índice
        gera_combinacoes(arr, atual, i + 1, indice + 1, k);
    }
}