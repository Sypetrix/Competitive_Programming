#include <bits/stdc++.h>
using namespace std;

// BigInt: inteiro não-negativo de precisão arbitrária
// Suporta: +, -, *, comparação, I/O
// Dígitos armazenados em base 10^9 (da posição menos significativa para a mais)
struct BigInt {
    static const int BASE = 1e9;
    vector<int> d; // d[0] = parte menos significativa
    bool neg = false;

    BigInt() : d(1, 0) {}

    BigInt(long long v) {
        neg = (v < 0); v = abs(v);
        do { d.push_back(v % BASE); v /= BASE; } while (v);
    }

    BigInt(const string& s) {
        neg = (!s.empty() && s[0] == '-');
        int i = neg ? 1 : 0;
        for (int j = (int)s.size(); j > i; j -= 9) {
            int from = max(i, j - 9);
            d.push_back(stoi(s.substr(from, j - from)));
        }
        trim();
    }

    void trim() {
        while (d.size() > 1 && d.back() == 0) d.pop_back();
        if (d.size() == 1 && d[0] == 0) neg = false;
    }

    bool isZero() const { return d.size() == 1 && d[0] == 0; }

    // ─── Comparação (módulo, sem sinal) ──────────────────────────────────
    int cmpAbs(const BigInt& o) const {
        if (d.size() != o.d.size()) return d.size() < o.d.size() ? -1 : 1;
        for (int i = (int)d.size() - 1; i >= 0; i--)
            if (d[i] != o.d[i]) return d[i] < o.d[i] ? -1 : 1;
        return 0;
    }

    bool operator==(const BigInt& o) const { return neg==o.neg && cmpAbs(o)==0; }
    bool operator< (const BigInt& o) const {
        if (neg != o.neg) return neg;
        return neg ? cmpAbs(o) > 0 : cmpAbs(o) < 0;
    }
    bool operator> (const BigInt& o) const { return o < *this; }
    bool operator<=(const BigInt& o) const { return !(o < *this); }
    bool operator>=(const BigInt& o) const { return !(*this < o); }

    // ─── Adição / Subtração de módulos ───────────────────────────────────
    static BigInt addAbs(const BigInt& a, const BigInt& b) {
        BigInt res;
        res.d.resize(max(a.d.size(), b.d.size()) + 1, 0);
        long long carry = 0;
        for (int i = 0; i < (int)res.d.size(); i++) {
            long long s = carry;
            if (i < (int)a.d.size()) s += a.d[i];
            if (i < (int)b.d.size()) s += b.d[i];
            res.d[i] = s % BASE;
            carry = s / BASE;
        }
        res.trim();
        return res;
    }

    // Subtrai módulo (assume |a| >= |b|)
    static BigInt subAbs(const BigInt& a, const BigInt& b) {
        BigInt res;
        res.d.resize(a.d.size(), 0);
        long long borrow = 0;
        for (int i = 0; i < (int)a.d.size(); i++) {
            long long diff = (long long)a.d[i] - borrow;
            if (i < (int)b.d.size()) diff -= b.d[i];
            if (diff < 0) { diff += BASE; borrow = 1; }
            else borrow = 0;
            res.d[i] = diff;
        }
        res.trim();
        return res;
    }

    // ─── Operadores + e - ────────────────────────────────────────────────
    BigInt operator+(const BigInt& o) const {
        if (neg == o.neg) {
            BigInt res = addAbs(*this, o);
            res.neg = neg;
            return res;
        }
        // sinais diferentes: subtrai o menor do maior
        if (cmpAbs(o) >= 0) {
            BigInt res = subAbs(*this, o);
            res.neg = neg;
            res.trim();
            return res;
        }
        BigInt res = subAbs(o, *this);
        res.neg = o.neg;
        res.trim();
        return res;
    }

    BigInt operator-(const BigInt& o) const {
        BigInt tmp = o; tmp.neg = !o.neg;
        return *this + tmp;
    }

    BigInt& operator+=(const BigInt& o) { return *this = *this + o; }
    BigInt& operator-=(const BigInt& o) { return *this = *this - o; }

    // ─── Multiplicação — O(N*M) ───────────────────────────────────────────
    BigInt operator*(const BigInt& o) const {
        BigInt res;
        res.d.assign(d.size() + o.d.size(), 0);
        for (int i = 0; i < (int)d.size(); i++) {
            long long carry = 0;
            for (int j = 0; j < (int)o.d.size() || carry; j++) {
                long long cur = (long long)res.d[i + j] + carry;
                if (j < (int)o.d.size()) cur += (long long)d[i] * o.d[j];
                res.d[i + j] = cur % BASE;
                carry = cur / BASE;
            }
        }
        res.neg = neg ^ o.neg;
        res.trim();
        return res;
    }

    BigInt& operator*=(const BigInt& o) { return *this = *this * o; }

    // ─── I/O ──────────────────────────────────────────────────────────────
    friend ostream& operator<<(ostream& os, const BigInt& b) {
        if (b.neg) os << '-';
        os << b.d.back();
        for (int i = (int)b.d.size() - 2; i >= 0; i--)
            os << setw(9) << setfill('0') << b.d[i];
        return os;
    }

    friend istream& operator>>(istream& is, BigInt& b) {
        string s; is >> s; b = BigInt(s); return is;
    }
};

// ─── Exemplo de uso ───────────────────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    BigInt a("12345678901234567890");
    BigInt b("98765432109876543210");

    cout << a + b << "\n";  // soma
    cout << b - a << "\n";  // subtração
    cout << a * b << "\n";  // multiplicação

    if (a < b) cout << "a < b\n";

    // Fatorial de 20 (ultrapassa long long)
    BigInt fat(1);
    for (int i = 2; i <= 20; i++) fat *= BigInt(i);
    cout << "20! = " << fat << "\n";

    return 0;
}