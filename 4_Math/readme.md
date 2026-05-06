# Math (Matemática Computacional)

Em maratonas de programação, problemas de matemática e geometria são conhecidos por serem os grandes "divisores de águas". Eles raramente perdoam simulações ineficientes ou "chutes": ou você conhece a propriedade/fórmula, ou provavelmente receberá um *Time Limit Exceeded* (TLE) por lentidão, ou *Wrong Answer* (WA) por precisão.

Este diretório guarda as implementações dos teoremas matemáticos, sequências numéricas e primitivas geométricas mais cobrados, todos otimizados para evitar gargalos computacionais.

### Principais Tópicos:

- **Teoria dos Números:** Testes de primalidade, Crivo de Eratóstenes otimizado, fatoração em primos e Algoritmo de Euclides (MDC/MMC).
- **Aritmética Modular:** Regras de adição/multiplicação sob módulo, Exponenciação Modular Rápida ($O(\log N)$) e Inverso Modular.
- **Combinatória:** Cálculo de Coeficientes Binomiais ($nCr$), Princípio da Inclusão-Exclusão e geração de sequências famosas (Fibonacci e Catalan).
- **Primitivas Geométricas:** Estruturas seguras para manipulação de Pontos e Vetores (evitando números de ponto flutuante sempre que possível).
- **Produto Vetorial (Cross Product):** A ferramenta suprema da geometria computacional para definir Orientação (CCW), intersecção de segmentos e cálculo de áreas (Fórmula do Cadarço), sem depender de trigonometria (senos/cossenos).
- **Polígonos:** Algoritmos como Ponto em Polígono e a construção do Fecho Convexo (Convex Hull) utilizando o clássico *Graham Scan* em $O(N \log N)$.

### 🛠️ Cuidados e Pré-requisitos:

As submissões dessa área exigem atenção dobrada aos detalhes da linguagem C++:

- **Overflow Múltiplo:** Ao multiplicar dois números da ordem de $10^9$, o resultado chega a $10^{18}$, ultrapassando o limite do `int` normal. Use sempre `long long` para produtos vetoriais e cálculos modulares.
- **Precisão de Ponto Flutuante (O problema do Epsilon):** Quando o uso de `double` for inevitável (como para calcular distâncias usando raízes), NUNCA compare igualdade de forma direta (`a == b`). Crie uma constante de tolerância `EPS = 1e-9` e verifique a diferença absoluta (`abs(a - b) < EPS`).
- **Módulo com Números Negativos:** Em C++, a operação `-5 % 3` resulta em `-2`. O correto na matemática modular para competições é usar `((a % m) + m) % m` para garantir que o resultado seja sempre positivo.

### Dicas de Estudo:

- **Para Matemática:** Sempre que encontrar um problema de contagem que pareça bizarro, calcule na mão as respostas para $N=1, 2, 3, 4$ e pesquise a sequência no [OEIS (Online Encyclopedia of Integer Sequences)](https://oeis.org).
- **Para Geometria:** Nunca tente deduzir um caso visual complexo de cabeça. **Desenhe os casos extremos no papel** (pontos colineares, segmentos sobrepostos, triângulos degenerados) antes de codar!