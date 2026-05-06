#include <bits/stdc++.h>

using namespace std;

/**
 * A função "Check" (Validar). É o coração da Busca Binária na Resposta.
 * * Funcionamento:
 * - Em vez de tentar "encontrar" a resposta gerando todas as combinações, 
 * você CHUTA uma resposta ('mid') e faz uma simulação Gulosa (O(N))
 * para verificar se é possível atingir o objetivo com esse chute.
 * - Retorna true se o chute é válido, false caso contrário.
 */
bool check(long long mid, const vector<int>& pesos, int caminhoes) {
    long long soma_atual = 0;
    int caminhoes_usados = 1;

    for (int p : pesos) {
        // Se um único item já excede a capacidade chutada, é impossível
        if (p > mid) return false; 
        
        if (soma_atual + p > mid) {
            caminhoes_usados++;
            soma_atual = p;
            if (caminhoes_usados > caminhoes) return false;
        } else {
            soma_atual += p;
        }
    }
    return true;
}

/**
 * Aplica Busca Binária para encontrar o MENOR valor válido no espaço de busca.
 * Complexidade de Tempo: O(N * log(Soma dos Pesos)), substituindo o que seria O(N!).
 */
long long busca_binaria_resposta(const vector<int>& pesos, int caminhoes) {
    if (pesos.empty()) return 0;

    // O limite inferior perfeito: o maior elemento do array
    long long esquerda = *max_element(pesos.begin(), pesos.end()); 
    
    // O limite superior perfeito: a soma de todos os elementos (1 único caminhão gigante)
    long long direita = accumulate(pesos.begin(), pesos.end(), 0LL); 
    
    long long melhor_resposta = direita;

    while (esquerda <= direita) {
        long long mid = esquerda + (direita - esquerda) / 2;

        if (check(mid, pesos, caminhoes)) {
            melhor_resposta = mid; // É possível. Salva a resposta e...
            direita = mid - 1;     // ...tenta apertar para achar um número AINDA MENOR.
        } else {
            esquerda = mid + 1;    // Não é possível. O chute foi baixo, precisa aumentar.
        }
    }
    
    return melhor_resposta;
}

/* * DICA PARA MARATONA: 
 * Se o problema pedir para "Maximizar o Mínimo" (ex: afastar vacas o máximo possível),
 * a lógica inverte! O 'check' verdadeiro vai buscar respostas maiores:
 * if (check(mid)) { resposta = mid; esquerda = mid + 1; } else { direita = mid - 1; }
 */