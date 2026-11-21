#include <algorithm>
#include <cstdio>
#include <vector>

static int failed = 0;

static void check(bool cond, const char *what, int line) {
    if (cond) {
        std::printf("  [PASS] %s\n", what);
    } else {
        std::printf("  [FAIL] %s   (line %d)\n", what, line);
        ++failed;
    }
}

// 宏：自动把表达式变成字符串，自动带上当前行号
#define CHECK(cond) check((cond), #cond, __LINE__)
#define CHECK_MSG(cond, msg) check((cond), (msg), __LINE__)

// 被测函数：两端取牌
static long long solve(int n, int k, const std::vector<long long> &a) {
    std::vector<long long> pre(n + 1, 0);
    for (int i = 0; i < n; ++i) pre[i + 1] = pre[i] + a[i];
    long long best = 0;
    for (int i = 0; i <= k; ++i) {
        best = std::max(best, pre[i] + pre[n] - pre[n - (k - i)]);
    }
    return best;
}

int main() {
    CHECK(solve(5, 2, {1, 2, 3, 4, 5}) == 9);
    CHECK(solve(4, 2, {10, 1, 100, 9}) == 109);
    CHECK(solve(3, 3, {1, 2, 3}) == 6);
    CHECK_MSG(solve(3, 1, {1, 9, 8}) == 8, "k=1 只能取两端");
    CHECK_MSG(solve(3, 1, {1, 9, 8}) == 9, "故意写错的断言（演示 FAIL）");

    if (failed) std::printf("FAILED: %d check(s)\n", failed);
    else std::printf("ALL PASSED\n");
    return failed ? 1 : 0;
}
