# Matemática (Math)

Na Programação Competitiva, a matemática não é apenas sobre fórmulas; é sobre **reduzir a complexidade**. Um problema que parece exigir uma busca exaustiva de $O(N)$ pode muitas vezes ser resolvido em $O(1)$ ou $O(\log N)$ com a observação matemática correta. 

Este capítulo cobre os pilares matemáticos que sustentam algoritmos de criptografia, contagem e otimização em maratonas.

---

## Organização da Pasta

| Subpasta | Principais Tópicos |
|---|---|
| **Number Theory** | Primalidade, Crivo de Eratóstenes, MDC/MMC, Fatoração e Busca de Ciclos. |
| **Modular Arithmetic** | Operações sob um módulo, Exponenciação Rápida e Inverso Modular. |
| **Combinatorics** | Regras de Contagem, Coeficientes Binomiais, Números de Catalan e Fibonacci. |
| **Probability** | Valor Esperado (Linearidade), Probabilidade Condicional e Eventos Independentes. |
| **Linear Algebra** | Exponenciação de Matrizes (para recorrências lineares) e Sistemas Lineares. |

---

## Conceitos Fundamentais e Limites

### Aritmética Computacional
Diferente da matemática teórica, no computador temos limites físicos:
- **`int` (32 bits):** Vai até aproximadamente $2 \cdot 10^9$. Útil para contagens simples.
- **`long long` (64 bits):** Vai até aproximadamente $9 \cdot 10^{18}$. Obrigatório para produtos de números grandes.
- **Overflow:** Se você multiplicar dois números da ordem de $10^9$, o resultado excede 32 bits. **Sempre use `long long` nesses casos.**

### Tabela Geral de Complexidades

| Operação / Algoritmo | Complexidade | N máximo (1s) |
|---|---|---|
| MDC (Euclides) | $O(\log (\min(a, b)))$ | $10^{18}$ |
| Exponenciação Modular | $O(\log \text{exp})$ | $10^{18}$ |
| Crivo de Eratóstenes | $O(N \log \log N)$ | $10^7$ |
| Fatoração Ingênua | $O(\sqrt{N})$ | $10^{14}$ |
| Coeficiente Binomial (DP) | $O(N \cdot K)$ | $5000 \times 5000$ |
| N-ésimo Fibonacci (Matriz)| $O(\log N \cdot \text{dim}^3)$ | $10^{18}$ |

---

## Como Abordar um Problema Matemático

    O problema pede para contar algo?
    ├── Sim → É Combinatória.
    │   ├── Os subproblemas se sobrepõem? → DP / Triângulo de Pascal.
    │   ├── Envolve parênteses ou caminhos em grade? → Números de Catalan.
    │   └── N é muito grande? → Procure uma fórmula fechada.
    └── Não
        ├── Envolve divisibilidade ou primos? → Number Theory.
        │   ├── Preciso de muitos primos? → Crivo de Eratóstenes.
        │   └── Preciso de um fator específico? → Fatoração em O(√N).
        ├── O resultado deve ser % M? → Modular Arithmetic.
        │   └── Preciso de divisão sob módulo? → Inverso Modular (Fermat).
        └── É uma recorrência linear (ex: N-ésimo termo)?
            └── N é gigante (10^18)? → Matrix Exponentiation.

---

## Dicas de Ouro (Pro-Tips)

1. **Aritmética Modular:** Sempre que o problema pedir "resultado módulo $M$", aplique o módulo em **todas** as operações intermediárias:
   - `(a + b) % M`
   - `(a - b + M) % M` (Sempre some $M$ antes do módulo em subtrações para evitar resultados negativos).
   - `(1LL * a * b) % M` (Cuidado com o cast para `long long` no produto).

2. **Precisão de Ponto Flutuante:** Evite `double` se puder resolver com inteiros (ex: trabalhar com frações irredutíveis). Se for inevitável usar `double`, use uma constante `EPS = 1e-9` para comparações: `if (abs(a - b) < EPS)`.

3. **Inclusão-Exclusão:** Se o problema pede para contar elementos que satisfazem "pelo menos uma" de várias condições, o Princípio de Inclusão-Exclusão é sua melhor aposta.

4. **BigInt:** O C++ não tem suporte nativo para números que ultrapassam $10^{18}$. Se o problema exigir, considere usar **Java (BigInteger)** ou **Python**, ou implemente/cole uma estrutura de BigInt customizada se a linguagem for restrita.