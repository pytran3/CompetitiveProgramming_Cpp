#include <vector>
#include <numeric>

#define all(x) (x).begin(),(x).end()
using namespace std;

struct PersistentUnionFind {
    vector<int> par;
    vector<int> time;
    vector<vector<pair<int, int>>> _size;
    int current = 0;
    PersistentUnionFind(int n) : par(n+1, -1), time(n+1, 1e9), _size(n+1, vector<pair<int, int>>(1, {0, -1})){}

    int root(int t, int x) {
      if (par[x] < 0) return x;
      if(time[x] > t) return x;
      return root(t, par[x]);
    }

    void unite(int x, int y) {
      x = root(current, x);
      y = root(current, y);
      current++;
      if (x == y)
        return;
      if (par[x] > par[y])
        swap(x, y);
      par[x] += par[y];
      _size[x].emplace_back(current, par[x]);
      par[y] = x;
      time[y] = current;
    }

    bool same(int t, int x, int y) {
      int rx = root(t, x);
      int ry = root(t, y);
      return rx == ry;
    }
    bool same(int x, int y) {
      return same(current, x, y);
    }
    int size(int t, int x) { return -(*--lower_bound(all(_size[root(t, x)]), make_pair(t, 0))).second ;}
};
template<class Abel> struct WeightedUnionFind {
    vector<int> par;
    vector<int> rank;
    vector<Abel> diff_weight;

    WeightedUnionFind(int n = 1, Abel SUM_UNITY = 0) {
      init(n, SUM_UNITY);
    }

    void init(int n = 1, Abel SUM_UNITY = 0) {
      par.resize(n); rank.resize(n); diff_weight.resize(n);
      for (int i = 0; i < n; ++i) par[i] = i, rank[i] = 0, diff_weight[i] = SUM_UNITY;
    }

    int root(int x) {
      if (par[x] == x) {
        return x;
      }
      else {
        int r = root(par[x]);
        diff_weight[x] += diff_weight[par[x]];
        return par[x] = r;
      }
    }

    Abel weight(int x) {
      root(x);
      return diff_weight[x];
    }

    bool same(int x, int y) {
      return root(x) == root(y);
    }

    bool unite(int x, int y, Abel w) {
      w += weight(x); w -= weight(y);
      x = root(x); y = root(y);
      if (x == y) return false;
      if (rank[x] < rank[y]) swap(x, y), w = -w;
      if (rank[x] == rank[y]) ++rank[x];
      par[y] = x;
      diff_weight[y] = w;
      return true;
    }

    Abel diff(int x, int y) {
      return weight(y) - weight(x);
    }
};

template<class VAL> struct RBST {
    VAL SUM_UNITY = 0;                              // to be set

    unsigned int randInt() {
      static unsigned int tx = 123456789, ty = 362436069, tz = 521288629, tw = 88675123;
      unsigned int tt = (tx ^ (tx << 11));
      tx = ty; ty = tz; tz = tw;
      return (tw = (tw ^ (tw >> 19)) ^ (tt ^ (tt >> 8)));
    }

    struct NODE {
        NODE *left, *right;
        VAL val;                        // the value of the node
        int size;                       // the size of the subtree
        VAL sum;                        // the value-sum of the subtree

        NODE() : val(SUM_UNITY), size(1), sum(SUM_UNITY) {
          left = right = NULL;
        }

        NODE(VAL v) : val(v), size(1), sum(v) {
          left = right = NULL;
        }

        /* additional update */
        inline void update() {

        }

        /* additional lazy-propagation */
        inline void push() {

          /* ex: reverse */
          /*
          if (this->rev) {
          swap(this->left, this->right);
          if (this->left) this->left->rev ^= true;
          if (this->right) this->right->rev ^= true;
          this->rev = false;
          }
          */
        }
    };

    NODE* root;
    RBST() : root(NULL) { }
    RBST(NODE* node) : root(node) { }

    inline int size(NODE *node) {
      return !node ? 0 : node->size;
    }
    inline int size() {
      return this->size(this->root);
    }

    inline VAL sum(NODE *node) {
      return !node ? SUM_UNITY : node->sum;
    }
    inline VAL sum() {
      return this->sum(this->root);
    }

    inline NODE* update(NODE *node) {
      node->size = size(node->left) + size(node->right) + 1;
      node->sum = sum(node->left) + sum(node->right) + node->val;
      node->update();
      return node;
    }

    inline void push(NODE *node) {
      if (!node) return;
      node->push();
    }

    inline int lowerBound(NODE *node, VAL val) {
      push(node);
      if (!node) return 0;
      if (val <= node->val) return lowerBound(node->left, val);
      else return size(node->left) + lowerBound(node->right, val) + 1;
    }
    inline int lowerBound(VAL val) {
      return this->lowerBound(this->root, val);
    }

    inline int upperBound(NODE *node, VAL val) {
      push(node);
      if (!node) return 0;
      if (val >= node->val) return size(node->left) + upperBound(node->right, val) + 1;
      else return upperBound(node->left, val);
    }
    inline int upperBound(VAL val) {
      return this->upperBound(this->root, val);
    }

    inline int count(VAL val) {
      return upperBound(val) - lowerBound(val);
    }

    /* get --- k: 0-index */
    inline VAL get(NODE *node, int k) {
      push(node);
      if (!node) return -1;
      if (k == size(node->left)) return node->val;
      if (k < size(node->left)) return get(node->left, k);
      else return get(node->right, k - size(node->left) - 1);
    }
    inline VAL get(int k) {
      return get(this->root, k);
    }

    NODE* merge(NODE *left, NODE *right) {
      push(left);
      push(right);
      if (!left || !right) {
        if (left) return left;
        else return right;
      }
      if (randInt() % (left->size + right->size) < left->size) {
        left->right = merge(left->right, right);
        return update(left);
      }
      else {
        right->left = merge(left, right->left);
        return update(right);
      }
    }
    void merge(RBST add) {
      this->root = this->merge(this->root, add.root);
    }
    pair<NODE*, NODE*> split(NODE* node, int k) { // [0, k), [k, n)
      push(node);
      if (!node) return make_pair(node, node);
      if (k <= size(node->left)) {
        pair<NODE*, NODE*> sub = split(node->left, k);
        node->left = sub.second;
        return make_pair(sub.first, update(node));
      }
      else {
        pair<NODE*, NODE*> sub = split(node->right, k - size(node->left) - 1);
        node->right = sub.first;
        return make_pair(update(node), sub.second);
      }
    }
    RBST split(int k) {
      pair<NODE*, NODE*> sub = split(this->root, k);
      this->root = sub.first;
      return RBST(sub.second);
    }

    void insert(const VAL val) {
      pair<NODE*, NODE*> sub = this->split(this->root, this->lowerBound(val));
      this->root = this->merge(this->merge(sub.first, new NODE(val)), sub.second);
    }

    void erase(const VAL val) {
      if (!this->count(val)) return;
      pair<NODE*, NODE*> sub = this->split(this->root, this->lowerBound(val));
      this->root = this->merge(sub.first, this->split(sub.second, 1).second);
    }
};

template<typename T> struct BIT {
    vector<T> bit;
    int n;

    BIT(const int size) : bit(size+1), n(size) {}

    void add(int index, T value) noexcept {
      index++;  // to 1-indexed
      for (int x = index; x <= n; x += x & -x) bit[x] += value;
    }

    constexpr T sum(int lft, int rgt) noexcept {
      return sum(rgt) - sum(lft);
    }

    constexpr T sum(int rgt) noexcept {
      //rgt++;rgt--;// to 1-indexed and open section
      T ret = 0;
      for (int x = rgt; x > 0; x -= x & -x) ret += bit[x];
      return ret;
    }
};

template<typename T> struct RangeAddBIT {
    BIT<T> p, q;
    RangeAddBIT(const int size) :p(size+1),q(size+1){}
    void add(int lft, int rgt, T w) {
      p.add(lft, -w * lft);
      p.add(rgt, w * rgt);
      q.add(lft, w);
      q.add(rgt, -w);
    }

    T sum(int rgt) {
      return p.sum(rgt) + q.sum(rgt) * rgt;
    }

    T sum(int lft, int rgt) {
      return sum(rgt) - sum(lft);
    }
};

template<typename T> struct SegmentTree {
    typedef function<T(T,T)> F;
    int n;
    T e;
    F f;
    vector<T> vec;
    SegmentTree(const int size, const T e, const F f) : e(e), f(f) {
      n = 1;
      while ( n  < size) n *= 2;
      vec = vector<T>(2 * n - 1, e);
    }

    void update(int i, T value) {
      i += n - 1;
      vec[i] = value;
      while(i > 0) {
        i = (i - 1) / 2;
        vec[i] = f(vec[2 * i + 1], vec[2 * i + 2]);
      }
    }

    const T query(int a, int b) {
      T vl = e;
      T vr = e;
      for(int l=a+n,r=b+n;l<r;l>>=1,r>>=1) {
        if(l&1) vl=f(vl,vec[(l++)-1]);
        if(r&1) vr=f(vec[(--r)-1],vr);
      }
      return f(vl,vr);
    }
};
