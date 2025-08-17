#include "../exercise.h"
#include <array>
#include <cstring>

// READ: std::array <https://zh.cppreference.com/w/cpp/container/array>

// TODO: 将下列 `?` 替换为正确的代码
int main(int argc, char **argv) {
    {
        std::array<int, 5> arr{{1, 2, 3, 4, 5}};
        ASSERT(arr.size() == 5, "Fill in the correct value.");// 数组的大小（元素个数）
        ASSERT(sizeof(arr) == 20, "Fill in the correct value.");// 数组的大小（5个int，每个4字节）
        int ans[]{1, 2, 3, 4, 5}; 
        ASSERT(std::memcmp(arr.data(), ans, sizeof(arr)) == 0, "Fill in the correct values.");//使用arr.data()获取指向底层数组的指针（等价于&arr[0]）
    }
    {
        std::array<double, 8> arr;
        ASSERT(arr.size() == 8, "Fill in the correct value.");
        ASSERT(sizeof(arr) == 64, "Fill in the correct value.");
    }
    {
        std::array<char, 21> arr{"Hello, InfiniTensor!"};
        ASSERT(arr.size() == 21, "Fill in the correct value.");
        ASSERT(sizeof(arr) == 21, "Fill in the correct value.");
        ASSERT(std::strcmp(arr.data(), "Hello, InfiniTensor!") == 0, "Fill in the correct value.");//strcmp仅用于以 \0结尾的字符串
    }
    return 0;
}
