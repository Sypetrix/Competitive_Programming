#include <bits/stdc++.h>
using namespace std;

void demo_string() {
    string s = "maratona";

    // Tamanho e acesso
    s.length();   // ou s.size()
    s[0];         // acesso O(1)
    s.front();    // primeiro char
    s.back();     // último char

    // Concatenação — use += (O(1)), nunca s = s + "x" em loop (O(N))
    s += "s";
    s += '!';
    s.push_back('X');

    // Substring, busca e modificação
    s.substr(0, 4);       // copia 4 chars a partir do índice 0
    s.find("rat");        // posição da 1ª ocorrência, ou string::npos
    s.rfind('a');         // busca da direita
    s.erase(0, 3);        // remove 3 chars a partir do índice 0
    s.insert(0, "mar");   // insere no índice 0
    s.replace(3, 4, "X"); // substitui 4 chars a partir do índice 3 por "X"

    // Conversão numérica
    string sn = to_string(123);
    int    n  = stoi("123");
    long long ll = stoll("12345678901234");

    // Algoritmos
    sort(s.begin(), s.end());
    reverse(s.begin(), s.end());
    int qtd = count(s.begin(), s.end(), 'a');

    // Leitura — ARMADILHA: cin >> n deixa '\n' no buffer
    // Use cin.ignore() antes de getline se misturar os dois
}