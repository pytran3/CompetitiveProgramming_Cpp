using ll = long long;

template <ll Modulus> struct ModInt {
    ll a;
    constexpr ModInt(const ll x = 0) noexcept  : a((x%mod+mod) % mod){}
    constexpr ll &value() noexcept {return a;}
    constexpr const ll &value() const noexcept {return a;}
    constexpr ModInt operator+(const ModInt x) const noexcept {
        return ModInt(*this) += x;
    }
    constexpr ModInt operator-(const ModInt x) const noexcept {
        return ModInt(*this) -= x;
    }
    constexpr ModInt operator*(const ModInt x) const noexcept {
        return ModInt(*this) *= x;
    }
    constexpr ModInt operator/(const ModInt x) const noexcept {
        return ModInt(*this) /= x;
    }
    constexpr ModInt &operator+=(const ModInt x) noexcept {
        a += x.a;
        if (a >= Modulus) {
            a -= Modulus;
        }
        return *this;
    }
    constexpr ModInt &operator-=(const ModInt x) noexcept {
        if (a < x.a) {
            a += Modulus;
        }
        a -= x.a;
        return *this;
    }
    constexpr ModInt &operator*=(const ModInt x) noexcept {
        a = a * x.a % Modulus;
        return *this;
    }
    constexpr ModInt &operator/=(ModInt x) noexcept {
        ll exp = Modulus - 2;
        while (exp) {
            if (exp % 2) {
                *this *= x;
            }
            x *= x;
            exp /= 2;
        }
        return *this;
    }
    constexpr  ModInt operator-() noexcept {
        return ModInt(-a);
    }
    friend ostream& operator<<(ostream& os, const ModInt& m){
        os << m.a;
        return os;
    }
};
