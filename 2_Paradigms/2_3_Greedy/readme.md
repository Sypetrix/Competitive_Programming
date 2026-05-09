# Guloso (Greedy)

Um algoritmo Guloso toma, a cada passo, a **escolha localmente ótima** sem reconsiderar decisões passadas, na esperança de que isso leve ao ótimo global. Quando funciona, é frequentemente a solução mais simples e mais eficiente disponível.

O desafio está em **provar** que a escolha greedy é correta. Em competições, dois argumentos são mais comuns:

- **Troca de argumentos (Exchange Argument):** assuma que existe uma solução ótima diferente da greedy; mostre que trocar um elemento dela pela escolha greedy não piora a solução. Logo, a greedy também é ótima.
- **Greedy permanece à frente (Greedy Stays Ahead):** após cada passo, prove que o estado do algoritmo greedy é pelo menos tão bom quanto o de qualquer outra solução.

> Desconfie se um problema pede o ótimo global mas a sua intuição greedy não consegue uma prova simples — pode ser necessário DP.

---

## Visão Geral de Problemas Clássicos

| Problema | Critério de escolha | Complexidade |
|---|---|---|
| Escalonamento de intervalos (máx. atividades) | Menor tempo de término | O(N log N) |
| Cobertura mínima de intervalos | Maior alcance a partir do ponto atual | O(N log N) |
| Problema da mochila fracionária | Maior valor por peso | O(N log N) |
| Código de Huffman | Menores frequências primeiro | O(N log N) |
| Minimizar tempo de espera (SPT) | Menor tempo de processamento primeiro | O(N log N) |
| **Busca binária na resposta** | Verificação gulosa dentro do `check()` | O(N log N) a O(N log V) |

---

## Escalonamento de Intervalos (Interval Scheduling)

**Problema:** dado N eventos com início e fim, selecionar o maior número de eventos não sobrepostos.

**Greedy:** sempre escolha o evento com o **menor tempo de término** disponível.

```cpp
int max_atividades(vector<pair<int,int>>& eventos) {
    // ordena pelo tempo de término
    sort(eventos.begin(), eventos.end(), [](auto& a, auto& b) {
        return a.second < b.second;
    });

    int count = 0, fim_atual = INT_MIN;
    for (auto& [inicio, fim] : eventos) {
        if (inicio >= fim_atual) {
            count++;
            fim_atual = fim;
        }
    }
    return count;
}
```

**Por que funciona?** Escolher o evento que termina mais cedo libera o máximo de tempo para os eventos seguintes. Qualquer outra escolha só pode deixar menos espaço.

---

## Cobertura Mínima de Intervalos

**Problema:** dado um ponto `S` e N intervalos, encontrar o menor número de intervalos para cobrir `[S, T]`.

**Greedy:** entre todos os intervalos que começam em ou antes do ponto atual, escolha o que **vai mais longe à direita**.

```cpp
int cobertura_minima(vector<pair<int,int>>& intervals, int S, int T) {
    sort(intervals.begin(), intervals.end());
    int count = 0, alcance = S, i = 0, n = intervals.size();

    while (alcance < T) {
        int melhor = alcance;
        while (i < n && intervals[i].first <= alcance)
            melhor = max(melhor, intervals[i++].second);

        if (melhor == alcance) return -1; // impossível
        alcance = melhor;
        count++;
    }
    return count;
}
```

---

## Minimizar Tempo de Espera (SPT — Shortest Processing Time)

**Problema:** N tarefas com durações diferentes devem ser executadas em sequência. Minimizar a soma dos tempos de conclusão.

**Greedy:** execute sempre a tarefa de **menor duração** primeiro.

```cpp
long long min_tempo_espera(vector<int>& durações) {
    sort(durações.begin(), durações.end());
    long long soma = 0, acumulado = 0;
    for (int d : durações) {
        acumulado += d;
        soma += acumulado;
    }
    return soma;
}
```

---

## Mochila Fracionária

**Problema:** N itens com peso e valor. Pode-se pegar frações. Maximizar o valor total com capacidade máxima W.

**Greedy:** ordene por valor/peso decrescente e pegue o máximo possível de cada item.

```cpp
double mochila_fracionaria(vector<pair<int,int>>& itens, int W) {
    // par = {valor, peso}
    sort(itens.begin(), itens.end(), [](auto& a, auto& b) {
        return (double)a.first / a.second > (double)b.first / b.second;
    });

    double total = 0;
    for (auto& [val, peso] : itens) {
        if (W <= 0) break;
        int tomar = min(peso, W);
        total += (double)tomar / peso * val;
        W -= tomar;
    }
    return total;
}
```

> **Atenção:** para a **mochila 0/1** (sem frações), greedy não funciona — use DP.

---

## Busca Binária na Resposta

Um padrão extremamente comum que combina busca binária com uma verificação gulosa. Em vez de buscar a resposta diretamente, aplica-se busca binária sobre o **espaço de valores possíveis** e verifica cada candidato com uma função `check()` eficiente.

**Quando usar:** o problema pede um valor ótimo (mínimo ou máximo) e é fácil verificar em O(N) se um valor `x` é viável.

```
"Qual a menor capacidade máxima para distribuir pesos em K caminhões?"
→ Busca Binária no intervalo [max(pesos), soma(pesos)]
→ Para cada 'mid', simula a distribuição greedy em O(N)
```

```cpp
bool check(long long mid, const vector<int>& pesos, int K) {
    long long soma = 0;
    int caminhoes = 1;
    for (int p : pesos) {
        if (p > mid) return false; // um item já excede
        if (soma + p > mid) { caminhoes++; soma = p; }
        else soma += p;
        if (caminhoes > K) return false;
    }
    return true;
}

long long busca_binaria_resposta(const vector<int>& pesos, int K) {
    long long esq = *max_element(pesos.begin(), pesos.end());
    long long dir = accumulate(pesos.begin(), pesos.end(), 0LL);
    long long res = dir;

    while (esq <= dir) {
        long long mid = esq + (dir - esq) / 2;
        if (check(mid, pesos, K)) { res = mid; dir = mid - 1; }
        else esq = mid + 1;
    }
    return res;
}
```

**Inversão para "Maximizar o Mínimo"** (ex: afastar vacas o máximo possível):

```cpp
// check retorna true quando mid é viável
if (check(mid)) { res = mid; esq = mid + 1; }
else            { dir = mid - 1; }
```

---

## Como Identificar um Problema Greedy

Sinais de que a abordagem greedy pode funcionar:

- O enunciado pede "mínimo", "máximo", "menor número de operações" e **não** menciona escolhas interdependentes complexas.
- Após ordenar os elementos por algum critério, a melhor escolha em cada passo é sempre o próximo elemento na ordem.
- A estrutura do problema lembra escalonamento, cobertura ou empacotamento.

Sinais de que greedy **não** vai funcionar:

- A escolha local ótima pode ser globalmente ruim (ex: mochila 0/1, troco com moedas arbitrárias).
- O valor de uma escolha depende de escolhas futuras ainda não feitas.
- Você consegue construir um contra-exemplo simples.