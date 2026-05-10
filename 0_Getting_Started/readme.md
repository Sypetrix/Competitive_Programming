# Introdução (Introduction)

Antes de resolver qualquer problema de programação competitiva, é preciso dominar três pilares que sustentam todas as decisões durante uma competição: saber **estimar** se uma solução caberá no tempo limite, saber **ler e escrever** dados eficientemente, e saber **escrever código rapidamente** sem cometer erros bobos. Este capítulo cobre exatamente isso.

---

## Visão Geral

| Seção | Utilização Típica |
|---|---|
| `Time Complexity` | Estimar se uma solução passa no tempo antes de implementá-la. |
| `I/O` | Evitar TLE causado por entrada e saída lentas em C++ e Python. |
| `Templates / Shortening Code` | Escrever código mais rápido e com menos chance de erro durante a prova. |

---

## O que é Programação Competitiva?

Em uma competição de programação (como ICPC, IOI, Codeforces ou Beecrowd), você recebe um conjunto de problemas e deve resolvê-los dentro de um limite de tempo — tanto da **prova** (tipicamente 3 a 5 horas) quanto da **máquina** (geralmente 1 a 3 segundos por submissão).

Cada problema tem uma entrada bem definida, uma saída esperada e restrições explícitas sobre o tamanho dos dados (ex: $N \leq 10^5$). Sua solução precisa ser **correta** e **eficiente** ao mesmo tempo.

Os possíveis vereditos de uma submissão são:

| Veredito | Significado |
|---|---|
| **AC** — Accepted | Solução correta e dentro do tempo. |
| **WA** — Wrong Answer | Saída incorreta em algum caso de teste. |
| **TLE** — Time Limit Exceeded | Solução correta mas lenta demais. |
| **MLE** — Memory Limit Exceeded | Solução usou mais memória do que o permitido. |
| **RE** — Runtime Error | Crash durante a execução (acesso fora de array, divisão por zero, etc). |
| **CE** — Compilation Error | Código não compilou. |

---

## Fluxo de Resolução

Para cada problema, o processo ideal é:

1. **Leia o problema inteiro** antes de começar a codar — preste atenção nas restrições ($N$, limites de valor, tempo).
2. **Estime a complexidade** necessária com base nas restrições (ver Time Complexity).
3. **Pense antes de codar** — para a maioria dos problemas, o tempo de implementação é menor que o tempo de pensar na solução certa.
4. **Implemente de forma limpa** usando seu template e os recursos da STL.
5. **Teste com os exemplos** do enunciado e com casos extremos (N=0, N=1, N máximo).
6. **Submeta** — se WA, pense em casos de borda; se TLE, repense a complexidade.

> **Princípio KISS (Keep It Short and Simple):** prefira sempre a solução mais simples que resolve o problema dentro do tempo. Código complexo é mais difícil de debugar durante a prova.