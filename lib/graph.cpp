#include <vector>

using namespace std;
using ll = long long;
const int inf = 1e9;

#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep_from(i, j, n) for (ll i = (j); i < (n); ++i)
#define rep_rev(i, n) for (ll i = n-1; i >= 0; --i)
#define rep_from_rev(i, j, n) for (ll i = n-1; i >= j; --i)
#define all(x) (x).begin(),(x).end()
#define sz(x) ll(x.size())


struct UnionFind {
    vector<int> par;
    vector<int> next;

    UnionFind(int n) : par(n, -1), next(n) {iota(all(next), 0);}

    int root(int x) {
      if (par[x] < 0)
        return x;
      return par[x] = root(par[x]);
    }

    void unite(int x, int y) {
      x = root(x);
      y = root(y);
      if (x == y)
        return;
      if (par[x] > par[y])
        swap(x, y);
      par[x] += par[y];
      par[y] = x;
      swap(next[x], next[y]);
    }

    bool same(int x, int y) {
      int rx = root(x);
      int ry = root(y);
      return rx == ry;
    }
    int size(int x) { return -par[root(x)]; }
};


struct DAG {
private:
    vector<int> visited;

public:
    int n;
    vector<vector<int>> adj;
    vector<int> sorted;
    bool isAcyclic = true;

    DAG(vector<vector<int>> &adj) : n(adj.size()), adj(adj), visited(adj.size()) {}

    void dfs(int v) {
      if (visited[v] == 2) {
        isAcyclic = false;
        return;
      } else if (!visited[v]) {
        visited[v] = 2;
        for (int s : adj[v]) {
          dfs(s);
        }
        visited[v] = 1;
        sorted.push_back(v);
      }
    }

    void tsort() {
      for (int i = 0; i < n; i++) {
        if (!visited[i]) dfs(i);
      }
      reverse(all(sorted));
    }
};

struct DoublingLCA {
    int root;
    vector<vector<int>> children;
    vector<vector<int>> d_parents;
    vector<int> depth;

    DoublingLCA(const vector<vector<int>> &children, const int root) noexcept: root(root), children(children) {}

    void build() {
      vector<int> path(1, root);
      d_parents.resize(31, vector<int>(sz(children), -1));
      depth.resize(sz(children), inf);
      dfs(path);
    }

    int get_parent(int node, int length) {
      for(int i = 0; length; i++) {
        if(length & 1<<i) {
          length -= 1<<i;
          node = d_parents[i][node];
        }
      }
      return node;
    }

    int lca(int n1, int n2) {
      if(depth[n1] < depth[n2]) swap(n1, n2);
      n1 = get_parent(n1, depth[n1] - depth[n2]);
      if(n1 == n2) return n1;
      rep_rev(i, 31) {
        if(1<<i > depth[n1]) continue;
        if (d_parents[i][n1] != d_parents[i][n2] ) {
          n1 = d_parents[i][n1];
          n2 = d_parents[i][n2];
        }
      }
      return d_parents[0][n1];
    }

private:
    void dfs(vector<int> &path) {
      int cur = path[sz(path)-1];
      depth[cur] = path.size()-1;
      for(int i = 1, j = 0; i < sz(path);i*=2,j++) {
        d_parents[j][cur] = path[sz(path)-1-i];
      }
      for(int child: children[cur]) {
        if(depth[child] != inf) continue;
        path.push_back(child);
        dfs(path);
        path.erase(--path.end());
      }
    }
};

struct FlowGraph {
    struct Edge {
        int rev, from, to, cap;
        Edge(int r, int f, int t, int c): rev(r), from(f), to(t), cap(c){};
    };

    vector<vector<Edge>> list;
    FlowGraph(int n = 0) : list(n) {};

    int size() {
      return sz(list);
    }

    vector<Edge> &operator [](int i) {
      return list[i];
    }

    Edge& redge(const Edge e) {
      return list[e.to][e.rev];
    }

    void runFlow(Edge &e, int f) {
      e.cap -= f;
      redge(e).cap += f;
    }

    void addEdge(int from, int to, int cap) {
      int fromRev = sz(list[from]);
      int toRev = sz(list[to]);
      list[from].emplace_back(toRev, from, to, cap);
      list[to].emplace_back(fromRev, to, from, 0);
    }
};

struct Flow {
    vector<int> seen;

    Flow() {};

    int solve(FlowGraph &g, int s, int t, int f) {
      int ret = 0;
      for(;;) {
        seen.assign(sz(g), 0);
        int cap = flow(g, s, t, f);
        if(cap == 0) break;
        ret += cap;
      }
      return ret;
    }

private:
    int flow(FlowGraph &g, int v, int t, int f) {
      if(v == t) return f;
      seen[v] = 1;

      for(auto &e: g[v]) {
        if(seen[e.to]) continue;
        if(e.cap == 0) continue;

        int cap = flow(g, e.to, t, min(e.cap, f));
        if(cap == 0) continue;
        g.runFlow(e, cap);
        return cap;
      }
      return 0;
    }
};

/**
 * 強連結成分分解（SCC）を出力します
 * @param g
 * @return
 */
vector<vector<int>> makescc(const vector<vector<int>> &g) {
  const int n = sz(g);
  vector<int> a(n), used(n);
  int c = 0;
  auto dfs1 = [&](auto dfs, int cur) -> void {
      if(used[cur]) return;
      used[cur] = 1;
      for(auto ne: g[cur]) dfs(dfs, ne);
      a[cur] = c++;
  };
  rep(i, n) dfs1(dfs1, i);
  vector<vector<int>> rg(n);
  rep(i, n) {
    for(auto to: g[i]) {
      rg[to].push_back(i);
    }
  }
  vector<int> b(n);
  vector<vector<int>> ans;
  vector<int> tmp;
  rep(i, n) b[n-a[i]-1] = i, used[i] = 0;
  auto dfs2 = [&](auto dfs, int cur) -> void {
      if(used[cur]) return;
      used[cur] = 1;
      tmp.push_back(cur);
      for(auto ne: rg[cur]) dfs(dfs, ne);
  };
  for(auto i: b) {
    dfs2(dfs2, i);
    if(sz(tmp)) {
      ans.emplace_back(tmp);
      tmp.clear();
    }
  }
  return ans;
}