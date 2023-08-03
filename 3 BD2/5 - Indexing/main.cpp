#include <iostream>
#include <algorithm>
#include "./GestorA/GestorAlmacenamiento.h"
#include "./BufferPoolManager/BufferPoolManager.h"
#include "./BPlusTree/BPlusTree.cpp"
// TODO:
//  1. Preguntar por el uso de la clase Node en BPlusTree (Que datos se guardan en los nodos)
// 2. Preguntar como estan vinculados BPlusTree y BufferPoolManager
void LRU_test()
{
    std::cout << "- LRU: Least Recently Used" << std::endl;
    // Crear un gestor de almacenamiento
    auto gestor = std::make_shared<GestorAlmacenamiento>("file.txt");

    // Crear un BufferPoolManager con la estrategia especificada
    BufferPoolManager bufferPoolManager(gestor, "LRU");

    // Agregar páginas al buffer pool
    int page_id = 0;
    bufferPoolManager.NewPage(page_id);
    bufferPoolManager.NewPage(1);
    bufferPoolManager.NewPage(2);

    // Obtener una página del buffer pool
    Page *page = bufferPoolManager.FetchPage(page_id);

    // Escribir y leer la página
    page->write_page();
    page->read_page();

    std::cout << "- Impresión de los marcos en el buffer pool" << std::endl;
    bufferPoolManager.PrintFrames();

    // Obtener otra página del buffer pool
    page = bufferPoolManager.FetchPage(3);

    // Desanclar una página y marcarla como modificada
    bool is_dirty = true;
    bufferPoolManager.UnpinPage(page_id, is_dirty);
    std::cout << "Desanclaje de la página 0" << std::endl;

    std::cout << "- Impresión de los marcos en el buffer pool" << std::endl;
    bufferPoolManager.PrintFrames();

    // Volcar la página modificada al disco
    bufferPoolManager.FlushPage(page_id);
    std::cout << "Volcado de la página al disco 0" << std::endl;

    // Eliminar una página del buffer pool
    bufferPoolManager.DeletePage(page_id);
    std::cout << "Eliminación de la página 0" << std::endl;

    std::cout << "- Impresión de los marcos en el buffer pool" << std::endl;
    bufferPoolManager.PrintFrames();
}

void BPTree_test()
{
    BPlusTree tree(4);

    tree.insert(10);
    tree.insert(20);
    tree.insert(5);
    tree.insert(15);
    tree.insert(25);
    tree.insert(30);
    tree.insert(17);
    tree.insert(27);
    tree.insert(28);
    tree.insert(29);
    tree.insert(31);
    tree.insert(32);

    tree.getValue(10);
    tree.getValue(77);

    tree.print();
}

int main()
{
    BPTree_test();
    return 0;
}