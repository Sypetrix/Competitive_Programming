# STL (Estrutra de Dados)

Uma Estrutura de Dados (ED) é um meio de armazenar e organizar dados. Diferentes estruturas possuem diferentes forças e fraquezas. Ao projetar um algoritmo para competições, é crucial escolher uma que permita inserções, buscas e atualizações eficientes, dependendo da necessidade do problema.

*Embora a estrutura de dados não resolva o problema sozinha (o algoritmo que opera sobre ela sim), a escolha da ED correta é a diferença entre receber um Accepted ou um Time Limit Exceeded (TLE).*

### Na programação competitiva, você deve usar a STL porque:

- **Velocidade de Implementação:** As estruturas já estão prontas e testadas. Isso permite que você foque na lógica do problema em vez de gastar tempo implementando a estrutura do zero.
- **Confiabilidade:** As implementações da STL são altamente otimizadas e seguras contra bugs comuns de implementação manual.
- **Eficiência de Tempo:** Elas garantem as complexidades ideais (como $O(\log N)$ para mapas ou filas de prioridade) necessárias para passar nos limites de tempo.

A regra de ouro é o princípio `KISS` (`Keep It Short and Simple`): utilize sempre as ferramentas prontas da linguagem. Muitos competidores usam `std::priority_queue` sem precisar implementar um Heap manualmente, ou `std::map` sem precisar construir uma Árvore Rubro-Negra. Para a maioria dos problemas, o uso correto da biblioteca é suficiente.

### O que você deve dominar:

- **Complexidade de Tempo/Espaço:** Entender o custo de cada operação (ex: busca em um `vector` é $O(N)$, mas em um `set` é $O(\log N)$).
- **Escolha da Estrutura:** Saber qual ED se encaixa nos requisitos de busca, inserção e ordenação do problema.
- **Limitações:** Entender quando a STL não é suficiente e você precisa de algo mais específico (como uma *Segment Tree*).

### Divisão do conteúdo:

- **Estruturas Lineares:** Manipulação básica com `std::vector`, `std::stack`, `std::queue` e tópicos especiais como **Bitmask** e **Big Integer**.
- **Estruturas Não-Lineares:** Uso de `std::priority_queue` (Heaps), `std::unordered_map` (Hash Tables) e `std::map/set` (Árvores de Busca Binária).
- **Utilidades:** Casos onde a STL não possui implementação pronta e você deve construir sua própria biblioteca (ex: *Union-Find Disjoint Sets*, *Fenwick Tree*).

*Lembre-se: O objetivo em uma competição é resolver o problema de forma rápida e correta. Conhecer bem a sua biblioteca de ferramentas é meio caminho andado para o sucesso.*