#include <bits/stdc++.h>

using namespace std;

// MAXN deve ser o limite da SOMA dos tamanhos de todas as strings inseridas
const int MAXN = 1e5 + 5; 
const int ALPHABET = 26;  // Considera apenas letras minúsculas ('a'-'z')

int trie[MAXN][ALPHABET];
int is_end[MAXN]; // Conta quantas palavras terminam neste nó (pode ser bool)
int node_cnt = 0; // Controla a "alocação" de novos nós

// Função essencial para resetar a Trie em problemas com múltiplos casos de teste
void init_trie() {
    for (int i = 0; i <= node_cnt; i++) {
        is_end[i] = 0;
        for (int j = 0; j < ALPHABET; j++) {
            trie[i][j] = 0;
        }
    }
    node_cnt = 0; // Nó 0 é sempre a raiz
}

// Insere uma string na Trie em tempo O(Tamanho da String)
void insert(const string& s) {
    int u = 0; // Começa na raiz
    for (char c : s) {
        int idx = c - 'a'; // Mapeia a letra para um índice de 0 a 25
        
        if (!trie[u][idx]) {
            trie[u][idx] = ++node_cnt; // "Aloca" um novo nó
        }
        u = trie[u][idx]; // Desce na árvore
    }
    is_end[u]++; // Marca o final da palavra
}

// Verifica se a string exata existe na Trie em O(Tamanho da String)
bool search(const string& s) {
    int u = 0;
    for (char c : s) {
        int idx = c - 'a';
        if (!trie[u][idx]) return false; // Caminho não existe
        u = trie[u][idx];
    }
    return is_end[u] > 0; // Retorna true só se for o final de uma palavra inserida
}

// Verifica se existe ALGUMA palavra na Trie que comece com o prefixo
bool startsWith(const string& prefix) {
    int u = 0;
    for (char c : prefix) {
        int idx = c - 'a';
        if (!trie[u][idx]) return false; // Prefixo quebrou no meio
        u = trie[u][idx];
    }
    return true; // Conseguiu percorrer todo o prefixo
}