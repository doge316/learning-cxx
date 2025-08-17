#include "../exercise.h"
#include <cstring>
// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        // TODO: 填入正确的 shape 并计算 size
        for (int i = 0; i < 4; ++i) {
        if (shape_[i] == 0) throw std::invalid_argument("Shape dimension cannot be zero");
        shape[i] = shape_[i];  // 拷贝形状
        size *= shape[i];       // 计算总元素数
    }
        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D&& other) noexcept 
    : shape{other.shape[0], other.shape[1], other.shape[2], other.shape[3]}, data(other.data) {
    other.data = nullptr;  // 避免析构时重复释放
}

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。

    //加法就是a[i][j][k]+b[i][j][k]的逻辑
    /// @brief 广播加法就是A+5的逻辑
   //存储方式：[batch, channel, height, width] --[2,3,2,2]:[
//   [  [ [0, 1], [2, 3] ],  [ [4, 5], [6, 7] ],  [ [8, 9], [10, 11] ]  ],  // 第0批（N=0）
//   [  [ [12,13], [14,15] ], [ [16,17], [18,19] ], [ [20,21], [22,23] ] ]   // 第1批（N=1）
// ] --2是第一个[]内的元素数，3是其中一个[]内的元素数...a[2][3][2][2]的元素数

    Tensor4D &operator+=(Tensor4D const &others) {
        // TODO: 实现单向广播的加法
        for (int i = 0; i < 4; ++i) {
        if (others.shape[i] != 1 && others.shape[i] != shape[i]) {
            throw std::invalid_argument("Incompatible shapes for broadcasting");
        }
    }

    // 计算各维度的步长（用于广播索引）
    //步长计算shape:​[2,3,2,2]​：
// ​**strides[3]（W维度）​**​：沿 W 移动1个单位（如 0→1），跳过1个元素 → strides[3]=1。
// ​**strides[2]（H维度）​**​：沿 H 移动1个单位（如 0→2），需跳过 W=2 个元素 → strides[2]=2。
// ​**strides[1]（C维度）​**​：沿 C 移动1个单位（如 0→4），需跳过 H×W=4 个元素 → strides[1]=4。
// ​**strides[0]（N维度）​**​：沿 N 移动1个单位（如 0→12），需跳过 C×H×W=12 个元素 → strides[0]=12。
    unsigned int strides[4] = {1, 1, 1, 1};
    for (int i = 3; i > 0; --i) {
        strides[i - 1] = strides[i] * shape[i];//strides[3]就是1
    }

    // 逐元素相加（支持广播）
    ///@brief 左侧补全可以保留原本含义（3，2）->(1,3,2)
    for (unsigned int n = 0; n < shape[0]; ++n) {
        for (unsigned int c = 0; c < shape[1]; ++c) {
            for (unsigned int h = 0; h < shape[2]; ++h) {
                for (unsigned int w = 0; w < shape[3]; ++w) {
                    // 计算当前元素的线性索引
                    unsigned int idx = n * strides[0] + c * strides[1] + h * strides[2] + w * strides[3];//在其中一个[]内计算增量，最后是元素的下标
                    // 计算 others 的索引（考虑广播）
                    unsigned int other_idx = 
                        (n % others.shape[0]) * (strides[0] / (shape[0] / others.shape[0])) +
                        (c % others.shape[1]) * (strides[1] / (shape[1] / others.shape[1])) +
                        (h % others.shape[2]) * (strides[2] / (shape[2] / others.shape[2])) +
                        (w % others.shape[3]) * (strides[3] / (shape[3] / others.shape[3]));
                    this->data[idx] += others.data[other_idx];
                }
            }
        }
    }
    return *this;
}
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D<int>{shape, data};
        auto t1 = Tensor4D<int>{shape, data};
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D<float>(s0, d0);
        auto t1 = Tensor4D<float>(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT((t0.data[i] - 7.f)<1e-6, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D<double>(s0, d0);
        auto t1 = Tensor4D<double>(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
