#include <bits/stdc++.h>
using namespace std;

void demo_bitset() {
    bitset<1024> b;

    b.set(10);    // liga o bit 10
    b.reset(10);  // desliga o bit 10
    b.flip(5);    // inverte o bit 5
    b.test(5);    // lê o bit 5 (true/false)
    b.count();    // popcount: quantidade de bits ligados
    b.size();     // tamanho total (1024)

    bitset<1024> a, c;
    a |= c;           // OR
    a &= (c << 2);    // AND com deslocamento
    a ^= c;           // XOR
    a = ~c;           // NOT (complemento)
}