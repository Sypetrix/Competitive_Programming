#include <bits/stdc++.h>

using namespace std;

/*
Verifica se existe algum subconjunto cuja soma dos elementos seja igual ao valor 'alvo' === O(2^(n/2) * log(2^(n/2)))

- Divisão: O array original é dividido em duas metades de tamanhos iguais (esq e dir).
- Geração de Somas: Aplica-se a técnica de Bitmasking (explicada acima) independentemente
em cada metade. Isso gera todas as somas de subconjuntos possíveis da metade esquerda
(armazenadas em 'soma_esq') e da metade direita (armazenadas em 'soma_dir').
- Ordenação: O vetor 'soma_dir' é ordenado. Isso é crucial para permitir consultas 
rápidas na etapa seguinte.
- Fusão (Meet-in-the-Middle): Itera-se sobre cada valor 'se' gerado em 'soma_esq'.
Calcula-se o valor exato que falta para atingir a resposta: falta = alvo - se.
- Busca Binária: Utiliza-se std::binary_search para procurar o valor 'falta' dentro 
do vetor ordenado 'soma_dir'. Se for encontrado, significa que a soma de uma parte
esquerda com uma parte direita forma o 'alvo', retornando true imediatamente.
*/
bool meet_in_middle(const vector<long long>& arr, long long alvo) {
    int n = arr.size();
    if (n == 0) return alvo == 0;

    // Função Lambda para gerar todas as somas de um sub-intervalo [start, end) do array
    auto get_sums = [&](int start, int end) {
        int len = end - start;
        // Já alocamos o vetor com o tamanho exato: 2^len
        vector<long long> sums(1 << len, 0); 
        
        for (int mask = 0; mask < (1 << len); mask++) {
            for (int i = 0; i < len; i++) {
                // Se o i-ésimo bit estiver ativo, somamos o elemento correspondente
                if (mask & (1 << i)) {
                    sums[mask] += arr[start + i];
                }
            }
        }
        return sums;
    };

    // Divide o problema na metade e gera as somas
    vector<long long> soma_esq = get_sums(0, n / 2);
    vector<long long> soma_dir = get_sums(n / 2, n);

    // Ordena a segunda metade para permitir a busca binária
    sort(soma_dir.begin(), soma_dir.end());

    // Combina os resultados
    for (long long se : soma_esq) {
        long long falta = alvo - se;
        if (binary_search(soma_dir.begin(), soma_dir.end(), falta)) {
            return true;
        }
    }

    return false;
}