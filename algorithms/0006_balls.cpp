// 0006 · 碰撞的小球（CCF CSP 2018.03 第二题，真题）
//
// 题目：一条长度为 L 的线段上有 n 个小球，初始位置各不相同（0 < 位置 < L），
//       速度都是 1 单位/秒，初始都向右移动。
//       碰到线段端点（0 或 L）会反弹；两个小球相撞时各自反向。
//       求 t 秒后每个小球的位置（按输入顺序输出）。
//
// 样例：
//   n=3, L=10, t=5, 初始位置 [4, 6, 8] → 输出 7 9 9
//
// ★★★ 这题最重要的一个顿悟 ★★★
//   两个「质量相同、速度相同」的小球相撞后反向，
//   和「两个小球互相穿过、继续走自己的路」——结果是一模一样的！
//   因为你根本分不出哪个球是哪个球，交换身份后世界没有变化。
//
//   所以解法分两步：
//   1) 先忽略碰撞，把每个球当成独立反弹的幽灵：
//      先都向右走，走出 L 就反弹。用一个公式算它 t 秒后的位置：
//      m = (初始位置 + t) % (2 * L)
//      若 m <= L → 位置就是 m
//      若 m >  L → 位置是 2 * L - m
//      （这是「镜子反射」公式，想想为什么成立）
//   2) 幽灵位置一共有 n 个，排序后，
//      和「初始位置排序」一一对应——初始在最左边的球，
//      最终就是幽灵里最左边的那个（球和球不会互相穿过）。
//
// 编译运行（在 D:\c++ 下执行）：
//   g++ -std=c++17 -Wall -Wextra algorithms/0006_balls.cpp -o algorithms/0006.exe
//   .\algorithms\0006.exe

#include <algorithm>
#include <cstdio>
#include <vector>

// n 个球，线段长 L，t 秒后，按初始输入顺序返回每个球的位置
std::vector<int> ball_positions(int n, int L, int t, const std::vector<int>& start) {
    // 在这里写你的代码
   
        
        std::vector<int> ghost(n);
        for (int i = 0; i < n; ++i) {
            long long m = (start[i] + (long long)t) % (2LL * L);
            ghost[i] = (m <= L) ? (int)m : (int)(2LL * L - m);
        }

        
        std::vector<int> order(n);
        for (int i = 0; i < n; ++i) order[i] = i;
        std::sort(order.begin(), order.end(),
            [&](int a, int b) { return start[a] < start[b]; });

        
        std::sort(ghost.begin(), ghost.end());
        std::vector<int> res(n);
        for (int k = 0; k < n; ++k) res[order[k]] = ghost[k];
        return res;
    
    return {};
}

// ---------------- 下面是测试，不要改 ----------------

static int failed = 0;

static void expect(int n, int L, int t, const std::vector<int>& start,
                   const std::vector<int>& want, const char* label) {
    std::vector<int> got = ball_positions(n, L, t, start);
    bool ok = (got == want);
    std::printf("  [%s] %s\n", ok ? "PASS" : "FAIL", label);
    if (!ok) {
        std::printf("       got : [");
        for (std::size_t i = 0; i < got.size(); ++i) std::printf("%s%d", i ? " " : "", got[i]);
        std::printf("]\n       want: [");
        for (std::size_t i = 0; i < want.size(); ++i) std::printf("%s%d", i ? " " : "", want[i]);
        std::printf("]\n");
        ++failed;
    }
}

int main() {
    expect(3, 10, 5, {4, 6, 8}, {7, 9, 9}, "官方样例");
    expect(1, 10, 5, {2}, {7}, "n=1 直接向右走");
    expect(1, 10, 15, {2}, {3}, "n=1 反弹一次（2+15=17>10 → 3）");
    expect(2, 10, 1, {1, 9}, {2, 10}, "n=2 不碰撞，右边的球撞墙");
    expect(4, 10, 3, {1, 4, 6, 9}, {4, 7, 8, 9}, "n=4 有碰撞有反弹");

    std::printf("\n%s  (%d failure%s)\n",
                failed == 0 ? "ALL PASSED" : "SOMETHING FAILED",
                failed, failed == 1 ? "" : "s");
    return failed == 0 ? 0 : 1;
}
