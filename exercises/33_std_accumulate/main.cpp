#include "../exercise.h"
#include <numeric>

// READ: `std::accumulate` <https://zh.cppreference.com/w/cpp/algorithm/accumulate>

int main(int argc, char **argv) {
    using DataType = float;
    int shape[]{1, 3, 224, 224};
    // TODO: 调用 `std::accumulate` 计算：
    //       - 数据类型为 float；
    //       - 形状为 shape；
    //       - 连续存储；
    //       的张量占用的字节数
    // int size =
     // 计算总元素数量：1 * 3 * 224 * 224
    int element_count = std::accumulate(
        shape, shape + sizeof(shape)/sizeof(shape[0]), 1, 
        [](int a, int b) { return a * b; }
    );
    
    // 计算总字节数 = 元素数量 * 每个元素占用的字节数
    size_t size = element_count * sizeof(DataType);
    ASSERT(size == 602112, "4x1x3x224x224 = 602112");
    return 0;
}
