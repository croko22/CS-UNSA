#ifndef NODE_H
#define NODE_H

#include <array>
#include <memory>
#include "DTree.h"

class DTree::Node
{
public:
    Node(char ch = '\0') : c(ch){};
    const char c = '\0';
    std::array<std::unique_ptr<Node>, N> children;
    bool isWord = false;
    int data;
};

#endif
