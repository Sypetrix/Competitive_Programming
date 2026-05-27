# Problemas Interativos (Interactive Problems)

Problemas interativos diferem dos problemas convencionais: em vez de ler toda a entrada de uma vez e imprimir a resposta, o seu programa **conversa** com um juiz oculto (a *interação*). Você faz perguntas ou palpites, o juiz responde, e você usa as respostas para guiar as próximas ações até encontrar a solução.

---

## Por que Problemas Interativos São Especiais?

Em um problema convencional, toda a entrada já existe antes do seu programa começar. Em um problema interativo:

- A entrada **depende das suas saídas anteriores** — o juiz decide o que responder com base no que você perguntou.
- O número de perguntas é limitado (ex: "use no máximo 30 queries").
- O juiz pode ser **adaptativo** (escolhe respostas para tentar te derrotar) ou **fixo** (a resposta já está determinada, mas você não sabe qual é).

---

## Regra de Ouro: Flush Obrigatório

O erro mais comum em problemas interativos é o **TLE silencioso por buffer não esvaziado**. Em C++, `cout` é bufferizado — ele acumula a saída e só envia em bloco. O juiz fica esperando sua resposta que nunca chegou, e você fica esperando a resposta do juiz que nunca virá: deadlock.

**Toda vez que imprimir uma query, esvazie o buffer:**

```cpp
// Método 1: endl (faz flush, mas é mais lento)
cout << resposta << endl;

// Método 2: flush explícito (preferível em loops)
cout << resposta << "\n";
cout.flush();

// Método 3: desabilitar sync e usar flush manual
ios_base::sync_with_stdio(false);
cin.tie(NULL);
// ... mas ainda precisa de cout.flush() após cada query!
```

> **Regra de bolso:** se o problema diz "interactive" ou "você pode perguntar", sempre use `endl` ou `cout.flush()` imediatamente após cada print de query.

**Em Python:**
```python
import sys
print(query, flush=True)          # flush=True é obrigatório
# ou
sys.stdout.flush()                 # explícito após print
```

---

## Estrutura Geral de um Problema Interativo

```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n; // lê o tamanho ou parâmetros iniciais (se houver)

    while (/* condição de parada */) {
        // 1. Monta a query
        int query = /* ... sua lógica ... */;

        // 2. Envia e faz flush IMEDIATAMENTE
        cout << query << "\n";
        cout.flush();

        // 3. Lê a resposta
        string resposta;
        cin >> resposta;

        // 4. Atualiza seu estado com base na resposta
        if (resposta == "alvo" || resposta == "-1") break; // condição de fim
    }

    return 0;
}
```

---

## Visão Geral de Padrões

| Padrão | Queries Típicas | Estratégia |
|---|---|---|
| Busca Binária | O(log N) | Dividir o espaço de busca ao meio a cada pergunta |
| Busca Ternária | O(log N) | Encontrar mínimo/máximo de função unimodal |
| Eliminação progressiva | O(log N) a O(√N) | Eliminar candidatos com cada resposta |
| Construção bit a bit | O(log V) | Descobrir um valor determinando cada bit |
| Grafo/Árvore interativo | O(N log N) | Descobrir estrutura desconhecida com queries de adjacência |
| Jogo com adversário | O(N) a O(N log N) | Estratégia minimax ou adaptativa |

---

## Busca Binária Interativa

O padrão mais comum. O juiz "pensa" em um número entre 1 e N; você precisa descobri-lo em O(log N) perguntas.

```cpp
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    int lo = 1, hi = n;

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;

        cout << mid << "\n";
        cout.flush();

        string resp;
        cin >> resp;

        if (resp == "=") {
            // Encontrou — alguns problemas encerram automaticamente
            break;
        } else if (resp == "<") {
            // Alvo é menor que mid
            hi = mid - 1;
        } else { // resp == ">"
            // Alvo é maior que mid
            lo = mid + 1;
        }
    }

    // Se sobrou um único candidato e o juiz não encerrou sozinho:
    if (lo == hi) {
        cout << lo << "\n";
        cout.flush();
    }

    return 0;
}
```

**Variação: resposta "Yes"/"No"**

Às vezes a query é "o número é ≤ X?" e o juiz responde "Yes" ou "No":

```cpp
while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    cout << "? " << mid << "\n";
    cout.flush();

    string resp;
    cin >> resp;

    if (resp == "Yes") hi = mid;
    else lo = mid + 1;
}

cout << "! " << lo << "\n";
cout.flush();
```

> **Detalhe importante:** muitos problemas usam prefixos `?` para query e `!` para resposta final. Verifique o formato exato no enunciado.

---

## Descoberta Bit a Bit

Quando a resposta é um número inteiro e você pode perguntar sobre bits individuais, ou quando uma query de máscara XOR/AND revela bits.

```cpp
// Exemplo: descobrir um número X de 0 a 2^30 - 1
// Query: "? mask" → juiz responde X & mask (número de bits em comum)
// ou perguntar bit a bit

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long resultado = 0;

    for (int bit = 29; bit >= 0; bit--) {
        long long mask = (1LL << bit);

        cout << "? " << mask << "\n";
        cout.flush();

        int resp;
        cin >> resp;

        if (resp > 0) {
            resultado |= mask; // este bit está ligado
        }
    }

    cout << "! " << resultado << "\n";
    cout.flush();

    return 0;
}
```

---

## Descoberta de Grafo/Árvore

Problemas onde a estrutura do grafo é desconhecida e você a descobre via queries do tipo "qual é o vizinho X do nó Y?" ou "qual é o caminho entre A e B?".

```cpp
// Exemplo: descobrir a árvore de N nós
// Query: "? u" → juiz responde o pai de u (ou -1 se raiz)

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    vector<int> pai(n + 1, 0);

    for (int u = 2; u <= n; u++) {
        cout << "? " << u << "\n";
        cout.flush();

        cin >> pai[u];
    }

    // Agora você tem a estrutura da árvore em pai[]
    // Processe e emita a resposta final
    cout << "! " << /* resposta */ 0 << "\n";
    cout.flush();

    return 0;
}
```

---

## Juiz Adaptativo vs. Fixo

**Juiz fixo:** o valor-alvo já está determinado antes do início. Qualquer estratégia de busca binária ou eliminação funciona.

**Juiz adaptativo:** o juiz pode alterar a resposta conforme suas perguntas, desde que mantenha consistência com tudo que já foi dito. Nesses casos, você precisa pensar em **conjuntos de candidatos consistentes** e garantir que sua estratégia seja correta para o pior caso.

```
Juiz adaptativo + busca binária ainda funciona se:
  - A estratégia de dividir ao meio é ótima no pior caso
  - O juiz sempre mantém o maior conjunto de candidatos possível
  - log₂(N) queries são suficientes para eliminar todos menos um
```

> **Como identificar:** o enunciado frequentemente diz "special judge" ou "adaptive" / "the interactor may be adaptive". Fique atento a problemas onde o juiz parece "mudar de ideia" — isso é intencional.

---

## Estratégias de Contagem de Queries

Antes de implementar, calcule se sua estratégia cabe no limite:

| N | log₂(N) | √N |
|---|---|---|
| 10³ | ≈ 10 | ≈ 32 |
| 10⁵ | ≈ 17 | ≈ 316 |
| 10⁶ | ≈ 20 | ≈ 1000 |
| 10⁹ | ≈ 30 | ≈ 31623 |

**Regra prática:** se o limite for ≈ 30, pense em busca binária sobre [1, 10⁹]. Se o limite for ≈ 20 para N ≤ 10⁶, pense em alguma estrutura logarítmica. Se o limite for ≈ N/2, pense em eliminação por torneio.

---

## Testando Localmente

Como o juiz real é externo, você precisa simular a interação para testar. Duas abordagens comuns:

### Abordagem 1: Juiz manual (stdin/stdout)
Rode seu programa e faça você mesmo o papel do juiz, digitando as respostas.

### Abordagem 2: Juiz automatizado com pipe

Crie um segundo arquivo `judge.cpp` que simula o juiz e rode os dois em paralelo com pipes:

```bash
# Linux/Mac: compile os dois e conecte via pipe
mkfifo pipe_in pipe_out
./judge < pipe_in > pipe_out &
./solution < pipe_out > pipe_in
```

### Abordagem 3: Juiz embutido (para testes automáticos)

```cpp
// judge.cpp — simula o juiz para o problema "adivinhe o número"
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n = 100, alvo = 42; // valor fixo ou aleatório
    cout << n << endl;      // envia parâmetro inicial

    string linha;
    while (getline(cin, linha)) {
        if (linha[0] == '?') {
            int guess = stoi(linha.substr(2));
            if (guess == alvo) cout << "=" << endl;
            else if (guess < alvo) cout << ">" << endl;
            else cout << "<" << endl;
        } else if (linha[0] == '!') {
            int final_guess = stoi(linha.substr(2));
            if (final_guess == alvo) cerr << "CORRETO\n";
            else cerr << "ERRADO (era " << alvo << ")\n";
            break;
        }
    }
    return 0;
}
```

---

## Armadilhas Comuns

| Problema | Causa | Solução |
|---|---|---|
| TLE / deadlock | Esqueceu `flush` após a query | Sempre use `endl` ou `cout.flush()` |
| WA na resposta final | Continuou fazendo queries após encontrar a resposta | Emita `!` imediatamente ao ter certeza |
| Limite de queries excedido por 1 | Off-by-one na busca binária | Revise os invariantes de `lo` e `hi` |
| Leitura bloqueada | Leu menos valores do que o juiz enviou | Leia exatamente o que o enunciado diz |
| Resposta errada em juiz adaptativo | Estratégia não é ótima no pior caso | Use análise minimax ou estratégia de bisseção comprovada |

---

## Template Completo

```cpp
#include <bits/stdc++.h>
using namespace std;

// Helper para query — garante flush sempre
auto query = [](auto... args) {
    (cout << ... << args);
    cout << "\n";
    cout.flush();
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;

    int lo = 1, hi = n;

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        query("? ", mid);

        string resp;
        cin >> resp;

        if (resp == "Yes" || resp == ">") lo = mid + 1;
        else hi = mid;
    }

    query("! ", lo);

    return 0;
}
```

---

## Checklist Antes de Submeter

- [ ] Todo `cout` de query tem `endl` ou `cout.flush()` imediatamente após?
- [ ] Sua estratégia usa no máximo o número de queries permitido no **pior caso**?
- [ ] Você lida com a resposta de encerramento do juiz (ex: `-1`, `"="`)?
- [ ] A resposta final usa o prefixo correto (ex: `"!"`) se o problema exigir?
- [ ] Você testou com casos extremos (alvo = 1, alvo = N, N = 1)?