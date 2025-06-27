#include "tensor.h"
#include <iostream>
#include <chrono>

int main()
{
    const int rows = 1000, cols = 1000;
    Tensor A({rows, cols});
    Tensor B({rows, cols});

    std::vector<float> data(rows * cols, 1.0f);
    A.from_vector(data);
    B.from_vector(data);

    auto start = std::chrono::high_resolution_clock::now();
    Tensor C = A + B;
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time for A + B (" << rows << "x" << cols << "): " << elapsed.count() << "s\n";

    return 0;
}
