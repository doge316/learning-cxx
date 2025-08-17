#include "../exercise.h"

// READ: 静态字段 <https://zh.cppreference.com/w/cpp/language/static>
// READ: 虚析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>

struct A {
    // TODO: 正确初始化静态字段
    static int num_a ;

    A() {
        ++num_a;
    }
   virtual ~A() {
        --num_a;
    }

    virtual char name() const {
        return 'A';
    }
};
int A::num_a =0;// 初始化静态字段不加 static 关键字，但要加int，类似声明

struct B final : public A {
    // TODO: 正确初始化静态字段
    static int num_b;

    B() {
        ++num_b;
    }
    ~B() {
        --num_b;
    }

    char name() const final {
        return 'B';
    }
};
int B::num_b=0;

int main(int argc, char **argv) {
    auto a = new A;
    auto b = new B;//创建派生类对象，会先调用基类构造函数，再调用派生类构造函数
    ASSERT(A::num_a == 2, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(a->name() == 'A', "Fill in the correct value for a->name()");
    ASSERT(b->name() == 'B', "Fill in the correct value for b->name()");

    delete a;
    delete b;//先调用派生类析构函数，再调用基类析构函数
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    A *ab = new B;// 派生类指针可以随意转换为基类指针 //ab作为基类指针，只能访问基类成员，但仍可以调用派生类覆盖的虚函数
    ASSERT(A::num_a == 1, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(ab->name() == 'B', "Fill in the correct value for ab->name()");

    // TODO: 基类指针无法随意转换为派生类指针，补全正确的转换语句
    B &bb =dynamic_cast<B&>(*ab); // 通过引用转换，转换失败会抛出异常
    ASSERT(bb.name() == 'B', "Fill in the correct value for bb->name()");

    // TODO: ---- 以下代码不要修改，通过改正类定义解决编译问题 ----
    delete ab;// 通过指针可以删除指向的对象，即使是多态对象
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");//若基类 A 的析构函数 ​非虚，只会调用 A::~A()

// B 的内存布局为 [A部分][B新增部分]。new B 会在堆上分配足够的内存（包含基类和派生类成员），并调用 B 的构造函数初始化对象
//int *a = new B; 将派生类对象的地址隐式转换为基类指针 int*。编译器仅保留基类部分的地址（即对象内存的起始位置），丢弃派生类新增部分的偏移信息。
//若基类成员函数是虚函数，且派生类重写了它，通过 a 调用时会触发动态绑定，实际执行派生类的实现（通过虚函数表查找）。   

    return 0;
}
