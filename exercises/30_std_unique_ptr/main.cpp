#include "../exercise.h"
#include <cstring>
#include <memory>
#include <string>
#include <vector>

// READ: `std::unique_ptr` <https://zh.cppreference.com/w/cpp/memory/unique_ptr>

std::vector<std::string> RECORDS;

class Resource {
    std::string _records;

public:
    void record(char record) {
        _records.push_back(record);// 记录操作
    }

    ~Resource() {
        RECORDS.push_back(_records);//析构时将记录添加到全局变量 RECORDS 中
    }
};

using Unique = std::unique_ptr<Resource>;//using type-alias = xxx 类型别名。typedef A B;  // A 类型别名为 B// #define 只能用于全局定义

//unipue_ptr是一个类型，不能拷贝，只能移动，独享所有权
//  std::unique_ptr<int> fPtr1;
//  std::unique_ptr<int> fPtr2(new int(4));
//  auto fPtr3 = std::make_unique<int>(); 调用了模板的move构造函数
   // fPtr1 = std::move(fPtr3);所有权转移，3变成空指针0x0000
   //场景：当我们动态申请内存后，有可能我们接下来的代码由于抛出异常或者提前退出（if语句）而没有执行delete操作。
Unique reset(Unique ptr) {
    if (ptr) ptr->record('r');
    return std::make_unique<Resource>();// 返回一个新的 Resource 对象的 unique_ptr
    // make_unique<Resource>() 等价于 std::unique_ptr<Resource>(new Resource())
    // make_unique是一个函数模板，返回一个std::unique_ptr<Resource>类型的对象
    // 这里的Resource是一个类，make_unique<Resource>()会调用Resource的构造函数
    // make_unique是C++14引入的，之前的版本可以使用 std::unique_ptr<Resource>(new Resource()) 来代替
    // make_unique的好处是可以避免手动调用new，减少内存泄漏的风险

}
Unique drop(Unique ptr) {
    if (ptr) ptr->record('d');
    return nullptr;
}
Unique forward(Unique ptr) {
    if (ptr) ptr->record('f');
    return ptr;
}

//std::vector<int> *a=new std::vector<int>{1, 2, 3, 4, 5};可以new一个类

int main(int argc, char **argv) {
    
    std::vector<std::string> problems[3];

// •当 unique_ptr被移动或销毁时，Resource 对象会被析构
// •析构顺序遵循函数调用栈的后进先出原则
    drop(forward(reset(nullptr)));
    problems[0] = std::move(RECORDS);

    forward(drop(reset(forward(forward(reset(nullptr))))));
    problems[1] = std::move(RECORDS);

    drop(drop(reset(drop(reset(reset(nullptr))))));
    problems[2] = std::move(RECORDS);

    // ---- 不要修改以上代码 ----

    std::vector<const char *> answers[]{
        {"fd"},
        // TODO: 分析 problems[1] 中资源的生命周期，将记录填入 `std::vector`
        // NOTICE: 此题结果依赖对象析构逻辑，平台相关，提交时以 CI 实际运行平台为准
        {"d","ffr"},
        {"d","d","r"},
    };

    // ---- 不要修改以下代码 ----

//     for (auto i = 0; i < 3; ++i) {
//     std::cout << "problems[" << i << "]: ";
//     for (const auto& s : problems[i]) std::cout << s << " ";
//     std::cout << "\nanswers[" << i << "]: ";
//     for (const auto& s : answers[i]) std::cout << s << " ";
//     std::cout << std::endl;
// }
    for (auto i = 0; i < 3; ++i) {
        ASSERT(problems[i].size() == answers[i].size(), "wrong size");
        for (auto j = 0; j < problems[i].size(); ++j) {
            ASSERT(std::strcmp(problems[i][j].c_str(), answers[i][j]) == 0, "wrong location");
        }
    }

    return 0;
}
