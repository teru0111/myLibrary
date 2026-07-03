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
// しかし、水を 1 流すごとに費用が d[i] かかる。
// 頂点 1 から V へ水を F だけ流すとき、合計費用の最小値を求める。
// 
// [応用]
// 最大二部マッチング問題は最大流に帰着し、重み付き最大二部マッチング問題は最小費用流問題に帰着する
// 
// 最小費用流 (Primal-Dual法 + ポテンシャル付きDijkstra)
// 計算量: O(F * E log V)
// ==========================================
struct Edge {
    int to;
    long long cap;
    long long cost;
    int rev; // 逆辺が adjacency list の何番目にあるか
};

struct MinCostFlow {
    int n;
    vector<vector<Edge>> G;
    vector<long long> h;    // ポテンシャル
    vector<long long> dist; // 最短距離
    vector<int> prevv;      // 直前の頂点
    vector<int> preve;      // 直前の辺

    const long long INF = 1e18; // 十分に大きな値

    // コンストラクタ: n頂点のグラフを作成
    explicit MinCostFlow(int n) : n(n), G(n), h(n), dist(n), prevv(n), preve(n) {}

    // fromからtoへ向かう容量cap, 費用costの辺を張る
    void add_edge(int from, int to, long long cap, long long cost) {
        G[from].push_back({to, cap, cost, (int)G[to].size()});
        // 逆辺の初期容量は0、費用は -cost
        G[to].push_back({from, 0, -cost, (int)G[from].size() - 1});
    }

    // sからtへ流量fを流すための最小費用を計算する
    // 流せない場合は -1 を返す
    long long min_cost_flow(int s, int t, long long f) {
        long long res = 0;
        fill(h.begin(), h.end(), 0); // 初期ポテンシャルは0

        while (f > 0) {
            // Dijkstra法で h を更新しつつ最短経路を求める
            priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> que;
            fill(dist.begin(), dist.end(), INF);
            
            dist[s] = 0;
            que.push({0, s});
            
            while (!que.empty()) {
                auto [d, v] = que.top();
                que.pop();
                
                if (dist[v] < d) continue;
                
                for (int i = 0; i < (int)G[v].size(); i++) {
                    Edge& e = G[v][i];
                    // 残余容量があり、距離が更新できる場合
                    // 実際の費用は e.cost + h[v] - h[e.to] となる
                    if (e.cap > 0 && dist[e.to] > dist[v] + e.cost + h[v] - h[e.to]) {
                        dist[e.to] = dist[v] + e.cost + h[v] - h[e.to];
                        prevv[e.to] = v;
                        preve[e.to] = i;
                        que.push({dist[e.to], e.to});
                    }
                }
            }
            
            // 目的の頂点に到達できない場合は流しきれない
            if (dist[t] == INF) {
                return -1; 
            }
            
            // ポテンシャルを更新
            for (int v = 0; v < n; v++) {
                if (dist[v] != INF) h[v] += dist[v];
            }
            
            // s-t 間の最短経路に沿って目一杯流す
            long long d = f;
            for (int v = t; v != s; v = prevv[v]) {
                d = min(d, G[prevv[v]][preve[v]].cap);
            }
            
            f -= d;
            res += d * h[t];
            
            // 流した分だけ残余グラフの容量を更新
            for (int v = t; v != s; v = prevv[v]) {
                Edge& e = G[prevv[v]][preve[v]];
                e.cap -= d;
                G[v][e.rev].cap += d;
            }
        }
        return res;
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
    long long F;
    // 頂点数、辺数、流したい流量を受け取る
    if (!(cin >> V >> E >> F)) return 0;

    // V頂点のグラフを作成 (0-index)
    MinCostFlow mcf(V);

    for (int i = 0; i < E; i++) {
        int u, v;
        long long c, d;
        cin >> u >> v >> c >> d;
        
        // 問題文は1-index(1〜V)なので、0-index(0〜V-1)に変換して辺を張る
        u--; 
        v--;
        mcf.add_edge(u, v, c, d);
    }

    // 街1 (0) から 街V (V-1) へ 流量 F を流す
    long long ans = mcf.min_cost_flow(0, V - 1, F);

    if (ans == -1) {
        // 流量 F を流しきれなかった場合
        cout << "Impossible\n";
    } else {
        // 最小費用を出力
        cout << ans << "\n";
    }

    return 0;
}