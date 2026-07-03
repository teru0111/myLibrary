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


struct UnionFind{
    vector<int> par; 
    vector<int> siz; //各頂点に対応する連結成分のサイズ
    int count; //連結成分の個数
    
    UnionFind(int n) : par(n), siz(n,1), count(n){ //初め、全ての点の根は自分自身である。
        for(int i=0;i<n;i++) par[i] = i;
    }

    int root(int x){
        if(par[x]==x) return x;
        return par[x] = root(par[x]);
    }
    void unite(int x,int y){
        int rx = root(x), ry = root(y);
        if(rx==ry) return;//根が同じなら連結操作はしない。
        
        // 大きい木に小さい木を結合する。
        // 合併の仕方は問題によって変わるかも
        if(siz[rx]<siz[ry]){
            par[rx]=ry;
            siz[ry]+=siz[rx];
        }else{
            par[ry]=rx;
            siz[rx]+=siz[ry];
        }
        count--;
    }
    bool same(int x,int y){ // 同じ部分木に属しているか判定する
        return root(x) == root(y);
    }
    int getcount(){ // 部分木の個数
        return count;
    }
    int getsiz(int x){ // x が属する部分木のサイズ
        return siz[root(x)];
    }
};
int main(){
    int n,q; cin >> n >> q;
    UnionFind UF(n);
    
    while(q--){
        int t,u,v; cin >> t >> u >> v;
        if(t==0) UF.unite(u,v);
        else cout << UF.same(u,v) << endl;
    }
}