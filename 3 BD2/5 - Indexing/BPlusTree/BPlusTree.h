#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <sstream>
#include <vector>
#include "../BufferPoolManager/BufferPoolManager.h"
#include "../GestorA/GestorAlmacenamiento.h"

using namespace std;

class BPlusTree
{
private:
    struct Node
    {
        vector<int> keys;
        vector<Node *> children;
        bool isLeaf;
        Node *parent;
        Node *nextLeaf;

        Node(bool isLeaf = true, Node *parent = nullptr) : isLeaf(isLeaf), parent(parent), nextLeaf(nullptr) {}
    };

    // TODO: Conexion con el buffer pool manager
    Node *root;
    size_t order;
    BufferPoolManager indexbufferPoolManager;
    BufferPoolManager dataBufferPoolManager;

public:
    int page_in_buffer;
    BPlusTree(int order, std::shared_ptr<GestorAlmacenamiento> gestorIndex, std::shared_ptr<GestorAlmacenamiento> gestorData);
    void insert(int key);
    void deleteNode(int key);
    void getValue(int key);
    void print();

private:
    Node *findLeaf(Node *node, int key);
    void splitLeaf(Node *leaf);
    void splitNonLeaf(Node *nonLeaf);
    void printTree(Node *node, int level);
};

#endif // BPLUSTREE_H
