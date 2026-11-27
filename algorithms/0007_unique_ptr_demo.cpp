// 0007 · 智能指针 Day 1：unique_ptr（阶段一验收 A2）
//
// 今天的任务不是「做题」，是做实验。按顺序：
//   ① 先直接编译运行，观察输出——重点看 Widget 什么时候「出生」「死亡」；
//   ② 取消注释「错误示范」那行，重新编译，仔细读编译器的报错；
//   ③ 再把注释加回去，跑通。
//
// 实验目标（做完能用自己的话回答）：
//   - unique_ptr 为什么叫「独占」？它和普通指针最大的区别是什么？
//   - 为什么 std::unique_ptr<Widget> c = b; 编译不过，加个 std::move 就能过？
//   - 离开作用域时自动 delete 这件事，和你在 String 类里写的析构函数有什么关系？
//
// 编译运行（在 D:\c++ 下执行）：
//   g++ -std=c++17 -Wall -Wextra algorithms/0007_unique_ptr_demo.cpp -o algorithms/0007.exe
//   .\algorithms\0007.exe

#include <cstdio>
#include <memory>
using namespace std;
struct Widget {
    int id;
    explicit Widget(int i) : id(i) { std::printf("Widget %d 出生\n", id); }
    ~Widget() { std::printf("Widget %d 死亡（自动释放，没人手写 delete）\n", id); }
};

int main() {
    std::printf("=== 实验 1：作用域结束，自动释放 ===\n");
    {
        std::unique_ptr<Widget> p(new Widget(1));
        std::printf("  Widget 1 正在被使用\n");
        // 离开这个花括号时，p 自动 delete 掉 Widget(1)
    }
    std::printf("  （花括号结束了，Widget 1 已经死亡）\n\n");

    std::printf("=== 实验 2：所有权转移 ===\n");
    std::unique_ptr<Widget> a(new Widget(2));
    std::unique_ptr<Widget> b = std::move(a);   // 所有权从 a 转到 b
    if (a == nullptr) {
        std::printf("  a 被掏空了（现在 a 是 nullptr）\n");
    }
    std::printf("  b 仍然活着，还能用：b->id = %d\n\n", b->id);

    std::printf("=== 实验 3：故意踩编译错误 ===\n");
    std::printf("  取消下面那行注释，重新编译，看看编译器怎么说。\n");
    std::printf("  看完报错后，再把注释加回来。\n\n");

    // 错误示范：unique_ptr 禁止拷贝——取消下面这行注释试试：
    // std::unique_ptr<Widget> c = b;

    std::printf("=== 实验完成 ===\n");
    return 0;
}
