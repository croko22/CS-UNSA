#include <iostream>
#include <algorithm>
#include "./GestorA/GestorAlmacenamiento.h"
#include "./BufferPoolManager/BufferPoolManager.h"
#include "./BPlusTree/BPlusTree.h"
constexpr int PAGE_SIZE = 4096;
constexpr int REG_SIZE = 12;
constexpr int REG_PER_BLOCK = PAGE_SIZE / REG_SIZE + 1;
constexpr int TOTAL_REGS = 891;
constexpr int TOTAL_PAGES = TOTAL_REGS / 17 + 1;
constexpr int TOTAL_NODES = TOTAL_REGS / REG_PER_BLOCK + 1;

// TODO:
//  1. Preguntar por el uso de la clase Node en BPlusTree (Que datos se guardan en los nodos)
// 2. Preguntar como estan vinculados BPlusTree y BufferPoolManager
void LRU_test()
{
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

// auto generateNodes()

int main()
{
    // BPTree_test();
    std::cout << REG_PER_BLOCK << " " << TOTAL_PAGES << " " << TOTAL_NODES << endl;
    //? Process file to get registers addresses for the B+ tree
    ifstream file("titanicFL.bin");
    //* Save page number
    int register_per_current_block, temp, address;
    //* Iterate TOTAL_NODES times
    vector<tuple<int, int, int>> registers;
    for (int j = 0; j < TOTAL_PAGES; j++)
    {
        file.seekg(j * PAGE_SIZE, ios::beg);
        file >> register_per_current_block >> temp >> temp;
        for (int i = 1; i < register_per_current_block + 1; i++)
        {
            file >> address >> temp;
            registers.push_back(make_tuple(i + j * 17, j, address));
        }
    }
    vector<vector<tuple<int, int, int>>> Nodes;
    for (int i = 0; i < TOTAL_NODES; i++)
    {
        vector<tuple<int, int, int>> slice(registers.begin() + i * REG_PER_BLOCK, min(registers.begin() + (i + 1) * REG_PER_BLOCK, registers.end()));
        Nodes.push_back(slice);
    }
    for (auto &node : Nodes)
    {
        cout << "Node: " << endl;
        for (auto &reg : node)
        {
            std::cout << get<0>(reg) << " " << get<1>(reg) << " " << get<2>(reg) << endl;
        }
        std::cout << endl;
    }
    //? Create B+ tree
    BPlusTree tree(4);
    for (auto &node : Nodes)
    {
        tree.insert(get<0>(node.at(0)));
    }
    tree.print();

    vector<char> buffer(115);
    file.seekg(get<2>(registers.at(800)), ios::beg);
    file.read(buffer.data(), 115);
    std::cout << buffer.data() << endl;

    return 0;
}