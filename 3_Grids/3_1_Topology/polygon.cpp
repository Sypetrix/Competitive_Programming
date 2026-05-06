#include <iostream>
#include <vector>
using namespace std;

const int MAX_POLIGONOS = 1e5 + 5;
vector<int> adj[MAX_POLIGONOS]; // Cada índice é um polígono/estado

// O grid poligonal é montado lendo a entrada do problema e conectando os vizinhos
void adicionar_fronteira(int poligono_A, int poligono_B) {
    adj[poligono_A].push_back(poligono_B);
    adj[poligono_B].push_back(poligono_A);
}

// A travessia se torna uma DFS ou BFS tradicional
bool visited[MAX_POLIGONOS];

void percorrer_poligonal(int u) {
    visited[u] = true;
    for (int vizinho : adj[u]) {
        if (!visited[vizinho]) {
            // Processar polígono vizinho
            percorrer_poligonal(vizinho);
        }
    }
}