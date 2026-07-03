#include <bits/stdc++.h>
using namespace std;
const long long INF = 1LL << 60; // 10^18 より大きい
const int inf = 1<<30; // 10^9 より大きい

typedef long long ll;
//a:97, A:65
const int dx[]={1,0,-1,0,-1,1,1,-1};
const int dy[]={0,1,0,-1,-1,-1,1,1};
const double PI = acos(-1);

// priority_queue<ll,vector<ll>,greater<ll>> q;
template<class T, class U> inline bool chmin(T& a, const U& b) { if (a > b) { a = b; return true; } return false; }
template<class T, class U> inline bool chmax(T& a, const U& b) { if (a < b) { a = b; return true; } return false; }

// ==========================================
// [基本問題]
// V 個の頂点、E 本の辺があり、i 本目の辺は u[i] から　v[i] まで水を最大で c[i] だけ流すことができる。
// 頂点 1 から V へ流せる水の量の最大値を求める。
// 
// [応用]
// 最大二部マッチング問題は最大流に帰着し、重み付き最大二部マッチング問題は最小費用流問題に帰着する
// 
// 最大流 (Dinic法) テンプレート
// 計算量: O(V^2 * E)
// ==========================================
struct Edge {
    int to;
    long long cap;
    int rev; // 逆辺が adjacency list の何番目にあるか
};

struct Dinic {
    int n;
    vector<vector<Edge>> G;
    vector<int> level; // sからの距離
    vector<int> iter;  // どこまで調べ終わったか

    // コンストラクタ: n頂点のグラフを作成
    explicit Dinic(int n) : n(n), G(n), level(n), iter(n) {}

    // fromからtoへ向かう容量capの辺を張る
    void add_edge(int from, int to, long long cap) {
        G[from].push_back({to, cap, (int)G[to].size()});
        // 逆辺の初期容量は0
        G[to].push_back({from, 0, (int)G[from].size() - 1});
    }

    // sからの最短距離をBFSで計算
    void bfs(int s) {
        fill(level.begin(), level.end(), -1);
        queue<int> que;
        level[s] = 0;
        que.push(s);
        while (!que.empty()) {
            int v = que.front();
            que.pop();
            for (const auto& e : G[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }

    // 増加パスをDFSで探す
    long long dfs(int v, int t, long long f) {
        if (v == t) return f;
        for (int& i = iter[v]; i < (int)G[v].size(); i++) {
            Edge& e = G[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                long long d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

    // sからtへの最大流を求める
    long long max_flow(int s, int t) {
        long long flow = 0;
        const long long INF = 1e18; // 十分に大きな値
        while (true) {
            bfs(s);
            if (level[t] < 0) return flow; // tに到達できなくなったら終了
            fill(iter.begin(), iter.end(), 0);
            long long f;
            while ((f = dfs(s, t, INF)) > 0) {
                flow += f;
            }
        }
    }
};
// ==========================================


// ==========================================
// 動作確認用サンプル
// ==========================================
int main() {
    // 入出力の高速化
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, E;
    // 頂点数と辺数を受け取る
    if (!(cin >> V >> E)) return 0;

    // V頂点のグラフを作成 (0-indexで扱うためそのままVを渡す)
    Dinic dinic(V);

    for (int i = 0; i < E; i++) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        
        // 問題文は1-index(1〜V)なので、0-index(0〜V-1)に変換して辺を張る
        u--; 
        v--;
        dinic.add_edge(u, v, c);
    }

    // 街1 (0) から 街V (V-1) への最大流を計算して出力
    cout << dinic.max_flow(0, V - 1) << "\n";

    return 0;
}