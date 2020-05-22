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