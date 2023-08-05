#include <iostream>
#include <algorithm>
#include "./GestorA/GestorAlmacenamiento.h"
#include "./BufferPoolManager/BufferPoolManager.h"
#include "./BPlusTree/BPlusTree.cpp"
constexpr int PAGE_SIZE = 4096;
constexpr int REG_SIZE = 12;
constexpr int REG_PER_BLOCK = PAGE_SIZE / REG_SIZE + 1;
constexpr int TOTAL_REGS = 891;
constexpr int TOTAL_BLOCKS = TOTAL_REGS / REG_PER_BLOCK + 1;
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

int main()
{
    // BPTree_test();
    std::cout << REG_PER_BLOCK << " " << TOTAL_BLOCKS << " " << TOTAL_PAGES << " " << TOTAL_NODES << endl;
    //? Process file to get registers addresses for the B+ tree
    // Read register
    ifstream file("titanicFL.bin");
    //* Save page number
    int register_per_current_block, temp, address;
    //? Checkpoint vals
    int remaining_regs = REG_PER_BLOCK;
    bool checkpoint = false;
    //* Iterate TOTAL_NODES times
    // for (int k = 0; k < TOTAL_NODES; k++)
    // {
    //     vector<tuple<int, int, int>> registers;

    //     if (checkpoint)
    //     {
    //         for (int i = 0; i < remaining_regs; i++)
    //         {
    //             file >> address >> temp;
    //             registers.push_back(make_tuple(i + k * 17, k, address));
    //         }
    //     }
    // }
    //* Save registers <register_number, page_address, register_address>
    vector<tuple<int, int, int>> registers;
    //* Iterate REG_PER_BLOCK/17 times
    for (int j = 0; j < REG_PER_BLOCK / 17 + 1; j++)
    {
        file.seekg(j * PAGE_SIZE, ios::beg);
        file >> register_per_current_block >> temp >> temp;
        // std::cout << register_per_current_block << endl;
        for (int i = 1; i < register_per_current_block + 1; i++)
        {
            file >> address >> temp;
            registers.push_back(make_tuple(i + j * 17, j, address));
            if (i + j * 17 == REG_PER_BLOCK)
            {
                std::cout << "Last register: " << i + j * 17 << endl;
                //*Checkpoint
                checkpoint = true;
                remaining_regs = register_per_current_block - i;
                std::cout << "Remaining registers: " << remaining_regs << endl;
                break;
            }
        }
    }
    //*Print registers
    for (auto &reg : registers)
    {
        std::cout << get<0>(reg) << " " << get<1>(reg) << " " << get<2>(reg) << endl;
    }

    vector<char> buffer(115);
    file.seekg(get<2>(registers.at(300)), ios::beg);
    file.read(buffer.data(), 115);
    std::cout << buffer.data() << endl;

    return 0;
}