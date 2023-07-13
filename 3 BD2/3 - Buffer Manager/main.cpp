#include <iostream>
#include <algorithm>
#include "GestorAlmacenamiento.h"
#include "BufferPoolManager.h"
#include "Tests.cpp"

int main()
{
    std::cout << "- LRU: Least Recently Used" << std::endl;
    test("LRU");
    std::cout << "- MRU: Most Recently Used" << std::endl;
    test("MRU");
    std::cout << "- CLOCK" << std::endl;
    test("CLOCK");
    // test_CLI();
    return 0;
}