// 0004 · 跳一跳（CCF CSP 2018.03 第一题，真题！）
//
// 这是 CSP 真题第一题的标准模样：读一串输入，按规则累加得分。
// 考的不是算法，是「细心读题 + 状态维护」。
//
// 游戏规则：
//   每次跳跃落点有三种情况：
//     1 ：没跳到方块中心 → 得 1 分，连续加分中断
//     2 ：跳到方块中心   → 得 2、4、6、8……连续跳中心，得分每次 +2 递增
//     0 ：游戏结束
//
// 示例：
//   输入序列  1 1 2 2 2 1 1 2 2 0
//   得分计算  1+1+2+4+6+1+1+2+4 = 22
//   输出     22
//
// 编译运行（在 D:\c++ 下执行）：
//   g++ -std=c++17 -Wall -Wextra algorithms/0004_jump.cpp -o algorithms/0004.exe
//   .\algorithms\0004.exe
//
// 提示：维护一个「当前连续跳到中心几次」的计数 combo：
//   遇到 1 → 加 1 分，combo 清零
//   遇到 2 → 加 (combo + 1) * 2 分，combo 加一
//   遇到 0 → 结束

#include <cstdio>
#include <vector>

// moves 是去掉结尾 0 之后的落点序列（只有 1 和 2），返回总分
long long jump_score(const std::vector<int>& moves) {
    int x = 0;
    int sum = 0;
    for (int i = 0;i<moves.size();i++) {
        if (moves[i] == 1) {
            sum += 1;
            x = 0;
        }
        else {
            x += 2;
            sum += x;
        }
    }

    return sum;
}

// ---------------- 下面是测试，不要改 ----------------

static int failed = 0;

static void expect(const std::vector<int>& moves, long long want, const char* label) {
    long long got = jump_score(moves);
    std::printf("  [%s] %s\n", got == want ? "PASS" : "FAIL", label);
    if (got != want) {
        std::printf("       got %lld, want %lld\n", got, want);
        ++failed;
    }
}

int main() {
    expect({1, 1, 2, 2, 2, 1, 1, 2, 2}, 22, "官方示例");
    expect({2, 2, 2, 2}, 20, "连续四个中心 2+4+6+8");
    expect({1, 1, 1}, 3, "全是一分");
    expect({}, 0, "空序列（直接结束）");
    expect({2, 1, 2}, 5, "中心被 1 打断：2+1+2");
    expect({2}, 2, "只有一个中心");

    std::printf("\n%s  (%d failure%s)\n",
                failed == 0 ? "ALL PASSED" : "SOMETHING FAILED",
                failed, failed == 1 ? "" : "s");
    return failed == 0 ? 0 : 1;
}
