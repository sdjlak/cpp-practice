#ifndef CPP_LAB_STRING_H
#define CPP_LAB_STRING_H

#include <cstddef>

// 一个最小可用的字符串类。目标不是替代 std::string，
// 而是让你亲手摸到「资源管理」这件事：谁申请，谁释放。
class String {
public:
    String();                            // 默认构造：空串
    explicit String(const char* s);      // 从 C 字符串构造（s 可能为 nullptr）
    String(const String& other);         // 拷贝构造：必须是深拷贝
    ~String();                           // 析构：释放自己那块内存

    const char* c_str() const;
    std::size_t size() const;

    // 测试用：当前还活着多少个 String 对象。
    // main 结束时它必须是 0，否则说明内存没还干净。
    static int alive();

private:
    char* data_;
    std::size_t size_;

    static int alive_;
};

#endif  // CPP_LAB_STRING_H
