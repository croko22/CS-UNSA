#include "npy.hpp"
#include "tensor.h"
#include <iostream>

template <typename T>
Tensor load_tensor_from_npy(const std::string &filename)
{
    std::vector<unsigned long> shape_ul;
    std::vector<T> data = npy::read_npy<T>(filename).data;
    shape_ul = npy::read_npy<T>(filename).shape;

    std::vector<int> shape(shape_ul.begin(), shape_ul.end());
    Tensor tensor(shape);
    tensor.from_vector(std::vector<float>(data.begin(), data.end()));
    return tensor;
}

int main()
{
    Tensor input = load_tensor_from_npy<float>("data/encoder_input.npy");

    const auto &shape = input.get_shape();
    std::cout << "Loaded shape: [";
    for (size_t i = 0; i < shape.size(); ++i)
        std::cout << shape[i] << (i + 1 < shape.size() ? ", " : "");
    std::cout << "]\n";

    input.print();
    return 0;
}
