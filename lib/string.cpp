#include <vector>

using namespace std;
using ll = long long;

#define rep(i, n) for (ll i = 0; i < (n); ++i)

using Pair = pair<ll, ll>;

struct RollingHash {
    static const int base1 = 1007, base2 = 2009;
    static const int mod1 = 1000000007, mod2 = 1000000009;
    vector<ll> hash1, hash2, power1, power2;

    RollingHash(const string &s) {
      int n = (int)s.size();
      hash1.assign(n+1, 0);
      hash2.assign(n+1, 0);
      power1.assign(n+1, 1);
      power2.assign(n+1, 1);
      rep(i, n) {
        hash1[i+1] = (hash1[i] * base1 + s[i]) % mod1;
        hash2[i+1] = (hash2[i] * base2 + s[i]) % mod2;
        power1[i+1] = (power1[i] * base1) % mod1;
        power2[i+1] = (power2[i] * base2) % mod2;
      }
    }

    inline Pair get(int l, int r) const {
      ll res1 = hash1[r] - hash1[l] * power1[r-l] % mod1;
      if (res1 < 0) res1 += mod1;
      ll res2 = hash2[r] - hash2[l] * power2[r-l] % mod2;
      if (res2 < 0) res2 += mod2;
      return {res1, res2};
    }
};