#ifndef DTREE_H
#define DTREE_H

#include <array>
#include <string>
#include <memory>
#include <variant>

constexpr int N = 26;

class DTree
{
public:
    DTree();
    void insert(std::string word, std::variant<int, std::string> data);
    void deleteWord(std::string word);
    std::variant<int, std::string> search(std::string word);
    bool startsWith(std::string prefix);

private:
    class Node;
    Node *getNode(std::string word);
    std::unique_ptr<Node> root;
};

#endif
