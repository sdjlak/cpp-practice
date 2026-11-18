// 两端取牌最大得分
// 思路：一共取 k 次，设从左边取 i 个、从右边取 k-i 个（i 在合法范围内枚举），
//       总和 = 前缀[i] + 后缀[k-i]，取最大值即可。O(n)。
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<long long> a(n);
    for (auto &x : a) cin >> x;

    // 前缀和 pre[i] = a[0] + ... + a[i-1]
    vector<long long> pre(n + 1, 0);
    for (int i = 0; i < n; ++i) pre[i + 1] = pre[i] + a[i];

    long long ans = 0;
    bool first = true;

    // 左边取 i 个，右边取 k-i 个，两边都不能超过 n 个
    int lo = max(0, k - n);
    int hi = min(k, n);
    for (int i = lo; i <= hi; ++i) {
        int j = k - i;  // 右边取的个数
        long long sum = pre[i] + (pre[n] - pre[n - j]);
        if (first || sum > ans) {
            ans = sum;
            first = false;
        }
    }

    cout << ans << '\n';
    return 0;
}
