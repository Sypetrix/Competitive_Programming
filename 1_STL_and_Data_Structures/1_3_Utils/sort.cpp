#include <bits/stdc++.h>

using namespace std;

// ==========================================
// 1. MERGE SORT
// Estável: Sim
// In-place: Não (Requer memória auxiliar O(N) para o merge)
// Complexidade: O(N log N)
// ==========================================
void merge(vector<int>& arr, int esq, int meio, int dir) {
    int n1 = meio - esq + 1;
    int n2 = dir - meio;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[esq + i];
    for (int j = 0; j < n2; j++) R[j] = arr[meio + 1 + j];

    int i = 0, j = 0, k = esq;
    while (i < n1 && j < n2) {
        // O sinal '<=' é o que garante a estabilidade do Merge Sort
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++]; // Conta uma inversão aqui em problemas de CP
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void merge_sort(vector<int>& arr, int esq, int dir) {
    if (esq >= dir) return;
    int meio = esq + (dir - esq) / 2;
    merge_sort(arr, esq, meio);
    merge_sort(arr, meio + 1, dir);
    merge(arr, esq, meio, dir);
}


// ==========================================
// 2. QUICK SORT
// Estável: Não (Os saltos distantes do pivô quebram a ordem original)
// In-place: Sim (O(1) de memória extra, ignorando a pilha de recursão O(log N))
// Complexidade: O(N log N) em média, Pior caso O(N^2)
// ==========================================
int particiona(vector<int>& arr, int baixo, int alto) {
    int pivo = arr[alto]; 
    int i = (baixo - 1); 

    for (int j = baixo; j <= alto - 1; j++) {
        if (arr[j] < pivo) {
            i++; 
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[alto]);
    return (i + 1);
}

void quick_sort(vector<int>& arr, int baixo, int alto) {
    if (baixo < alto) {
        int pi = particiona(arr, baixo, alto);
        quick_sort(arr, baixo, pi - 1);
        quick_sort(arr, pi + 1, alto);
    }
}


// ==========================================
// 3. COUNTING SORT
// Estável: Sim (O loop reverso preenchendo o output garante a estabilidade)
// In-place: Não (Requer vetores 'count' e 'output')
// Complexidade: O(N + K), onde K é a amplitude dos valores
// ==========================================
void counting_sort(vector<int>& arr) {
    if (arr.empty()) return;
    int max_val = *max_element(arr.begin(), arr.end());
    int min_val = *min_element(arr.begin(), arr.end());
    int alcance = max_val - min_val + 1;

    vector<int> count(alcance, 0), output(arr.size());
    
    for (int i = 0; i < arr.size(); i++) count[arr[i] - min_val]++;
    for (int i = 1; i < count.size(); i++) count[i] += count[i - 1]; // Prefix sum

    // Loop reverso é o segredo da estabilidade aqui
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i] - min_val] - 1] = arr[i];
        count[arr[i] - min_val]--;
    }
    arr = output;
}


// ==========================================
// 4. RADIX SORT
// Estável: Sim (Necessário ser estável para não estragar a ordenação dos dígitos anteriores)
// In-place: Não (Usa o Counting Sort internamente)
// Complexidade: O(N * D), onde D é a quantidade de dígitos do maior número
// ==========================================
void count_sort_por_digito(vector<int>& arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    vector<int> count(10, 0);

    for (int i = 0; i < n; i++) count[(arr[i] / exp) % 10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
    for (int i = 0; i < n; i++) arr[i] = output[i];
}

void radix_sort(vector<int>& arr) {
    if (arr.empty()) return;
    int max_val = *max_element(arr.begin(), arr.end());
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        count_sort_por_digito(arr, exp);
    }
}


// ==========================================
// 5. SELECTION SORT
// Estável: Não (Pode trocar um elemento menor passando por cima de um igual)
// In-place: Sim
// Complexidade: O(N^2)
// ==========================================
void selection_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        if (min_idx != i) swap(arr[i], arr[min_idx]);
    }
}


// ==========================================
// 6. INSERTION SORT
// Estável: Sim (O elemento só "caminha" para a esquerda até encontrar um menor ou igual)
// In-place: Sim
// Complexidade: O(N^2) - Mas é O(N) para arrays quase ordenados
// ==========================================
void insertion_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int chave = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > chave) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = chave;
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    vector<int> array_original = {170, 45, 75, 90, 802, 24, 2, 66};
    
    // Exemplo de uso
    vector<int> copia_merge = array_original;
    merge_sort(copia_merge, 0, copia_merge.size() - 1);
    
    vector<int> copia_radix = array_original;
    radix_sort(copia_radix);

    cout << "Ordenacao concluida com sucesso.\n";

    return 0;
}