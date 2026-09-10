// 中国剰余定理 & 拡張ユークリッドの互除法
// 拡張ユークリッドの互除法: ax + by = gcd(a, b) を解き、gcd を返す。
// x,y の解も参照渡しで返される。
ll extgcd(ll a,ll b,ll &x,ll &y){
    if(b==0){
        x=1; y=0;
        return a;
    }
    ll d=extgcd(b,a%b,y,x);
    y-=a/b*x;
    return d;
}

// 中国剰余定理 (CRT) の拡張版
// 連立合同式 x ≡ r1 (mod m1), x ≡ r2 (mod m2) 
// {rem,m} x ≡ rem (mod m)。解がない場合は {0, -1}
pair<ll,ll> chinese_remainder(ll r1,ll m1,ll r2,ll m2){
    ll p, q;
    ll g = extgcd(m1, m2, p, q);
    if ((r2 - r1) % g != 0) return {0, -1}; // 解なし
    
    ll mod = m1 / g * m2; // 最小公倍数
    ll step = m2 / g;
    ll diff = (r2 - r1) / g;
    
    // p * diff % step を正の範囲に正規化
    ll x = diff % step * (p % step) % step;
    if (x < 0) x += step;
    
    ll rem = (r1 + m1 * x) % mod;
    if (rem < 0) rem += mod;
    
    return {rem, mod};
}
