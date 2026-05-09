# Encurtamento de Código (Shortening Code)

Em uma competição com limite de 5 horas e 10 problemas, cada minuto conta. Usar um template bem preparado elimina o trabalho repetitivo de escrever includes, typedefs e configurações de I/O a cada problema, reduz erros de digitação e permite focar no que importa: a lógica da solução.

---

## Por que usar templates?

- **Velocidade:** evita reescrever o boilerplate (includes, fast I/O, typedefs) a cada problema.
- **Consistência:** você sempre trabalha com os mesmos atalhos, reduzindo erros.
- **Confiança:** um template testado em centenas de problemas raramente esconde bugs.

> **Atenção:** templates excessivamente complexos podem dificultar a leitura do código e esconder bugs. O ideal é um template enxuto que cubra apenas o que você usa frequentemente.

---

## Template Base (C++)

```cpp
#include <bits/stdc++.h>
using namespace std;

// ─── Tipos ────────────────────────────────────────────────────────────────────
typedef long long           ll;
typedef long double         ld;
typedef pair<int, int>      pii;
typedef pair<ll, ll>        pll;
typedef vector<int>         vi;
typedef vector<ll>          vll;
typedef vector<pii>         vpii;

// ─── Constantes ───────────────────────────────────────────────────────────────
const int  INF  = 1e9;
const ll   LINF = 1e18;
const ld   EPS  = 1e-9;
const int  MOD  = 1e9 + 7;

// ─── Macros úteis ─────────────────────────────────────────────────────────────
#define all(v)      (v).begin(), (v).end()
#define rall(v)     (v).rbegin(), (v).rend()
#define pb          push_back
#define fi          first
#define se          second
#define sz(v)       (int)(v).size()

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // seu código aqui

    return 0;
}
```

---

## Typedefs — Por que usá-los?

`long long` é o tipo mais usado em competições (evita overflow do `int`). Escrever `ll` em vez de `long long` economiza tempo e reduz erros:

```cpp
// Sem typedef — verboso e propenso a esquecer o 'long long'
long long soma = 0;
for (long long i = 0; i < n; i++) soma += arr[i];

// Com typedef — limpo e rápido de escrever
ll soma = 0;
for (ll i = 0; i < n; i++) soma += arr[i];
```

> **Cuidado com overflow:** a causa mais comum de WA silencioso em C++ é usar `int` onde deveria ser `ll`. Se $N \leq 10^5$ e você multiplica dois valores de $N$, o resultado pode chegar a $10^{10}$ — que estoura o `int` (limite ~$2.1 \times 10^9$) mas cabe num `ll` (limite ~$9.2 \times 10^{18}$).

---

## Macros — Uso e Moderação

```cpp
#define all(v)  (v).begin(), (v).end()

// Permite escrever:
sort(all(v));
reverse(all(v));
int mn = *min_element(all(v));
```

```cpp
#define pb push_back
v.pb(42);  // equivalente a v.push_back(42)
```

```cpp
#define sz(v) (int)(v).size()
// Evita o warning de comparação entre int e size_t (unsigned):
for (int i = 0; i < sz(v); i++) { ... }  // correto
for (int i = 0; i < v.size(); i++) { ... } // warning em alguns compiladores
```

> **Moderação:** macros escondem o que o código realmente faz e dificultam a depuração. Use apenas atalhos que você memorizou completamente. Macros complexas (como `#define rep(i,a,b) for(int i=a;i<b;i++)`) podem confundir na hora do stress.

---

## Constantes Importantes

```cpp
const int  INF  = 1e9;      // "infinito" para int — seguro em somas de até 2 valores
const ll   LINF = 1e18;     // "infinito" para long long
const ld   EPS  = 1e-9;     // tolerância para comparação de floats
const int  MOD  = 1e9 + 7;  // módulo primo padrão das competições
```

**Por que `1e9 + 7`?** É o primo mais próximo de $10^9$ que ainda permite multiplicar dois valores menores que ele sem overflow em `long long`: $(10^9)^2 = 10^{18} < 9.2 \times 10^{18}$.

---

## Pragma de Otimização (uso avançado)

Em alguns juízes, pragmas de compilador podem acelerar soluções lentas:

```cpp
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")
```

> **Use com cautela:** pragmas são específicos do GCC e podem ser ignorados ou até proibidos em alguns juízes. Nunca substitua uma solução $O(N^2)$ lenta por pragmas — resolva o problema algorítmico primeiro.

---

## Template Python

```python
import sys
from collections import defaultdict, deque, Counter
from heapq import heappush, heappop
from math import gcd, ceil, floor, log2, isqrt
from itertools import combinations, permutations, accumulate
from functools import lru_cache

input = sys.stdin.readline  # I/O rápido

def main():
    n = int(input())
    # seu código aqui

main()
```

> **`@lru_cache`:** em Python, recursão com memoização é simples com o decorator `@lru_cache(maxsize=None)`. Para problemas de DP recursivo, isso elimina a necessidade de gerenciar manualmente um dicionário de memo.

```python
from functools import lru_cache

@lru_cache(maxsize=None)
def dp(i, j):
    if caso_base: return 0
    return min(dp(i+1, j), dp(i, j+1)) + custo[i][j]
```