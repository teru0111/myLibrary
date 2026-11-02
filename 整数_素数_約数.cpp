// 重複組み合わせ
struct Combination {
    vector<long long> fact, finv, inv;
    long long MOD; // ここをlong longにしておくと安心

    Combination(int sz, long long mod) : fact(sz + 1), finv(sz + 1), inv(sz + 1), MOD(mod) {
        fact[0] = 1;
        finv[0] = 1;
        inv[1] = 1;
        for (int i = 1; i <= sz; i++) {
            fact[i] = fact[i - 1] * i % MOD;
            // i=1のときはスキップ (inv[1]は設定済み)
            if (i > 1) {
                // MOD % i は int だが inv[] の index なのでOK
                // MOD / i も計算結果は整数
                inv[i] = MOD - inv[MOD % i] * (MOD / i) % MOD;
            }
            finv[i] = finv[i - 1] * inv[i] % MOD;
        }
    }

    // nCk: 組み合わせ
    long long nCr(int n, int k) {
        if (n < k || n < 0 || k < 0) return 0;
        return fact[n] * (finv[k] * finv[n - k] % MOD) % MOD;
    }

    // nPk: 順列
    long long nPr(int n, int k) {
        if (n < k || n < 0 || k < 0) return 0;
        return fact[n] * finv[n - k] % MOD;
    }

    // nHk: 重複組み合わせ (n種類から重複を許してk個選ぶ)
    // 仕切り板の考え方: (n+k-1)Ck
    long long nHr(int n, int k) {
        if (n == 0 && k == 0) return 1;
        if (n < 0 || k < 0) return 0;
        return nCr(n + k - 1, k);
    }
    
    // 階乗そのものが欲しい時用
    long long getFact(int n) {
        return fact[n];
    }
};
          
// 素因数分解
vector<pair<ll,ll>> prime_factorize(ll n){
    ll x=n;
    vector<pair<ll,ll>> pf;
    for(ll p=2;p*p<=n;p++){
        if(n%p!=0) continue;
        int e=0;
        while(n%p==0){
            e++;
            n/=p;
        }
        pf.emplace_back(p,e);
    }
    if(n!=1) pf.emplace_back(n,1);
    return pf;
}

// 約数列挙
vector<ll> get_divisors(ll n){
    vector<ll> res;
    for(ll i=1;i*i<=n;i++){
        if(n%i==0){
            res.push_back(i);
            if(i*i!=n) res.push_back(n/i);
        }
    }
    sort(res.begin(),res.end());
}

// 累乗判定 a^b <= N なら true
bool pow_le(ll a,ll b,ll N){ // b <= 60 とする。
    ll x=1LL;
    for(int i=0;i<b;i++){
        if(x>N/a) return false;
        x*=a;
    }
    return true;
}
