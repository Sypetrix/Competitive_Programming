# Teoria dos Jogos (Game Theory)

Teoria dos jogos combinatórios estuda jogos de dois jogadores onde ambos jogam **otimamente**, sem aleatoriedade e com informação perfeita. O objetivo é determinar qual jogador vence a partir de um estado inicial.

---

## Visão Geral

| Conceito | Complexidade | Uso típico |
|---|---|---|
| Nim clássico | O(N) | Múltiplas pilhas, mover remove objetos |
| Sprague-Grundy (Grundy values) | O(estados · movimentos) | Qualquer jogo imparcial |
| XOR de Grundy | O(1) | Combinar jogos independentes |
| Staircase Nim | O(N) | Nim em escadas/grafos DAG |

---

## Conceitos Fundamentais

### Posições P e N

Todo estado de um jogo imparcial é classificado como:

- **P-position (Previous player wins):** o jogador que **acabou de mover** vence. Quem está nesta posição e é sua vez de jogar **perde** com jogo ótimo.
- **N-position (Next player wins):** o jogador que **vai mover** vence com jogo ótimo.

**Regras de classificação:**
- Estado terminal (sem movimentos) → **P-position** (quem deve mover perde)
- Estado com pelo menos um movimento para P-position → **N-position**
- Estado onde todos os movimentos levam a N-positions → **P-position**

---

## Nim Clássico

**Jogo:** N pilhas com aᵢ objetos cada. Dois jogadores alternam: cada um remove qualquer quantidade positiva de objetos de **uma** pilha. Quem não puder mover perde (variante normal).

**Teorema de Bouton:** o estado atual é P-position se e somente se o XOR de todos os tamanhos de pilha é **zero**.

```cpp
bool nim_first_player_wins(vector<int>& piles) {
    int xor_sum = 0;
    for (int p : piles) xor_sum ^= p;
    return xor_sum != 0; // true = primeiro jogador vence
}
```

**Estratégia ótima:** encontre uma pilha `i` tal que `piles[i] XOR xor_sum < piles[i]` e reduza-a para `piles[i] XOR xor_sum`.

---

## Sprague-Grundy

O **Teorema de Sprague-Grundy** generaliza o Nim: todo jogo imparcial finito e determinístico é equivalente a uma pilha de Nim de tamanho igual ao seu **Grundy value** (ou nimber).

```
G(estado) = mex{ G(s') : s' é alcançável a partir de estado }
```

onde **mex** (minimum excludant) é o menor inteiro não-negativo ausente no conjunto.

```cpp
// mex de um conjunto de valores
int mex(set<int>& s) {
    int m = 0;
    while (s.count(m)) m++;
    return m;
}

// Calcula Grundy value por memoização
map<int, int> memo;

int grundy(int state) {
    if (memo.count(state)) return memo[state];
    set<int> reachable;
    for (int next_state : get_moves(state))
        reachable.insert(grundy(next_state));
    return memo[state] = mex(reachable);
}
```

**Estado é P-position ⟺ Grundy value == 0.**

### Combinando Jogos Independentes

Se um problema é composto por N jogos independentes simultâneos, o Grundy value total é o **XOR** dos Grundy values individuais:

```
G(jogo1 + jogo2 + ... + jogoN) = G(jogo1) XOR G(jogo2) XOR ... XOR G(jogoN)
```

O estado é P-position ⟺ esse XOR total é zero.

---

## Exemplos Clássicos de Grundy Values

### Nim em Grafo (Green Hackenbush em DAG)

Cada aresta tem um Grundy value. Para caminhos simples, o valor do jogo é igual ao número de arestas mod 2 (para bamboo stalks).

### Subtraction Game

Pode-se remover entre 1 e K objetos de uma pilha. O Grundy value de uma pilha de tamanho n é `n % (K + 1)`.

```cpp
int grundy_subtraction(int n, int K) {
    return n % (K + 1);
}
```

### Jogo de Euclides (Euclid's Game)

Dois jogadores alternam subtraindo um múltiplo positivo do menor número do maior. Pode ser resolvido com análise de casos e Grundy.

### Staircase Nim

Objetos em degraus 0, 1, 2, ..., N. Um movimento leva qualquer quantidade de objetos de um degrau para o degrau imediatamente abaixo. O primeiro jogador vence se e somente se o XOR dos degraus **ímpares** é não-zero.

```cpp
bool staircase_nim_wins(vector<int>& stairs) {
    int xor_odd = 0;
    for (int i = 1; i < (int)stairs.size(); i += 2)
        xor_odd ^= stairs[i];
    return xor_odd != 0;
}
```

---

## Nim com Regra Misère

Na variante **misère**, quem pegar o último objeto **perde**. A estratégia é quase idêntica ao Nim normal:

- Se todas as pilhas têm tamanho ≤ 1: vence quem tiver número **par** de pilhas.
- Caso contrário: aplique a estratégia normal do Nim, mas ao deixar apenas pilhas de tamanho 1, garanta deixar número **ímpar** delas.

```cpp
bool misere_nim_wins(vector<int>& piles) {
    int xor_sum = 0;
    bool all_le_one = true;
    for (int p : piles) {
        xor_sum ^= p;
        if (p > 1) all_le_one = false;
    }
    if (all_le_one) {
        // Conta pilhas não-vazias
        int count = count_if(piles.begin(), piles.end(), [](int x){ return x > 0; });
        return count % 2 == 0; // vence se par (próximo a mover perde)
    }
    return xor_sum != 0;
}
```

---

## Checklist para Problemas de Teoria dos Jogos

1. O jogo é **imparcial** (ambos os jogadores têm os mesmos movimentos disponíveis)?
   - Sim → Sprague-Grundy / Nim
   - Não (ex: Xadrez) → teoria de jogos partizanos (mais raro em CP)
2. O jogo é **finito** e sem empate? (necessário para Sprague-Grundy)
3. Os jogos são **independentes**? → XOR dos Grundy values
4. Quantos estados existem? Se poucos → memoização direta; se muitos → identifique padrão no Grundy
5. É variante **misère** (quem move por último perde)?

> **Dica de identificação de padrão:** calcule os primeiros 20-30 Grundy values na mão ou com código, e procure periodicidade. A maioria dos jogos em CP tem período pequeno.