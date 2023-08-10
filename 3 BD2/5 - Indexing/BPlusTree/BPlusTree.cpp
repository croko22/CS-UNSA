#include "BPlusTree.h"
using namespace std;

BPlusTree::BPlusTree(int order, std::shared_ptr<GestorAlmacenamiento> gestorIndex, std::shared_ptr<GestorAlmacenamiento> gestorData) : order(order), indexbufferPoolManager(gestorIndex, "LRU"), dataBufferPoolManager(gestorData, "LRU")
{
    root = new Node();
}

void BPlusTree::insert(int key)
{
    Node *leaf = findLeaf(root, key);

    if (leaf->keys.empty())
    {
        leaf->keys.push_back(key);
    }
    else
    {
        vector<int>::iterator it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
        leaf->keys.insert(it, key);
    }

    if (leaf->keys.size() > order - 1)
        splitLeaf(leaf);
}

void BPlusTree::deleteNode(int key)
{
    Node *leaf = findLeaf(root, key);

    vector<int>::iterator it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
    if (it != leaf->keys.end() && *it == key)
    {
        leaf->keys.erase(it);
    }
    else
    {
        cout << "Key " << key << " not found" << endl;
    }
}

void BPlusTree::getValue(int key)
{
    Node *leaf = findLeaf(root, key);

    if (key < leaf->keys.back())
    {
        //* Buscar en el buffer pool manager
        if (indexbufferPoolManager.GetPageCount() < 3)
            indexbufferPoolManager.NewPage(leaf->keys.front() / 170);
        Page *page = indexbufferPoolManager.FetchPage(leaf->keys.front() / 170);
        page->read_page();
        //* Convertir buffer a stringstream
        std::stringstream ss(string(page->buffer.begin(), page->buffer.end()));
        int total_regs, reg, page_id, address;
        ss >> total_regs;
        vector<tuple<int, int, int>> records;
        for (int i = 0; i < total_regs; i++)
        {
            ss >> reg >> page_id >> address;
            records.push_back(make_tuple(reg, page_id, address));
        }
        // for (auto &reg : records)
        //     std::cout << get<0>(reg) << " " << get<1>(reg) << " " << get<2>(reg) << endl;
        auto val = records.at(key % 171 - 1);
        std::cout << get<0>(val) << " " << get<1>(val) << " " << get<2>(val) << endl;
        page_in_buffer = leaf->keys.front() / 170;

        //* Read data page
        if (dataBufferPoolManager.GetPageCount() < 3)
            dataBufferPoolManager.NewPage(get<1>(val));
        Page *data_page = dataBufferPoolManager.FetchPage(get<1>(val));
        data_page->read_page();
        //* Convertir buffer a stringstream
        string record = data_page->read_record(get<2>(val));
        std::cout << record << endl;
        // std::stringstream ss2(string(data_page->buffer.begin(), data_page->buffer.end()));
        // std::cout << ss2.str() << endl;
        return;
    }

    vector<int>::iterator it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
    if (it != leaf->keys.end() && *it == key)
    {
        cout << "Key " << key << " found" << endl;
    }
    else
    {
        cout << "Key " << key << " not found" << endl;
    }
}

void BPlusTree::print()
{
    printTree(root, 0);
}

BPlusTree::Node *BPlusTree::findLeaf(Node *node, int key)
{
    while (!node->isLeaf)
    {
        size_t i = 0;
        while (i < node->keys.size() && key >= node->keys[i])
        {
            i++;
        }
        node = node->children[i];
    }
    return node;
}

void BPlusTree::splitLeaf(Node *leaf)
{
    Node *newNode = new Node(true, leaf->parent);
    newNode->nextLeaf = leaf->nextLeaf;
    leaf->nextLeaf = newNode;

    int middle = leaf->keys.size() / 2;
    for (size_t i = middle; i < leaf->keys.size(); i++)
    {
        newNode->keys.push_back(leaf->keys[i]);
    }
    leaf->keys.erase(leaf->keys.begin() + middle, leaf->keys.end());

    if (leaf->parent == nullptr)
    {
        // Creating a new root node
        Node *newRoot = new Node(false);
        root = newRoot;
        leaf->parent = newRoot;
        newNode->parent = newRoot;
        newRoot->children.push_back(leaf);
        newRoot->children.push_back(newNode);
        newRoot->keys.push_back(newNode->keys[0]);
    }
    else
    {
        // Insert newNode in the parent node
        Node *parent = leaf->parent;
        int newKey = newNode->keys[0];

        vector<int>::iterator it = lower_bound(parent->keys.begin(), parent->keys.end(), newKey);
        vector<Node *>::iterator child_it = parent->children.begin() + (it - parent->keys.begin()) + 1;

        parent->keys.insert(it, newKey);
        parent->children.insert(child_it, newNode);

        if (parent->keys.size() > order - 1)
        {
            splitNonLeaf(parent);
        }
    }
}

void BPlusTree::splitNonLeaf(Node *nonLeaf)
{
    Node *newNonLeaf = new Node(false, nonLeaf->parent);

    int middle = nonLeaf->keys.size() / 2;
    int middleKey = nonLeaf->keys[middle];

    for (size_t i = middle + 1; i < nonLeaf->keys.size(); i++)
    {
        newNonLeaf->keys.push_back(nonLeaf->keys[i]);
    }
    nonLeaf->keys.erase(nonLeaf->keys.begin() + middle, nonLeaf->keys.end());

    for (size_t i = middle + 1; i < nonLeaf->children.size(); i++)
    {
        newNonLeaf->children.push_back(nonLeaf->children[i]);
        nonLeaf->children[i]->parent = newNonLeaf;
    }
    nonLeaf->children.erase(nonLeaf->children.begin() + middle + 1, nonLeaf->children.end());

    if (nonLeaf->parent == nullptr)
    {
        // Creating a new root node
        Node *newRoot = new Node(false);
        root = newRoot;
        nonLeaf->parent = newRoot;
        newNonLeaf->parent = newRoot;
        newRoot->children.push_back(nonLeaf);
        newRoot->children.push_back(newNonLeaf);
        newRoot->keys.push_back(middleKey);
    }
    else
    {
        // Insert newNonLeaf in the parent node
        Node *parent = nonLeaf->parent;

        vector<int>::iterator it = lower_bound(parent->keys.begin(), parent->keys.end(), middleKey);
        vector<Node *>::iterator child_it = parent->children.begin() + (it - parent->keys.begin()) + 1;

        parent->keys.insert(it, middleKey);
        parent->children.insert(child_it, newNonLeaf);

        if (parent->keys.size() > order - 1)
            splitNonLeaf(parent);
    }
}

void BPlusTree::printTree(Node *node, int level)
{
    if (node != nullptr)
    {
        cout << "Level " << level << ": ";
        for (int key : node->keys)
            cout << key << " ";
        cout << endl;

        if (!node->isLeaf)
            for (Node *child : node->children)
                printTree(child, level + 1);

        cout << "-----------" << endl;
    }
}