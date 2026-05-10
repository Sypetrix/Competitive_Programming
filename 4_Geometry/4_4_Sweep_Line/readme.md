# Linha de Varredura (Sweep Line)

A Sweep Line é um paradigma de geometria computacional onde uma linha imaginária varre o plano da esquerda para a direita (ou de baixo para cima), processando **eventos** na ordem em que são encontrados. Em vez de comparar todos os pares de objetos em O(N²), mantemos uma estrutura de dados com os objetos "ativos" no momento e processamos apenas interações relevantes.

**Estrutura geral:**

```
1. Transforme o problema em uma lista de eventos (pontos notáveis no eixo x ou y)
2. Ordene os eventos pelo eixo de varredura
3. Processe cada evento, mantendo uma estrutura com os objetos ativos
4. A estrutura ativa permite responder queries em O(log N) por evento
```

---

## Visão Geral

| Problema | Técnica | Complexidade |
|---|---|---|
| Interseção de N segmentos (existe alguma?) | Sweep + BST | O(N log N) |
| Área de união de retângulos | Sweep + Segment Tree | O(N log N) |
| Perímetro de união de retângulos | Sweep + Segment Tree | O(N log N) |
| Par de pontos mais próximo | Sweep + BST | O(N log N) |
| Coordenada comprimida | Ordenar + mapear | O(N log N) |

---

## Compressão de Coordenadas

Pré-requisito frequente: quando as coordenadas são grandes (até 10⁹) mas N é pequeno (até 10⁵), comprimimos para índices contíguos.

```cpp
vector<int> comprimir(vector<int> vals) {
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    return vals;
}

// Uso: idx = lower_bound(vals.begin(), vals.end(), x) - vals.begin()
// vals[idx] == x  (índice comprimido)
```

---

## Área de União de Retângulos — O(N log N)

**Problema:** dados N retângulos com lados paralelos aos eixos, calcular a área da sua união.

**Ideia:** varrer da esquerda para a direita. Cada retângulo gera dois eventos:
- Borda esquerda (x = x₁): adiciona o intervalo [y₁, y₂] como "ativo"
- Borda direita (x = x₂): remove o intervalo [y₁, y₂]

Entre eventos consecutivos xₐ e x_b, a contribuição é `(x_b − xₐ) × comprimento_coberto`.

O "comprimento coberto" é o tamanho da **união** dos intervalos ativos no eixo Y — mantido com uma Segment Tree com lazy (contador de cobertura).

```cpp
struct SegTree {
    int n;
    vector<int> cnt;   // quantas vezes o intervalo está coberto
    vector<long long> len; // comprimento coberto

    SegTree(int n) : n(n), cnt(4*n, 0), len(4*n, 0) {}

    void update(int node, int s, int e, int l, int r, int val) {
        if (r <= s || e <= l) return;
        if (l <= s && e <= r) { cnt[node] += val; }
        else {
            int mid = (s + e) / 2;
            update(2*node, s, mid, l, r, val);
            update(2*node+1, mid, e, l, r, val);
        }
        if (cnt[node] > 0) len[node] = e - s;
        else if (s + 1 == e) len[node] = 0; // folha sem cobertura
        else len[node] = len[2*node] + len[2*node+1];
    }

    long long query() { return len[1]; } // comprimento total coberto
};

long long areaUniao(vector<array<int,4>>& rects) {
    // rect = {x1, y1, x2, y2}
    // Comprime coordenadas Y
    vector<int> ys;
    for (auto& r : rects) { ys.push_back(r[1]); ys.push_back(r[3]); }
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    int M = ys.size();

    // Cria eventos
    vector<array<int,4>> events; // {x, tipo (+1 ou -1), y1_idx, y2_idx}
    for (auto& r : rects) {
        int y1 = lower_bound(ys.begin(), ys.end(), r[1]) - ys.begin();
        int y2 = lower_bound(ys.begin(), ys.end(), r[3]) - ys.begin();
        events.push_back({r[0],  1, y1, y2}); // borda esquerda
        events.push_back({r[2], -1, y1, y2}); // borda direita
    }
    sort(events.begin(), events.end());

    SegTree seg(M);
    long long area = 0;
    int prev_x = 0;

    for (int i = 0; i < (int)events.size(); i++) {
        int x = events[i][0];
        if (i > 0) area += (long long)(x - prev_x) * seg.query();
        seg.update(1, 0, M-1, events[i][2], events[i][3], events[i][1]);
        prev_x = x;
    }
    return area;
}
```

---

## Perímetro de União de Retângulos — O(N log N)

Semelhante à área, mas também precisamos rastrear o **número de segmentos ativos** (intervalos disjuntos) para calcular as bordas horizontais.

A contribuição horizontal em cada evento é `|comprimento_coberto_depois − comprimento_coberto_antes|`.
A contribuição vertical de cada aresta é `comprimento_coberto × 1` (apenas a borda lateral).

---

## Interseção de Segmentos — Algoritmo de Bentley-Ottmann — O((N + K) log N)

**Problema:** dados N segmentos, encontrar todos os K pontos de interseção.

**Ideia:** mantém os segmentos ativos em uma BST ordenada por y na posição atual da linha de varredura. Em cada evento:
- **Início de segmento:** insere na BST e verifica interseção com vizinhos.
- **Fim de segmento:** remove da BST e verifica se os ex-vizinhos se intersectam.
- **Interseção:** troca a ordem dos dois segmentos e verifica novos vizinhos.

> A implementação completa de Bentley-Ottmann é complexa. Para verificar apenas se **existe alguma** interseção (sem encontrar todas), uma versão simplificada é suficiente.

**Versão simplificada — detectar se há alguma interseção — O(N log N):**

```cpp
// Usa BST (std::set) com segmentos ordenados pelo y atual
// Ao inserir um segmento, verifica com predecessor e sucessor
// Ao remover, verifica se predecessor e sucessor se intersectam

bool temIntersecao(vector<pair<Point,Point>>& segs) {
    // Eventos: {x, tipo, índice}
    // tipo: 0 = início, 1 = interseção, 2 = fim
    // ... (implementação completa no arquivo correspondente)
    // Para N ≤ 10^4, checar todos os pares O(N²) é mais prático em competições
    for (int i = 0; i < (int)segs.size(); i++)
        for (int j = i+1; j < (int)segs.size(); j++)
            if (segmentsIntersect(segs[i].first, segs[i].second,
                                  segs[j].first, segs[j].second))
                return true;
    return false;
}
```

> Em competições, Bentley-Ottmann raramente é cobrado na íntegra. O problema mais comum é **área/perímetro de união de retângulos**, que tem implementação muito mais limpa.

---

## Par de Pontos Mais Próximo — O(N log N)

**Ideia:** varre da esquerda para a direita mantendo um conjunto de pontos "candidatos" dentro de uma faixa de largura `2d` (onde `d` é a menor distância vista até agora). A estrutura é um `std::set` ordenado por y.

```cpp
typedef pair<ld,ld> pdd;

ld parMaisProximo(vector<Point> pts) {
    sort(pts.begin(), pts.end()); // ordena por x
    set<pdd> ativos; // {y, x}
    ld d = 1e18;
    int l = 0;

    for (int r = 0; r < (int)pts.size(); r++) {
        // Remove pontos fora da faixa
        while (l < r && pts[r].x - pts[l].x >= d) {
            ativos.erase({pts[l].y, pts[l].x});
            l++;
        }

        // Busca candidatos com y próximo
        auto it = ativos.lower_bound({pts[r].y - d, -1e18});
        while (it != ativos.end() && it->first <= pts[r].y + d) {
            Point q = {it->second, it->first};
            d = min(d, pts[r].dist(q));
            ++it;
        }

        ativos.insert({pts[r].y, pts[r].x});
    }
    return d;
}
```

---

## Template Geral de Sweep Line

```
1. Identifique os eventos (início/fim de objeto, cruzamentos)
2. Crie pares (x_evento, tipo, dados)
3. Ordene por x (desempate pelo tipo: remoções antes de inserções, ou vice-versa)
4. Processe da esquerda para a direita:
   - Mantenha estrutura de objetos ativos (set, segment tree, fenwick)
   - A cada evento, atualize a estrutura e compute a contribuição
```

> **Armadilha de desempate:** a ordem em que eventos com o mesmo x são processados importa. Para área de união de retângulos, processe inserções antes de remoções (ou vice-versa, mas seja consistente). Para par mais próximo, o sentido de desempate pode afetar qual ponto é processado.