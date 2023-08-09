#include <iostream>
#include <algorithm>
#include "./GestorA/GestorAlmacenamiento.h"
#include "./BufferPoolManager/BufferPoolManager.h"
#include "./BPlusTree/BPlusTree.h"
constexpr int PAGE_SIZE = 4096;
//?Old val
// constexpr int REG_SIZE = 12;
constexpr int REG_SIZE = 24;
constexpr int REG_PER_BLOCK = PAGE_SIZE / REG_SIZE + 1;
constexpr int TOTAL_REGS = 891;
constexpr int TOTAL_PAGES = TOTAL_REGS / 17 + 1;
constexpr int TOTAL_NODES = TOTAL_REGS / REG_PER_BLOCK + 1;

// TODO:
// 1. Preguntar como se guardarian las direcciones en los nodos hoja
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

// auto generateNodes()

int main()
{
    std::cout << REG_PER_BLOCK << " " << TOTAL_PAGES << " " << TOTAL_NODES << endl;
    //? Process file to get records addresses for the B+ tree
    ifstream file("titanicFL.bin");
    //* Save page number
    int register_per_current_block, temp, address;
    //* Iterate TOTAL_NODES times
    vector<tuple<int, int, int>> records;
    for (int j = 0; j < TOTAL_PAGES; j++)
    {
        file.seekg(j * PAGE_SIZE, ios::beg);
        file >> register_per_current_block >> temp >> temp;
        for (int i = 1; i < register_per_current_block + 1; i++)
        {
            file >> address >> temp;
            records.push_back(make_tuple(i + j * 17, j, address));
        }
    }
    vector<vector<tuple<int, int, int>>> Nodes;
    for (int i = 0; i < TOTAL_NODES; i++)
    {
        vector<tuple<int, int, int>> slice(records.begin() + i * REG_PER_BLOCK, min(records.begin() + (i + 1) * REG_PER_BLOCK, records.end()));
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
    // TODO: Crear un archivo index.bin para guardar los nodos
    fstream index("index.bin", ios::out | ios::binary);
    //?Header values (root, order, total_nodes)
    int root = 0, order = 4, total_nodes = TOTAL_NODES, count = 0;
    // index << root << " " << order << " " << total_nodes << endl;
    index << root << " " << order << " " << total_nodes << " ";
    for (auto &node : Nodes)
    {
        index.seekg(count * PAGE_SIZE + 12, ios::beg);
        for (auto &reg : node)
            index << get<0>(reg) << " " << get<1>(reg) << " " << get<2>(reg) << " ";
        index << endl;
        count++;
    }
    //? Create B+ tree
    BPlusTree tree(4);
    for (auto &node : Nodes)
    {
        tree.insert(get<0>(node.at(0)));
    }
    tree.print();

    vector<char> buffer(115);
    file.seekg(get<2>(records.at(800)), ios::beg);
    file.read(buffer.data(), 115);
    std::cout << buffer.data() << endl;

    return 0;
}