#include <vector>

#define rep(i, n) for (int i = 0; i < (n); ++i)
#define sz(x) ll(x.size())


template<typename T>
vector<vector<T>> matrix_multiplication(const vector<vector<T>> &a, const vector<vector<T>> &b) {
  const int n = sz(a), m = sz(b[0]);
  vector ret(n, vector<T>(n));
  rep(i, n) rep(j, n) rep(k, n) ret[i][j] += a[i][k] * b[k][j];
  return ret;
}

template<typename T>
vector<vector<vector<T>>> matrix_doubling(const vector<vector<T>> &a, const ll k) {
  // return [A^0, A^1, A^2, ...]
  const int n = sz(a), m = sz(a);
  int l = 0;
  while((1ll << l) <= k) l++;
  l++;
  vector ret(l + 1, vector(n, vector<T>(m)));
  rep(i, n) ret[0][i][i] = T(1);
  ret[1] = a;
  rep_from(t, 1, l) ret[t + 1] = matrix_multiplication(ret[t], ret[t]);
  return ret;
}