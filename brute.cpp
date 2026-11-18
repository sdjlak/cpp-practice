// 暴力参考解：区间递归，O(n^2 * k)，仅用于小数据对拍
#include <bits/stdc++.h>
using namespace std;

int n, k;
vector<long long> a;
unordered_map<long long, long long> memo;

long long f(int l, int r, int cnt) {
    if (cnt == 0) return 0;
    long long key = ((long long)l * 1000 + r) * 1000 + cnt;
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    long long res = max(a[l] + f(l + 1, r, cnt - 1),
                         a[r] + f(l, r - 1, cnt - 1));
    memo[key] = res;
    return res;
}

int main() {
    cin >> n >> k;
    a.resize(n);
    for (auto &x : a) cin >> x;
    cout << f(0, n - 1, k) << '\n';
    return 0;
}
