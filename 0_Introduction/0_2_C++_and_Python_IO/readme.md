# Entrada e Saída (Input / Output)

Em competições, uma solução correta pode receber **TLE** simplesmente por usar I/O lento. Com $N = 10^6$ linhas de entrada, a diferença entre `cin` sem otimização e `scanf` pode ser de 3 a 5 segundos. Este guia cobre as configurações de I/O corretas para C++ e Python, os padrões de leitura mais comuns e as armadilhas clássicas.

---

## Visão Geral

| Técnica | Linguagem | Quando usar |
|---|---|---|
| `sync_with_stdio(false)` + `cin.tie(NULL)` | C++ | Sempre — habilitar no início de todo código |
| `scanf` / `printf` | C++ | Alternativa ao cin quando há mistura com C |
| `sys.stdin` / `sys.stdout` | Python | Entradas grandes; substitui `input()` |
| Leitura até EOF | C++ e Python | Quando o número de casos não é fornecido |
| `getline` | C++ | Leitura de linhas com espaços |

---

## C++ — I/O Rápido

Por padrão, `cin`/`cout` em C++ sincronizam com `scanf`/`printf` do C, adicionando overhead desnecessário. As duas linhas abaixo eliminam essa sincronização e desacoplam `cin` de `cout`, tornando o I/O até 5x mais rápido:

```cpp
ios_base::sync_with_stdio(false);
cin.tie(NULL);
```

> **Regra de ouro:** essas duas linhas devem ser a **primeira coisa dentro da `main()`** em todo código de competição em C++. Após habilitá-las, **não misture** `cin`/`cout` com `scanf`/`printf` no mesmo programa — o comportamento se torna indefinido.

---

## C++ — Padrões de Leitura

**Número conhecido de casos:**
```cpp
int n;
cin >> n;
while (n--) {
    // processa caso
}
```

**Leitura até EOF (número de casos desconhecido):**
```cpp
int x;
while (cin >> x) {
    // processa x
}
```

**Linha inteira com espaços:**
```cpp
// ARMADILHA: cin >> n deixa '\n' no buffer
int n;
cin >> n;
cin.ignore();          // consome o '\n' restante
string linha;
getline(cin, linha);   // lê a linha seguinte corretamente
```

**Múltiplos valores por linha:**
```cpp
int a, b, c;
cin >> a >> b >> c;    // separa por qualquer espaço/tab/newline
```

**Saída formatada:**
```cpp
// Casas decimais fixas
cout << fixed << setprecision(6) << 3.14159 << "\n";

// Use "\n" no lugar de endl — endl força flush e é ~10x mais lento
cout << "resposta: " << x << "\n";   // correto
cout << "resposta: " << x << endl;  // lento — evitar em loops
```

---

## C++ — `scanf` / `printf`

Úteis quando a compatibilidade com código C é necessária ou em casos onde a formatação de `printf` é mais conveniente:

```cpp
int n;
scanf("%d", &n);

long long x;
scanf("%lld", &x);        // long long usa %lld

double d;
scanf("%lf", &d);         // double usa %lf no scanf

char s[100005];
scanf("%s", s);           // lê palavra (para em espaço/newline)

printf("%.6f\n", d);      // 6 casas decimais
printf("%d %d\n", a, b);
```

---

## Python — I/O Padrão

O `input()` do Python é lento para grandes volumes de dados. Substitua por `sys.stdin`:

```python
import sys
input = sys.stdin.readline   # substitui input() globalmente — mais rápido

# Leitura de inteiro
n = int(input())

# Leitura de múltiplos inteiros na mesma linha
a, b, c = map(int, input().split())

# Leitura de lista de inteiros
arr = list(map(int, input().split()))

# Leitura até EOF
import sys
for linha in sys.stdin:
    x = int(linha.strip())
    # processa x
```

**Saída rápida em Python:**
```python
import sys
# Use sys.stdout.write para saídas grandes — mais rápido que print()
sys.stdout.write(str(resultado) + "\n")

# Ou acumule tudo numa lista e imprima de uma vez ao final
saida = []
saida.append(str(resultado))
print("\n".join(saida))
```

> **Atenção com `input().split()`:** em Python, `input()` já remove o `\n` do final. Não é necessário `.strip()` nesse caso — mas é necessário ao usar `sys.stdin.readline` diretamente.

---

## Casos Especiais Frequentes

**Múltiplos casos de teste com separador em branco:**
```cpp
int t;
cin >> t;
while (t--) {
    // processa caso
    cout << "\n"; // linha em branco entre casos (se exigido)
}
```

**Leitura de matriz:**
```cpp
int n, m;
cin >> n >> m;
vector<vector<int>> mat(n, vector<int>(m));
for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
        cin >> mat[i][j];
```

**Leitura de grafo (lista de arestas):**
```cpp
int n, m;
cin >> n >> m;
vector<pair<int,int>> arestas(m);
for (auto& [u, v] : arestas)
    cin >> u >> v;
```

**Python — leitura de matriz:**
```python
n, m = map(int, input().split())
mat = [list(map(int, input().split())) for _ in range(n)]
```