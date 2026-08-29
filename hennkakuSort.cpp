/*
[偏角ソート]
単位円をイメージする。
原点からの角度でソートする。
*/
struct point{ int x,y; };
ll cross(const point &a, const point &b){
    return (ll)a.x*b.y-(ll)a.y*b.x;
}
bool cmp(const point &a, const point &b){
    int ah = (a.y < 0 or (a.y == 0 and a.x < 0));
    int bh = (b.y < 0 or (b.y == 0 and b.x < 0));
    if (ah != bh) return ah < bh; // 上下どちらの半円に位置するか
    return cross(a, b) > 0; // 外積
}
void argument_sort(vector<point> &points){
    sort(points.begin(),points.end(),cmp);
}
