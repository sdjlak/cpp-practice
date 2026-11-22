#include "my_string.h"

#include <cstring>

int String::alive_ = 0;

// ---------------------------------------------------------------
// 规则：每个 new char[] 都必须有且只有一个 delete[] 与之配对；
//       alive_ 在构造里 +1，在析构里 -1。
//       任何一条没做到，测试就会给你脸色看。
// ---------------------------------------------------------------

// TODO(1) 默认构造
// 要求：造出一个「空串」——size_ 为 0，c_str() 返回一个合法的空字符串。
// 提示：不要留下野指针。你可以让 data_ 指向一块只装 '\0' 的内存。
String::String() {
    data_ = new char[1];
    data_[0] = '\0';
    size_ = 0;
    ++alive_;

}

// TODO(2) 从 C 字符串构造
// 要求：s 为 nullptr 时当作空串处理；否则按 s 的长度申请内存并复制内容。
// 提示：strlen(s) + 1 —— 别忘了给结尾的 '\0' 留位置。
String::String(const char* s)  {
    if (s == nullptr) {
        data_ = new char[1];
        data_[0] = '\0';
        size_ = 0;
    }
    else {
            size_ = strlen(s);
            data_ = new char[size_ + 1];
            strcpy(data_, s);
    }
    ++alive_;
}

// TODO(3) 拷贝构造
// 要求：深拷贝——两个对象各自持有一块独立的内存。
// 提示：如果你写成 data_ = other.data_，析构时就会同一块内存 free 两次，
//       程序会在你意想不到的地方崩掉。这正是这个练习的核心。
String::String(const String& other){
    size_ = other.size_;
    data_ = new char[size_ + 1];
    strcpy(data_, other.data_);
    ++alive_;
}

// TODO(4) 析构
// 要求：把 data_ 指向的内存还回去。
// 提示：new char[] 配 delete[]，不是 delete。写错了测试会崩。
String::~String() {
    delete[] data_;
    --alive_;
}

// TODO(5) 拷贝赋值运算符
// 场景：对象已经存在，现在要把另一个对象的内容「搬进来」。
//       和拷贝构造不同——这里 data_ 已经有旧内存了，必须先处理掉。
//
// 三个坑，一个都不能踩：
//   坑一：自赋值。a = a 的时候，如果先把 data_ 删了，再想从 other 复制，
//         而 other 就是自己——你读的是一块已经被释放的内存。
//   坑二：内存泄漏。旧的 data_ 不删，直接覆盖指针，那块内存就永远找不回来了。
//   坑三：返回值。必须返回 String&（= *this），否则 c = b = a 这样的链式赋值会编译失败。
//
// 提示：先想清楚这三步的顺序，再动键盘。
//       想不明白就先用最朴素的办法写出来，让测试来告诉你哪里不对。
String& String::operator=(const String& other) {
    if (this == &other) {
        return *this;

    }
    size_ = other.size_;

    delete[] data_;
    data_ = new char[size_ + 1];
    strcpy(data_, other.data_);

    return *this;
}

const char* String::c_str() const {
    return data_;
}

std::size_t String::size() const {
    return size_;
}

int String::alive() {
    return alive_;
}
