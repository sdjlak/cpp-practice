// 拷贝赋值运算符的测试。不要修改这个文件，只改 string.cpp。
//
// 编译（在 D:\c++ 下执行）：
//   g++ -std=c++17 -Wall -Wextra -g string_lab/string.cpp string_lab/assign_test.cpp -o string_lab/assign_test.exe
//
// 运行：
//   .\string_lab\assign_test.exe

#include "string.h"

#include <cstdio>
#include <cstring>

static int failed = 0;

static void check(bool cond, const char* what, int line) {
    if (cond) {
        std::printf("  [PASS] %s\n", what);
    } else {
        std::printf("  [FAIL] %s   (line %d)\n", what, line);
        ++failed;
    }
}

#define CHECK(cond) check((cond), #cond, __LINE__)

int main() {
    std::printf("==== Test A: plain assignment ====\n");
    {
        String a("hello");
        String b("world");
        b = a;  // b 原来是 "world"，现在要变成 "hello"
        CHECK(b.size() == 5);
        CHECK(std::strcmp(b.c_str(), "hello") == 0);
        CHECK(std::strcmp(a.c_str(), "hello") == 0);  // a 不能被改动
        CHECK(b.c_str() != a.c_str());                // 深拷贝：各有一块内存
        CHECK(String::alive() == 2);                  // 赋值不该创建新对象
    }
    CHECK(String::alive() == 0);

    std::printf("==== Test B: self assignment ====\n");
    {
        String a("hello");
        a = a;  // 自己赋值给自己，最容易踩「先删后读」的坑
        CHECK(a.size() == 5);
        CHECK(std::strcmp(a.c_str(), "hello") == 0);
        CHECK(String::alive() == 1);
    }
    CHECK(String::alive() == 0);

    std::printf("==== Test C: chained assignment ====\n");
    {
        String a("a");
        String b("b");
        String c("c");
        c = b = a;  // 需要 operator= 返回 String& 才能这样写
        CHECK(std::strcmp(a.c_str(), "a") == 0);
        CHECK(std::strcmp(b.c_str(), "a") == 0);
        CHECK(std::strcmp(c.c_str(), "a") == 0);
        CHECK(String::alive() == 3);
    }
    CHECK(String::alive() == 0);

    std::printf("==== Test D: source destroyed after assignment ====\n");
    {
        String* target = new String("temp");
        {
            String source("hello");
            *target = source;
        }  // source 在这里析构；target 必须仍然有效
        CHECK(target->size() == 5);
        CHECK(std::strcmp(target->c_str(), "hello") == 0);
        delete target;
    }
    CHECK(String::alive() == 0);

    std::printf("==== Test E: assign empty and back ====\n");
    {
        String a("hello");
        String empty;
        a = empty;  // 把 a 变成空串，旧的 "hello" 内存必须被释放
        CHECK(a.size() == 0);
        CHECK(std::strlen(a.c_str()) == 0);

        a = String("again");  // 赋一个临时对象（右值）
        CHECK(a.size() == 5);
        CHECK(std::strcmp(a.c_str(), "again") == 0);
        CHECK(String::alive() == 2);  // a 和 empty 都还活着，赋值不增加对象数
    }
    CHECK(String::alive() == 0);

    std::printf("\n%s  (%d failure%s)\n",
                failed == 0 ? "ALL PASSED" : "SOMETHING FAILED",
                failed, failed == 1 ? "" : "s");
    return failed == 0 ? 0 : 1;
}
