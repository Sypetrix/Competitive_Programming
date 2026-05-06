# Topologias de Grids (Grids Topology)

Embora a maioria dos problemas de maratonas utilize matrizes quadradas clássicas, alguns problemas modelam o mundo utilizando formas geométricas diferentes. A topologia de um grid define como as células se conectam umas às outras. O segredo para resolver esses problemas sem enlouquecer é descobrir a forma matemática de calcular os vizinhos, transformando o grid em um grafo implícito para rodar uma DFS ou BFS tradicional.

---

## Visão Geral

| Topologia | Vizinhança Padrão | Transição / Lógica | Utilização típica |
|---|---|---|---|
| `Square` (Quadrada) | 4 ou 8 direções | Matriz `(r, c)` com vetores `dr`, `dc` | Mapas clássicos, labirintos, jogos 2D. |
| `Hexagonal` | 6 direções | Coordenadas Axiais `(q, r)` | Tabuleiros táticos, expansão espacial otimizada. |
| `Triangular` | 3 direções | Depende da paridade `(r + c) % 2` | Triangulações e pavimentações específicas. |
| `Circular` | Radial / Lateral | `(raio, setor)` com wrap-around (módulo) | Radares, alvos de dardos, anéis concêntricos. |
| `Polygon` (Irregular) | Variável | Grafo explícito (Lista de Adjacência) | Mapas geopolíticos (países/estados), regiões irregulares. |

---

## Square (Quadrada)

O grid padrão da programação competitiva. Cada célula `(r, c)` pode ter vizinhos compartilhando arestas (4 direções) ou vértices (8 direções).

```cpp
// Deslocamentos para 4 direções (N, S, O, L)
int dr4[] = {-1, 1, 0, 0};
int dc4[] = {0, 0, -1, 1};

// Deslocamentos para 8 direções (incluindo diagonais)
int dr8[] = {-1, 1, 0, 0, -1, -1, 1, 1};
int dc8[] = {0, 0, -1, 1, -1, 1, -1, 1};

// Padrão de caminhamento
for (int i = 0; i < 4; i++) {
    int nr = r + dr4[i];
    int nc = c + dc4[i];
    if (nr >= 0 && nr < R && nc >= 0 && nc < C) { /* Vizinho válido */ }
}
```

> **Use a topologia `Square` quando** o problema fornecer explicitamente uma matriz regular ou um mapa pixelizado padrão.

---

## Hexagonal

Os grids hexagonais são muito comuns em jogos de estratégia. A melhor forma de representá-los no código (para evitar índices complexos em matrizes intercaladas) é através do **Sistema de Coordenadas Axiais** (eixos $q$ e $r$).

```cpp
// 6 Direções no sistema Axial (q = coluna inclinada, r = linha)
int dq[] = { 1,  1,  0, -1, -1,  0};
int dr[] = { 0, -1, -1,  0,  1,  1};

// A verificação de limites em um hexágono depende do formato global do tabuleiro
for (int i = 0; i < 6; i++) {
    int nq = q + dq[i];
    int nr = r + dr[i];
    // if (limite_valido(nq, nr)) { ... }
}
```

> **Use a topologia `Hexagonal` quando** cada célula tocar exatamente 6 vizinhos por aresta e a distância radial for perfeitamente uniforme.

---

## Triangular

Em um grid de triângulos regulares, cada célula tem 3 vizinhos que compartilham arestas. A grande "armadilha" computacional é que triângulos adjacentes apontam para direções opostas (um para cima, outro para baixo), o que altera a posição do vizinho vertical.

```cpp
// Vizinhos horizontais (sempre existem na mesma linha)
int dr_horiz[] = {0, 0};
int dc_horiz[] = {-1, 1};

// O vizinho vertical depende se o triângulo aponta para cima ou para baixo
int nr_vert = r;
if ((r + c) % 2 == 0) {
    nr_vert += 1; // Aponta para cima, logo a base (vizinho) está embaixo
} else {
    nr_vert -= 1; // Aponta para baixo, logo a base (vizinho) está em cima
}
```

> **Use a topologia `Triangular` quando** o espaço for particionado com 3 conexões adjacentes e a paridade da soma dos eixos ditar o formato geométrico da célula.

---

## Circular

Uma topologia polar (como uma fatia de pizza ou um alvo de dardos). É caracterizada pela distância ao centro (`raio`) e pelo ângulo/fatia (`setor`). Possui anomalias importantes: o movimento lateral dá a volta (*wrap-around*) e o centro é um polo singular.

```cpp
// 1. Vizinhança lateral (wrap-around com módulo)
int esq = (setor - 1 + MAX_SETORES) % MAX_SETORES;
int dir = (setor + 1) % MAX_SETORES;

// 2. O caso extremo do Centro absoluto
if (raio == 0) {
    // O centro não tem "esquerda/direita", ele toca TODOS os setores do raio 1!
    for (int s = 0; s < MAX_SETORES; s++) { /* Vizinho: (1, s) */ }
}

// 3. Vizinho de dentro (se raio == 1, todos convergem para o centro (0,0))
int raio_dentro = raio - 1;
```

> **Use a topologia `Circular` quando** o espaço for cilíndrico ou esférico e houver características de ciclo contínuo em um dos eixos (como os graus de um radar que voltam a 0 após 359).

---

## Polygon (Irregular)

Quando o mapa perde a regularidade geométrica (como o mapa de países ou continentes em um jogo de *War*), abandonamos as coordenadas e tratamos cada polígono como um nó genérico em um grafo explícito.

```cpp
const int MAX_POLIGONOS = 1e5 + 5;
vector<int> adj[MAX_POLIGONOS]; // Lista de adjacência

// Conectar fronteiras (Polígonos compartilhando fronteira comum)
void adicionar_fronteira(int poligono_A, int poligono_B) {
    adj[poligono_A].push_back(poligono_B);
    adj[poligono_B].push_back(poligono_A);
}

// O caminhamento torna-se uma BFS/DFS tradicional
for (int vizinho : adj[u]) { /* Processar região adjacente */ }
```

> **Use a topologia `Polygon` quando** não houver uma fórmula puramente matemática para deduzir as adjacências a partir da posição da célula, exigindo a pré-construção de um grafo.