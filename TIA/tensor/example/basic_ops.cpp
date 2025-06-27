#include "tensor.h"
#include <iostream>

void print_with_title(const std::string &title, const Tensor &tensor)
{
    std::cout << "=== " << title << " ===\n";
    tensor.print();
    std::cout << "\n";
}

int main()
{
    Tensor A({2, 3});
    A.from_vector({1, 2, 3, 4, 5, 6});

    Tensor B({2, 3});
    B.from_vector({6, 5, 4, 3, 2, 1});

    Tensor C = A + B;
    Tensor D = A - B;
    Tensor E = A * 2.0f;
    Tensor F = A.transpose();

    print_with_title("Tensor A", A);
    print_with_title("Tensor B", B);
    print_with_title("A + B", C);
    print_with_title("A - B", D);
    print_with_title("A * 2", E);
    print_with_title("Transpose of A", F);

    return 0;
}
