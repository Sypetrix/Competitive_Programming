# Paradigmas de Resolução de Problemas

Um **paradigma** é uma estratégia geral de ataque a um problema. Diferente de algoritmos específicos (Dijkstra, KMP, etc.), paradigmas são moldes mentais reutilizáveis: dado o formato do problema, você reconhece qual estratégia aplicar e depois adapta os detalhes.

Dominar paradigmas é o que separa competidores que resolvem 3 problemas dos que resolvem 6.

---

## Os Paradigmas

| Seção | Estratégia central | Complexidade típica |
|---|---|---|
| **Complete Search** | Explorar todo (ou parte) do espaço de busca | O(2ᴺ), O(N!), O(3ᴺ/²) com otimizações |
| **Pointer Techniques** | Dois ponteiros movendo-se pelo array/string | O(N) ou O(N log N) |
| **Greedy** | Escolha localmente ótima a cada passo | O(N log N) geralmente |
| **Divide and Conquer** | Dividir o problema, resolver partes, combinar | O(N log N) geralmente |
| **Dynamic Programming** | Memorizar subproblemas sobrepostos | O(N·K) geralmente |

---

## Como Identificar o Paradigma Certo

```
O espaço de busca é pequeno (N ≤ 20)?
└── Complete Search (força bruta, backtracking, meet in the middle)

O array/string tem propriedade monotônica?
├── Sim, janela de tamanho variável → Two Pointers
└── Sim, janela de tamanho fixo → Sliding Window

Existe uma escolha "gananciosa" provável?
└── Greedy (verifique se a escolha local garante o ótimo global)

O problema se divide em subproblemas independentes?
└── Divide and Conquer (merge sort, busca binária, D&C clássico)

O problema tem subproblemas sobrepostos e subestrutura ótima?
└── Dynamic Programming

Não se encaixa em nenhum?
└── Volte ao Complete Search — uma solução correta lenta
    pode ser otimizada depois, ou ser suficiente para o N dado
```

---

## Princípio KISS e a Ordem de Tentativa

Em maratonas, tempo é recurso escasso. A ordem recomendada de tentativa:

1. **Complete Search** — sempre correto, pode dar TLE. Implemente primeiro para validar a lógica.
2. **Greedy** — se a estrutura do problema permitir.
3. **DP** — quando subproblemas sobrepostos aparecem.
4. **D&C / Busca Binária** — quando o espaço é monotônico ou divisível.
5. **Pointer Techniques** — quando há propriedade de janela.

> Um Complete Search sem bugs nunca dá Wrong Answer. Use-o para gerar casos de teste e validar soluções mais sofisticadas.

---

## Análise de Complexidade antes de Codar

Antes de implementar, estime se a solução passa no limite de tempo. Regra geral para 1 segundo:

| Complexidade | N máximo aproximado |
|---|---|
| O(N!) | N ≤ 11 |
| O(2ᴺ) | N ≤ 25 |
| O(N³) | N ≤ 500 |
| O(N² log N) | N ≤ 3.000 |
| O(N²) | N ≤ 5.000 |
| O(N log N) | N ≤ 10⁷ |
| O(N) | N ≤ 10⁸ |
| O(log N) | N ≤ 10¹⁸ |

> Processadores modernos executam ~10⁸ operações simples por segundo. Multiplique por ~3–5 para operações leves (comparação, soma), divida por ~3–5 para operações pesadas (divisão, cache miss).