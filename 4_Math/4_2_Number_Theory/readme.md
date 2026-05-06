# Teoria dos Números (Number Theory)

Em maratonas de programação, problemas de Teoria dos Números raramente aceitam soluções de força bruta. Se o problema envolve propriedades de inteiros, divisibilidade, primos ou fatoriais gigantes, você precisará de algoritmos com complexidade de tempo sub-linear, geralmente baseados em matemática discreta pesada.

---

## Visão Geral

| Algoritmo / Arquivo | Complexidade | Utilização Típica |
|---|---|---|
| `erastotenes_sieve` | O(N log log N) | Pré-computar números primos usando `std::bitset` para economizar memória. |
| `dividers` | O(√N) ou O(π(N)) | Contar e somar divisores usando fatoração prima. |
| `euclides` | O(log(min(A, B))) | Encontrar MDC, MMC e o Inverso Modular (para divisão com módulo). |
| `diophantine` | O(log(min(A, B))) | Encontrar soluções inteiras $(x, y)$ para equações do tipo $Ax + By = C$. |
| `phi_euler` | O(√N) | Descobrir quantos números menores que $N$ são coprimos a $N$. |
| `legendre` | O(log_p(N)) | Descobrir a maior potência de um primo $P$ que divide $N!$ (Fatorial). |
| `cycle_floyd` | O(μ + λ) | Encontrar o início ($\mu$) e o tamanho ($\lambda$) de um ciclo em sequências. |

---

## 1. Crivo de Eratóstenes e Fatoração

O motor principal para a maioria dos problemas matemáticos. Usamos um `bitset` para comprimir a memória, permitindo gerar primos até $10^7$ quase instantaneamente.

```cpp
ll tam_crivo;
bitset<10000010> bs;
vector<ll> primos;

void crivo(ll limite) {
    tam_crivo = limite + 1;
    bs.set();
    bs[0] = bs[1] = 0;

    for (ll i = 2; i <= tam_crivo; i++) {
        if (bs[i]) {
            // Otimização: começa a eliminar a partir de i*i
            for (ll j = i * i; j <= tam_crivo; j += i) bs[j] = 0;
            primos.push_back(i);
        }
    }
}
```

> **Use o Crivo quando** precisar fazer múltiplas consultas de primalidade ou fatorar diversos números rapidamente. Tentar fatorar $10^5$ números em $O(\sqrt{N})$ sem o crivo pré-computado causará *Time Limit Exceeded* (TLE).

---

## 2. Algoritmo de Euclides Estendido e Inverso Modular

O Euclides Estendido não apenas encontra o MDC (Maior Divisor Comum), mas também os coeficientes de Bézout ($x$ e $y$) que satisfazem $Ax + By = MDC(A, B)$. Isso é a base para a divisão modular.

```cpp
ll extGCD(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1; y = 0;
        return a;
    }
    ll x1, y1;
    ll d = extGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

// Divisão com módulo: (A / B) % M = (A * invMod(B, M)) % M
ll invMod(ll a, ll m) {
    ll x, y;
    if (extGCD(a, m, x, y) != 1) return -1; // Não existe inverso
    return (x % m + m) % m; // Ajuste para C++ (evita módulo negativo)
}
```

> **Use o Inverso Modular quando** precisar realizar divisões em fórmulas de análise combinatória ($nCr$) sob um módulo primo (como $10^9 + 7$).

---

## 3. Equações Diofantinas Lineares

Resolve problemas do tipo "com moedas de valor A e notas de valor B, como pagar exatamente o valor C?".

```cpp
bool diophantine(ll a, ll b, ll c, ll &x0, ll &y0, ll &g) {
    g = extGCD(abs(a), abs(b), x0, y0);
    if (c % g != 0) return false; // C tem que ser múltiplo do MDC(A,B)

    x0 *= c / g;
    y0 *= c / g;
    if (a < 0) x0 = -x0;
    if (b < 0) y0 = -y0;
    return true;
}
```

> **Use Diofantinas quando** precisar encontrar uma solução inteira para a intersecção de equações de restrição linear.

---

## 4. Fórmula de Legendre e "Factovisors"

Se o problema falar em dividir $N!$ (fatorial gigante) por algum número, esqueça os tipos comuns (`long long` estoura em $21!$). A Fórmula de Legendre conta a multiplicidade de um primo $p$ em $N!$.

```cpp
// Retorna a maior potência do primo 'p' que divide 'n!'
ll legendre(ll n, ll p) {
    ll count = 0;
    while (n > 0) {
        count += n / p;
        n /= p;
    }
    return count;
}
```

> **Use Legendre quando** o problema perguntar "quantos zeros à direita tem $N!$?" (basta contar as potências de 5) ou se $M$ divide $N!$.

---

## 5. Função Totiente de Euler ($\phi$) e Divisores

A função $\phi(N)$ retorna a quantidade de inteiros de $1$ a $N$ que são coprimos com $N$ (ou seja, $MDC(x, N) = 1$). A contagem de divisores usa a fatoração prima.

```cpp
// Se a fatoração prima de N é p1^a * p2^b...
// A quantidade de divisores é (a + 1) * (b + 1)...

ll phi(ll n) {
    ll result = n;
    for (ll i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0) n /= i;
            result -= result / i; // Fórmula de Euler
        }
    }
    if (n > 1) result -= result / n;
    return result;
}
```

> **Use a Função $\phi$ (Phi) quando** trabalhar com o Teorema de Euler ou criptografia (RSA).

---

## 6. Busca de Ciclos (Algoritmo de Floyd)

Também conhecido como o algoritmo da "Lebre e da Tartaruga" (Hare and Tortoise). Ocupa apenas $O(1)$ de memória para encontrar ciclos numa sequência pseudo-aleatória $x_{i} = f(x_{i-1})$.

```cpp
pair<int, int> floyd_cycle_finding(int (*func)(int), int x0) {
    int tart = func(x0), lebr = func(func(x0));

    // Fase 1: Achando a interseção
    while (tart != lebr) {
        tart = func(tart);
        lebr = func(func(lebr));
    }
    // Fase 2 e 3: Buscando o início (mu) e tamanho (lambda)
    // (Veja a implementação completa no arquivo cycle_floyd.cpp)
}
```

> **Use a Busca de Ciclos de Floyd quando** o limite de memória for restrito (não dá para salvar os estados num `std::set`) ou quando o espaço de estados for muito denso.