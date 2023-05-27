#ifndef NODE_H
#define NODE_H

#include <array>
#include <memory>
#include <variant>
#include "DTree.h"

// class Hoja
// {
// public:
//     Hoja(){};
//     Hoja(std::variant<int, std::string> data) : data(data){};
//     std::variant<int, std::string> data;
// };

class DTree::Node
{
public:
    Node(char ch = '\0') : c(ch){};
    const char c = '\0';
    std::array<std::unique_ptr<Node>, N> children;
    bool isWord = false;
    //*CLASE HOJA
    //? Variant para poder guardar tanto int como string
    std::variant<int, std::string> data;
};

#endif
