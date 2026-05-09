#include <bits/stdc++.h>

using namespace std;

/*
Permutações (Backtracking) === O(n!)

A função recebe o array e as posições de início (l) e fim (r).
No laço for, fixa-se um elemento na posição atual fazendo um swap.
A função é chamada recursivamente para permutar o restante do array (l + 1).
Após o retorno da recursão, um segundo swap desfaz a troca original (backtracking), restaurando o estado do array para a próxima iteração.
Quando l == r, uma permutação completa foi formada e é impressa.
==========
==========
Alternativa Nativa em C++ (STL)

Para competições, a função std::next_permutation da biblioteca <algorithm> é o padrão ouro.
Ela modifica o array in-place para a próxima permutação na ordem lexicográfica e retorna false quando as permutações se esgotam.
Atenção: O array deve estar ordenado (usando std::sort) antes de iniciar o do-while, caso contrário, você não irá iterar por todas as permutações possíveis.
*/
void gera_permutacoes(vector<int>& arr, int l, int r) {
    if (l == r) {
        for (int x : arr) 
            cout << x << " ";
        cout << "\n";

        return;
    }
    for (int i = l; i <= r; i++) {
        swap(arr[l], arr[i]);
        gera_permutacoes(arr, l + 1, r);
        swap(arr[l], arr[i]);
    }
}

/* outra versão, baseada em um vetor de booleano"usado"

ele verificar se o número "N" já foi utilizado, caso contrário, passa para o seguinte
*/
void gera_permutacao2(int n, int k, vector<int>& perm, vector<bool>& usados) {
    // Caso base: se a posição atual (k) chegou ao tamanho total (n) 
    if (n == k) {
        // processa permutação
        for (int x : perm) 
            cout << x << " ";
        cout << "\n";
        return;
    }

    // Tenta colocar cada número i (de 0 a n-1) na posição k
    for (int i = 0; i < n; ++i) {
        // Verifica se o elemento i ainda não está na permutação parcial [cite: 32]
        if (!usados[i]) { 
            usados[i] = true;       // Marca o elemento como usado
            perm[k] = i;            // Inclui o elemento na permutação na posição k [cite: 32]
            
            // Chama a recursão para gerar o restante da permutação [cite: 33]
            gera_permutacao2(n, k + 1, perm, usados); 
            
            // Backtracking: desfaz a escolha para tentar a próxima alternativa [cite: 17]
            usados[i] = false;      
        }
    }
}