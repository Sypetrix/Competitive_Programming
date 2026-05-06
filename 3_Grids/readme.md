# Grids (Grades e Matrizes)

Muitos problemas de programação competitiva utilizam matrizes para representar mapas, tabuleiros de jogos ou áreas geográficas. Embora possam ser visualizados como grafos implícitos (onde cada célula é um nó e as células adjacentes são conectadas por arestas), os problemas em **Grids** possuem particularidades de implementação que permitem resoluções mais diretas e otimizadas.

### Alguns dos tópicos incluem:

- **Representação e Coordenadas:** Uso de sistemas cartesianos `(x, y)` ou matrizes `(i, j)`.
- **Navegação de Vizinhança:** Implementação eficiente de vetores de direção (`dx`, `dy`) para vizinhanças de 4 e 8 direções.
- **Caminhamento:** Varredura por linhas (*row-major*), colunas (*column-major*) e diagonais.
- **Flood Fill:** Algoritmo clássico para encontrar e marcar componentes conectados em uma grade (essencial para contar "ilhas" ou áreas isoladas).
- **Grades Especiais:** Introdução a conceitos de grades triangulares e hexagonais.

### Pré-requisitos:

Para dominar o trabalho com grids, você deve estar confortável com a manipulação de **matrizes (arrays bidimensionais)** e ter uma base sólida em **travessias (DFS/BFS)** aplicada sobre os índices da matriz.

Diferente de um grafo genérico (conteúdo a ser visto mais a frente), em uma grade quadrada regular, cada vértice interno toca exatamente 4 arestas e 4 células (ou 8, se considerarmos as diagonais). Saber explorar essa regularidade é a chave para não receber um **TLE** desnecessário.

### Dica de Estudo:

Para visualizar como algoritmos de busca se comportam especificamente em grades (como o algoritmo A* encontrando o caminho mais curto entre dois pontos evitando obstáculos), recomendo o uso de ferramentas de visualização como o [VisuAlgo](https://visualgo.net/en/dfsbfs).

---
*Dica de implementação: Sempre verifique se `(ni, nj)` está dentro dos limites da matriz `[0..N-1][0..M-1]` antes de acessar a célula vizinha!*