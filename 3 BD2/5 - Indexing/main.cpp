#include <iostream>
#include <sstream>
#include <algorithm>
#include "./GestorA/GestorAlmacenamiento.h"
#include "./BufferPoolManager/BufferPoolManager.h"
#include "./BPlusTree/BPlusTree.h"
constexpr int PAGE_SIZE = 4096;
constexpr int REG_SIZE = 24;
constexpr int REG_PER_BLOCK = PAGE_SIZE / REG_SIZE + 1;
constexpr int TOTAL_REGS = 891;
constexpr int TOTAL_PAGES = TOTAL_REGS / 17 + 1;
constexpr int TOTAL_NODES = TOTAL_REGS / REG_PER_BLOCK + 1;

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

std::vector<std::tuple<int, int, int>> processFile(std::string filename)
{
    ifstream file(filename);
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
            records.push_back(make_tuple(i + j * 17, j, address - 115));
        }
    }
    return records;
}

// Function to create and populate nodes for B+ tree
std::vector<std::vector<std::tuple<int, int, int>>> createNodes(const std::vector<std::tuple<int, int, int>> &records)
{
    std::vector<std::vector<std::tuple<int, int, int>>> Nodes;
    for (int i = 0; i < TOTAL_NODES; i++)
    {
        std::vector<std::tuple<int, int, int>> slice(records.begin() + i * REG_PER_BLOCK,
                                                     std::min(records.begin() + (i + 1) * REG_PER_BLOCK, records.end()));
        Nodes.push_back(slice);
    }
    return Nodes;
}

// Function to save nodes into an index file
void saveNodesToFile(const std::vector<std::vector<std::tuple<int, int, int>>> &Nodes)
{
    std::fstream index("index.bin", std::ios::out | std::ios::binary);
    int root = 0, order = 4, total_nodes = TOTAL_NODES, count = 0;

    index << root << " " << order << " " << total_nodes << " ";
    for (const auto &node : Nodes)
    {
        index.seekg(count * PAGE_SIZE + 10, std::ios::beg);
        index << node.size() << " ";
        for (const auto &reg : node)
            index << std::get<0>(reg) << " " << std::get<1>(reg) << " " << std::get<2>(reg) << " ";
        index << std::endl;
        count++;
    }
    index.close();
}

int main()
{
    // std::cout << REG_PER_BLOCK << " " << TOTAL_PAGES << " " << TOTAL_NODES << endl;
    std::vector<std::tuple<int, int, int>> records = processFile("titanicFL.bin");
    vector<vector<tuple<int, int, int>>> Nodes = createNodes(records);
    saveNodesToFile(Nodes);
    //? Create B+ tree
    cout << "Cuantity of nodes: " << Nodes.size() << endl;
    BPlusTree tree(4);
    for (auto &node : Nodes)
        tree.insert(get<0>(node.at(0)));
    tree.print();
    //? Search for a record
    tree.getValue(800);
    int page_id_index = 800 / 170;
    //? Load a record from buffer pool
    auto gestor = std::make_shared<GestorAlmacenamiento>("index.bin");
    BufferPoolManager bufferPoolManager(gestor, "LRU");
    bufferPoolManager.NewPage(0);
    Page *page = bufferPoolManager.FetchPage(page_id_index);
    // Page *page = bufferPoolManager.FetchPage(1);
    page->read_page();
    std::stringstream ss(string(page->buffer.begin(), page->buffer.end()));
    int total_regs, reg, page_id, addresss;
    ss >> total_regs;
    vector<tuple<int, int, int>> records2;
    for (int i = 0; i < total_regs; i++)
    {
        ss >> reg >> page_id >> addresss;
        records2.push_back(make_tuple(reg, page_id, addresss));
    }
    for (auto &reg : records2)
        std::cout << get<0>(reg) << " " << get<1>(reg) << " " << get<2>(reg) << endl;
    //? Read record from file
    vector<char> buffer(115);
    auto val = records2.at(800 % 171 - 1);
    std::cout << get<0>(val) << " " << get<1>(val) << " " << get<2>(val) << endl;
    // file.seekg(get<2>(val), ios::beg);
    // file.read(buffer.data(), 115);
    // std::cout << buffer.data() << endl;
    return 0;
}