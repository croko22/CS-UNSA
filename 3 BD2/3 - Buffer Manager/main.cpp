#include <iostream>
#include <algorithm>
#include "GestorAlmacenamiento.h"
#include "BufferPoolManager.h"
#include "Tests.cpp"

int main()
{
    test_LRU();
    // test_CLI();
    return 0;
}