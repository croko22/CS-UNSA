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
    auto gestorIndex = std::make_shared<GestorAlmacenamiento>("index.bin", 10);
    auto gestorData = std::make_shared<GestorAlmacenamiento>("titanicFL.bin", 0);
    BPlusTree tree(4, gestorIndex, gestorData);
    for (auto &node : Nodes)
        tree.insert(get<0>(node.at(0)));
    tree.print();
    //? Search for a record
    tree.getValue(800);
    tree.getValue(777);
    tree.getValue(100);
    tree.getValue(1000);
    return 0;
}