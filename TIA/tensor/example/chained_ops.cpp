#include "tensor.h"
#include <iostream>

int main()
{
    Tensor A({2, 3});
    A.from_vector({1, 2, 3, 4, 5, 6});

    Tensor B = ((A * 3.0f) + A - A).transpose();

    std::cout << "Tensor A:\n";
    A.print();
    std::cout << "((A * 3.0) + A - A)^T =\n";
    B.print();

    return 0;
}
