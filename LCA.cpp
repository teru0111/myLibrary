struct Edge{
    ll to;
};
using Graph = vector<vector<Edge>>;
/*  LCA(G,root) : 木 G に対する根を root とし、LCAを求める構造体を構築
    get(u,v): u,v の LCA を求める。 O(log n)
    前処理: o(n log n), 空間: o(n log n)
*/
struct LCA{
    vector<vector<int>> parent; // parent[k][u] := u の 2^k 先の親
    vector<int> dist;
    LCA(const Graph &G,int root=0){init(G,root);}
    
    void init(const Graph &G,int root=0){
        int V=G.size();
        int K=1;
        while((1<<K) < V) K++;
        parent.assign(K,vector<int>(V,-1));
        dist.assign(V,-1);
        dfs(G,root,-1,0);
        for(int k=0;k+1<K;k++){
            for(int v=0;v<V;v++){
                if(parent[k][v]<0) parent[k+1][v]=-1;
                else parent[k+1][v]=parent[k][parent[k][v]];
            }
        }
    }
    
    // 根からの距離と一つ先の頂点を求める
    void dfs(const Graph &G,int v,int pa,int dep){
        parent[0][v]=pa;
        dist[v]=dep;
        for(auto e:G[v]){
            if(e.to != pa) dfs(G,e.to,v,dep+1);
        }
    }
    
    int get(int u,int v){
        if(dist[u] < dist[v]) swap(u,v); // u の方が深いとする
        int K = parent.size();
        // LCA までの距離を同じにする
        for(int k=0;k<K;k++){
            if((dist[u]-dist[v])>>k & 1) u = parent[k][u];
        }
        // 二分探索
        if(u==v) return u;
        for(int k=K-1;k>=0;k--){
            if(parent[k][u] != parent[k][v]){
                u=parent[k][u];
                v=parent[k][v];
            }
        }
        return parent[0][u]; // u の 2^0 先の親
    }
    
    int get_dist(int u,int v){return dist[u]+dist[v]-2*dist[get(u,v)];}
    bool is_on_path(int u,int v,int a){return get_dist(u,a)+get_dist(a,v) == get_dist(u,v);}
};
