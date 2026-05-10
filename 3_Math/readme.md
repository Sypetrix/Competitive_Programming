# Matemática (Math)

Na Programação Competitiva, a matemática não é apenas sobre fórmulas — é sobre **reduzir a complexidade**. Um problema que parece exigir O(N) pode ser resolvido em O(1) ou O(log N) com a observação matemática correta.

---

## Organização da Pasta

| Subpasta | Principais Tópicos |
|---|---|
| **Number Theory** | Primalidade, Crivo de Eratóstenes, MDC/MMC, Fatoração, Divisores, Euler φ, Legendre, Ciclos de Floyd |
| **Modular Arithmetic** | Operações mod, Exponenciação Rápida, Inverso Modular, Fermat/Euler, CRT |
| **Combinatorics** | Regras de contagem, Coeficientes Binomiais, Catalan, Fibonacci, Inclusão-Exclusão, Desarranjos |
| **Probability** | Valor Esperado, Linearidade, Probabilidade Condicional, Eventos Independentes, DP probabilístico |
| **Linear Algebra** | Exponenciação de Matrizes, Eliminação Gaussiana, Sistemas Lineares mod p |

---

## Tabela Geral de Complexidades

| Operação / Algoritmo | Complexidade | N máximo (1s) |
|---|---|---|
| MDC (Euclides) | O(log min(a,b)) | 10¹⁸ |
| Exponenciação Modular | O(log exp) | 10¹⁸ |
| Inverso Modular (Fermat) | O(log MOD) | 10¹⁸ |
| Crivo de Eratóstenes | O(N log log N) | 10⁷ |
| Fatoração ingênua | O(√N) | 10¹⁴ |
| Coeficiente Binomial (pré-computado) | O(N) prep + O(1) query | 10⁶ |
| Coeficiente Binomial (Pascal) | O(N²) | 5 · 10³ |
| N-ésimo Fibonacci (matriz) | O(log N) | 10¹⁸ |
| Recorrência linear (matriz) | O(M³ log N) | N = 10¹⁸, M ≤ 50 |
| Eliminação Gaussiana | O(N² M) | N, M ≤ 500 |

---

## Como Abordar um Problema Matemático

```
O problema pede para contar algo?
├── Sim → Combinatória
│   ├── N é gigante (10^18)? → fórmula fechada + inverso modular
│   ├── Subproblemas se sobrepõem? → DP / Triângulo de Pascal
│   └── Envolve parênteses, árvores, caminhos? → Números de Catalan
└── Não
    ├── Envolve divisibilidade, primos ou fatoriais?
    │   ├── Muitos primos? → Crivo de Eratóstenes
    │   └── Fator específico? → Fatoração em O(√N)
    ├── Resultado deve ser % M? → Aritmética Modular
    │   └── Precisa de divisão mod? → Inverso Modular (Fermat)
    ├── É uma recorrência linear?
    │   └── N até 10^18? → Exponenciação de Matrizes
    └── Envolve probabilidades ou valor esperado?
        └── Probabilidade Condicional / DP probabilístico
```

---

## Dicas de Ouro

**Aritmética Modular:** aplique o módulo em **todas** as operações intermediárias:
```cpp
(a + b) % MOD
(a - b + MOD) % MOD   // sempre some MOD antes para evitar negativo
(1LL * a * b) % MOD   // cast para long long antes de multiplicar
```

**Precisão de ponto flutuante:** evite `double` se puder resolver com inteiros. Se for inevitável, use `long double` e compare com `EPS = 1e-9`.

**Overflow:** ao multiplicar dois valores da ordem de 10⁹, use `long long`. Para produtos mod p, use `__int128` se necessário.

**BigInt:** C++ não tem suporte nativo para números > 10¹⁸. Considere Python, Java `BigInteger`, ou a estrutura `BigInt` customizada deste repositório.