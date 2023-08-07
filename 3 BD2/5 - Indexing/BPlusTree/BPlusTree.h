#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <vector>

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

    Node *root;
    size_t order;

public:
    BPlusTree(int order);

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
